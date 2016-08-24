// ListBoxSelect.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
#include "ListBoxSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListBoxSelect

CListBoxSelect::CListBoxSelect()
{
	// Par défaut, texte aligné à gauche
	m_AlignType=DT_LEFT;
}

CListBoxSelect::~CListBoxSelect()
{
}


BEGIN_MESSAGE_MAP(CListBoxSelect, CListBox)
	//{{AFX_MSG_MAP(CListBoxSelect)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxSelect message handlers

void CListBoxSelect::OnLButtonDown(UINT nFlags, CPoint point) 
{
		CObj_LBox::OnLButtonDown(nFlags, point);
}


void CListBoxSelect::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	CDC *dc=CDC::FromHandle(lpDrawItemStruct->hDC);
	int etat;

	// Récupération du rectangle
	CRect Rect(lpDrawItemStruct->rcItem);

	// Rectangle pour colonne de droite
	RECT right;

	// Rectangle pour colonne de gauche
	RECT left;

	// Séparation des substrings et recupération
	CString txt;
	int index=lpDrawItemStruct->itemID;
	if(index>=0)
		CListBox::GetText(index,txt);

	int n = txt.Find('\t', 0); 
	CString txtleft;
	CString txtright;
	if (n == -1 )
	{
		txtleft = txt;
	} 
	else 
	{
		txtleft = txt.Left(n);
		txt = txt.Mid(n+1);
		txtright = txt;
		left.bottom = Rect.bottom;
		left.top = Rect.top;
		left.left = Rect.left;
		left.right = 50;
		right.bottom = Rect.bottom;
		right.top = Rect.top;
		right.left = 50;
		right.right = Rect.right;
	}
	if(lpDrawItemStruct->itemState&ODS_SELECTED)
	{
		CBrush Brush;
		Brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		dc->FillRect(Rect,&Brush);
		dc->SetBkMode(TRANSPARENT);
		dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}	

	etat = GetItemData(lpDrawItemStruct->itemID); 

	if(etat==1)
	{
		CBrush Brush;
		Brush.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
		dc->FillRect(Rect,&Brush);
		dc->SetBkMode(TRANSPARENT);
		dc->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
	{
		if(lpDrawItemStruct->itemState!=ODS_SELECTED)
		{
			CBrush Brush;
			Brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
			dc->FillRect(Rect,&Brush);
			dc->SetBkMode(TRANSPARENT);
			dc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		}
	}

	if(n>=0)
	{
		dc->DrawText(txtleft,&left,DT_SINGLELINE|m_AlignType);
		dc->DrawText(txtright,&right,DT_SINGLELINE|m_AlignType);
	}
	else dc->DrawText(txtleft,&Rect,DT_SINGLELINE|m_AlignType);
	
	// Affichage des lignes de séparation
	int lineposX2=50;
	if(n>=0)
	{
		GetClientRect(Rect);
		dc->MoveTo(lineposX2, Rect.top);
		dc->LineTo(lineposX2, Rect.bottom);
	}
}



int CListBoxSelect::GetSelCount()
{
	return CObj_LBox::GetSelCount();
}

int CListBoxSelect::GetSelItems(int nMaxItems, LPINT rgIndex )
{
	return CObj_LBox::GetSelItems(nMaxItems,rgIndex);
}

void CListBoxSelect::SetTextAlign(int halign)
{
	m_AlignType=halign;
}

void CListBoxSelect::OnMouseMove(UINT nFlags, CPoint point) 
{
	CObj_LBox::OnMouseMove(nFlags, point);
}

int CListBoxSelect::SetSel(int nIndex, BOOL bSelect)
{
	return CListBox::SetSel(nIndex,bSelect);
}
