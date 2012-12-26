// Builders.hpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#ifndef __BUILDERS_HPP__
#define __BUILDERS_HPP__

#include <cstdlib>

namespace stein {

class Object; //Forward declaration

// Building functions
void buildTriangle(Object &object);
void buildSquare(Object &object, const float side = 0.5);
bool buildObjectGeometryFromOBJ(Object &object, const char* fileName, bool smoothObject);
void buildSphere(Object &object, const float radius = 0.5, std::size_t discLat = 36, std::size_t discLong = 36);
void buildCube(Object &object, float side = 0.5);
} // namespace stein

#endif
