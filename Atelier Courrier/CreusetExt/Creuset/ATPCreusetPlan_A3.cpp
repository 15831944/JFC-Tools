// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPCreusetPlan_A3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetPlan_A3::CATPCreusetPlan_A3()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetPlan_A3::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetPlan_A3::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetPlan_A3::~CATPCreusetPlan_A3()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetPlan_A3, JFCTitle)
	//{{AFX_MSG_MAP(CATPCreusetPlan_A3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'élément

JVoid CATPCreusetPlan_A3::OnDrawItem(CDC & dc, RECT rect)
{
	// on écrit le libellé
	dc.DrawText(_T("Total"), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetPlan_A3::OnUpdate()
{
	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPCreusetPlan_A3 message handlers

void CATPCreusetPlan_A3::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
