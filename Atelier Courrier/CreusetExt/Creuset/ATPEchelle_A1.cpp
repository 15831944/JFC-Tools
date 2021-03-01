// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPEchelle_Metriques.h"
#include "ATPEchelle_A1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEchelle_A1::CATPEchelle_A1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPEchelle_A1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPEchelle_A1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelle_A1::~CATPEchelle_A1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPEchelle_A1, JFCGrid)
	//{{AFX_MSG_MAP(CATPEchelle_A1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPEchelle_A1::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on calcule le rectangle
	RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;

	// on dessine le fond de la cellule
	// dc.FillRect(&Rect, &BrushB);
	static COLORREF clr_degrad[CATPColors::m_NbGrad];
	CATPColors::GetColorDarkGraduation(CATPColors::COLORCREUSET, clr_degrad, CATPColors::m_NbGrad);
	CATPColors::FillRectGraduateWithSurround(dc, Rect, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD, CATPColors::COLORCREUSET);

	// on écrit le titre
	CString str;
	str.FormatMessage(IDS_CR_SUPPORTS);
	// LONG OldCol = dc.SetTextColor(CATPColors::GetColorWhite());
	LONG OldCol = dc.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	dc.DrawText(_T(str), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	dc.SetTextColor(OldCol);

	CPen pen;
	CPen*  pOldPen  = dc.GetCurrentPen();
	pen.CreatePen(PS_SOLID, 1, CATPColors::GetColorBlack());
	dc.SelectObject(&pen);

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on vérifie la validité de l'étude
		if (m_pATPDocCreuset->IsValid())
		{
			// on teste le tri de la colonne
			if (m_pATPDocCreuset->m_AFFEchelle.IsSortBySupportsUp())
			{
				// on dessine la flèche vers le haut
				dc.MoveTo(Rect.right - 6, Rect.bottom - 6);
				dc.LineTo(Rect.right - 9, Rect.bottom - 3);
				dc.LineTo(Rect.right - 3, Rect.bottom - 3);
				dc.LineTo(Rect.right - 6, Rect.bottom - 6);
			}
			else if (m_pATPDocCreuset->m_AFFEchelle.IsSortBySupportsDn())
			{
				// on dessine la flèche vers le bas
				dc.MoveTo(Rect.right - 6, Rect.bottom - 3);
				dc.LineTo(Rect.right - 9, Rect.bottom - 6);
				dc.LineTo(Rect.right - 3, Rect.bottom - 6);
				dc.LineTo(Rect.right - 6, Rect.bottom - 3);
			}
		}
	}
	dc.SelectObject(pOldPen);
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPEchelle_A1::OnLButtonDown(LONG i, LONG j)
{
	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on vérifie la validité de l'étude
		if (m_pATPDocCreuset->IsValid())
		{
			// on modifie le tri de l'échelle
			m_pATPDocCreuset->m_AFFEchelle.SetSortBySupports();

			// on positionne l'indicateur de modification
//			m_pATFR05Doc->SetModifiedFlag(TRUE);

			// on met à jour les vues du document
			m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_ECHELLE);
		}
	}
}

JVoid CATPEchelle_A1::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPEchelle_A1::OnUpdate()
{
	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPEchelle_A1 message handlers

void CATPEchelle_A1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPEchelle_Metriques::CX_Ax(), TRUE, FALSE);
	this->SetVertItemSize(CATPEchelle_Metriques::CY_x1(), TRUE, FALSE);

	// on initialise le nombre de cellules
	this->SetHorzCount(1, FALSE);
	this->SetVertCount(1, FALSE);
}
