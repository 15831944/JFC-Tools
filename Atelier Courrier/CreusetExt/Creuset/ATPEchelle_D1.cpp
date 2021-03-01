// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPEchelle_Metriques.h"
#include "ATPEchelle_Grandeurs.h"
#include "ATPEchelle_D1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEchelle_D1::CATPEchelle_D1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPEchelle_D1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	m_grandeurs.SetDocument(pATPDocCreuset); 

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPEchelle_D1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelle_D1::~CATPEchelle_D1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPEchelle_D1, JFCGrid)
	//{{AFX_MSG_MAP(CATPEchelle_D1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPEchelle_D1::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on initialise le tri
	JInt32 Sort = 0;

	// on calcule le rectangle
	RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;

	// on dessine le fond de la cellule
	// dc.FillRect(&Rect, &BrushB);
	// LONG OldCol = dc.SetTextColor(CATPColors::GetColorWhite());

	static COLORREF clr_degrad[CATPColors::m_NbGrad];
	CATPColors::GetColorDarkGraduation(CATPColors::COLORCREUSET, clr_degrad, CATPColors::m_NbGrad);

	// CATPColors::FillRectGraduate(dc, Rect, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD);
	CATPColors::FillRectGraduateWithSurround(dc, Rect, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD, CATPColors::COLORCREUSET);

	LONG OldCol = dc.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	CString str;
	// on teste la grandeur à afficher
	if (m_grandeurs.IsXGRP(i) != FALSE)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			// on vérifie la validité de l'étude
			if (m_pATPDocCreuset->IsValid())
			{
				// on récupère le tri courant
				if (m_pATPDocCreuset->m_AFFEchelle.IsSortByGRPUp()) Sort = +1; else if (m_pATPDocCreuset->m_AFFEchelle.IsSortByGRPDn()) Sort = -1;
			}
		}

		// on écrit le libellé
		str.LoadString(IDS_CR_GRPACTIF);
		dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
	else if (m_grandeurs.IsGRP(i) != FALSE)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			// on vérifie la validité de l'étude
			if (m_pATPDocCreuset->IsValid())
			{
				// on récupère le tri courant
				if (m_pATPDocCreuset->m_AFFEchelle.IsSortByLDPUp()) Sort = +1; else if (m_pATPDocCreuset->m_AFFEchelle.IsSortByLDPDn()) Sort = -1;
			}
		}

		// on écrit le libellé
		str.LoadString(IDS_CR_GRP);
		dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
	else if (m_grandeurs.IsCoutGRP(i) != FALSE)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			// on vérifie la validité de l'étude
			if (m_pATPDocCreuset->IsValid())
			{
				// on récupère le tri courant
				if (m_pATPDocCreuset->m_AFFEchelle.IsSortByCoutsGRPUp()) Sort = +1; else if (m_pATPDocCreuset->m_AFFEchelle.IsSortByCoutsGRPDn()) Sort = -1;
			}
		}

		// on écrit le libellé
		str.LoadString(IDS_CR_CTGRP);
		dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
	else if (m_grandeurs.IsRepetition(i) != FALSE)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			// on vérifie la validité de l'étude
			if (m_pATPDocCreuset->IsValid())
			{
				// on récupère le tri courant
				if (m_pATPDocCreuset->m_AFFEchelle.IsSortByRepetitionsUp()) Sort = +1; else if (m_pATPDocCreuset->m_AFFEchelle.IsSortByRepetitionsDn()) Sort = -1;
			}
		}

		// on écrit le libellé
		if(m_pATPDocCreuset->m_bRepriseEnMain)
			str.LoadString(IDS_CR_REPETPLUS);
		else
			str.LoadString(IDS_CR_NBJOURSLECT);
//		str.FormatMessage(IDS_CR_NBJOURSLECT);
		dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
	else if (m_grandeurs.IsNbIndividus(i) != FALSE)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			// on vérifie la validité de l'étude
			if (m_pATPDocCreuset->IsValid())
			{
				// on récupère le tri courant
				if (m_pATPDocCreuset->m_AFFEchelle.IsSortByNbIndividusUp()) Sort = +1; else if (m_pATPDocCreuset->m_AFFEchelle.IsSortByNbIndividusDn()) Sort = -1;
			}
		}

		// on écrit le libellé
		str.LoadString(IDS_CR_AUDIENCE_ABR);
		dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
	else if (m_grandeurs.IsNbContacts(i) != FALSE)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			// on vérifie la validité de l'étude
			if (m_pATPDocCreuset->IsValid())
			{
				// on récupère le tri courant
				if (m_pATPDocCreuset->m_AFFEchelle.IsSortByNbContactsUp()) Sort = +1; else if (m_pATPDocCreuset->m_AFFEchelle.IsSortByNbContactsDn()) Sort = -1;
			}
		}

		// on écrit le libellé
		if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
			str.LoadString(IDS_CR_CONTACTS_ABR);
		else
 			str.LoadString(IDS_CR_CONTACTSPLUS_ABR);
		
		dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
	else if (m_grandeurs.IsIndAffinite(i) != FALSE)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			// on vérifie la validité de l'étude
			if (m_pATPDocCreuset->IsValid())
			{
				// on récupère le tri courant
				if (m_pATPDocCreuset->m_AFFEchelle.IsSortByIndAffiniteUp()) Sort = +1; else if (m_pATPDocCreuset->m_AFFEchelle.IsSortByIndAffiniteDn()) Sort = -1;
			}
		}

		// on écrit le libellé
		str.LoadString(IDS_CR_INDAFFINITE);
		dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
	else if (m_grandeurs.IsPrcAffinite(i) != FALSE)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			// on vérifie la validité de l'étude
			if (m_pATPDocCreuset->IsValid())
			{
				// on récupère le tri courant
				if (m_pATPDocCreuset->m_AFFEchelle.IsSortByPrcAffiniteUp()) Sort = +1; else if (m_pATPDocCreuset->m_AFFEchelle.IsSortByPrcAffiniteDn()) Sort = -1;
			}
		}

		// on écrit le libellé
		str.LoadString(IDS_CR_PRCAFFINITE);
		dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}

	else if (m_grandeurs.IsApport(i) != FALSE)
	{
		// on vérifie la validité du document
		if (m_pATPDocCreuset != 0)
		{
			// on vérifie la validité de l'étude
			if (m_pATPDocCreuset->IsValid())
			{
				// on récupère le tri courant
				if (m_pATPDocCreuset->m_AFFEchelle.IsSortByApportUp()) 
					Sort = +1; 
				else if (m_pATPDocCreuset->m_AFFEchelle.IsSortByApportDn()) 
					Sort = -1;
			}
		}

		// on écrit le libellé
		str.LoadString(IDS_CR_APPORT_COUV);
		
		dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}

	CPen pen;
	CPen*  pOldPen  = dc.GetCurrentPen();
	pen.CreatePen(PS_SOLID, 1, CATPColors::GetColorBlack());
	dc.SelectObject(&pen);
	// on teste le tri de la colonne
	if (Sort > 0)
	{
		// on dessine la flèche vers le haut
		dc.MoveTo(Rect.right - 6, Rect.bottom - 6);
		dc.LineTo(Rect.right - 9, Rect.bottom - 3);
		dc.LineTo(Rect.right - 3, Rect.bottom - 3);
		dc.LineTo(Rect.right - 6, Rect.bottom - 6);
	}
	else if (Sort < 0)
	{
		// on dessine la flèche vers le bas
		dc.MoveTo(Rect.right - 6, Rect.bottom - 3);
		dc.LineTo(Rect.right - 9, Rect.bottom - 6);
		dc.LineTo(Rect.right - 3, Rect.bottom - 6);
		dc.LineTo(Rect.right - 6, Rect.bottom - 3);
	}
	dc.SelectObject(pOldPen);
	dc.SetTextColor(OldCol);
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPEchelle_D1::OnLButtonDown(LONG i, LONG j)
{
	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on vérifie la validité de l'étude
		if (m_pATPDocCreuset->IsValid())
		{
			// on modifie le tri de l'échelle
			if      (m_grandeurs.IsXGRP(i)         != FALSE)	
				m_pATPDocCreuset->m_AFFEchelle.SetSortByGRP();
			else if (m_grandeurs.IsGRP(i)			!= FALSE)	
				m_pATPDocCreuset->m_AFFEchelle.SetSortByLDP();
			else if (m_grandeurs.IsCoutGRP(i)		!= FALSE)	
				m_pATPDocCreuset->m_AFFEchelle.SetSortByCoutsGRP();
			else if (m_grandeurs.IsRepetition(i)	!= FALSE)	
				m_pATPDocCreuset->m_AFFEchelle.SetSortByRepetitions();
			else if (m_grandeurs.IsNbIndividus(i)	!= FALSE)	
				m_pATPDocCreuset->m_AFFEchelle.SetSortByNbIndividus();
			else if (m_grandeurs.IsNbContacts(i)	!= FALSE)	
				m_pATPDocCreuset->m_AFFEchelle.SetSortByNbContacts();
			else if (m_grandeurs.IsIndAffinite(i)	!= FALSE)	
				m_pATPDocCreuset->m_AFFEchelle.SetSortByIndAffinite();
			else if (m_grandeurs.IsPrcAffinite(i)	!= FALSE)	
				m_pATPDocCreuset->m_AFFEchelle.SetSortByPrcAffinite();
			else if (m_grandeurs.IsApport(i)		!= FALSE)	
				m_pATPDocCreuset->m_AFFEchelle.SetSortByApport();


			// on positionne l'indicateur de modification
//			m_pATPDocCreuset->SetModifiedFlag(TRUE);

			// on met à jour les vues du document
			m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_ECHELLE);
		}
	}
}

JVoid CATPEchelle_D1::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPEchelle_D1::OnUpdate()
{
	// on initialise le nombre d'éléments
	LONG cx = 0;
	LONG cy = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on récupère le nombre d'éléments
		cx = m_grandeurs.GetNbGrandeurs();
		cy = 1;
	}

	// on met à jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);
}

/////////////////////////////////////
// CATPEchelle_D1 message handlers

void CATPEchelle_D1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPEchelle_Metriques::CX_Dx(), TRUE, FALSE);
	this->SetVertItemSize(CATPEchelle_Metriques::CY_x1(), TRUE, FALSE);
}
