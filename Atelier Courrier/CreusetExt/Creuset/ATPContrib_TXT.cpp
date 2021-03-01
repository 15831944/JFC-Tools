// on inclut les définitins nécessaires
#include "stdafx.h"
#include "ATPContrib_TXT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "resource.h"
//////////////////
// le constructeur

CATPContrib_TXT::CATPContrib_TXT()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}


//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPContrib_TXT::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

CATPContrib_TXT::~CATPContrib_TXT()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPContrib_TXT, JFCGrid)
	//{{AFX_MSG_MAP(CATPContrib_TXT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

JVoid CATPContrib_TXT::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPContrib_TXT::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
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
			Text.LoadString(IDS_CR_PLMOINSINSER);
			break;
		case 1:
			Text.LoadString(IDS_CR_DELTACOUV);
			break;
		case 2:
			if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
                Text.LoadString(IDS_CR_DELTAGRP);
			else
				Text.LoadString(IDS_CR_DELTAGRPPLUS);
			break;
		case 3:
			Text.LoadString(IDS_CR_DELTACTGRP);
			break;
		case 4:
			if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
                Text.LoadString(IDS_CR_DELTAREPRISE);
			else
				Text.LoadString(IDS_CR_DELTAREPRISEPLUS);
			break;
		}

		// on écrit le texte
		dc.DrawText(Text, &rect, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);		
	}

}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPContrib_TXT::OnUpdate()
{
	// on redessine le composant
	this->InvalidateRect(NULL, FALSE);
}

//////////////////////////////////
// CATPContrib_TXT message handlers

void CATPContrib_TXT::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on récupère les dimensions du composant
	RECT Rect; this->GetClientRect(&Rect);

	// on initialise la taille des cellules
	this->SetHorzItemSize(((Rect.right  - Rect.left) + 1) / 1, TRUE, FALSE);
	this->SetVertItemSize(((Rect.bottom - Rect.top ) + 2) / 5, TRUE, FALSE);

	// on initialise le nombre de cellules
	this->SetHorzCount(1, FALSE);
	this->SetVertCount(5, FALSE);
}
