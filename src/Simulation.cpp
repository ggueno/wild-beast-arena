#include "Simulation.hpp"

mmGrid::key_type fromPosToGrid( const glm::vec2& pos ){
    float tmp = ( pos.x < -1.0/(float)gridWidth  )? 0 : 1;
    tmp += (pos.x < 1.0/(float)gridWidth )?0:1;
    tmp += ( pos.y < -1.0/(float)gridHeight )? 0:gridWidth;
    tmp += ( pos.y < 1.0/(float)gridHeight )?0:gridWidth;
    mmGrid::key_type index = (mmGrid::key_type)( tmp );
    //index += (mmGrid::key_type)( (pos.x+1)*10 );
    return index;
}

float rand_FloatRange(float a, float b)
{
    return ((b-a)*((float)rand()/RAND_MAX))+a;
}