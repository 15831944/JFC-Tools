// on inclut les définitins nécessaires
#include "stdafx.h"
#include "ATPContrib_TXTA1.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPContrib_TXTA1::CATPContrib_TXTA1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}


//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPContrib_TXTA1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

CATPContrib_TXTA1::~CATPContrib_TXTA1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPContrib_TXTA1, JFCGrid)
	//{{AFX_MSG_MAP(CATPContrib_TXTA1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

JVoid CATPContrib_TXTA1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPContrib_TXTA1::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
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
			Text.LoadString(IDS_CR_CONTRIBBUDGET);
			break;
		case 1:
			if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
                Text.LoadString(IDS_CR_CONTRIBGRP);
			else
				Text.LoadString(IDS_CR_CONTRIBGRPPLUS);
			break;
		}

		// on écrit le texte
		dc.DrawText(Text, &rect, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);		
	}

}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPContrib_TXTA1::OnUpdate()
{
	// on redessine le composant
	this->InvalidateRect(NULL, FALSE);
}

//////////////////////////////////
// CATPContrib_TXTA1 message handlers

void CATPContrib_TXTA1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on récupère les dimensions du composant
	RECT Rect; this->GetClientRect(&Rect);

	// on initialise la taille des cellules
	this->SetHorzItemSize(((Rect.right  - Rect.left) + 1) / 1, TRUE, FALSE);
	this->SetVertItemSize(((Rect.bottom - Rect.top ) + 2) / 2, TRUE, FALSE);

	// on initialise le nombre de cellules
	this->SetHorzCount(1, FALSE);
	this->SetVertCount(2, FALSE);
}
