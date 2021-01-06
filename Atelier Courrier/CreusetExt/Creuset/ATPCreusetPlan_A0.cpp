// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPCreusetPlan_A0.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetPlan_A0::CATPCreusetPlan_A0()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetPlan_A0::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetPlan_A0::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetPlan_A0::~CATPCreusetPlan_A0()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetPlan_A0, JFCTitle)
	//{{AFX_MSG_MAP(CATPCreusetPlan_A0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'élément

JVoid CATPCreusetPlan_A0::OnDrawItem(CDC & dc, RECT rect)
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
	CString str;
	str = "Grille calendaire";
	if (m_pATPDocCreuset)
	{
		if (m_pATPDocCreuset->m_IdxHypotheseActif == 0)
			str.LoadString(IDS_CR_PLAN_M);
		else
		{
			m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseActif);
			if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
			{
				CPlanInsertions plan = m_pATPDocCreuset->m_TblHypotheses.GetItem();

				if (plan.GetLabel().IsEmpty())
					str.FormatMessage(IDS_CR_HYPOTHESE, m_pATPDocCreuset->m_IdxHypotheseActif);
				else
					str = plan.GetLabel().AsJCharPtr();
				
				//if (m_pATPDocCreuset->m_IdxHypotheseActif == 0)
				//	str.LoadString(IDS_CR_PLAN_E);//plan etendu
			}
		}
	}
	dc.DrawText(_T(str), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE | DT_WORD_ELLIPSIS);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetPlan_A0::OnUpdate()
{
	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPCreusetPlan_A0 message handlers

void CATPCreusetPlan_A0::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
