#include "WbaApplication.h"

#include <lib/stein/Object.hpp>
#include <lib/stein/Tools.hpp>
#include <lib/stein/Builders.hpp>
#include <lib/math/StreamUtils.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace stein;
using namespace glm;

static inline float frand() {
    return float(rand()) / RAND_MAX;
}



float dt = 0.001; // Pas de simulation, choix arbitraire mais dois rester petit pour que le systeme n'explose pas
float gravity(0.981);
float hook1 = 10.0;
float hook2 = 1.0;
float cb1(0.000001);
float attr(0.001);
float wba1(60);
float wba2(0.05);
float wallElasticity(1.4);

WbaApplication::WbaApplication() :
    Application(800, 800), package(false),
    m_Simulation(40,dt,gravity,hook1,hook2,cb1, attr, wba1, wba2, wallElasticity)
    {

    const float size = .04;
    const float near = 0.1;
    const float far = 100;

    //load shaders

    uv_shader = loadProgram("../shaders/uvShader.glsl");
    color_shader = loadProgram("../shaders/colorShader.glsl");
    beast_texture = loadTexture("../textures/beast.ppm");
    arena_texture = loadTexture("../textures/arena.ppm");
    arena_spec_texture = loadTexture("../textures/arena_spec.ppm");
    wall_texture = loadTexture("../textures/wall_3.ppm");
    wall_spec_texture = loadTexture("../textures/wall_3_spec.ppm");
    hp_texture = loadTexture("../textures/hp.ppm");
    hp_spec_texture = loadTexture("../textures/hp_spec.ppm");

    m_Scene.camera.setPerspectiveProjection(-size, size, -size, size, near, far);
    m_Scene.camera.setPosition(vec3(0, 0, 50));
    m_Scene.setDefaultShaderID(color_shader);

    //draw beast

    //draw the arena
    Object &beast = m_Scene.createObject(GL_TRIANGLES);
    buildSphere(beast);

    m_arena_width = getWidth()*size;
    m_arena_height = getHeight()*size;
    m_arena_decalage = vec3(-m_arena_width/4.0,0.0,-0.5);

    int element;

    vec3 color = vec3(1.0,1.0,1.0);

    std::cout << m_Simulation.getNbParticles() <<std::endl;
    for (size_t i = 0; i < m_Simulation.getNbParticles(); ++i) {
        element = m_Scene.addObjectToDraw(beast.id);
        // //TO DO : Get the right color vector
        m_Scene.setDrawnObjectShaderID(element, uv_shader);
        m_Scene.setDrawnObjectTextureID0(element, beast_texture);
        //m_Scene.setDrawnObjectTextureID1(element, loadTexture("../textures/beast.ppm"));

        vec3 position = m_Simulation.getParticlePos(i);

        m_Simulation.m_particles[i].index = element;
        color = m_Simulation.getParticleColor(i);
        m_Scene.setDrawnObjectColor(element, Color(color.x, color.y, color.z));
        m_Scene.setDrawnObjectModel(element,  translation(vec3(position.x*m_arena_width, position.y*m_arena_width,0.0)) * scalee(vec3(1.5,1.5,1.0)) );
    }


    //draw the arena
    Object &object = m_Scene.createObject(GL_TRIANGLES);
    buildSquare(object);

    background = m_Scene.addObjectToDraw(object.id);
    m_Scene.setDrawnObjectShaderID(background, uv_shader);
    m_Scene.setDrawnObjectTextureID0(background, arena_texture);
    m_Scene.setDrawnObjectTextureID1(background, arena_spec_texture);
    m_Scene.setDrawnObjectColor(element, Color(color.x, color.y, color.z));
    m_Scene.setDrawnObjectModel(background, translation(vec3(0.0,0.0,-0.5)) *scalee(vec3(m_arena_width, m_arena_height,1.0)));


    //create wall with polygons
    Object &wall = m_Scene.createObject(GL_TRIANGLES);
    buildCube(wall);
    const float wall_size = 2.0;
    const std::vector<Polygon> polygons = m_Simulation.getPolygons();
    for (size_t i = 1; i < polygons.size(); ++i) {
        const Polygon poly_temp =  polygons[i];

        for(size_t j=0; j<poly_temp.size(); ++j){
            vec2 a = poly_temp.getVertex(j);
            vec2 b;
            if(j+1!=poly_temp.size())
                b = poly_temp.getVertex(j+1);
            else
                b = poly_temp.getVertex(0);

            vec2 ab = normalize(vec2(b.x-a.x, b.y-a.y));
            vec2 ivect = vec2(1.0, 0.0);

            float angle = -1;
            angle = glm::orientedAngle(ab, ivect);
            if(angle==-1) angle = 0;

            float wall_width = ab.length();

            element = m_Scene.addObjectToDraw(wall.id);
            m_Scene.setDrawnObjectShaderID(element, uv_shader);
            m_Scene.setDrawnObjectTextureID0(element, wall_texture);
            m_Scene.setDrawnObjectTextureID1(element, wall_spec_texture);
            m_Scene.setDrawnObjectColor(element, Color(1.0f, 1.0f, 1.0f));
            m_Scene.setDrawnObjectModel(element, translation(vec3(ab.y*((m_arena_width+wall_size)/2), ab.x*m_arena_height/2.0+ab.x*wall_size/2.0, 0.0)) * zRotation(angle) * scalee(vec3(wall_width*m_arena_width/2.0, wall_size, 1.0)));
        }
    }
}


void WbaApplication::handleEvent(const SDL_Event& event) {
    Application::handleEvent(event);

    switch (event.type) {
        case SDL_MOUSEBUTTONUP:
            if(event.button.button == SDL_BUTTON_LEFT){
                package = true;
            }
            break;
        default:
            break;
    }
}


void WbaApplication::animate() {

    if(package){
        //m_Simulation.addPackage();
        Object &object = m_Scene.createObject(GL_TRIANGLES);
        buildCube(object);

        const float width = 5.0;
        const float height = 5.0;

        GLint x = getPressedMouseXPos();
        GLint y = getPressedMouseYPos();
        GLfloat z;

        glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);

        vec3 position = unProject(
                vec3(x,y,z),
                m_Scene.camera.getView() * m_Scene.getDrawnObjectModel(background),
                m_Scene.camera.getProjection(),
                getViewport()
            );


        GLuint element = m_Scene.addObjectToDraw(object.id);
        m_Simulation.addHealthPack(position.x*2,-position.y*2, element);

        position.x = position.x*m_arena_width;
        position.y = -position.y*m_arena_height;
        position.z = 1.0;

        //m_Scene.setDrawnObjectTextureID0(element, loadTexture("../textures/arena.ppm"));
        m_Scene.setDrawnObjectShaderID(element, uv_shader);
        m_Scene.setDrawnObjectTextureID0(element, hp_texture);
        m_Scene.setDrawnObjectTextureID1(element, hp_spec_texture);
        m_Scene.setDrawnObjectColor(element, Color(1.0f, 1.0f, 1.0f));
        m_Scene.setDrawnObjectModel(element, translation(position));

        package = false;
    }


    for (size_t i = 0; i < m_Simulation.getNbParticles(); ++i) {
        const Beast beast = m_Simulation.m_particles[i];
        vec3 pos = m_Simulation.getParticlePos(i);

        pos.x = pos.x*m_arena_width/2.0;
        pos.y = pos.y*m_arena_width/2.0;

        m_Scene.setDrawnObjectModel(beast.index, translation(pos));

        if(!beast.isAlive()){
            m_Simulation.deleteParticles(i);
            m_Scene.deleteDrawnObject(beast.index);
        }
    }

    for (size_t i = 0; i < m_Simulation.getNbHealthPack(); ++i) {
        HealthPack hp = m_Simulation.getHealthPack(i);

        if(!hp.isAlive()){
            m_Simulation.deleteHealthPack(i);
            m_Scene.deleteDrawnObject(hp.index);
        }
    }


    m_Simulation.update();
}
