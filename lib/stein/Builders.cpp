// Builders.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "Builders.hpp"
#include "Color.hpp"
#include "Object.hpp"
#include "Tools.hpp"
#include "MeshBuilder.h"
#include "math/StreamUtils.h"
#include "../glm/glm.hpp"

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cfloat>

using namespace std;
using namespace glm;

namespace stein {

// Builds one triangle
void buildTriangle(Object &object) {
    MeshBuilder builder;
    builder.addVertex(-0.2, 0, 0);
    builder.addVertex(0.2, 0, 0);
    builder.addVertex(0, 1, 0);
    builder.addNormal(0, 0, 1);
    builder.addFace(0, 1, 2).setNormalIndices(0, 0, 0);

    vector<unsigned int> indices;
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendPrimitives(vertices, indices);
}

// Builds one square
void buildSquare(Object &object, const GLfloat side) {
    MeshBuilder builder;
    builder.addVertex(-side, -side, 0); // bottom left
    builder.addVertex(side, -side, 0); // bottom right
    builder.addVertex(-side, side, 0); // top left
    builder.addVertex(side, side, 0); // top right

    builder.addNormal(0, 0, 1);

    builder.addUV(0.0f, 1.0f);
    builder.addUV(1.0f, 1.0f);
    builder.addUV(0.0f, 0.0f);
    builder.addUV(1.0f, 0.0f);


    MeshBuilder::Face &face1 = builder.addFace(0, 1, 2);
    face1.setNormalIndices(0, 0, 0);
    face1.setUvIndices(0, 1, 2);

    MeshBuilder::Face &face2 = builder.addFace(2, 1, 3);
    face2.setNormalIndices(0, 0, 0);
    face2.setUvIndices(2, 1, 3);

    vector<unsigned int> indices;
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendUvs(uvs);
    object.sendNormals(normals);
    object.sendPrimitives(vertices, indices);
}


struct Triple {
    GLuint a, b, c;
};

static Triple readTriple(string& line) {
    replace(line.begin(), line.end(), '/', ' ');
    istringstream ss(line);
    size_t a, b, c;
    ss >> a >> b >> c;
    Triple value = { --a, --b, --c };
    return value;
}

// Builds an object made from an OBJ file, taking only geometry into account (not materials)
bool buildObjectGeometryFromOBJ(Object &object, const char* fileName, bool smoothObject) {
    ifstream file(fileName, ios_base::in);
    if (!file) {
        cout << "       Error while loading object from .obj file : " << fileName << "." << endl;
        return false;
    }
    cout << "       Loading object from .obj file : " << fileName << "." << endl;

    MeshBuilder builder;
    bool hasVt = false;
    bool hasVn = false;
    string buf, key, name, MTLFileName;

    while (getline(file, buf)) {
        istringstream line(buf);
        line >> key;
        if (key == "o")
            line >> name;
        else if (key == "v") {
            float x, y, z;
            line >> x >> y >> z;
            builder.addVertex(x, y, z);
        } else if (key == "vt") {
            float u, v;
            line >> u >> v;
            builder.addUV(u, 1 - v);
            hasVt = true;
        } else if (key == "vn") {
            float x, y, z;
            line >> x >> y >> z;
            builder.addNormal(x, y, z);
            hasVn = true;
        } else if (key == "f") {
            string a, b, c;
            line >> a >> b >> c;
            const Triple t1 = readTriple(a);
            const Triple t2 = readTriple(b);
            const Triple t3 = readTriple(c);
            builder.addFace(t1.a, t2.a, t3.a).setUvIndices(t1.b, t2.b, t3.b).setNormalIndices(t1.c, t2.c, t3.c);
        } else if (key == "mltlib")
            line >> MTLFileName;
    }

    cout << "       Obj mesh " << name << " loading..." << endl;
    cout << "       Obj meshes should only be made of triangles (for this loader), make sure this is correct in file." << endl;

    //    if (smoothObject)
//        reorderUvsAndNormalsIfSmooth(&vertices, &uvs, &normals, &indices, &uvIndices, &normalIndices);
//    else
//        reorderUvsAndNormalsIfNonSmooth(&vertices, &uvs, &normals, &indices, &uvIndices, &normalIndices);

//    conformToObject(&vertices, &uvs, &normals);

    builder.centerAndNormalizeMesh();
    builder.normalizeNormals();

    vector<unsigned int> indices;
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    object.sendPrimitives(vertices, indices);

    if (hasVn) {
        const bool oneNormalPerTriangle = (normals.size() == indices.size());
        if (oneNormalPerTriangle) {
            cout << "       Obj file " << name << " was not smoothed in modeler." << endl;
            if (smoothObject)
                cout << "       WARNING : smoothObject==true. Normals will be wrong : change it to false." << endl;
        }
        object.sendNormals(normals);
    } else
        cout << "       WARNING : Obj file " << name << " has no normals, add some in modeler." << endl;

    if (hasVt)
        object.sendUvs(uvs);
    else
        cout << "       WARNING : Obj file " << name << " has no texture coordinates, add some in modeler." << endl;

    cout << "       Material files are not taken into account by this loader." << endl;

    return true;
}


void buildSphere(Object &object, const float r,std::size_t discLat, std::size_t discLong){
    MeshBuilder builder;

    GLfloat rcpLat = 1.f / discLat, rcpLong = 1.f / discLong;
    GLfloat dPhi = 2 * M_PI * rcpLat, dTheta = M_PI * rcpLong;

    // Construit l'ensemble des vertex
    for(GLsizei j = 0; j <= discLong; ++j) {
        GLfloat cosTheta = cos(-M_PI / 2 + j * dTheta);
        GLfloat sinTheta = sin(-M_PI / 2 + j * dTheta);

        for(GLsizei i = 0; i < discLat; ++i) {


            builder.addVertex(  r*sin(i * dPhi) * cosTheta,
                                r*sinTheta,
                                r*cos(i * dPhi) * cosTheta
                            );

            builder.addNormal(
                sin(i * dPhi) * cosTheta,
                sinTheta,
                cos(i * dPhi) * cosTheta);

            builder.addUV(i * rcpLat, 1.f - j * rcpLong);


        }
    }


    // Construit les vertex finaux en regroupant les données en triangles:
    // Pour une longitude donnée, les deux triangles formant une face sont de la forme:
    // (i, i + 1, i + discLat + 1), (i, i + discLat + 1, i + discLat)
    // avec i sur la bande correspondant à la longitude
    for(GLsizei j = 0; j < discLong; ++j) {
        //GLsizei offset = j * discLat;
        for(GLsizei i = 0; i < discLat; ++i) {

            if(i+1!=discLat){
                MeshBuilder::Face & face = builder.addFace(discLat*j+i, discLat*j+i+1, discLat*(j+1)+i+1);
                face.setNormalIndices(discLat*j+i, discLat*j+i+1, discLat*(j+1)+i+1);
                face.setUvIndices(discLat*j+i, discLat*j+i+1, discLat*(j+1)+i+1);

                MeshBuilder::Face & face2 = builder.addFace(discLat*j+i, discLat*(j+1)+i+1, discLat*(j+1)+i);
                face2.setNormalIndices(discLat*j+i, discLat*(j+1)+i+1, discLat*(j+1)+i);
                face2.setUvIndices(discLat*j+i, discLat*(j+1)+i+1, discLat*(j+1)+i);
            }else{
                MeshBuilder::Face & face = builder.addFace(discLat*j+i,discLat*j,discLat*(j+1));
                face.setNormalIndices(discLat*j+i,discLat*j,discLat*(j+1));
                face.setUvIndices(discLat*j+i,discLat*j,discLat*(j+1));

                MeshBuilder::Face & face2 = builder.addFace(discLat*j+i,discLat*(j+1),discLat*(j+1)+i);
                face2.setNormalIndices(discLat*j+i,discLat*(j+1),discLat*(j+1)+i);
                face2.setUvIndices(discLat*j+i,discLat*(j+1),discLat*(j+1)+i);
            }
        }
    }

    vector<unsigned int> indices;
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendUvs(uvs);
    object.sendNormals(normals);
    object.sendPrimitives(vertices, indices);
}


void buildCube(Object &object, float side) {
    MeshBuilder builder;

    builder.addVertex(-side, -side, side); // 0 - front bottom left
    builder.addVertex(side, -side, side); // 1 - front bottom right
    builder.addVertex(-side, side, side); // 2 - front top left
    builder.addVertex(side, side, side); // 3 - front top right
    builder.addVertex(-side, -side, -side); // 4 - back bottom left
    builder.addVertex(side, -side, -side); // 5 - back bottom right
    builder.addVertex(-side, side, -side); // 6 - back top left
    builder.addVertex(side, side, -side); // 7 - back top right

    builder.addNormal(-1, 0, 0);
    builder.addNormal(1, 0, 0);
    builder.addNormal(0, 1, 0);
    builder.addNormal(0, -1, 0);
    builder.addNormal(0, 0, -1);
    builder.addNormal(0, 0, 1);

    builder.addUV(0., 0.);
    builder.addUV(0., 1.);
    builder.addUV(1., 0.);
    builder.addUV(1., 1.);

    // left
    MeshBuilder::Face &left1 = builder.addFace(0, 2, 4);
    left1.setNormalIndices(0, 0, 0);
    left1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &left2 = builder.addFace(4, 2, 6);
    left2.setNormalIndices(0, 0, 0);
    left2.setUvIndices(2, 1, 3);

    // right
    MeshBuilder::Face &right1 = builder.addFace(1, 3, 5);
    right1.setNormalIndices(1, 1, 1);
    right1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &right2 = builder.addFace(5, 3, 7);
    right2.setNormalIndices(1, 1, 1);
    right2.setUvIndices(2, 1, 3);

    // top
    MeshBuilder::Face &top1 = builder.addFace(2, 6, 3);
    top1.setNormalIndices(2, 2, 2);
    top1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &top2 = builder.addFace(3, 6, 7);
    top2.setNormalIndices(2, 2, 2);
    top2.setUvIndices(2, 1, 3);

    // bottom
    MeshBuilder::Face &bottom1 = builder.addFace(0, 1, 4);
    bottom1.setNormalIndices(3, 3, 3);
    bottom1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &bottom2 = builder.addFace(4, 1, 5);
    bottom2.setNormalIndices(3, 3, 3);
    bottom2.setUvIndices(2, 1, 3);

    // back
    MeshBuilder::Face &back1 = builder.addFace(4, 5, 6);
    back1.setNormalIndices(4, 4, 4);
    back1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &back2 = builder.addFace(6, 5, 7);
    back2.setNormalIndices(4, 4, 4);
    back2.setUvIndices(2, 1, 3);

    // front
    MeshBuilder::Face &front1 = builder.addFace(0, 2, 1);
    front1.setNormalIndices(5, 5, 5);
    front1.setUvIndices(0, 1, 2);
    MeshBuilder::Face &front2 = builder.addFace(1, 2, 3);
    front2.setNormalIndices(5, 5, 5);
    front2.setUvIndices(2, 1, 3);

    vector<unsigned int> indices;
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendUvs(uvs);
    object.sendNormals(normals);
    object.sendPrimitives(vertices, indices);
}

} // namespace stein
