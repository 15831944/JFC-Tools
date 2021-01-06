// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPCreusetHypo_T1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetHypo_T1::CATPCreusetHypo_T1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPCreusetHypo_T1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPCreusetHypo_T1::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetHypo_T1::~CATPCreusetHypo_T1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetHypo_T1, JFCTitle)
	//{{AFX_MSG_MAP(CATPCreusetHypo_T1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'�l�ment

JVoid CATPCreusetHypo_T1::OnDrawItem(CDC & dc, RECT rect)
{
	// Font sp�cifique (bold et taille moyenne 12)
	LOGFONT lf;
	dc.GetCurrentFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	CFont fontBold;
	fontBold.CreateFontIndirect(&lf);

	// Sauve ancienne font
	CFont* pOldFont = dc.SelectObject(&fontBold);

	// on �crit le titre
	CString str;
	if (m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD().IsValid()
		&& m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF().IsValid()
		&& ! m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		str.FormatMessage(IDS_CR_TTLCREUSETHYPOD, CFormatStr::FormatDate(m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD()),
						CFormatStr::FormatDate(m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF()));
	else
		str.FormatMessage(IDS_CR_TTLCREUSETHYPO);

	
	dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE | DT_WORD_ELLIPSIS);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPCreusetHypo_T1::OnUpdate()
{
	// on redessine la fen�tre
	this->InvalidateRect(NULL, FALSE);
}

///////////////////////////////////////
// CATPCreusetHypo_T1 message handlers

void CATPCreusetHypo_T1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
