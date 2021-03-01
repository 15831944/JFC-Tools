// GridCellMerge.h: interface for the CGridCellMerge class.
//
// Written by Jeremy DEVILERS
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDCELLMERGE_H__3479ED0D_B57D_4940_B83D_9E2296ED75B5__INCLUDED_)
#define AFX_GRIDCELLMERGE_H__3479ED0D_B57D_4940_B83D_9E2296ED75B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCell.h"
#include "CellRange.h"

class AFX_EXT_CLASS CGridCellMerge : public CGridCell  
{
    DECLARE_DYNCREATE(CGridCellMerge)
public:
    CGridCellMerge();

	CCellRange m_MergeRange;

    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
	virtual BOOL PrintCell(CDC* pDC, int nRow, int nCol, CRect rect);

	static bool MergeCells(CGridCtrl * pGrid, int nStartRow, int nStartCol, int nEndRow, int nEndCol);

};

#endif // !defined(AFX_GRIDCELLMERGE_H__3479ED0D_B57D_4940_B83D_9E2296ED75B5__INCLUDED_)
