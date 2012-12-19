#include <iostream>
#include <vector>
#include <cmath>
#include <SDL/SDL.h>
#include <cstdlib>

#include "api/Particle.hpp"
#include "api/GravitySpring.hpp"
#include "api/LeapFrogSolver.hpp"
#include "api/Polygon.hpp"
#include "api/Box.hpp"
#include "api/PolygonSpring.hpp"
#include "api/HookSpring.hpp"
#include "api/CineticBrake.hpp"
#include "api/Polygon.hpp"
#include "api/Attraction.hpp"
#include "renderer/GLRenderer.hpp"

#include "api/Beast.hpp"
#include "api/BeastSpring.hpp"
#include "api/HealthPack.hpp"
#include <map>
//#include "SDL_ttf.h"
using namespace std;

static const size_t WINDOW_WIDTH = 512;
static const size_t WINDOW_HEIGHT = 512;
static const size_t BYTES_PER_PIXEL = 32;
static const char* WINDOW_TITLE = "Wild Beast Arena";

////////////////////////////////////////////////////////////////
/*    TODO  FLAG BEAST  */

typedef std::multimap<short unsigned int, size_t> mmGrid ;
static const int gridWidth = 5;
static const int gridHeight = 5;

mmGrid::key_type fromPosToGrid( const glm::vec2& pos ){
    float tmp = ( pos.x < -1.0/(float)gridWidth  )? 0 : 1;
    tmp += (pos.x < 1.0/(float)gridWidth )?0:1;
    tmp += ( pos.y < -1.0/(float)gridHeight )? 0:gridWidth;
    tmp += ( pos.y < 1.0/(float)gridHeight )?0:gridWidth;
    mmGrid::key_type index = (mmGrid::key_type)( tmp );

    return index;
}

////////////////////////////////////////////////////////////////////

float rand_FloatRange(float a, float b)
{
    return ((b-a)*((float)rand()/RAND_MAX))+a;
}

bool mouseEvent( const SDL_Event& event, float* posX, float* posY){

    if( event.button.button == SDL_BUTTON_LEFT ){
        *posX = event.button.x / (float)WINDOW_WIDTH ;
        *posX = *posX*2 - 1;
        *posY = event.button.y / (float)WINDOW_HEIGHT;
        *posY = *posY*2 - 1;

        return true;
    }
    return false;
    
}


int main(int argc, char** argv) {
    // Initialisation de la SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BYTES_PER_PIXEL, SDL_OPENGL);
    SDL_WM_SetCaption(WINDOW_TITLE, 0);


    /* TTF
    SDL_Surface* txtBackground = NULL ;
    SDL_Surface* txtMessage = NULL ;
    SDL_Surface* txtScreen = NULL ;
    TTF_Font* font;
    SDL_Color txtColor = { 255, 255, 255 };
    //*/
    

    // Creation du renderer
    GLRenderer renderer;

    /** Placez votre code d'initialisation de la simulation ici **/
    static const float dt = 0.0001; // Pas de simulation, choix arbitraire mais dois rester petit pour que le systeme n'explose pas
    LeapFrogSolver solver;
    GravitySpring gravity(0.981);
    HookSpring hook(10.0, 1.0);;
    CineticBrake cb(0.000001, dt);
    Attraction attr(0.001);


    // BEAST
    mmGrid grid;
    mmGrid::const_iterator gridIt;
    mmGrid::const_iterator gridIt2;
    BeastSpring wba( 60, 0.05, dt );
    std::vector<HealthPack> hpList ; 


    // Code d'exemple pour le rendu: des particles placées en cercle
    std::vector<Beast> particles(50);
    float delta = 2 * 3.14 / particles.size(); // 2pi / nombre de particules
    for(size_t i = 0; i < particles.size(); ++i) {
        float c = cos(i * delta), s = sin(i * delta);
        particles[i].position = glm::vec2(0.8*c,0.8*s);//glm::vec2(rand_FloatRange(-0.8,0.8) , rand_FloatRange(-0.8,0.8));
        //particles[i].color = glm::vec3(1.,1.,1.);
        particles[i].mass = 1.f;
    }

    Polygon poly( glm::vec3(1.0, 0.8, 0.0), false );
    renderer.addPolygon( &poly.m_Vertices[0], poly.m_Vertices.size(), poly.color );

    ///////////////////////////////////////////////////////////////////////// LAST MODIFIED
    std::vector<Polygon> obstacles;
    obstacles.push_back( Polygon(glm::vec3(1, 0, 0), true) ); // polygone interne
    Box::build( glm::vec2(-0.9, -0.9), 1.8, 1.8, obstacles.back() );

    // Ici ajout d'autres obstacles eventuels
        // polygone irrégulier
    // Polygon irr( glm::vec3(1, 0.8, 0.2), true );
    // irr.addVertex( glm::vec2(-1, -1) );
    // irr.addVertex( glm::vec2(1, -0.8) );
    // irr.addVertex( glm::vec2(0.8, 0.7) );
    // irr.addVertex( glm::vec2(-0.6, 1) );
    // obstacles.push_back( irr );
    // glm::vec2 irrGCenter = irr.getGCenter();

    // Ajoute des obstacles au renderer
    for( size_t i = 0; i < obstacles.size(); ++i ){
	    renderer.addPolygon(&obstacles[i].getVertices()[0], obstacles[i].size(), obstacles[i].color);
    }

    float wallElasticity = 1.4; // Les particules vont tomber après quelques rebonds
    // Creation des ressort de collision
    std::vector<PolygonSpring> polygonSprings;
    for( size_t i = 0; i < obstacles.size(); ++i )
        polygonSprings.push_back(PolygonSpring(obstacles[i], wallElasticity, solver, dt));

    

    /////////////////////////////////////////////////////////////////////////

    bool done = false;
    while(!done) {
        // Rendu
        renderer.render(&particles[0], particles.size());
        SDL_GL_SwapBuffers();

        grid.clear();// BEAST

        /** Placez votre code de simulation ici */
        for( size_t i = 0; i < particles.size(); ++i ){
            grid.insert( std::pair<short unsigned int, size_t>( fromPosToGrid(particles[i].position), i ) ); // BEAST construction de la multimap
            //std::cout << fromPosToGrid( particles[i].position ) << std::endl;
        }

        // BEAST pour chaque zone de la multimap
        for( short unsigned int i=0 ; i < gridWidth*gridHeight ; ++i ){
            for( gridIt = grid.equal_range(i).first ; gridIt != grid.equal_range(i).second ; ++gridIt ){
                gridIt2 = gridIt;
                ++gridIt2;
                while( gridIt2 != grid.equal_range(i).second ){
                    wba.generateForces( &particles[gridIt->second], &particles[gridIt2->second] );
                    //cb.generateForces( &particles[gridIt->second], &particles[gridIt2->second] );
                    ++ gridIt2;
                }
                for( size_t i=0 ; i < hpList.size() ; ++i ){
                    attr.generateForces( &particles[gridIt->second], hpList[i].getGCenter(), 200 ); // Health packs / particules attraction
                    if( glm::distance( particles[gridIt->second].position, hpList[i].getGCenter() ) < 0.08 ){
                        // If close enough, particle picks up the health pack
                        particles[gridIt->second].heal();
                        // Remove health pack
                        renderer.removePolygon( hpList[i].index );
                        hpList.erase( hpList.begin()+i );
                        --i;
                    }
                }
            }
        }

        // Après l'application des autres forces (gravité seulement pour l'instant):
        for(size_t i = 0; i < particles.size(); ++i){            // Pour chaque particule
            for(size_t j = 0; j < polygonSprings.size(); ++j) { // Pour chaque ressort de collision
                polygonSprings[j].generateForces(&particles[i], 0); // Test de collision et génération des forces
            }
        }

        // Résolution pour chaque particule:
        for(size_t i = 0; i < particles.size(); ++i){
            solver.solve( particles[i], dt );
        }

        //if(particles.size()>0) std::cout << particles.size() << std::endl;

        for(size_t i = 0; i < particles.size(); ++i){            // Pour chaque particule
            if(!particles[i].isAlive()) particles.erase(particles.begin()+i);

        }


        SDL_Event e;
        while( SDL_PollEvent(&e) ) {
            switch( e.type ) {
                case SDL_QUIT:  done = true;
                                break;
                case SDL_MOUSEBUTTONUP: float hpX, hpY;
                                        if( mouseEvent(e,&hpX, &hpY) ){
                                            hpList.push_back( HealthPack( hpX, -hpY ) );
                                            hpList.back().index = renderer.addPolygon(&hpList.back().m_Vertices[0], hpList.back().m_Vertices.size(), hpList.back().color );
                                        }
                                        break;
                default:break;
            }

            /** Placez votre gestion des évenements ici */
        }
    }

    SDL_Quit();

    return 0;
}


