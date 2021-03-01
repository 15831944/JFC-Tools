////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											TITRE LISTE DES CIBLES											  //	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPPupitre_A3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPPupitre_A3::CATPPupitre_A3()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPPupitre_A3::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPPupitre_A3::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPPupitre_A3::~CATPPupitre_A3()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPPupitre_A3, JFCTitle)
	//{{AFX_MSG_MAP(CATPPupitre_A3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'élément

JVoid CATPPupitre_A3::OnDrawItem(CDC & dc, RECT rect)
{
	// Font spécifique (bold et taille moyenne 12)
	LOGFONT lf;
	dc.GetCurrentFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	CFont fontBold;
	fontBold.CreateFontIndirect(&lf);

	// Sauve ancienne font
	CFont* pOldFont = dc.SelectObject(&fontBold);

	if (!m_pATPDocCreuset->m_CiblesPondsActives)
		// Mode cible de l'offre
		dc.DrawText(_T("Cible"), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	else
		// Mode cible pondérée
		dc.DrawText(_T("Cible Pondérée"), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPPupitre_A3::OnUpdate()
{
	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPPupitre_A3 message handlers

void CATPPupitre_A3::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
