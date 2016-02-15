// Obj_Carr.cpp : implementation file
//
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Objet JFC	: Obj_Carr v1.1a
Tâche		: Découpage journalier
Fichiers	: Obj_Carr.cpp, Obj_Carr.h, Obj_Carr.rco, Obj_Carr.icon1
Auteur		: VLF
Creer le	: 14/05/98 
Mise à jour : 10/07/00
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#include "stdafx.h"
#include "Obj_Carr.h"
#include "Obj_Carr.rch"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObj_Carr

CObj_Carr::CObj_Carr()
{
	m_CarrB_GrisC.CreateSolidBrush(RGB(192,192,192));
	m_brushwhite.CreateSolidBrush(RGB(255, 255, 255));
	m_brushyellow.CreateSolidBrush(RGB(0, 255, 0));
	m_hIcon1=AfxGetApp()->LoadIcon(IDI_CARR_ICO1);

	m_Matrice=new CPtrList;
	m_Matrice->RemoveAll();
}

CObj_Carr::~CObj_Carr()
{	
	LibereJour();
	delete m_Matrice;
}


BEGIN_MESSAGE_MAP(CObj_Carr, CStatic)
	//{{AFX_MSG_MAP(CObj_Carr)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObj_Carr message handlers


void CObj_Carr::OnLButtonDown(UINT nFlags, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	CRect R;

	GetClientRect(&R);
	if(!R.PtInRect(pt))	return;

	short ligne=(pt.y/m_HautLig);
	short col=(pt.x/m_LargCol);
	if(ligne>=7 || col>=7)	return;
	BYTE sel=GetByteSelection(ligne, col);
	switch(sel)
	{
		case 0:m_Selections->SetAt(col, 1);break;
		case 1:	m_Selections->SetAt(col, 0);break;
		case 2:MessageBeep(-1);break;
		case 3:MessageBeep(-1);break;
	}
	MajMatrice();
	InvalidateRect(NULL,0);
	CStatic::OnLButtonDown(nFlags, pt);
}

void CObj_Carr::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CObj_Carr::OnLButtonDown(nFlags, point);
//	CStatic::OnLButtonDblClk(nFlags, point);
}

void CObj_Carr::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DessineCarr(&dc);
	// Do not call CStatic::OnPaint() for painting messages
}

void CObj_Carr::LibereJour(void)
{
	int n=m_Matrice->GetCount();
	for(short i=0 ; i<n ; i++)
	{
		POSITION pos=m_Matrice->FindIndex(i);
		m_Selections=(CByteArray*)m_Matrice->GetAt(pos);
		m_Selections->RemoveAll();
		delete m_Selections;
	}
	m_Matrice->RemoveAll();
}

short CObj_Carr::Initialise(short style, CByteArray * Selections)
{
	BYTE flag;
	m_Style=style;
	
	LibereJour();
	
	int nbsel=Selections->GetSize();
	GetClientRect(&m_R);
	m_LargCol=(m_R.right-m_R.left)/7;
	m_HautLig=(m_R.bottom-m_R.top)/7;

	// initialisation de la matrice:
	for(short l=0;l<7;l++)
	{
		m_Selections=new CByteArray;
		m_Selections->RemoveAll();
		m_Selections->SetSize(7);

		for(short c=0;c<7;c++)
		{
			flag=1<<c;
			if(l<nbsel)
			{
				if(Selections->GetAt(l) & flag)
					m_Selections->SetAt(c, 1);
				else
					m_Selections->SetAt(c, 0);
			}
			else
				m_Selections->SetAt(c, 0);
		}
		m_Matrice->AddTail((CByteArray *)m_Selections);
	}
	MajMatrice();
	InvalidateRect(NULL,0);
	return(1);
}

// retourne 0 si OK
// sinon le numéro de l'erreur
short CObj_Carr::DonneResult(CByteArray * Selections)
{
	int l, c;
	BYTE flag, sel, ou,test=0;
	

	Selections->RemoveAll();
	Selections->SetSize(7);
	for(l=0;l<7;l++)
	{
		for(c=0;c<7;c++)
		{
			sel=GetByteSelection(l, c);
			if(sel==1)
			{
				flag=1<<c;
				ou=Selections->GetAt(l)|flag;
				Selections->SetAt(l, ou);
				test|=flag;
			}
		}
		flag=0;
	}
	Selections->FreeExtra();
	return((test==127)?0:1);
}

void CObj_Carr::Carr_DessineIcone(HDC hDC, RECT R, HICON hIco, short TailleX, short TailleY)
{
	short posx,posy;
	posx=R.left+(R.right-R.left-TailleX)/2;
	posy=R.top+(R.bottom-R.top-TailleY)/2;
	DrawIcon(hDC, posx, posy, hIco);
}

// dessin du bouton des horaires
//void CObj_Carr::DessineCarr(HDC hDCdest, RECT Rdest)
void CObj_Carr::DessineCarr(CPaintDC * dcDest)
{

	RECT R, Rdest;
	CPen * pen;
	CBitmap hBmpNew, * hBmpOld;
	CDC DC;
	BYTE sel;
	long l, h, c;
	short i;

	GetClientRect(&R);

	DC.CreateCompatibleDC(dcDest);
	l=R.right-R.left;
	h=R.bottom-R.top;
	hBmpNew.CreateCompatibleBitmap(dcDest, l,h);
	hBmpOld=(CBitmap *)DC.SelectObject(&hBmpNew);
	DC.BitBlt(0,0,l,h,0,0,0,WHITENESS);

	// Dessin proprement dit du bouton
	DC.FillRect(&m_R, &m_brushwhite);
	for(i=0;i<=7;i++)
	{
		DC.MoveTo(i*m_LargCol,0);
		DC.LineTo(i*m_LargCol,7*m_HautLig);
	}
	for(i=0;i<=7;i++)
	{
		DC.MoveTo(0,i*m_HautLig);
		DC.LineTo(7*m_LargCol,i*m_HautLig);
	}

	for(l=0;l<7;l++)
	{
		for(c=0;c<7;c++)
		{
			R.left=1+m_LargCol*c;
			R.right=m_LargCol*(c+1);
			R.top=1+m_HautLig*l;
			R.bottom=m_HautLig*(l+1);

			sel=GetByteSelection((short)l, (short)c);
			switch(sel)
			{
				case 1:
					Carr_DessineIcone(DC.m_hDC, R, m_hIcon1, CARRX_COTEICONE, CARRX_COTEICONE);
				break;

				case 3:
					DC.FillRect(&R, &m_CarrB_GrisC);

					//dessine les contours du bouton
					//bord blanc haut/gauche
					CPen penwhite(PS_SOLID, 1, RGB(255, 255, 255));
					pen=DC.GetCurrentPen();
					DC.SelectObject(&penwhite);
					DC.MoveTo(R.right-1, R.top);
					DC.LineTo(R.left, R.top);
					DC.LineTo(R.left, R.bottom);
					DC.MoveTo(R.right-2, R.top+1);
					DC.LineTo(R.left+1, R.top+1);
					DC.LineTo(R.left+1, R.bottom-1);

					//bord gris fonce bas/droite
					CPen penblack(PS_SOLID, 1, RGB(125, 125, 125));
					DC.SelectObject(&penblack);
					DC.MoveTo(R.right-1, R.top+1);
					DC.LineTo(R.right-1, R.bottom-1);
					DC.LineTo(R.left, R.bottom-1);
					DC.MoveTo(R.right-2, R.top+2);
					DC.LineTo(R.right-2, R.bottom-2);
					DC.LineTo(R.left+1, R.bottom-2);
					
					DC.SelectObject(pen);
				break;
			}
		}
	}
	// copy de la mem sur ecran
	Rdest=dcDest->m_ps.rcPaint;
	dcDest->BitBlt(Rdest.left,Rdest.top,Rdest.right,Rdest.bottom,&DC,Rdest.left,Rdest.top,SRCCOPY);
	dcDest->SelectObject(&hBmpOld);
	DC.DeleteDC();
}


void CObj_Carr::MajMatrice(void)
{
	short l,c;
	BYTE flag, sel;
	for (c=0;c<7;c++)
	{
		for (l=0;l<7;l++)
		{
			sel=GetByteSelection(l, c);
			if(sel==1)break;
		}
		if(l<7)
		{
			flag=0;
			for (l=0;l<7;l++)
			{
				sel=GetByteSelection(l, c);
				if(sel==1)
				{
					flag=1;
					continue;
				}
				m_Selections->SetAt(c, 2+flag);
			}
		}
		else
		{
			for (l=0;l<7;l++)
			{
				sel=GetByteSelection(l, c);
				if(sel!=1)
					m_Selections->SetAt(c, 0);
			}
		}
	}
	// on interdit les lignes du bas inutiles
	for (l=0;l<7;l++)
	{
		for (c=0;c<7;c++)
		{
			sel=GetByteSelection(l, c);
			if(sel==1)break;
		}
		if(c<7)continue;
		else break;
	}
	for (l++;l<7;l++)
		for (c=0;c<7;c++)
		{
			sel=GetByteSelection(l, c);
			m_Selections->SetAt(c, 3);
		}
}

BYTE CObj_Carr::GetByteSelection(short ligne, short col)
{
	POSITION pos=m_Matrice->FindIndex(ligne);
	m_Selections=(CByteArray*)m_Matrice->GetAt(pos);
	return (m_Selections->GetAt(col));
}
