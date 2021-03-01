// GridCellMerge.cpp: implementation of the CGridCellMerge class.
//
// Written by Jérémy DEVILERS
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCellMerge.h"
#include "GridCtrl.h"

IMPLEMENT_DYNCREATE(CGridCellMerge, CGridCell)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
CGridCellMerge::CGridCellMerge()
{
}

BOOL CGridCellMerge::PrintCell(CDC* pDC, int nRow, int nCol, CRect rect)
{
	if(m_MergeRange.InRange(nRow, nCol))
	{
		if(m_MergeRange.GetMinCol() == nCol && m_MergeRange.GetMinRow() == nRow)
		{
			CRect rectRange;
			m_pGrid->GetCellRangeRect(m_MergeRange,rectRange);
			return CGridCell::PrintCell(pDC, nRow, nCol, rectRange);
		}
		return TRUE;
	}
	else
		return CGridCell::PrintCell(pDC, nRow, nCol, rect);

}

BOOL CGridCellMerge::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd)
{
	if(m_MergeRange.InRange(nRow, nCol))
	{
		if(m_MergeRange.GetMinCol() == nCol && m_MergeRange.GetMinRow() == nRow)
		{
			CRect rectRange;
			m_pGrid->GetCellRangeRect(m_MergeRange,rectRange);
			return CGridCell::Draw(pDC, nRow, nCol, rectRange, bEraseBkgnd);
		}
		return TRUE;
	}
	else
		return CGridCell::Draw(pDC, nRow, nCol, rect, bEraseBkgnd);

}

bool CGridCellMerge::MergeCells(CGridCtrl * pGrid, int nStartRow, int nStartCol, int nEndRow, int nEndCol)
{
		for(int row=nStartRow;row<=nEndRow;row++)
		{
			for(int col=nStartCol;col<=nEndCol;col++)
			{
				
				ASSERT(pGrid->GetCell(row,col)->IsKindOf(RUNTIME_CLASS(CGridCellMerge)));
				CGridCellMerge *pCell = (CGridCellMerge*) pGrid->GetCell(row,col);
				pCell->m_MergeRange.Set(nStartRow,  nStartCol,  nEndRow,  nEndCol);
			}
		}
		return true;
}
