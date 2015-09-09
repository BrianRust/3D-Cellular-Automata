#include "MatrixStack.hpp"

//---------------------------------------
MatrixStack::MatrixStack()
{
	PushMatrix();
}

//---------------------------------------
void MatrixStack::PushMatrix()
{
	m_MatrixStack.push_back(Matrix4x4());
	topOfStackIndex = m_MatrixStack.size()-1;
	SetToIdentity();
}

//---------------------------------------
void MatrixStack::PopMatrix()
{
	if(m_MatrixStack.size() > 0)
	{
		m_MatrixStack.pop_back();
		topOfStackIndex = m_MatrixStack.size()-1;
	}
	
}

//---------------------------------------
void MatrixStack::SetToPerspectiveMatrix( float fovy, float aspect, float zNear, float zFar )
{
	m_MatrixStack[topOfStackIndex].m_Matrix[0] = fovy/aspect;
	m_MatrixStack[topOfStackIndex].m_Matrix[1] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[2] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[3] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[4] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[5] = fovy;
	m_MatrixStack[topOfStackIndex].m_Matrix[6] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[7] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[8] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[9] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[10] = (zFar + zNear)/(zNear - zFar);
	m_MatrixStack[topOfStackIndex].m_Matrix[11] = -1.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[12] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[13] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[14] = (2.f * zFar * zNear)/(zNear - zFar);
	m_MatrixStack[topOfStackIndex].m_Matrix[15] = 0.0f;
}

//---------------------------------------
void MatrixStack::SetToOrthoMatrix(float left, float right, float bottom, float top, float nearVal, float farVal)
{
	m_MatrixStack[topOfStackIndex].m_Matrix[0] = 2.0f / (right - left);
	m_MatrixStack[topOfStackIndex].m_Matrix[1] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[2] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[3] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[4] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[5] = 2.0f / (top - bottom);
	m_MatrixStack[topOfStackIndex].m_Matrix[6] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[7] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[8] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[9] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[10] = -2.0f / (farVal - nearVal);
	m_MatrixStack[topOfStackIndex].m_Matrix[11] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[12] = -(right + left) / (right - left);
	m_MatrixStack[topOfStackIndex].m_Matrix[13] = -(top + bottom) / (top - bottom);
	m_MatrixStack[topOfStackIndex].m_Matrix[14] = -(farVal + nearVal) / (farVal - nearVal);
	m_MatrixStack[topOfStackIndex].m_Matrix[15] = 1.0f;
}

//---------------------------------------
void MatrixStack::SetToIdentity()
{
	for(int counter = 0; counter < 16; counter++)
	{
		m_MatrixStack[topOfStackIndex].m_Matrix[counter] = 0.f;
	}
	m_MatrixStack[topOfStackIndex].m_Matrix[0] = 1.f;
	m_MatrixStack[topOfStackIndex].m_Matrix[5] = 1.f;
	m_MatrixStack[topOfStackIndex].m_Matrix[10] = 1.f;
	m_MatrixStack[topOfStackIndex].m_Matrix[15] = 1.f;
}

//---------------------------------------
void MatrixStack::SetToTranspose()
{
	std::swap(m_MatrixStack[topOfStackIndex].m_Matrix[1],m_MatrixStack[topOfStackIndex].m_Matrix[4]);
	std::swap(m_MatrixStack[topOfStackIndex].m_Matrix[2],m_MatrixStack[topOfStackIndex].m_Matrix[8]);
	std::swap(m_MatrixStack[topOfStackIndex].m_Matrix[3],m_MatrixStack[topOfStackIndex].m_Matrix[12]);
	std::swap(m_MatrixStack[topOfStackIndex].m_Matrix[6],m_MatrixStack[topOfStackIndex].m_Matrix[9]);
	std::swap(m_MatrixStack[topOfStackIndex].m_Matrix[7],m_MatrixStack[topOfStackIndex].m_Matrix[13]);
	std::swap(m_MatrixStack[topOfStackIndex].m_Matrix[11],m_MatrixStack[topOfStackIndex].m_Matrix[14]);
}

//---------------------------------------
void MatrixStack::SetToRotationMatrixX(float angle)
{
	m_MatrixStack[topOfStackIndex].m_Matrix[0] = 1.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[1] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[2] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[3] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[4] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[5] = cos(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[6] = sin(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[7] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[8] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[9] = -sin(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[10] = cos(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[11] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[12] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[13] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[14] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[15] = 1.0f;
}

//---------------------------------------
void MatrixStack::SetToRotationMatrixY(float angle)
{
	m_MatrixStack[topOfStackIndex].m_Matrix[0] =  cos(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[1] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[2] = -sin(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[3] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[4] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[5] = 1.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[6] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[7] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[8] = sin(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[9] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[10] = cos(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[11] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[12] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[13] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[14] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[15] = 1.0f;
}

//---------------------------------------
void MatrixStack::SetToRotationMatrixZ(float angle)
{
	m_MatrixStack[topOfStackIndex].m_Matrix[0] =  cos(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[1] = sin(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[2] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[3] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[4] = -sin(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[5] = cos(angle);
	m_MatrixStack[topOfStackIndex].m_Matrix[6] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[7] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[8] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[9] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[10] = 1.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[11] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[12] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[13] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[14] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[15] = 1.0f;
}

//---------------------------------------
void MatrixStack::SetToTranslationMatrix(float x, float y, float z)
{
	m_MatrixStack[topOfStackIndex].m_Matrix[0] = 1.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[1] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[2] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[3] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[4] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[5] = 1.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[6] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[7] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[8] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[9] = 0.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[10] = 1.0f;
	m_MatrixStack[topOfStackIndex].m_Matrix[11] = 0.0f;

	m_MatrixStack[topOfStackIndex].m_Matrix[12] = x;
	m_MatrixStack[topOfStackIndex].m_Matrix[13] = y;
	m_MatrixStack[topOfStackIndex].m_Matrix[14] = z;
	m_MatrixStack[topOfStackIndex].m_Matrix[15] = 1.0f;
}