// Obj_Gril.cpp : implementation file
//

#include "stdafx.h"
#include "Obj_Gril.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_Gril

CObj_Gril::CObj_Gril()//:CObj_Cal()
{
	m_Style=0;
	m_NbCelluleX=0;		m_NbCelluleY=0;
	m_dX=0;				m_dY=0;
	m_PosActiveX=0;		m_PosActiveY=0;
	m_ScrollX=NULL;		m_ScrollY=NULL;
	m_NbCelluleVisuX=0;	m_NbCelluleVisuY=0;
	m_NbCurseurs=0;
	m_fInit=0;			m_fCoupures=0;
	m_fSelection=0;
	m_nbCol=0;			m_nbLig=0;
	m_ACurseur=NULL;
	m_CoupureCalend=NULL;
	m_PrntDC=NULL;
}

CObj_Gril::~CObj_Gril()
{
	for(short i=m_NbCurseurs-1 ; i>=0 ; i--)	DelCurseur(i);
	if(m_ACurseur)	delete m_ACurseur;
	if(m_Style==STYLE_GRIL_PIGE && m_CoupureCalend)	delete m_CoupureCalend;
	m_fInit=0;
	switch(m_Style)
	{
	case STYLE_GRIL_PIGE:
		m_Pen2Black.DeleteObject();	
		m_PenGray.DeleteObject();
		break;
	}
//	FreeInitialisation();
}


BEGIN_MESSAGE_MAP(CObj_Gril, CStatic)
	//{{AFX_MSG_MAP(CObj_Gril)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Gril message handlers
short CObj_Gril::Initialise(short Style, short NbCelluleX, short NbCelluleY, short UTHorizontal, short UTVertical, short PosDefX, short PosDefY, CRect * pR_Print)
{
	m_Style=Style;
	m_NbCelluleX=NbCelluleX;
	m_NbCelluleY=NbCelluleY;
	m_dX=UTHorizontal;
	m_dY=UTVertical;
	m_PosActiveX=PosDefX;
	m_PosActiveY=PosDefY;
	if(m_Style & OPTION_GRIL_PRINTER)	m_PrntRect=*pR_Print;
	InitialiseCellule();

	if(!m_ACurseur)m_ACurseur=new CPtrArray;
	else m_ACurseur->RemoveAll();

	switch(m_Style&0x0f)
	{
	case STYLE_GRIL_DEFAUT:
		break;
	case STYLE_GRIL_PIGE:
		m_PenGray.CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
		m_Pen2Black.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		m_CoupureCalend=new char[m_NbCelluleX];
		break;
	}
	m_fInit=1;
	return 1;
}

void CObj_Gril::InitialiseCellule()
{
	if(m_Style&OPTION_GRIL_PRINTER)	m_RClient=m_PrntRect;
	else	GetClientRect(&m_RClient);
	m_NbCelluleVisuX=m_RClient.Width()/m_dX;
	if(m_NbCelluleVisuX>m_NbCelluleX){
		m_NbCelluleVisuX=m_NbCelluleX;
		m_RClient.right=m_dX*m_NbCelluleX+1;
	}
	m_NbCelluleVisuY=m_RClient.Height()/m_dY;
	if(m_NbCelluleVisuY>m_NbCelluleY){
		m_NbCelluleVisuY=m_NbCelluleY;
		m_RClient.bottom=m_dY*m_NbCelluleY+1;
	}
}

short CObj_Gril::InitScrollBar(CScrollBar * ScrollX, CScrollBar * ScrollY)
{
	int range;
	if(ScrollX)
	{
		m_nbCol=m_NbCelluleVisuX;
		m_ScrollX=ScrollX;
		range=m_NbCelluleX-m_NbCelluleVisuX;
		if(range<0)range=0;
		m_ScrollX->SetScrollRange(0,range,0);
		m_PosActiveX=min(m_PosActiveX,range);
		m_ScrollX->SetScrollPos(m_PosActiveX,true);
	}
	if(ScrollY)
	{
		m_nbLig=m_NbCelluleVisuY;
		m_ScrollY=ScrollY;
		range=m_NbCelluleY-m_NbCelluleVisuY;
		m_ScrollY->SetScrollRange(0, range,0);
		m_PosActiveY=min(m_PosActiveY,range);
		m_ScrollY->SetScrollPos(m_PosActiveY,true);
	}
	return 1;
}

short CObj_Gril::SetPosition(short PosX, bool flag)
{
	m_PosActiveX=PosX;
	if(flag)	InvalidateRect(NULL,0);
	return 1;
}

short CObj_Gril::SetPositionV(short PosY, bool flag)
{
	m_PosActiveY=PosY;
	if(flag)	InvalidateRect(NULL,0);
	return 1;
}

short CObj_Gril::SetCurseur(short NumCurs, short Pos, COLORREF Color)
{
	SCurseur * curseur=new SCurseur;
	curseur->Pos=Pos;
	if(curseur->Pos>m_NbCelluleX)	curseur->Pos=m_NbCelluleX;
	if(curseur->Pos<0)				curseur->Pos=0;
	curseur->Color=Color;
	curseur->NumCurseur=NumCurs;
	curseur->Select=0;
	curseur->R.SetRectEmpty();
	m_NbCurseurs++;
	m_ACurseur->Add(curseur);
	return m_NbCurseurs;
}

short CObj_Gril::SetPosCurseur(short NumCurseur, short Pos)
{
	if(Pos<0 || Pos>m_NbCelluleX)	return 0;
	SCurseur * curseur;
	for(short i=0 ; i<m_NbCurseurs ; i++)
	{
		if((curseur = (SCurseur *)m_ACurseur->GetAt(i)) != NULL)
			if(curseur->NumCurseur==NumCurseur){
				curseur->Pos=Pos;
				InvalidateRect(NULL, 0);
				return 1;
			}
	}
	return 0;
}

short CObj_Gril::DelCurseur(short NumCursor)
{
	SCurseur * curseur;
	if((curseur = (SCurseur *)m_ACurseur->GetAt(NumCursor)) != NULL)
	{
		m_ACurseur->RemoveAt(NumCursor);
		delete curseur;
		m_ACurseur->FreeExtra();
	}
	return 1;
}

void CObj_Gril::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(!m_fInit)	return;
	CRect R, Rdest;
	CBitmap hBmpNew, * hBmpOld;
	CDC DC;
	long l, h;

	// copie de l'écran en mémoire ****************
	if(m_Style&OPTION_GRIL_PRINTER)	R=m_PrntRect;
	else	GetClientRect(&R);

	DC.CreateCompatibleDC(&dc);
	l=R.right-R.left;
	h=R.bottom-R.top;
	hBmpNew.CreateCompatibleBitmap(&dc, l,h);
	hBmpOld=(CBitmap *)DC.SelectObject(&hBmpNew);
	DC.BitBlt(0,0,l,h,0,0,0,WHITENESS);
	// *********************************************

	DessineGrille(&DC,&R,NULL);

	// copy de la mem sur ecran
	Rdest=dc.m_ps.rcPaint;
	dc.BitBlt(Rdest.left,Rdest.top,Rdest.right,Rdest.bottom,&DC,Rdest.left,Rdest.top,SRCCOPY);
	dc.SelectObject(hBmpOld);
	DC.DeleteDC();
	// ************************
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CObj_Gril::PaintToPrinter(CDC * pDC, CFont * pFont)
{
	DessineGrille(pDC,&m_PrntRect,pFont, 1);
}

void CObj_Gril::DessineGrille(CDC * pDC,CRect * R, CFont * pFont, bool fPrinter)
{
	pDC->SetBkMode(TRANSPARENT);
	if(pFont)pDC->SelectObject(pFont);

	CRect *R2=new CRect;
	*R2=*R;
	if(m_Style & OPTION_GRIL_RETAILLABLE){
		R2->right=min(m_dX*(m_NbCelluleX-m_PosActiveX)+1,R->right);
		R2->bottom=min(m_dY*(m_NbCelluleY-m_PosActiveY)+1,R->bottom);
		if(*R!=*R2)DessineFondInutilise(pDC, R);
	}
	
	DessineFond(pDC,R2);
	DessineContenu(pDC,R2);
	
	short OffsetX=0, OffsetY=0;
	if(fPrinter){
		OffsetX=R2->left;
		OffsetY=R2->top;
	}
	DessineCadrillage(pDC,R2,OffsetX, OffsetY);

	//on trace un rectangle autour de l'objet static
	if(!(m_Style&OPTION_GRIL_NO_CONTOUR)){
		pDC->MoveTo(R2->left, R2->top);
		pDC->LineTo(R2->right-1,R2->top);
		pDC->LineTo(R2->right-1,R2->bottom-1);
		pDC->LineTo(R2->left,R2->bottom-1);
		pDC->LineTo(R2->left,R2->top);
	}

	DessineCurseur(pDC);
	delete R2;
}

void CObj_Gril::DessineCadrillage(CDC * dc,CRect * R, short OffsetX, short OffsetY)
{
	CPen * OldPen;
	CPen mypen(PS_SOLID,1,RGB(128,128,128));
	if(!(m_Style&OPTION_GRIL_NO_COLONNE)){
		for(short i=m_PosActiveX+1 ; i<=m_NbCelluleVisuX+m_PosActiveX ; i++)
		{
			if(i>m_NbCelluleX)		break;
			if(m_fCoupures)
			{
				switch(m_CoupureCalend[i])
				{
				case 0:
				case 2:
				case 4:	//Annee + Jour
				case 6:	//Annee + Mois + Jour
					OldPen=(CPen*)dc->SelectObject(&m_PenGray);
					dc->MoveTo((i-m_PosActiveX)*m_dX+OffsetX, R->top);
					dc->LineTo((i-m_PosActiveX)*m_dX+OffsetX, R->bottom);
					dc->SelectObject(OldPen);
					break;
				default:
					dc->MoveTo((i-m_PosActiveX)*m_dX+OffsetX, R->top);
					dc->LineTo((i-m_PosActiveX)*m_dX+OffsetX, R->bottom);
					break;
				}
			}
			else
			{
				if(m_Style&OPTION_GRIL_TRAITSGRIS)OldPen=(CPen*)dc->SelectObject(&mypen);
				dc->MoveTo((i-m_PosActiveX)*m_dX+OffsetX, R->top);
				dc->LineTo((i-m_PosActiveX)*m_dX+OffsetX, R->bottom);
				if(m_Style&OPTION_GRIL_TRAITSGRIS)dc->SelectObject(OldPen);
			}
			//tracages des traits doubles noirs pour délimité la sélection
			//lignes horizontales
			if(m_fSelection)
			{
				switch(m_Selections[i-1])
				{
				case 0:
				case 1:
					break;
				case 2:
					OldPen=(CPen*)dc->SelectObject(&m_Pen2Black);
					dc->MoveTo(((i-1)-m_PosActiveX)*m_dX+OffsetX, R->top);
					dc->LineTo(((i-1)-m_PosActiveX)*m_dX+OffsetX, R->bottom);
					dc->SelectObject(OldPen);
					break;
				case 3:
					OldPen=(CPen*)dc->SelectObject(&m_Pen2Black);
					dc->MoveTo((i-m_PosActiveX)*m_dX+OffsetX, R->top);
					dc->LineTo((i-m_PosActiveX)*m_dX+OffsetX, R->bottom);
					dc->SelectObject(OldPen);
					break;
				case 4:
					OldPen=(CPen*)dc->SelectObject(&m_Pen2Black);
					dc->MoveTo(((i-1)-m_PosActiveX)*m_dX+OffsetX, R->top);
					dc->LineTo(((i-1)-m_PosActiveX)*m_dX+OffsetX, R->bottom);
					dc->MoveTo((i-m_PosActiveX)*m_dX+OffsetX, R->top);
					dc->LineTo((i-m_PosActiveX)*m_dX+OffsetX, R->bottom);
					dc->SelectObject(OldPen);
					break;
				}
			}
		}
	}
	//lignes horizontales
	if(!(m_Style&OPTION_GRIL_NO_LIGNE)){
		if(m_Style&OPTION_GRIL_TRAITSGRIS)OldPen=(CPen*)dc->SelectObject(&mypen);
		for(short i=m_PosActiveY+1 ; i<=m_NbCelluleVisuY+m_PosActiveY ; i++){
			dc->MoveTo(R->left, (i-m_PosActiveY)*m_dY+OffsetY);
			dc->LineTo(R->right, (i-m_PosActiveY)*m_dY+OffsetY);
		}
		if(m_Style&OPTION_GRIL_TRAITSGRIS)dc->SelectObject(OldPen);
	}
}

void CObj_Gril::DessineCurseur(CDC * dc)
{
	for(short i=0 ; i<m_NbCurseurs ; i++)
	{
		SCurseur * curseur;
		curseur = (SCurseur *)m_ACurseur->GetAt(i);
		if(curseur->Pos>=m_PosActiveX && curseur->Pos<=m_PosActiveX+m_NbCelluleVisuX)
		{
			short width=0;
			if(curseur->Select)	width=3;
			else				width=2;
				
			CPen pen(PS_SOLID, width, curseur->Color);
			CPen * PenOld=dc->SelectObject(&pen);

			curseur->R.left=m_dX*(curseur->Pos-m_PosActiveX);
			curseur->R.right=curseur->R.left+width;
			curseur->R.top=m_RClient.top+2;
			curseur->R.bottom=m_RClient.bottom-2;

			dc->MoveTo(curseur->R.left, curseur->R.top);
			dc->LineTo(curseur->R.left, curseur->R.bottom);
			dc->SelectObject(PenOld);
		}
	}
}

//fonctions virtuelles
//////////////////////////////////////////////////////////////////////
void CObj_Gril::DessineFond(CDC * dc, CRect RectObj)
{
	CBrush brush(RGB(255, 255, 255));
	dc->FillRect(&RectObj, &brush);
}

void CObj_Gril::DessineFondInutilise(CDC * dc, CRect RectObj)
{
	CBrush brush(RGB(192,192,192));
	dc->FillRect(&RectObj, &brush);
}

void CObj_Gril::DessineContenu(CDC * dc, CRect RectObj)
{
	CRect R;
	if(m_Style & OPTION_GRIL_RETAILLABLE){
		for(short i=m_PosActiveY ; i<=m_NbCelluleVisuY+m_PosActiveY ; i++)
		{
			if(i>=m_NbCelluleY)continue;
			for(short j=m_PosActiveX; j<=m_NbCelluleVisuX+m_PosActiveX ; j++)
			{
				if(j>=m_NbCelluleX)continue;
				R.left=((j-m_PosActiveX)*m_dX)+RectObj.left;
				R.top=((i-m_PosActiveY)*m_dY)+RectObj.top;
				R.right=R.left+m_dX;
				R.bottom=R.top+m_dY;
				DessineCellule(dc, &R, j, i, j-m_PosActiveX, i-m_PosActiveY);
			}
		}
	}
	else {
		for(short i=m_PosActiveY ; i<m_NbCelluleVisuY+m_PosActiveY ; i++)
		{
			for(short j=m_PosActiveX; j<m_NbCelluleVisuX+m_PosActiveX ; j++)
			{
				R.left=((j-m_PosActiveX)*m_dX)+RectObj.left;
				R.top=((i-m_PosActiveY)*m_dY)+RectObj.top;
				R.right=R.left+m_dX;
				R.bottom=R.top+m_dY;
				DessineCellule(dc, &R, j, i, j-m_PosActiveX, i-m_PosActiveY);
			}
		}
	}
}

void CObj_Gril::DessineCellule(CDC * dc, CRect RectCel, short X, short Y, short PosX, short PosY)
{
}

bool CObj_Gril::OnLeftClick(short X, short Y, short PosX, short PosY,CPoint pos)
{

	return true;
}

bool CObj_Gril::OnRightClick(short X, short Y, short PosX, short PosY,CPoint pos)
{
	return true;
}

bool CObj_Gril::OnMousePosition(short X, short Y, short PosX, short PosY,CPoint pos)
{
	return true;
}
//////////////////////////////////////////////////////////////////////


void CObj_Gril::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_fInit)	return;
	//extremités X et Y
	short CellX=point.x/m_dX;
	short CellY=point.y/m_dY;
	if(CellX+m_PosActiveX>=m_NbCelluleX)	return;
	if(CellY+m_PosActiveY>=m_NbCelluleY)	return;
	CPoint pos;
	pos.x=point.x-CellX*m_dX;
	pos.y=point.y-CellY*m_dY;
	OnMousePosition(CellX+m_PosActiveX, CellY+m_PosActiveY, CellX, CellY,pos);
	CStatic::OnMouseMove(nFlags, point);
}

void CObj_Gril::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_fInit)	return;
	//extremités X et Y
	short CellX=point.x/m_dX;
	short CellY=point.y/m_dY;
	if(CellX+m_PosActiveX>=m_NbCelluleX)	return;
	if(CellY+m_PosActiveY>=m_NbCelluleY)	return;
	CPoint pos;
	pos.x=point.x-CellX*m_dX;
	pos.y=point.y-CellY*m_dY;
	OnLeftClick(CellX+m_PosActiveX, CellY+m_PosActiveY, CellX, CellY,pos);
	CStatic::OnLButtonDown(nFlags, point);

}

void CObj_Gril::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(!m_fInit)	return;
	//extremités X et Y
	short CellX=point.x/m_dX;
	short CellY=point.y/m_dY;
	if(CellX+m_PosActiveX>=m_NbCelluleX)	return;
	if(CellY+m_PosActiveY>=m_NbCelluleY)	return;
	CPoint pos;
	pos.x=point.x-CellX*m_dX;
	pos.y=point.y-CellY*m_dY;
	OnRightClick(CellX+m_PosActiveX, CellY+m_PosActiveY, CellX, CellY,pos);
	CStatic::OnRButtonDown(nFlags, point);
}

bool CObj_Gril::OnSelectionChanged(char * Selections)
{
	return true;
}

bool CObj_Gril::OnCurseurMoved(short NumCurs, short Pos)
{
	for(short i=0 ; i<m_NbCurseurs ; i++)
	{
		SCurseur * curseur;
		curseur = (SCurseur *)m_ACurseur->GetAt(i);
		if(curseur->NumCurseur==NumCurs)
		{
			curseur->Pos=Pos;
			InvalidateRect(0, NULL);
			return true;
		}
	}
	return false;
}

short CObj_Gril::SetNbLines(short NbLines, bool fUpdate)
{
	if(NbLines<=0)	return 0;
	m_NbCelluleY=NbLines;
	
	InitialiseCellule();
	if(m_PosActiveY>m_NbCelluleY)	m_PosActiveY=m_NbCelluleY;
	if(m_ScrollY){
		m_ScrollY->SetScrollRange(0, (m_NbCelluleY-m_NbCelluleVisuY),0);
		m_ScrollY->SetScrollPos(m_PosActiveY, true);
	}
	m_nbLig=m_NbCelluleVisuY;
	if(fUpdate)	InvalidateRect(0, NULL);
	return 1;
}

short CObj_Gril::SetNbRows(short NbRows, bool fUpdate)
{
	if(NbRows<=0)	return 0;
	m_NbCelluleX=NbRows;
	
	InitialiseCellule();
	if(m_PosActiveX>m_NbCelluleX)	m_PosActiveX=m_NbCelluleX;
	if(m_ScrollX){
		m_ScrollX->SetScrollRange(0, (m_NbCelluleX-m_NbCelluleVisuX),0);
		m_ScrollX->SetScrollPos(m_PosActiveX, true);
	}
	m_nbCol=m_NbCelluleVisuX;
	if(fUpdate)	InvalidateRect(0, NULL);
	return 1;
}


short CObj_Gril::MAJPosScrollV(UINT nSBCode, UINT nPos, short flag)
{
	int i,mini,maxi;

	i=m_ScrollY->GetScrollPos();
	m_ScrollY->GetScrollRange(&mini,&maxi);

	switch(nSBCode) {
		case SB_BOTTOM:		m_ScrollY->SetScrollPos(maxi,1); break;
		case SB_ENDSCROLL:	break;
		case SB_LINEDOWN:	m_ScrollY->SetScrollPos(min(i+1, maxi),1); break;
		case SB_LINEUP:		m_ScrollY->SetScrollPos(max(i-1, mini),1); break;
		case SB_PAGEDOWN:	m_ScrollY->SetScrollPos(min((i+m_nbLig-1), maxi),1); break;
		case SB_PAGEUP:		m_ScrollY->SetScrollPos(max((i-m_nbLig-1), mini),1); break;
		case SB_TOP:		m_ScrollY->SetScrollPos(mini,1); break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:	m_ScrollY->SetScrollPos(nPos,1); break;

	}
	short pos=m_ScrollY->GetScrollPos();
	if(flag)
	{
		SetPositionV(pos, 0);
		InvalidateRect(NULL,0);
	}
	return pos;
}

short CObj_Gril::MAJPosScrollH(UINT nSBCode, UINT nPos, short flag)
{
	int i,mini,maxi;

	i=m_ScrollX->GetScrollPos();
	m_ScrollX->GetScrollRange(&mini,&maxi);

	switch(nSBCode) {
		case SB_LEFT:		m_ScrollX->SetScrollPos(mini,1); break;
		case SB_ENDSCROLL:break;
		case SB_LINELEFT:	m_ScrollX->SetScrollPos(max(i-1, mini),1); break;
		case SB_LINERIGHT:	m_ScrollX->SetScrollPos(min(i+1, maxi),1); break;
		case SB_PAGELEFT:	m_ScrollX->SetScrollPos(max((i-m_nbCol-1), mini),1); break;
		case SB_PAGERIGHT:	m_ScrollX->SetScrollPos(min((i+m_nbCol-1), maxi),1); break;
		case SB_RIGHT:		m_ScrollX->SetScrollPos(maxi,1); break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:	m_ScrollX->SetScrollPos(nPos,1); break;
	}

	short pos=m_ScrollX->GetScrollPos();
	if(flag)
	{
		SetPosition(pos, 0);
		InvalidateRect(NULL,0);
	}
	return(pos);
}

void CObj_Gril::SetCoupureCalend(char * TabCoupures)
{
	if(TabCoupures)
	{
		memcpy(m_CoupureCalend, TabCoupures, m_NbCelluleX);
		m_fCoupures=1;
	}
}

void CObj_Gril::SetSelectionCalend(CByteArray * Selections)
{
	if(Selections && Selections->GetSize())
	{
		m_Selections.Copy(*Selections);
		m_fSelection=1;
	}
	else m_fSelection=0;
	InvalidateRect(NULL,0);
}
void CObj_Gril::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	if(!(m_Style & OPTION_GRIL_RETAILLABLE))return;
	InitialiseCellule();
	InitScrollBar(m_ScrollX,m_ScrollY);
	Invalidate(0);
}

void CObj_Gril::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags,point);
	//CStatic::OnLButtonDblClk(nFlags, point);
}

void CObj_Gril::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	OnRButtonDown(nFlags,point);
	//CStatic::OnRButtonDblClk(nFlags, point);
}
