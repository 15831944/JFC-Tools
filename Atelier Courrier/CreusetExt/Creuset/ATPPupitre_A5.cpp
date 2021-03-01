// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPPupitre_A5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPPupitre_A5::CATPPupitre_A5()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPPupitre_A5::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPPupitre_A5::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPPupitre_A5::~CATPPupitre_A5()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPPupitre_A5, JFCTitle)
	//{{AFX_MSG_MAP(CATPPupitre_A5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'élément

JVoid CATPPupitre_A5::OnDrawItem(CDC & dc, RECT rect)
{
	// Font spécifique (bold et taille moyenne 12)
	LOGFONT lf;
	dc.GetCurrentFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	CFont fontBold;
	fontBold.CreateFontIndirect(&lf);

	// Sauve ancienne font
	CFont* pOldFont = dc.SelectObject(&fontBold);

	// on écrit le libellé
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		dc.DrawText(_T("Hypothèses"), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	else
		dc.DrawText(_T("Formats"), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPPupitre_A5::OnUpdate()
{
	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPPupitre_A5 message handlers

void CATPPupitre_A5::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
