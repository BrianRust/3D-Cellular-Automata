#ifndef included_MyMathFiles
#define included_MyMathFiles
#pragma once

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
//#include "Vertex.hpp"
#include "IntVector2.hpp"
#include "IntVector3.hpp"
#include "RGBA.hpp"

float ClampFloat( float value, float min, float max );

inline float ClampFloat( float value, float min, float max ) {
	if ( value < min ) {
		return min;
	}

	if ( value > max ) {
		return max;
	}

	return value;
}



#endif //included_MyMathFiles