#ifndef included_CubeCell
#define included_CubeCell
#pragma once

//-------------------------------------------------------------


//-------------------------------------------------------------
class CubeCell
{
public:
	CubeCell()
	{
		m_isSolid = false;
	};
	CubeCell(bool isSolid)
	{
		m_isSolid = isSolid;
	}
	bool m_isSolid;
};

#endif //included_Vertex