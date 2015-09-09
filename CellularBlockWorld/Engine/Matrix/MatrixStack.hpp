#ifndef included_MatrixStack
#define included_MatrixStack
#pragma once

//-------------------------------------------------------------
#include "../Math/Vector3.hpp"
#include "../Math/Vector4.hpp"
#include "Matrix4x4.hpp"
#include <utility>
#include <vector>

/*
ROW-major memory layout: from float[16] to matrix
0  1  2  3   |  0  1  2  3
4  5  6  7   |  4  5  6  7
8  9  10 11  |  8  9  10 11
12 13 14 15  |  12 13 14 15

Column-major:
0 4 8  12
1 5 9  13
2 6 10 14
3 7 11 15
*/

//-------------------------------------------------------------
class MatrixStack
{
public:
	MatrixStack();

	std::vector<Matrix4x4> m_MatrixStack;

	void PushMatrix();
	void PopMatrix();

	void SetToTranspose();

	void SetToIdentity();
	void SetToPerspectiveMatrix( float fovy, float aspect, float zNear, float zFar );
	void SetToOrthoMatrix(float left, float right, float bottom, float top, float nearVal, float farVal);
	void SetToTranslationMatrix(float x, float y, float z);
	void SetToRotationMatrixX(float angle);
	void SetToRotationMatrixY(float angle);
	void SetToRotationMatrixZ(float angle);

	int topOfStackIndex;
};

inline Matrix4x4 operator * (const Matrix4x4& first, const Matrix4x4& second)
{
	Matrix4x4 result;

	// 	Column-major:
	// 	0 4 8  12		0 4 8 12
	// 	1 5 9  13		1 5 9 13
	// 	2 6 10 14		2 6 10 14
	// 	3 7 11 15		3 7 11 15

	result.m_Matrix[0] = first.m_Matrix[0]*second.m_Matrix[0] + first.m_Matrix[4]*second.m_Matrix[1] + first.m_Matrix[8]*second.m_Matrix[2] + first.m_Matrix[12]*second.m_Matrix[3];
	result.m_Matrix[1] = first.m_Matrix[1]*second.m_Matrix[0] + first.m_Matrix[5]*second.m_Matrix[1] + first.m_Matrix[9]*second.m_Matrix[2] + first.m_Matrix[13]*second.m_Matrix[3];
	result.m_Matrix[2] = first.m_Matrix[2]*second.m_Matrix[0] + first.m_Matrix[6]*second.m_Matrix[1] + first.m_Matrix[10]*second.m_Matrix[2] + first.m_Matrix[14]*second.m_Matrix[3];
	result.m_Matrix[3] = first.m_Matrix[3]*second.m_Matrix[0] + first.m_Matrix[7]*second.m_Matrix[1] + first.m_Matrix[11]*second.m_Matrix[2] + first.m_Matrix[15]*second.m_Matrix[3];

	result.m_Matrix[4] = first.m_Matrix[0]*second.m_Matrix[4] + first.m_Matrix[4]*second.m_Matrix[5] + first.m_Matrix[8]*second.m_Matrix[6] + first.m_Matrix[12]*second.m_Matrix[7];
	result.m_Matrix[5] = first.m_Matrix[1]*second.m_Matrix[4] + first.m_Matrix[5]*second.m_Matrix[5] + first.m_Matrix[9]*second.m_Matrix[6] + first.m_Matrix[13]*second.m_Matrix[7];
	result.m_Matrix[6] = first.m_Matrix[2]*second.m_Matrix[4] + first.m_Matrix[6]*second.m_Matrix[5] + first.m_Matrix[10]*second.m_Matrix[6] + first.m_Matrix[14]*second.m_Matrix[7];
	result.m_Matrix[7] = first.m_Matrix[3]*second.m_Matrix[4] + first.m_Matrix[7]*second.m_Matrix[5] + first.m_Matrix[11]*second.m_Matrix[6] + first.m_Matrix[15]*second.m_Matrix[7];

	result.m_Matrix[8] = first.m_Matrix[0]*second.m_Matrix[8] + first.m_Matrix[4]*second.m_Matrix[9] + first.m_Matrix[8]*second.m_Matrix[10] + first.m_Matrix[12]*second.m_Matrix[11];
	result.m_Matrix[9] = first.m_Matrix[1]*second.m_Matrix[8] + first.m_Matrix[5]*second.m_Matrix[9] + first.m_Matrix[9]*second.m_Matrix[10] + first.m_Matrix[13]*second.m_Matrix[11];
	result.m_Matrix[10] = first.m_Matrix[2]*second.m_Matrix[8] + first.m_Matrix[6]*second.m_Matrix[9] + first.m_Matrix[10]*second.m_Matrix[10] + first.m_Matrix[14]*second.m_Matrix[11];
	result.m_Matrix[11] = first.m_Matrix[3]*second.m_Matrix[8] + first.m_Matrix[7]*second.m_Matrix[9] + first.m_Matrix[11]*second.m_Matrix[10] + first.m_Matrix[15]*second.m_Matrix[11];

	result.m_Matrix[12] = first.m_Matrix[0]*second.m_Matrix[12] + first.m_Matrix[4]*second.m_Matrix[13] + first.m_Matrix[8]*second.m_Matrix[14] + first.m_Matrix[12]*second.m_Matrix[15];
	result.m_Matrix[13] = first.m_Matrix[1]*second.m_Matrix[12] + first.m_Matrix[5]*second.m_Matrix[13] + first.m_Matrix[9]*second.m_Matrix[14] + first.m_Matrix[13]*second.m_Matrix[15];
	result.m_Matrix[14] = first.m_Matrix[2]*second.m_Matrix[12] + first.m_Matrix[6]*second.m_Matrix[13] + first.m_Matrix[10]*second.m_Matrix[14] + first.m_Matrix[14]*second.m_Matrix[15];
	result.m_Matrix[15] = first.m_Matrix[3]*second.m_Matrix[12] + first.m_Matrix[7]*second.m_Matrix[13] + first.m_Matrix[11]*second.m_Matrix[14] + first.m_Matrix[15]*second.m_Matrix[15];

	return result;
};

#endif //included_Matrix4x4