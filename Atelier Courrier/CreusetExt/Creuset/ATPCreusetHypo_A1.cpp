// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPCreusetHypo_A1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetHypo_A1::CATPCreusetHypo_A1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetHypo_A1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetHypo_A1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetHypo_A1::~CATPCreusetHypo_A1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetHypo_A1, JFCTitle)
	//{{AFX_MSG_MAP(CATPCreusetHypo_A1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'élément

JVoid CATPCreusetHypo_A1::OnDrawItem(CDC & dc, RECT rect)
{
	// on écrit le libellé
	CString str;
	str.FormatMessage(IDS_CR_SUPPORTS);

	// dc.DrawText(_T(str), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

	// Font spécifique (bold et taille moyenne 12)
	LOGFONT lf;
	dc.GetCurrentFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	// lf.lfHeight = -16;
	CFont fontBold;
	fontBold.CreateFontIndirect(&lf);

	// Sauve ancienne font
	CFont* pOldFont = dc.SelectObject(&fontBold);

	// Ecriture texte avec font courante
	dc.DrawText(_T(str), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

	// Remet ancienne Font
	dc.SelectObject(pOldFont);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetHypo_A1::OnUpdate()
{
	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPCreusetHypo_A1 message handlers

void CATPCreusetHypo_A1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
