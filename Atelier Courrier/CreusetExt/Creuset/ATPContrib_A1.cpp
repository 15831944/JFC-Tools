// on inclut les définitins nécessaires
#include "stdafx.h"
#include "ATPContrib_A1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPContrib_A1::CATPContrib_A1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPContrib_A1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{		
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPContrib_A1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

CATPContrib_A1::~CATPContrib_A1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPContrib_A1, JFCGrid)
	//{{AFX_MSG_MAP(CATPContrib_A1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPContrib_A1::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on initialise le texte
	CString Text;
	CString Fmt;

	if (i == 0)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			switch(j)
			{
			case 0:
				//Text.Format("%d %d %d",m_pATPDocCreuset->m_AFFContribution.m_idHypo,m_pATPDocCreuset->m_AFFContribution.m_idFormat,m_pATPDocCreuset->m_AFFContribution.m_idSupport);
				Text = m_pATPDocCreuset->m_AFFContribution_New.GetValue(
					m_pATPDocCreuset->m_AFFContribution.m_idHypo+1,
					m_pATPDocCreuset->m_AFFContribution.m_idFormat,
					m_pATPDocCreuset->m_AFFContribution.m_idSupport,
					JATPAFFContrib_New::PARTBUDGET);
				//if(m_pATPDocCreuset->m_AFFContribution.m_PartBudget > 0.0)
				//{
				//	Text.Format(_T("%.1f %% "), m_pATPDocCreuset->m_AFFContribution.m_PartBudget);
				//	Fmt.Format(_T("### ### ### ### ###,#"));
				//	CFormatStr::FormatNum(Fmt, Text);
				//}
				break;
			case 1:
				Text = m_pATPDocCreuset->m_AFFContribution_New.GetValue(
					m_pATPDocCreuset->m_AFFContribution.m_idHypo+1,
					m_pATPDocCreuset->m_AFFContribution.m_idFormat,
					m_pATPDocCreuset->m_AFFContribution.m_idSupport,
					JATPAFFContrib_New::PARTGRP);
				//if(m_pATPDocCreuset->m_AFFContribution.m_PartXGRP > 0.0)
				//{
				//	Text.Format(_T("%.1f %% "), m_pATPDocCreuset->m_AFFContribution.m_PartXGRP);
				//	Fmt.Format(_T("### ### ### ### ###,#"));
				//	CFormatStr::FormatNum(Fmt, Text);
				//}
				break;

			//case 0:
			//	m_pATPDocCreuset->m_AFFContribution_New.GetValue(
			//	break;
			//case 1:

			//	break;
			}
		}
	}
	if (i == 1)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			switch(j)
			{
			case 0:
				Text = m_pATPDocCreuset->m_AFFContribution_New.GetValue(
					m_pATPDocCreuset->m_AFFContribution.m_idHypo+1,
					m_pATPDocCreuset->m_AFFContribution.m_idFormat,
					m_pATPDocCreuset->m_AFFContribution.m_idSupport,
					JATPAFFContrib_New::BUDGET);
				//if (m_pATPDocCreuset->m_AFFContribution.m_Prix > 0.0)
				//{
				//	if (m_pATPDocCreuset->m_AFFContribution.m_Prix > 1000000)
				//	{
				//		Text.Format(_T("%.0f"), m_pATPDocCreuset->m_AFFContribution.m_Prix/1000);
				//		Fmt.Format(_T("### ### ### ### ### K€ "));
				//	}
				//	else
				//	{
				//		Text.Format(_T("%.0f"), m_pATPDocCreuset->m_AFFContribution.m_Prix);
				//		Fmt.Format(_T("### ### ### ### ### € "));
				//	}
				//}
				//CFormatStr::FormatNum(Fmt, Text);
				break;
			case 1:
				Text = m_pATPDocCreuset->m_AFFContribution_New.GetValue(
					m_pATPDocCreuset->m_AFFContribution.m_idHypo+1,
					m_pATPDocCreuset->m_AFFContribution.m_idFormat,
					m_pATPDocCreuset->m_AFFContribution.m_idSupport,
					JATPAFFContrib_New::GRP);
				//if (m_pATPDocCreuset->m_AFFContribution.m_XGRP > 0.0)
				//{
				//	Text.Format(_T("%.1f "), m_pATPDocCreuset->m_AFFContribution.m_XGRP);
				//	Fmt.Format(_T("### ### ### ### ###,#"));
				//	CFormatStr::FormatNum(Fmt, Text);
				//}
				break;
			}
		}
	}
	// on écrit le texte
	dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPContrib_A1::OnUpdate()
{
	// on redessine le composant
	this->InvalidateRect(NULL, FALSE);
}

//////////////////////////////////
// CATPContrib_A1 message handlers

void CATPContrib_A1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on récupère les dimensions du composant
	RECT Rect; this->GetClientRect(&Rect);

	// on initialise la taille des cellules
	this->SetHorzItemSize(((Rect.right  - Rect.left) + 1) / 2, TRUE, FALSE);
	this->SetVertItemSize(((Rect.bottom - Rect.top ) + 2) / 2, TRUE, FALSE);

	// on initialise le nombre de cellules
	this->SetHorzCount(2, FALSE);
	this->SetVertCount(2, FALSE);
}
