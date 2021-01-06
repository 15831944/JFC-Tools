// on inclut les définitins nécessaires
#include "stdafx.h"
#include "ATPZoomPlan_A3.h"
#include "ATPZoomPlan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPZoomPlan_A3::CATPZoomPlan_A3()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}



///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPZoomPlan_A3::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

void CATPZoomPlan_A3::SetParent(CATPZoomPlan* ATPZoomPlan)
{
	m_pZoom = ATPZoomPlan;
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPZoomPlan_A3::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

CATPZoomPlan_A3::~CATPZoomPlan_A3()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPZoomPlan_A3, JFCGrid)
	//{{AFX_MSG_MAP(CATPZoomPlan_A3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////
// la fonction pour récupérer le GRP d'une insertion



/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPZoomPlan_A3::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
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
			Text.LoadString(IDS_CR_CTGRP);break;
		case 1:
			Text.LoadString(IDS_CR_CT1000I);break;
		case 2:
			Text.LoadString(IDS_CR_CT1000C);break;
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
			switch(j)
			{
			case 0:
				if(m_pZoom->m_CtGRP != -1.0)
					Text = CFormatStr::FormatCout(m_pZoom->m_CtGRP, m_pATPDocCreuset->m_NbDecCout);
				break;
			case 1:
				if(m_pZoom->m_CtInd != -1.0)
					Text = CFormatStr::FormatCout(m_pZoom->m_CtInd, m_pATPDocCreuset->m_NbDecCout);
				break;
			case 2:
				if(m_pZoom->m_CtCts != -1.0)
					Text = CFormatStr::FormatCout(m_pZoom->m_CtCts, m_pATPDocCreuset->m_NbDecCout);
				break;
			}

			dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
		}
	}
}


//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPZoomPlan_A3::OnUpdate()
{
	// on redessine le composant
	this->InvalidateRect(NULL, FALSE);
}

//////////////////////////////////
// CATPZoomPlan_A3 message handlers

void CATPZoomPlan_A3::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on récupère les dimensions du composant
	RECT Rect; this->GetClientRect(&Rect);

	// on initialise la taille des cellules
	this->SetHorzItemSize(((Rect.right  - Rect.left) + 1) / 2, TRUE, FALSE);
	this->SetVertItemSize(((Rect.bottom - Rect.top ) + 2) / 3, TRUE, FALSE);

	// on initialise le nombre de cellules
	this->SetVertCount(3, FALSE);
	this->SetHorzCount(2, FALSE);
}
