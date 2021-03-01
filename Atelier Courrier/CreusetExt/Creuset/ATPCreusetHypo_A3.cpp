// on inclut les d�finitions n�cesaires
#include "stdafx.h"
#include "ATPCreusetHypo_A3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetHypo_A3::CATPCreusetHypo_A3()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPCreusetHypo_A3::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPCreusetHypo_A3::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetHypo_A3::~CATPCreusetHypo_A3()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetHypo_A3, JFCTitle)
	//{{AFX_MSG_MAP(CATPCreusetHypo_A3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'�l�ment

JVoid CATPCreusetHypo_A3::OnDrawItem(CDC & dc, RECT rect)
{
	// on �crit le libell�
	dc.DrawText(_T("Nombre total d'insertions"), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPCreusetHypo_A3::OnUpdate()
{
	// on redessine la fen�tre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPCreusetHypo_A3 message handlers

void CATPCreusetHypo_A3::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
