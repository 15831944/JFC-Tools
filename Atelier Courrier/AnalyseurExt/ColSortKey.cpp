#include "StdAfx.h"
#include ".\colsortkey.h"

CColSortKey::CColSortKey(void)
{
	m_Question = 0;
	m_Value	= 0.0;
	m_Row	= 0;
}

CColSortKey::~CColSortKey(void)
{
}

bool CColSortKey::operator < (const CColSortKey &rhs) const
{
	if(m_Question == rhs.m_Question)
	{
		if(m_Value == rhs.m_Value)
			return (m_Row < rhs.m_Row);
		else
			return (m_Value < rhs.m_Value);
	}
	else
		return m_Question < rhs.m_Question;
}

bool CColSortKey::operator > (const CColSortKey &rhs) const
{
	if(m_Question == rhs.m_Question)
	{
		if(m_Value == rhs.m_Value)
			return (m_Row > rhs.m_Row);
		else
			return (m_Value > rhs.m_Value);
	}
	else
		return m_Question > rhs.m_Question;
}

CTableSortKey::CTableSortKey(void)
{
	m_Page		=	0;
	m_Row		=	0;
	m_Col		=	0;
	m_Value		=	0.0;
	m_Universe	=	0;
}

CTableSortKey::CTableSortKey(const CTableSortKey &rhs)
{
	*this = rhs;
}

CTableSortKey & CTableSortKey::operator = (const CTableSortKey &rhs)
{
	m_Page		=	rhs.m_Page;
	m_Row		=	rhs.m_Row;
	m_Col		=	rhs.m_Col;
	m_Value		=	rhs.m_Value;
	m_Universe	=	rhs.m_Universe; 
	return *this;
}

CTableSortKey::~CTableSortKey(void)
{
}

bool CTableSortKey::operator < (const CTableSortKey &rhs) const
{
	if(m_Value == rhs.m_Value)
		if(m_Universe == rhs.m_Universe)
			if(m_Page == rhs.m_Page)
				if(m_Row == rhs.m_Row)
					return (m_Col < rhs.m_Col);
				else
					return (m_Row < rhs.m_Row);
			else
				return (m_Page < rhs.m_Page);
		else
			return (m_Universe < rhs.m_Universe);
	else
		return (m_Value < rhs.m_Value);
}

bool CTableSortKey::operator > (const CTableSortKey &rhs) const
{
	if(m_Value == rhs.m_Value)
		if(m_Universe == rhs.m_Universe)
			if(m_Page == rhs.m_Page)
				if(m_Row == rhs.m_Row)
					return (m_Col > rhs.m_Col);
				else
					return (m_Row > rhs.m_Row);
			else
				return (m_Page > rhs.m_Page);
		else
			return (m_Universe > rhs.m_Universe);
	else
		return (m_Value > rhs.m_Value);
}


CTableKey::CTableKey(void)
{
	m_Universe	=	0;
	m_Page		=	0;
	m_Row		=	0;
	m_Col		=	0;
}

CTableKey::CTableKey(const CTableKey &rhs)
{
	*this = rhs;
}

CTableKey::CTableKey(const CTableSortKey &rhs)
{
	*this = rhs;
}

CTableKey & CTableKey::operator = (const CTableKey &rhs)
{
	m_Universe	=	rhs.m_Universe;
	m_Page		=	rhs.m_Page;
	m_Row		=	rhs.m_Row;
	m_Col		=	rhs.m_Col;
	return *this;
}

CTableKey & CTableKey::operator = (const CTableSortKey &rhs)
{
	m_Universe	=	rhs.m_Universe;
	m_Page		=	rhs.m_Page;
	m_Row		=	rhs.m_Row;
	m_Col		=	rhs.m_Col;
	return *this;
}
