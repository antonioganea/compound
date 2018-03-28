#include "Geometry.h"

#define _USE_MATH_DEFINES
#include <math.h>

float Geometry::getPointAngle( const float& x1, const float& y1, const float& x2, const float& y2 )
{
    float angle = atan2( y1- y2, x1 - x2 );
    angle = angle * 180.f / M_PI;
    return angle;
}
