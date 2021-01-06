// on inclut les d�finitions n�cesaires
#include "stdafx.h"
#include "ATPCreusetPlan_A1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetPlan_A1::CATPCreusetPlan_A1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPCreusetPlan_A1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPCreusetPlan_A1::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetPlan_A1::~CATPCreusetPlan_A1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetPlan_A1, JFCTitle)
	//{{AFX_MSG_MAP(CATPCreusetPlan_A1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'�l�ment

JVoid CATPCreusetPlan_A1::OnDrawItem(CDC & dc, RECT rect)
{
	// on �crit le libell�
	CString str;
	str.FormatMessage(IDS_CR_SUPPORTS);
	dc.DrawText(_T(str), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPCreusetPlan_A1::OnUpdate()
{
	// on redessine la fen�tre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPCreusetPlan_A1 message handlers

void CATPCreusetPlan_A1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
