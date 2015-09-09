#ifndef included_Vertex
#define included_Vertex
#pragma once

//-------------------------------------------------------------
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "RGBA.hpp"

//-------------------------------------------------------------

/*
	x y z r g b a u v
*/

//-------------------------------------------------------------
class Vertex
{
public:
	Vertex() {
		vertexPosition = Vector3(0.f, 0.f, 0.f);
		side = 0;
	};

	Vertex(const Vector3& InitialPosition) {
		vertexPosition = InitialPosition;
		side = 0;
	}

	Vertex(const Vector3& InitialPosition, unsigned char Side) {
		vertexPosition = InitialPosition;
		side = Side;
	}
	Vector3 vertexPosition;
	unsigned char side;
};

#endif //included_Vertex