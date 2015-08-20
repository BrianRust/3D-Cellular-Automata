#ifndef included_Vertex
#define included_Vertex
#pragma once

//-------------------------------------------------------------
#include "../Math/MyMathFiles.hpp"

//-------------------------------------------------------------

/*
	x y z r g b a u v
*/

//-------------------------------------------------------------
class Vertex
{
public:
	Vertex()
	{
		vertexPosition = Vector3(0.f, 0.f, 0.f);
		color = RGBA(0.f, 0.f, 0.f, 1.f);
		texCoordinates = Vector2(0.f, 0.f);
	};
	Vertex(const Vector3& InitialPosition, const RGBA& InitialColor, const Vector2& InitialUVCoordinates)
	{
		vertexPosition = InitialPosition;
		color = InitialColor;
		texCoordinates = InitialUVCoordinates;
	}
	Vector3 vertexPosition;
	RGBA color;
	Vector2 texCoordinates;
};

#endif //included_Vertex