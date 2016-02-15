// MoveManager.cpp: implementation of the CMoveManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMoveManager::CMoveManager()
{
	m_fInit=FALSE;
}

CMoveManager::~CMoveManager()
{

}

void CMoveManager::AddWindow(CWnd*pCWnd, short style)
{
	SWndPos t;
	t.pCWnd=pCWnd;
	t.Style=style;
	t.pCWnd->GetWindowRect(t.Rect);
	m_ChildWnd.Add(t);
}

void CMoveManager::Resize(short cx, short cy)
{
	if(!m_fInit)return;
	CRect r;
	short ralongeX,ralongeY;
	ralongeX=cx-m_RectMouvement.Width();
	ralongeY=cy-m_RectMouvement.Height();

	for(short n=0;n<m_ChildWnd.GetSize();n++)
	{
		r=m_ChildWnd[n].Rect;
		if(m_ChildWnd[n].Style & MM_SIZE_X)if(ralongeX>0)r.right+=ralongeX;
		if(m_ChildWnd[n].Style & MM_SIZE_Y)if(ralongeY>0)r.bottom+=ralongeY;
		if(m_ChildWnd[n].Style & MM_MOVE_X)if(ralongeX>0)r.OffsetRect(ralongeX,0);
		if(m_ChildWnd[n].Style & MM_MOVE_Y)if(ralongeY>0)r.OffsetRect(0,ralongeY);
		m_ChildWnd[n].pCWnd->MoveWindow(r,1); 
	}
	m_oldcx=cx;
	m_oldcy=cy;
}

// !! pour que cette fonction fonctionne, 
// il faut être passé au moins une fois par resize() !!
void CMoveManager::HResize(short dx,int tailleAbsolue)
{
	if(!m_fInit)return;
	CRect r;

	for(short n=0;n<m_ChildWnd.GetSize();n++)
	{
		r=m_ChildWnd[n].Rect;
		if(m_ChildWnd[n].Style & MM_HRESIZE_RIGHT)
		{
			if(tailleAbsolue)r.right=dx;
			else r.right+=dx;
		}
		if(m_ChildWnd[n].Style & MM_HRESIZE_LEFT)
		{
			if(tailleAbsolue)r.left=dx+3;
			else r.left+=dx;
		}
		if(m_ChildWnd[n].Style & MM_HRESIZE_POSITION)
		{
			if(tailleAbsolue) 
			{
				if(r.left>dx)
				{
					r.OffsetRect(-(r.left-dx),0);
				}
				else 
				{
					r.OffsetRect((dx-r.left),0);
				}
			}
			else r.OffsetRect(dx,0);
		}
		m_ChildWnd[n].Rect=r;
		//m_ChildWnd[n].pCWnd->MoveWindow(r,1);
	}
	Resize(m_oldcx,m_oldcy);
}

void CMoveManager::Move(short x, short y)
{
	if(!m_fInit)return;
	CRect r;
	short MoveX,MoveY;
	MoveX=(short)(x-m_RectMouvement.left);
	MoveY=(short)(y-m_RectMouvement.top);
	if(MoveX<0)MoveX=0;
	if(MoveY<0)MoveY=0;

	for(short n=0;n<m_ChildWnd.GetSize();n++){
		r=m_ChildWnd[n].Rect;
		r.OffsetRect(MoveX,0);
		r.OffsetRect(0,MoveY);
		m_ChildWnd[n].pCWnd->MoveWindow(r,1);
	}
}

// pRectOrigine est l'origine de MoveWindow
// pRectMouvement est le rectangle qui pilote les mouvements
void CMoveManager::SetMasterRect(CRect  RectOrigine,CRect RectMouvement)
{
	m_RectOrigine=RectOrigine;
	// Ajustement des rectangles...
	// Je n'ai pas le temps pour l'instant,
	// Mais normalement on devrait pouvoir
	// Eviter ces bidouilles,
	// Et arriver trouver les bons paramêtres (bord des fentêtres)
	// 
	// Je rappelle que le pb n'est pas simples, car window 
	// est à cheval entre les position client et les 
	// position des fenêtres (extérieurs!)
	m_RectOrigine.OffsetRect(2,2);
	RectMouvement.right-=5;
	RectMouvement.bottom-=5;
	// **************************************

	m_RectMouvement=RectMouvement;
	for(short n=0;n<m_ChildWnd.GetSize();n++){
		m_ChildWnd[n].Rect.OffsetRect(-m_RectOrigine.left,-m_RectOrigine.top);
	}
	m_RectMouvement.OffsetRect(-m_RectOrigine.left,-m_RectOrigine.top);

	m_fInit=TRUE;
}

