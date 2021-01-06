// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPCreusetPlan_B1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetPlan_B1::CATPCreusetPlan_B1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetPlan_B1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetPlan_B1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetPlan_B1::~CATPCreusetPlan_B1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetPlan_B1, JFCTitle)
	//{{AFX_MSG_MAP(CATPCreusetPlan_B1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'élément

JVoid CATPCreusetPlan_B1::OnDrawItem(CDC & dc, RECT rect)
{

	dc.DrawText(_T("Insert."), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

/*
// on crée les rectangles
	RECT RectH, RectB;

	// on initialise le rectangle du haut
	RectH.left = rect.left;     RectH.right  = rect.right;
	RectH.top  = rect.top + 8;  RectH.bottom = (rect.bottom + rect.top) / 2;

	// on initialise le rectangle du bas
	RectB.left   = rect.left;       RectB.right = rect.right;
	RectB.bottom = rect.bottom - 8; RectB.top   = (rect.bottom + rect.top) / 2;

	// on écrit le libellé
	dc.DrawText(_T("Nb."),     &RectH, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	dc.DrawText(_T("Insert."), &RectB, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	*/
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetPlan_B1::OnUpdate()
{
	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPCreusetPlan_B1 message handlers

void CATPCreusetPlan_B1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
