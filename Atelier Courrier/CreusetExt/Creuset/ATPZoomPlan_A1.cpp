// on inclut les définitins nécessaires
#include "stdafx.h"
#include "ATPZoomPlan_A1.h"
#include "ATPZoomPlan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPZoomPlan_A1::CATPZoomPlan_A1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;

}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPZoomPlan_A1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

void CATPZoomPlan_A1::SetParent(CATPZoomPlan* ATPZoomPlan)
{
	m_pZoom = ATPZoomPlan;
}


//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPZoomPlan_A1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

CATPZoomPlan_A1::~CATPZoomPlan_A1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPZoomPlan_A1, JFCGrid)
	//{{AFX_MSG_MAP(CATPZoomPlan_A1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////
// la fonction pour récupérer le GRP d'une insertion


/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPZoomPlan_A1::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on initialise le texte
	CString Text;

	// on teste la colonne
	if (i == 0)
	{
		// on calcule le rectangle
		RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;

		// on dessine le fond de la cellule
		dc.FillRect(&Rect, &BrushB);

		// on teste la ligne
		switch(j)
		{
		case 0:
			Text.Format(IDS_CR_NUMERO);break;
		case 1:
			Text.LoadString(IDS_CR_DATE);break;
		case 2:
			Text.LoadString(IDS_CR_COEFALPHA);break;
		case 3:
			Text.LoadString(IDS_CR_PARTBUDGET);break;
		case 4:
			Text.LoadString(IDS_CR_TARIF);break;
		case 5:
			if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
                Text.LoadString(IDS_CR_PARTGRP);
			else
				Text.LoadString(IDS_CR_PARTGRPPLUS);
			break;
		case 6:
			if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
                Text.LoadString(IDS_CR_APPORTGRP);
			else
				Text.LoadString(IDS_CR_APPORTGRPPLUS);
			break;
		case 7:
			Text.LoadString(IDS_CR_APPCOUV);break;
		case 8:
			if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
                Text.LoadString(IDS_CR_APPREPET);
			else
				Text.LoadString(IDS_CR_APPREPETPLUS);
			break;
		case 9:
			Text.LoadString(IDS_CR_APPCOUVM);break;
		}

		// on écrit le texte
		dc.DrawText(Text, &rect, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);		
	}
	else if (i == 1)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			// on récupère l'indice de l'hypothèse
			// on teste la ligne
			CString Fmt;
			switch(j)
			{
			case 0:
				if(m_pZoom->m_Numero)
					Text.Format(_T("%d"), m_pZoom->m_Numero);
				Fmt.Format(_T("### ### ### ### ### "));
				CFormatStr::FormatNum(Fmt, Text);
				break;
			case 1:
				if(m_pZoom->m_dateD != m_pZoom->m_dateD.GetMin())
					Text = CFormatStr::FormatDate(m_pZoom->m_dateD);
				break;
			case 2:
				if(m_pZoom->m_CoefAlpha != -1.0)
					Text.Format(_T("%.1f %% "), m_pZoom->m_CoefAlpha);
				Fmt.Format(_T("### ### ### ### ###,#"));
				CFormatStr::FormatNum(Fmt, Text);
				break;
			case 3:
				if(m_pZoom->m_partBubget != -1.0)
					Text.Format(_T("%.1f %% "), m_pZoom->m_partBubget);
				Fmt.Format(_T("### ### ### ### ###,#"));
				CFormatStr::FormatNum(Fmt, Text);
				break;
			case 4:
				if(m_pZoom->m_prix != -1.0)
					Text.Format(_T("%.0f"), m_pZoom->m_prix);
				Fmt.Format(_T("### ### ### ### ### € "));
				CFormatStr::FormatNum(Fmt, Text);
				break;
			case 5:
				if(m_pZoom->m_partXGRP != -1.0)
					Text.Format(_T("%.1f %% "), m_pZoom->m_partXGRP);
				Fmt.Format(_T("### ### ### ### ###,#"));
				CFormatStr::FormatNum(Fmt, Text);
				break;
			case 6:
				if(m_pZoom->m_pointsXGRP != -1.0)
					Text.Format(_T("%.1f "), m_pZoom->m_pointsXGRP);
				Fmt.Format(_T("### ### ### ### ###,#"));
				CFormatStr::FormatNum(Fmt, Text);
				break;
			case 7:
				if(m_pZoom->m_ApportCouv != -1.0)
					Text.Format(_T("%.1f %% "), m_pZoom->m_ApportCouv);
				Fmt.Format(_T("### ### ### ### ###,#"));
				CFormatStr::FormatNum(Fmt, Text);
				break;
			case 8:
				if(m_pZoom->m_ApportRepet != -1.0)
					Text.Format(_T("%.2f "), m_pZoom->m_ApportRepet);
				Fmt.Format(_T("### ### ### ### ###,##"));
				CFormatStr::FormatNum(Fmt, Text);
				break;
			case 9:
				if(m_pZoom->m_ApportCouvMem != -1.0)
					Text.Format(_T("%.1f %% "), m_pZoom->m_ApportCouvMem);
				Fmt.Format(_T("### ### ### ### ###,#"));
				CFormatStr::FormatNum(Fmt, Text);
				break;
			}

			dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
		}
	}
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPZoomPlan_A1::OnUpdate()
{
	// on redessine le composant
	this->InvalidateRect(NULL, FALSE);
}

//////////////////////////////////
// CATPZoomPlan_A1 message handlers

void CATPZoomPlan_A1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on récupère les dimensions du composant
	RECT Rect; this->GetClientRect(&Rect);

	// on initialise la taille des cellules
	this->SetHorzItemSize(((Rect.right  - Rect.left) + 1) / 2, TRUE, FALSE);
	this->SetVertItemSize(((Rect.bottom - Rect.top ) + 2) / 10, TRUE, FALSE);

	// on initialise le nombre de cellules
	this->SetHorzCount(2, FALSE);
	this->SetVertCount(10, FALSE);

}
