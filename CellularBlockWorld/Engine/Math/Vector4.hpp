#ifndef included_Vector4
#define included_Vector4
#pragma once

//-------------------------------------------------------------
class Vector4
{
public:
	Vector4(){};
	Vector4(float InitialX, float InitialY, float InitialZ, float InitialW) {
		x = InitialX;
		y = InitialY;
		z = InitialZ;
		w = InitialW;
	};
	float x;
	float y;
	float z;
	float w;
};

#endif //included_Vector4