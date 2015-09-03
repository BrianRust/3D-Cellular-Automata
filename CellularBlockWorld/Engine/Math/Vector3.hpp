#ifndef included_Vector3
#define included_Vector3
#pragma once

#include <math.h>

//-------------------------------------------------------------
class Vector3
{
public:
	Vector3(){};
	Vector3(float InitialX, float InitialY, float InitialZ) {
		x = InitialX;
		y = InitialY;
		z = InitialZ;
	};
	float x;
	float y;
	float z;
};

inline Vector3 Normalize(const Vector3& first)
{
	Vector3 result;
	float Magnitude = sqrt( (first.x * first.x) + (first.y * first.y) + (first.z * first.z));
	result.x = first.x / Magnitude;
	result.y = first.y / Magnitude;
	result.z = first.z / Magnitude;

	return result;
};

inline Vector3 operator - (const Vector3& first, const Vector3& second)
{
	Vector3 result;
	result.x = first.x - second.x;
	result.y = first.y - second.y;
	result.z = first.z - second.z;
	return result;
};

inline Vector3 operator * (const Vector3& first, float constantValue)
{
	Vector3 result;
	result.x = first.x * constantValue;
	result.y = first.y * constantValue;
	result.z = first.z * constantValue;
	return result;
};

inline float DotProduct(const Vector3& first, const Vector3& second)
{
	float result;
	result = (first.x * second.x) + (first.y * second.y) + (first.z * second.z);
	return result;
}

inline Vector3 CrossProduct(const Vector3& first, const Vector3& second)
{
	Vector3 result;
	result.x = (first.y * second.z) - (first.z * second.y);
	result.y = (first.z * second.x) - (first.x * second.z);
	result.z = (first.x * second.y) - (first.y * second.x);
	return result;
}

#endif //included_Vector3