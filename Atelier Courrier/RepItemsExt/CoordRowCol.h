#pragma once

class AFX_EXT_CLASS CCoordRowCol
{
public:
	CCoordRowCol(void);
	CCoordRowCol(int Page, int Row, int Col) { m_Page = Page; m_Row = Row; m_Col = Col;} ;
	virtual ~CCoordRowCol(void);
	void SetCoord(int Page, int Row, int Col) { m_Page = Page; m_Row = Row; m_Col = Col;} ;
	int Page() const { return m_Page; }
	int Col() const { return m_Col; }
	int Row() const { return m_Row; }
	bool operator < (const CCoordRowCol &rhs) const;
	bool operator > (const CCoordRowCol &rhs) const;

private:

	int m_Page;
	int m_Col;
	int m_Row;
};
