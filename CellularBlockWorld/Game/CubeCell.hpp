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
		m_cellType = 0;
		//m_isBlack = false;
	};
	CubeCell(bool isSolid)
	{
		m_isSolid = isSolid;
		m_cellType = 0;
		//m_isBlack = false;
	};
	CubeCell(bool isSolid, char cellType)
	{
		m_isSolid = isSolid;
		m_cellType = cellType;
		//m_isBlack = isBlack;
	};

	bool m_isSolid;
	//bool m_isBlack;
	char m_cellType;
};

#endif //included_CubeCell