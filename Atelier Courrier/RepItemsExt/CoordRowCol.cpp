#include "StdAfx.h"
#include ".\coordrowcol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCoordRowCol::CCoordRowCol(void)
{
	m_Page		= 0;
	m_Col		= 0;
	m_Row		= 0;
}

CCoordRowCol::~CCoordRowCol(void)
{
}

bool CCoordRowCol::operator < (const CCoordRowCol &rhs) const
{
	if(m_Page == rhs.m_Page)
	{
		if(m_Col == rhs.m_Col)
			return (m_Row < rhs.m_Row);
		else
			return (m_Col < rhs.m_Col);
	}
	else
		return (m_Page < rhs.m_Page);
}

bool CCoordRowCol::operator > (const CCoordRowCol &rhs) const
{
	if(m_Page == rhs.m_Page)
	{
		if(m_Col == rhs.m_Col)
			return (m_Row > rhs.m_Row);
		else
			return (m_Col > rhs.m_Col);
	}
	else
		return (m_Page > rhs.m_Page);
}
