#ifndef included_Vertex
#define included_Vertex
#pragma once

//-------------------------------------------------------------
#include "MyMathFiles.hpp"

//-------------------------------------------------------------

/*
	x y z r g b a u v
*/

//-------------------------------------------------------------
class Vertex
{
public:
	Vertex() {
		//vertexPosition = Vector3(0.f, 0.f, 0.f);
		side = 0;
	};

	Vertex( const Vector3& InitialPosition, unsigned char Side = 0 ) {
		//vertexPosition = InitialPosition;
		SetPosBytesFromFloats( InitialPosition.x, InitialPosition.y, InitialPosition.z);
		side = Side;
	}

	Vertex( unsigned char x, unsigned char y, unsigned char z, unsigned char Side ) {
		positionBytes[0] = x;
		positionBytes[1] = y;
		positionBytes[2] = z;
		side = Side;
	}

	void SetPosBytesFromFloats( float x, float y, float z) {
		x = ClampFloat( x, 0.f, 255.f );
		y = ClampFloat( y, 0.f, 255.f );
		z = ClampFloat( z, 0.f, 255.f );

		positionBytes[0] = (unsigned char) x;
		positionBytes[1] = (unsigned char) y;
		positionBytes[2] = (unsigned char) z;
		
	}

	//Vector3 vertexPosition;
	unsigned char positionBytes[3];  //x, y, z
	unsigned char side;
};

#endif //included_Vertex