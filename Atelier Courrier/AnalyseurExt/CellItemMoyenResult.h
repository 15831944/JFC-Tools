#pragma once

#include "icellresult.h"
#include "CoordRowCol.h"
#include "AnalyseurEnums.h"

typedef JMap<CCoordRowCol,PTR_ICELLRESULT> MAPCELLRESULTS;

class CCrossTable;

class CCellItemMoyenResult :
	public ICellResult
{
public:
	CCellItemMoyenResult(int Page, int Row, int Col,CCrossTable *pCrossTable,MAPCELLRESULTS  * CellResults);
	virtual ~CCellItemMoyenResult(void);

	virtual const CCellItemMoyenResult *	AsCCellItemMoyenResult()const	{ return this; } ;
	virtual CCellItemMoyenResult *			AsCCellItemMoyenResult()		{ return this; } ;
	virtual bool	IsCCellItemMoyenResult()  { return true; }
	virtual double GetResult(RESULTTYPE Type, const JFLT32VECTOR * pQuanti = 0, bool ModeMoyenne0Exclu = 0,ITEMTYPE ElmtType = ITYPE_ITEM, bool ModeExclusif = false);

	virtual CCoordRowCol & GetCoord() { return m_Coord; }
	virtual void SetCoord(CCoordRowCol & Coord) { m_Coord.SetCoord(Coord.Page(),Coord.Row(),Coord.Col()); }

protected:
	virtual bool Calculate(RESULTTYPE Type, const JFLT32VECTOR * pQuanti  = 0);

	CCoordRowCol	m_Coord;
	CCrossTable	*	m_pCrossTable;
	MAPCELLRESULTS  * m_pCellResults;

	double			m_Effective;
	double			m_NbCas;
	double			m_Quantity;

	double			m_Effective_0Exclu;
	double			m_Quantity_0Exclu;

	double			m_EffectiveBrutNonCible;
	DWORD			m_Calculated;

};
