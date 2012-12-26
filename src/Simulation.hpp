#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <vector>
#include <cmath>
#include <SDL/SDL.h>
#include <cstdlib>
#include <iostream>
#include <map>

#include "physics/Particle.hpp"
#include "physics/GravitySpring.hpp"
#include "physics/LeapFrogSolver.hpp"
#include "physics/Polygon.hpp"
#include "physics/Box.hpp"
#include "physics/PolygonSpring.hpp"
#include "physics/HookSpring.hpp"
#include "physics/CineticBrake.hpp"
#include "physics/Polygon.hpp"
#include "physics/Attraction.hpp"

#include "physics/Beast.hpp"
#include "physics/BeastSpring.hpp"
#include "physics/HealthPack.hpp"

#include "../lib/glm/glm.hpp"

using namespace std;

typedef std::multimap<short unsigned int, size_t> mmGrid ;
static const int gridWidth = 5;
static const int gridHeight = 5;

mmGrid::key_type fromPosToGrid( const glm::vec2& pos );
float rand_FloatRange(float a, float b);


class PhysicSimulation{
public:
    PhysicSimulation(
        std::size_t particles, float dt, float gravity,
        float hook1, float hook2, float cb1, float attr,
        float wba1, float wba2, float wallElasticity):

        m_nbParticles(particles),
        m_dt(dt),
        m_solver(),
        m_gravity(gravity),
        m_hook(hook1,hook2),
        m_cb(cb1,dt),
        m_attr(attr),
        m_wba(wba1,wba2,dt),
        m_wallElasticity(wallElasticity)
        {
            initParticles();
            initPolygon();
        };


    void initParticles(){
        // initaliser le vecteur avec m_nbParticles
        m_particles.reserve(m_nbParticles);

        float delta = 2 * 3.14 / m_nbParticles; // 2pi / nombre de particules
        for(size_t i = 0; i < m_nbParticles; ++i) {
            float c = cos(i * delta), s = sin(i * delta);
            m_particles.push_back(Beast());
            //glm::vec2 pos = glm::vec2(rand_FloatRange(-0.8,0.8) , rand_FloatRange(-0.8,0.8));
            m_particles[i].position = glm::vec2(0.8*c,0.8*s);
            //m_particles[i].position = pos;
            //particles[i].color = glm::vec3(1.,1.,1.);
            m_particles[i].mass = 1.f;
        }
    }

    void initPolygon(){

         ///////////////////////////////////////////////////////////////////////// LAST MODIFIED
        m_polygons.push_back( Polygon(glm::vec3(1, 0, 0), true) ); // polygone interne
        Box::build( glm::vec2(-1.0, -1.0), 2.0, 2.0, m_polygons.back() );

         m_polygons.push_back(Polygon( glm::vec3(1.0, 0.8, 0.0), true ));
        Box::build( glm::vec2(-0.96, -0.96), 1.93, 1.93, m_polygons.back() );

        for( size_t i = 0; i < m_polygons.size(); ++i )
            m_polygonSprings.push_back(PolygonSpring(m_polygons[i], m_wallElasticity, m_solver, m_dt));
    }

    void update(){
        m_grid.clear();

        for( size_t i = 0; i < m_particles.size(); ++i ){
            m_grid.insert( std::pair<short unsigned int, size_t>( fromPosToGrid(m_particles[i].position), i ) ); // BEAST construction de la multimap
            //std::cout << fromPosToGrid( m_particles[i].position ) << std::endl;
        }

        // BEAST pour chaque zone de la multimap
        mmGrid::const_iterator gridIt;
        mmGrid::const_iterator gridIt2;
        // for( short unsigned int i=0 ; i < gridWidth*gridHeight ; ++i ){
        //     for( gridIt = m_grid.equal_range(i).first ; gridIt != m_grid.equal_range(i).second ; ++gridIt ){
        //         gridIt2 = gridIt;
        //         ++gridIt2;
        //         while(gridIt2 != m_grid.equal_range(i).second ){
        //             m_wba.generateForces( &m_particles[gridIt->second], &m_particles[gridIt2->second] );
        //             ++gridIt2;
        //         }
        //     }
        // }

               // BEAST pour chaque zone de la multimap
        for( short unsigned int i=0 ; i < gridWidth*gridHeight ; ++i ){
            for( gridIt = m_grid.equal_range(i).first ; gridIt != m_grid.equal_range(i).second ; ++gridIt ){
                gridIt2 = gridIt;
                ++gridIt2;
                while( gridIt2 != m_grid.equal_range(i).second ){
                    m_wba.generateForces( &m_particles[gridIt->second], &m_particles[gridIt2->second] );
                    //cb.generateForces( &particles[gridIt->second], &particles[gridIt2->second] );
                    ++ gridIt2;
                }
                for( size_t i=0 ; i < m_hpList.size() ; ++i ){
                    m_attr.generateForces( &m_particles[gridIt->second], m_hpList[i].getGCenter(), 1000 ); // Health packs / particules attraction
                    if(m_hpList[i].isAlive() && glm::distance( m_particles[gridIt->second].position, m_hpList[i].getGCenter() ) < 0.08 ){
                        // If close enough, particle picks up the health pack
                        m_particles[gridIt->second].heal();
                        // Remove health pack
                        m_hpList[i].kill();
                        //break;
                    }
                }
            }
        }

        // Après l'application des autres forces (gravité seulement pour l'instant):
        for(size_t i = 0; i < m_particles.size(); ++i){            // Pour chaque particule
            for(size_t j = 0; j < m_polygonSprings.size(); ++j) { // Pour chaque ressort de collision
                m_polygonSprings[j].generateForces(&m_particles[i], 0); // Test de collision et génération des forces
            }
        }

        // Résolution pour chaque particule:
        for(size_t i = 0; i < m_particles.size(); ++i){
            m_solver.solve( m_particles[i], m_dt );
        }

        //if(m_particles.size()>0) std::cout << m_particles.size() << std::endl;

        // for(size_t i = 0; i < m_particles.size(); ++i){            // Pour chaque particule
        //     if(!m_particles[i].isAlive()){
        //         m_particles.erase(m_particles.begin()+i);
        //     }
        // }
    }

    size_t getNbParticles(){ return m_particles.size(); };
    size_t getNbHealthPack(){ return m_hpList.size(); };
    glm::vec3 getParticleColor(int i){ return m_particles[i].color; }
    glm::vec3 getParticlePos(int i){ return glm::vec3(m_particles[i].position.x, m_particles[i].position.y  , 0.0); }

    HealthPack getHealthPack(size_t i){ return m_hpList[i]; };
    void addHealthPack(float x, float y, size_t index){
        m_hpList.push_back( HealthPack( x, y ) );
        m_hpList.back().index = index;
    }

    const std::vector<Polygon>& getPolygons() const { return m_polygons; }
    void deleteParticles(size_t i){ m_particles.erase(m_particles.begin()+i);};
    void deleteHealthPack(size_t i){ m_hpList.erase( m_hpList.begin()+i );};

    std::vector<Beast> m_particles;

private:
    float m_dt;
    LeapFrogSolver m_solver;
    GravitySpring m_gravity;
    HookSpring m_hook;
    CineticBrake m_cb;
    Attraction m_attr;
    BeastSpring m_wba;

    float m_wallElasticity;

    mmGrid m_grid;

    size_t m_nbParticles;

    std::vector<Polygon> m_polygons;
    std::vector<PolygonSpring> m_polygonSprings;
    std::vector<HealthPack> m_hpList;
};

#endif