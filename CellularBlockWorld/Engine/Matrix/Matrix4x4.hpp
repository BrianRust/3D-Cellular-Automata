#ifndef included_Matrix4x4
#define included_Matrix4x4
#pragma once

//-------------------------------------------------------------

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
class Matrix4x4
{
public:
	Matrix4x4()
	{
		for(int counter = 0; counter < 16; counter++)
		{
			m_Matrix[counter] = 0.f;
		}
		m_Matrix[0] = 1.f;
		m_Matrix[5] = 1.f;
		m_Matrix[10] = 1.f;
		m_Matrix[15] = 1.f;
	};

	float m_Matrix[16];
};

#endif //included_Matrix4x4