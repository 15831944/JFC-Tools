// VertSplitterWndEx.cpp : implementation file
//

#include "stdafx.h"
#include "VertSplitterWndEx.h"
#include ".\vertsplitterwndex.h"
#include "ATPColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//The CHideSplitterWndEx class primary extened use is for Replacing Views.
//In the function CHideSplitterWndEx::ReplaceView pDoc->AutoDelete has to be changed to prevent
//the one and only doc object from being deleted.
/////////////////////////////////////////////////////////////////////////////
// CHideSplitterWndEx

CHideSplitterWndEx::CHideSplitterWndEx()
{
	m_bClick = FALSE;
	m_bDblClick = FALSE;
}

CHideSplitterWndEx::~CHideSplitterWndEx()
{
}


BEGIN_MESSAGE_MAP(CHideSplitterWndEx, CSplitterWndEx)
	//{{AFX_MSG_MAP(CHideSplitterWndEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHideSplitterWndEx message handlers

void CHideSplitterWndEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bClick)
	{
		HideBar();
		return;
	}

	CSplitterWndEx::OnLButtonDown(nFlags, point);
}

void CHideSplitterWndEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_bClick || 	m_bDblClick)
	{
		HideBar(Sens());
		return;
	}

	CSplitterWndEx::OnLButtonDblClk(nFlags, point);
}

int CHideSplitterWndEx::Sens()
{
	int cx0, cx1;
	int xmin0, xmin1;

	if(GetColumnCount() != 1)
	{
		GetColumnInfo(0, cx0, xmin0);
		GetColumnInfo(1, cx1, xmin1);
	}
	
	if(GetRowCount() != 1)
	{
		GetRowInfo(0, cx0, xmin0);
		GetRowInfo(1, cx1, xmin1);
	}

	if (cx0<cx1)
		return 0;
	else
		return 1;
	
}
void CHideSplitterWndEx::HideBar(int nCol, bool bShow)
{
	int cx0, cx1;
	int xmin0, xmin1;
	
	//orientation

	//vertical (colonnes)
	if(GetColumnCount() != 1)
	{
		GetColumnInfo(0, cx0, xmin0);
		GetColumnInfo(1, cx1, xmin1);
		if(nCol == 1)
		{
			//on grandit
			if(bShow)
			{
				SetColumnInfo(0, xmin0, xmin0);
				SetColumnInfo(1, 0, xmin1);
			}
			//on reduit
			else
			{
				SetColumnInfo(0, 2*cx0+cx1, xmin0);
				SetColumnInfo(1, 0, xmin1);
			}
		}
		if(nCol == 0)
		{
			//on grandit
			if(bShow)
			{
				SetColumnInfo(0, 0, xmin0);
				SetColumnInfo(1, xmin1, xmin1);
			}
			//on reduit
			else
			{
				SetColumnInfo(0, 0, xmin0);
				SetColumnInfo(1, 2*cx0+cx1, xmin1);
			}
		}
	}

	// horizontal (row)
	if(GetRowCount() != 1)
	{
		GetRowInfo(0, cx0, xmin0);
		GetRowInfo(1, cx1, xmin1);
		if(nCol == 1)
		{
			if(bShow)
			{
				SetRowInfo(0, xmin0, xmin0);
				SetRowInfo(1, 0, xmin1);
			}
			else
			{
				SetRowInfo(0, 2*cx0+cx1, xmin0);
				SetRowInfo(1, 0, xmin1);
			}
		}
		if(nCol == 0)
		{
			if(bShow)
			{
				SetRowInfo(0, 0, xmin0);
				SetRowInfo(1, xmin1, xmin1);
			}
			else
			{
				SetRowInfo(0, 0, xmin0);
				SetRowInfo(1, 2*cx0+cx1, xmin1);
			}
		}
	}

	RecalcLayout();
}

void CHideSplitterWndEx::HideBar(int nCol)
{
	int cx0, cx1;
	int xmin0, xmin1;
	
	//orientation

	//vertical (colonnes)
	if(GetColumnCount() != 1)
	{
		GetColumnInfo(0, cx0, xmin0);
		GetColumnInfo(1, cx1, xmin1);
		if(nCol == 1)
		{
			//on reduit
			if(cx1)
			{
				SetColumnInfo(0, 2*cx0+cx1, xmin0);
				SetColumnInfo(1, 0, xmin1);
			}
			//on grandit
			else
			{
				SetColumnInfo(0, xmin0, xmin0);
				SetColumnInfo(1, 0, xmin1);
			}
		}
		if(nCol == 0)
		{
			//on reduit
			if(cx0)
			{
				SetColumnInfo(0, 0, xmin0);
				SetColumnInfo(1, 2*cx0+cx1, xmin1);
			}
			//on grandit
			else
			{
				SetColumnInfo(0, 0, xmin0);
				SetColumnInfo(1, xmin1, xmin1);
			}
		}
	}

	// horizontal (row)
	if(GetRowCount() != 1)
	{
		GetRowInfo(0, cx0, xmin0);
		GetRowInfo(1, cx1, xmin1);
		if(nCol == 1)
		{
			if(cx1)
			{
				SetRowInfo(0, 2*cx0+cx1, xmin0);
				SetRowInfo(1, 0, xmin1);
			}
			else
			{
				SetRowInfo(0, xmin0, xmin0);
				SetRowInfo(1, 0, xmin1);
			}
		}
		if(nCol == 0)
		{
			if(cx0)
			{
				SetRowInfo(0, 0, xmin0);
				SetRowInfo(1, 2*cx0+cx1, xmin1);
			}
			else
			{
				SetRowInfo(0, 0, xmin0);
				SetRowInfo(1, xmin1, xmin1);
			}
		}
	}

	RecalcLayout();
}



void CHideSplitterWndEx::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
	CSplitterWndEx::OnDrawSplitter(pDC, nType, rect);
/*
	if ( pDC && nType == splitBar )
	{
		CBrush BrushF(CATPColors::GetColorDark(CATPColors::COLORCREUSET));
		int cx0, cx1;
		int xmin0, xmin1;

		//if(GetColumnCount() != 1)
		//{
		//	GetColumnInfo(0, cx0, xmin0);
		//	GetColumnInfo(1, cx1, xmin1);
		//}
		//
		//if(GetRowCount() != 1)
		//{
		//	GetRowInfo(0, cx0, xmin0);
		//	GetRowInfo(1, cx1, xmin1);
		//}

		//if (cx0 == 0 || cx1 == 0)
			pDC->FillRect(rect, &BrushF);
	}  
*/
}
