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
		color = RGBA(0.f, 0.f, 0.f, 1.f);
		normal = Vector3(1.f, 0.f, 0.f);
//		texCoordinates = Vector2(0.f, 0.f);
	};
	Vertex(const Vector3& InitialPosition, const RGBA& InitialColor, const Vector2& InitialUVCoordinates) {
		vertexPosition = InitialPosition;
		color = InitialColor;
//		texCoordinates = InitialUVCoordinates;
		normal = Vector3(1.f, 0.f, 0.f);
	}

	Vertex(const Vector3& InitialPosition, const RGBA& InitialColor, const Vector2& InitialUVCoordinates, const Vector3& initialNormal) {
		vertexPosition = InitialPosition;
		color = InitialColor;
//		texCoordinates = InitialUVCoordinates;
		normal = initialNormal;
	}
	Vector3 vertexPosition;
	RGBA color;
//	Vector2 texCoordinates;
	Vector3 normal;
};

#endif //included_Vertex