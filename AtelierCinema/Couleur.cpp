// Couleur.cpp : implementation file
//

#include "stdafx.h"
#include "cinema.h"
extern CCinemaApp theApp;
#include "Couleur.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCouleur

CCouleur::CCouleur()
{
	m_Numero=0;
}

CCouleur::~CCouleur()
{
}


BEGIN_MESSAGE_MAP(CCouleur, CStatic)
	//{{AFX_MSG_MAP(CCouleur)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCouleur message handlers

void CCouleur::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect R;
	GetClientRect(R);
	FillRect(dc.m_hDC,&R,theApp.hBrush[CLR_HYPOTHESE1+m_Numero]);
	
	// Do not call CStatic::OnPaint() for painting messages
}
