#ifndef included_RGBA
#define included_RGBA
#pragma once

//-------------------------------------------------------------
class RGBA
{
public:
	RGBA(){};
	RGBA(float InitialR, float InitialG, float InitialB, float InitialA) {
		r = InitialR;
		g = InitialG;
		b = InitialB;
		a = InitialA;
	};
	float r;
	float g;
	float b;
	float a;
};

inline RGBA operator * (const RGBA& first, float second)
{
	RGBA result;
	result.r = first.r * second;
	result.g = first.g * second;
	result.b = first.b * second;
	result.a = first.a * second;
	return result;
};

#endif //included_RGBA