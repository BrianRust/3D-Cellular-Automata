#ifndef included_Vector2
#define included_Vector2
#pragma once

#include <math.h>

//-------------------------------------------------------------
class Vector2
{
public:
	Vector2(){};
	Vector2(float InitialX, float InitialY) {
		x = InitialX;
		y = InitialY;
	};
	float x;
	float y;
};

inline Vector2 Normalize(const Vector2& first)
{
	Vector2 result;
	float Magnitude = sqrt( (first.x * first.x) + (first.y * first.y) );

	if (Magnitude == 0)
	{
		result.x = 0;
		result.y = 0;
		return result;
	}

	result.x = first.x / Magnitude;
	result.y = first.y / Magnitude;

	return result;
};

inline Vector2 operator - (const Vector2& first, const Vector2& second)
{
	Vector2 result;
	result.x = first.x - second.x;
	result.y = first.y - second.y;
	return result;
};

inline Vector2 operator - (const Vector2& first, float second)
{
	Vector2 result;
	result.x = first.x - second;
	result.y = first.y - second;
	return result;
};

inline Vector2 operator + (const Vector2& first, const Vector2& second)
{
	Vector2 result;
	result.x = first.x + second.x;
	result.y = first.y + second.y;
	return result;
};

inline Vector2 operator + (const Vector2& first, float second)
{
	Vector2 result;
	result.x = first.x + second;
	result.y = first.y + second;
	return result;
};

inline Vector2 operator * (const Vector2& first, float second)
{
	Vector2 result;
	result.x = first.x * second;
	result.y = first.y * second;
	return result;
};

inline Vector2 operator / (const Vector2& first, float second)
{
	Vector2 result;
	result.x = first.x / second;
	result.y = first.y / second;
	return result;
};

inline bool operator == (const Vector2& first, const Vector2& second)
{
	if ( first.x == second.x && first.y == second.y )
	{
		return true;
	}
	else
	{
		return false;
	}
};

#endif //included_Vector2