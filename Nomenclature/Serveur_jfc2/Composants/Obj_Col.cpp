// obj_col.cpp : implementation file
//

#include "stdafx.h"
#include "obj_col.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_Col

CObj_Col::CObj_Col()
{
	m_Style=0;
	m_dY=0;
	m_NbCelluleY=0;
	m_PosActiveY=0;
	m_NbCelluleVisuY=0;
	m_nblines=0;
	m_nbLigScrollPage=0;
	m_fInit=FALSE;
	m_ScrollY=NULL;
	m_PrntDC=NULL;
}

CObj_Col::~CObj_Col()
{
}

short CObj_Col::Initialise(short Style,short NbCelluleY,short UTVertical, short PosDefY, CScrollBar * ScrollY, CRect * pR_Print)
{
	m_Style=Style;
	m_NbCelluleY=NbCelluleY;
	m_dY=UTVertical;
	m_PosActiveY=PosDefY;
	if(m_Style & OPTION_COL_PRINTER){
		m_PrntRect=*pR_Print;
		m_RClient=m_PrntRect;
	}
	else GetClientRect(&m_RClient);
	m_nblines=m_NbCelluleVisuY=m_RClient.Height()/m_dY;
	if(m_NbCelluleVisuY>m_NbCelluleY)
	{
		m_NbCelluleVisuY=m_NbCelluleY;
		m_RClient.bottom=m_dY*m_NbCelluleY+1;
	}
	else m_RClient.bottom=m_dY*m_NbCelluleVisuY+1;
	m_nbLigScrollPage=m_NbCelluleVisuY;
	if(ScrollY)
	{
		m_ScrollY=ScrollY;
		m_ScrollY->SetScrollRange(0, (m_NbCelluleY-m_NbCelluleVisuY),0);
		m_ScrollY->SetScrollPos(m_PosActiveY,TRUE);
	}

	m_fInit=TRUE;
	return(0);
}


BEGIN_MESSAGE_MAP(CObj_Col, CStatic)
	//{{AFX_MSG_MAP(CObj_Col)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Col message handlers

void CObj_Col::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(!m_fInit)	return;
	CRect R, Rdest;
	CBitmap hBmpNew, * hBmpOld;
	CDC DC;
	long l, h;

	// copie de l'écran en mémoire ****************
	if(m_Style&OPTION_COL_PRINTER)	R=m_PrntRect;
	else	GetClientRect(&R);
	DC.CreateCompatibleDC(&dc);
	l=R.right-R.left;
	h=R.bottom-R.top;
	hBmpNew.CreateCompatibleBitmap(&dc, l,h);
	hBmpOld=(CBitmap *)DC.SelectObject(&hBmpNew);
	DC.BitBlt(0,0,l,h,0,0,0,WHITENESS);
	// *********************************************

	DessineColonne(&DC,&R,NULL);

	// copy de la mem sur ecran
	Rdest=dc.m_ps.rcPaint;
	dc.BitBlt(Rdest.left,Rdest.top,Rdest.right,Rdest.bottom,&DC,Rdest.left,Rdest.top,SRCCOPY);
	dc.SelectObject(hBmpOld);
	DC.DeleteDC();
	// ************************	
	// Do not call CStatic::OnPaint() for painting messages
}

void CObj_Col::PaintToPrinter(CDC * pDC, CFont * pFont)
{
	DessineColonne(pDC,&m_PrntRect,pFont, 1);
}

void CObj_Col::DessineColonne(CDC * pDC,CRect * R, CFont * pFont, bool fPrinter)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(pFont);

	short OffsetY=0;
	if(fPrinter)	OffsetY=R->top;
	DessineFond(pDC, R);

	DessineContenu(pDC, R);

	if((m_Style&0x0F)!=STYLE_COL_NO_GRILLE)
	{
		DessineGrille(pDC, R, OffsetY);

		//on trace un rectangle autour de l'objet static
		pDC->MoveTo(R->left, R->top);
		pDC->LineTo(R->right-1, R->top);
		pDC->LineTo(R->right-1, R->bottom-1);
		pDC->LineTo(R->left, R->bottom-1);
		pDC->LineTo(R->left, R->top);
	}
}

//fonction à ecraser:
void CObj_Col::DessineFond(CDC * dc, CRect RectObj)
{
	CBrush brush(RGB(255, 255, 255));
	dc->FillRect(&RectObj, &brush);
}

//fonction à ecraser:
void CObj_Col::DessineCellule(CDC * dc, CRect RectCel, short Y, short PosY)
{
}


void CObj_Col::DessineContenu(CDC * dc, CRect RectObj)
{
	CRect R;
	for(short i=m_PosActiveY ; i<m_NbCelluleVisuY+m_PosActiveY ; i++)
	{
		R=m_RClient;
		R.top=((i-m_PosActiveY)*m_dY)+RectObj.top;
		R.bottom=R.top+m_dY;
		DessineCellule(dc, &R,i,i-m_PosActiveY);
	}
}

void CObj_Col::DessineGrille(CDC * dc, CRect * R, short OffsetY)
{
	for(short i=m_PosActiveY+1 ; i<=m_NbCelluleVisuY+m_PosActiveY ; i++){
		dc->MoveTo(R->left, (i-m_PosActiveY)*m_dY+OffsetY);
		dc->LineTo(R->right, (i-m_PosActiveY)*m_dY+OffsetY);
	}
}


short CObj_Col::SetPosition(short PosY,bool fRedraw)
{
	m_PosActiveY=PosY;
	if(fRedraw)InvalidateRect(NULL,0);
	return 1;
}

void CObj_Col::SetNbLine(short NbCelluleY,bool fRedraw)
{
	m_NbCelluleY=NbCelluleY;
	if(m_Style & OPTION_COL_PRINTER)
		m_RClient=m_PrntRect;
	else GetClientRect(&m_RClient);
	m_NbCelluleVisuY=m_RClient.Height()/m_dY;
	if(m_NbCelluleVisuY>m_NbCelluleY)
	{
		m_NbCelluleVisuY=m_NbCelluleY;
		m_RClient.bottom=m_dY*m_NbCelluleY+1;
	}
	else m_RClient.bottom=m_dY*m_NbCelluleVisuY+1;
	if(m_NbCelluleVisuY<m_nblines)	m_PosActiveY=0;
	if(m_PosActiveY>=m_NbCelluleY)	m_PosActiveY=m_NbCelluleY-1;
	if(m_ScrollY)
	{
		m_ScrollY->SetScrollRange(0, (m_NbCelluleY-m_NbCelluleVisuY),0);
		m_ScrollY->SetScrollPos(m_PosActiveY, TRUE);
	}
	if(fRedraw)	InvalidateRect(NULL, false);
}

short CObj_Col::MAJPosScrollV(UINT nSBCode, UINT nPos, short flag)
{
	int i,mini,maxi;

	i=m_ScrollY->GetScrollPos();
	m_ScrollY->GetScrollRange(&mini,&maxi);

	switch(nSBCode) {
		case SB_BOTTOM:		m_ScrollY->SetScrollPos(maxi,1); break;
		case SB_ENDSCROLL:	break;
		case SB_LINEDOWN:	m_ScrollY->SetScrollPos(min(i+1, maxi),1); break;
		case SB_LINEUP:		m_ScrollY->SetScrollPos(max(i-1, mini),1); break;
		case SB_PAGEDOWN:	m_ScrollY->SetScrollPos(min((i+m_nblines-1), maxi),1); break;
		case SB_PAGEUP:		m_ScrollY->SetScrollPos(max((i-m_nblines-1), mini),1); break;
		case SB_TOP:		m_ScrollY->SetScrollPos(mini,1); break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:	m_ScrollY->SetScrollPos(nPos,1); break;

	}
	if(flag)	SetPosition(m_ScrollY->GetScrollPos(), true);
	return m_ScrollY->GetScrollPos();
}

