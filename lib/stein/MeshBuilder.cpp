/*
 * MeshBuilder.cpp
 *
 *  Created on: Dec 11, 2011
 *      Author: Guillaume Chatelet
 */

#include "MeshBuilder.h"

#include <stdexcept>
#include <cfloat>
#include <algorithm>

using namespace std;
using namespace glm;

namespace stein {

struct MinMax {
    vec3 min;
    vec3 max;

    MinMax() :
        min(FLT_MAX, FLT_MAX, FLT_MAX), max(-FLT_MAX, -FLT_MAX, -FLT_MAX) {
    }

    void operator()(const vec3& value) {
        min.x = std::min(min.x, value.x);
        min.y = std::min(min.y, value.y);
        min.z = std::min(min.z, value.z);
        max.x = std::max(max.x, value.x);
        max.y = std::max(max.y, value.y);
        max.z = std::max(max.z, value.z);
    }
};

struct MoveAndScale {
    const vec3 add;
    const float mul;

    MoveAndScale(const vec3 &move, const float scale) :
        add(move), mul(scale) {
    }

    void operator()(vec3& value) {
        value += add;
        value *= mul;
    }
};

MeshBuilder::Face& MeshBuilder::Face::setUvIndices(unsigned int a, unsigned int b, unsigned int c) {
    t1 = a;
    t2 = b;
    t3 = c;
    return *this;
}

MeshBuilder::Face& MeshBuilder::Face::setNormalIndices(unsigned int a, unsigned int b, unsigned int c) {
    n1 = a;
    n2 = b;
    n3 = c;
    return *this;
}

void MeshBuilder::addVertex(float x, float y, float z) {
    addVertex(vec3(x, y, z));
}

void MeshBuilder::addVertex(const vec3&p) {
    m_Vertices.push_back(p);
}

void MeshBuilder::addUV(float u, float v) {
    m_Uvs.push_back(make_pair(u, v));
}

void MeshBuilder::addNormal(float x, float y, float z) {
    addNormal(vec3(x, y, z));
}

void MeshBuilder::addNormal(const vec3&n) {
    m_Normals.push_back(n);
}

MeshBuilder::Face & MeshBuilder::addFace(unsigned int v1, unsigned int v2, unsigned int v3) {
    m_Faces.push_back(Face(v1, v2, v3));
    return m_Faces.back();
}

void MeshBuilder::centerAndNormalizeMesh() {
    const MinMax minMax = for_each(m_Vertices.begin(), m_Vertices.end(), MinMax());
    const vec3 size(minMax.max - minMax.min);
    vec3 center = (minMax.max + minMax.min);
    center /= 2;
    const float maxDimension = std::max(size.x, std::max(size.y, size.z));
    const float scale = maxDimension == 0 ? 1 : 1 / maxDimension;
    for_each(m_Vertices.begin(), m_Vertices.end(), MoveAndScale(-center, scale));
}

void MeshBuilder::normalizeNormals() {
    //for_each(m_Normals.begin(), m_Normals.end(), mem_fun_ref(&glm::normalize));
    for(int i = 0; m_Normals.size(); ++i) m_Normals[i] = glm::normalize(m_Normals[i]);
}

const vector<vec3>& MeshBuilder::vertices() const {
    return m_Vertices;
}

const vector<UV>& MeshBuilder::uvs() const {
    return m_Uvs;
}

const vector<vec3>& MeshBuilder::normals() const {
    return m_Normals;
}

void MeshBuilder::unpack( //
    vector<unsigned int> &indices, //
    vector<vec3>& vertices, //
    vector<vec3>& normals, //
    vector<UV>& uvs) const {

    const size_t elements = m_Faces.size() * 3;

    indices.clear();
    vertices.clear();
    normals.clear();
    uvs.clear();

    indices.reserve(elements);
    vertices.reserve(elements);
    normals.reserve(elements);
    uvs.reserve(elements);

    size_t i = 0;
    for (vector<Face>::const_iterator itr = m_Faces.begin(); itr != m_Faces.end(); ++itr) {
        indices.push_back(i++);
        indices.push_back(i++);
        indices.push_back(i++);
        vertices.push_back(m_Vertices[itr->v1]);
        vertices.push_back(m_Vertices[itr->v2]);
        vertices.push_back(m_Vertices[itr->v3]);
        if (itr->n1 != Face::NA) {
            normals.push_back(m_Normals[itr->n1]);
            normals.push_back(m_Normals[itr->n2]);
            normals.push_back(m_Normals[itr->n3]);
        }
        if (itr->t1 != Face::NA) {
            uvs.push_back(m_Uvs[itr->t1]);
            uvs.push_back(m_Uvs[itr->t2]);
            uvs.push_back(m_Uvs[itr->t3]);
        }
    }
}

} // namespace stein
