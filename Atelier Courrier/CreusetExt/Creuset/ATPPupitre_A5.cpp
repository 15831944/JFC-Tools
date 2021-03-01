// on inclut les d�finitions n�cessaires
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
// la fonction pour d�finir le document

JVoid CATPPupitre_A5::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPPupitre_A5::Update()
{
	// on met � jour le composant
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
// la fonction pour dessiner l'�l�ment

JVoid CATPPupitre_A5::OnDrawItem(CDC & dc, RECT rect)
{
	// Font sp�cifique (bold et taille moyenne 12)
	LOGFONT lf;
	dc.GetCurrentFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	CFont fontBold;
	fontBold.CreateFontIndirect(&lf);

	// Sauve ancienne font
	CFont* pOldFont = dc.SelectObject(&fontBold);

	// on �crit le libell�
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		dc.DrawText(_T("Hypoth�ses"), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	else
		dc.DrawText(_T("Formats"), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPPupitre_A5::OnUpdate()
{
	// on redessine la fen�tre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPPupitre_A5 message handlers

void CATPPupitre_A5::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
