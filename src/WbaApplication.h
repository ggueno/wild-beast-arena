#ifndef WBAAPPLICATION_H_
#define WBASAPPLICATION_H_

#include "../lib/stein/Application.hpp"
#include "../lib/glm/gtx/vector_angle.hpp"

#include "Simulation.hpp"

class WbaApplication : public stein::Application {
public:
    WbaApplication();
    virtual void animate();
    virtual void handleEvent(const SDL_Event& event);
private:
    //BoidSimulation m_Simulation;
    bool package;
    GLuint background;
    PhysicSimulation m_Simulation;
    float m_arena_width;
    float m_arena_height;
    glm::vec3 m_arena_decalage;


    GLint uv_shader;
    GLint color_shader;
    GLint beast_texture;
    GLint arena_texture;
    GLint arena_spec_texture;
    GLint wall_texture;
    GLint wall_spec_texture;
    GLint hp_spec_texture;
    GLint hp_texture;
};

#endif /* WBASAPPLICATION_H_ */
