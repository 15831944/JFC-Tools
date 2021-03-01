// CGridCellNumClick.h: interface for the CGridCellNumClick class.
//
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCELLNUMCLICK_H__INCLUDED_)
#define AFX_GRIDCELLNUMCLICK_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCell.h"

class AFX_EXT_CLASS CGridCellNumClick : public CGridCell  
{
public:
	DECLARE_DYNCREATE(CGridCellNumClick)

    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();

    virtual void OnClick( CPoint PointCellRelative);
    virtual void OnClickDown( CPoint PointCellRelative);
    virtual void OnDblClick( CPoint PointCellRelative);
    virtual void OnRClick( CPoint PointCellRelative);

};

#endif // !defined(AFX_GRIDCELLNUMCLICK_H__INCLUDED_)
