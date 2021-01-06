#pragma once

#include "CoordRowCol.h"

class CColSortKey
{
public:
	CColSortKey(void);
	virtual ~CColSortKey(void);
	CColSortKey(int Question, double Value,int Row) { m_Question = Question; m_Value = Value; m_Row = Row;} ;
	CColSortKey(double Value,int Row) { m_Value = Value; m_Row = Row;} ;
	void SetValueRow(int Question, double Value, int Row) { m_Question = Question; m_Value = Value; m_Row = Row;} ;
	void SetValueRow(double Value, int Row) { m_Question = 0; m_Value = Value; m_Row = Row;} ;
	bool operator < (const CColSortKey &rhs) const;
	bool operator > (const CColSortKey &rhs) const;

	int		m_Question;
	double	m_Value;
	int		m_Row;
};

class CTableSortKey
{
public:
	virtual ~CTableSortKey(void);
	CTableSortKey(void);
	CTableSortKey(const CTableSortKey &rhs);
	CTableSortKey(double Value,int Universe, int Page, int Row, int Col) { m_Value = Value; m_Universe = Universe; m_Page = Page; m_Row = Row; m_Col = Col; } ;
	void SetValue(double Value,int Universe, int Page, int Row, int Col) { m_Value = Value; m_Universe = Universe; m_Page = Page; m_Row = Row; m_Col = Col; } ;
	bool operator < (const CTableSortKey &rhs) const;
	bool operator > (const CTableSortKey &rhs) const;
	CTableSortKey & operator = (const CTableSortKey &rhs);

	double m_Value;
	int m_Universe;
	int m_Page;
	int m_Row;
	int m_Col;
};

class CTableKey
{
public:
	virtual ~CTableKey(void) {};
	CTableKey(void);
	CTableKey(const CTableKey &rhs);
	CTableKey(const CTableSortKey &rhs);
	CTableKey(int Universe, int Page, int Row, int Col) { m_Universe = Universe; m_Page = Page; m_Row = Row; m_Col = Col; } ;
	void SetValue(int Universe, int Page, int Row, int Col) { m_Universe = Universe; m_Page = Page; m_Row = Row; m_Col = Col; } ;
	CTableKey & operator = (const CTableKey &rhs);
	CTableKey & operator = (const CTableSortKey &rhs);

	int m_Universe;
	int m_Page;
	int m_Row;
	int m_Col;
};
