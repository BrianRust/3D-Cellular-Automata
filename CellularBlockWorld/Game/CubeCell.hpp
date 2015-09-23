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
		m_isBlack = false;
	};
	CubeCell(bool isSolid)
	{
		m_isSolid = isSolid;
		m_isBlack = false;
	};
	CubeCell(bool isSolid, bool isBlack)
	{
		m_isSolid = isSolid;
		m_isBlack = isBlack;
	};

	bool m_isSolid;
	bool m_isBlack;
};

#endif //included_Vertex