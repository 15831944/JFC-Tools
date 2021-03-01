/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//													COLONNE TITRE TARIFICATION DES SUPPORTS
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPEchelle_Metriques.h"
#include "ATPEchelle_C1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEchelle_C1::CATPEchelle_C1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPEchelle_C1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPEchelle_C1::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelle_C1::~CATPEchelle_C1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPEchelle_C1, JFCGrid)
	//{{AFX_MSG_MAP(CATPEchelle_C1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'int�rieur des cellules

JVoid CATPEchelle_C1::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on calcule le rectangle
	RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;

	// on dessine le fond de la cellule
	// dc.FillRect(&Rect, &BrushB);
	// const int NbGrad = 10;
	static COLORREF clr_degrad[CATPColors::m_NbGrad];
	CATPColors::GetColorDarkGraduation(CATPColors::COLORCREUSET, clr_degrad, CATPColors::m_NbGrad);
	// CATPColors::FillRectGraduate(dc, Rect, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD);
	CATPColors::FillRectGraduateWithSurround(dc, Rect, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD, CATPColors::COLORCREUSET);

	// on �crit le titre
	CString TxtTarif;
	TxtTarif.LoadString(IDS_CR_TARIF);
	LONG OldCol = dc.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));
	dc.DrawText(_T(TxtTarif), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	dc.SetTextColor(OldCol);

	CPen pen;
	CPen*  pOldPen  = dc.GetCurrentPen();
	pen.CreatePen(PS_SOLID, 1, CATPColors::GetColorBlack());
	dc.SelectObject(&pen);

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{
		// on v�rifie la validit� de l'�tude
		if (m_pATPDocCreuset->IsValid())
		{
			// on teste le tri de la colonne
			if (m_pATPDocCreuset->m_AFFEchelle.IsSortByTarifsUp())
			{
				// on dessine la fl�che vers le haut
				dc.MoveTo(Rect.right - 6, Rect.bottom - 6);
				dc.LineTo(Rect.right - 9, Rect.bottom - 3);
				dc.LineTo(Rect.right - 3, Rect.bottom - 3);
				dc.LineTo(Rect.right - 6, Rect.bottom - 6);
			}
			else if (m_pATPDocCreuset->m_AFFEchelle.IsSortByTarifsDn())
			{
				// on dessine la fl�che vers le bas
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

JVoid CATPEchelle_C1::OnLButtonDown(LONG i, LONG j)
{
	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{
		// on v�rifie la validit� de l'�tude
		if (m_pATPDocCreuset->IsValid())
		{
			// on modifie le tri de l'�chelle
			m_pATPDocCreuset->m_AFFEchelle.SetSortByTarifs();

			// on positionne l'indicateur de modification
//			m_pATPDocCreuset->SetModifiedFlag(TRUE);

			// on met � jour les vues du document
			m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_ECHELLE);
		}
	}
}

JVoid CATPEchelle_C1::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPEchelle_C1::OnUpdate()
{
	// on redessine la fen�tre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPEchelle_C1 message handlers

void CATPEchelle_C1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPEchelle_Metriques::CX_Cx(), TRUE, FALSE);
	this->SetVertItemSize(CATPEchelle_Metriques::CY_x1(), TRUE, FALSE);

	// on initialise le nombre de cellules
	this->SetHorzCount(1, FALSE);
	this->SetVertCount(1, FALSE);
}
