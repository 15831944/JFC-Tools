// on inclut les définitions nécessaires
#include "stdafx.h"

#include "JFCListBoxM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur

JFCListBoxM::JFCListBoxM()
{
	// on initialise le nombre d'éléments
	m_Count = 0;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le nombre d'éléments
/////////////////////////////////////////////////////////////////////////////

void JFCListBoxM::SetCount(LONG nombre, BOOL update)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on initialise le nombre d'éléments
	m_Count = (nombre > 0) ? nombre : 0;

	// on initialise l'intervalle des éléments
	this->SetVertItemRange(0, (nombre > 0) ? (nombre - 1) : 0);

	// on redessine la fenêtre
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);
}

LONG JFCListBoxM::GetCount()
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on renvoie le nombre d'éléments
	return (m_Count);
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur

JFCListBoxM::~JFCListBoxM()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCListBoxM, JFCViewportSelectEx)
	//{{AFX_MSG_MAP(JFCListBoxM)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer le libellé des éléments
/////////////////////////////////////////////////////////////////////////////

const TCHAR* JFCListBoxM::OnGetItemLibelle(LONG indice)
{
	// on renvoie le libellé de l'élément
	return (NULL);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer le sélection des éléments
/////////////////////////////////////////////////////////////////////////////

LONG JFCListBoxM::OnGetItemSelection(LONG indice)
{
	// on renvoie la sélection de l'élément
	return (0);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour sélectionner les éléments
/////////////////////////////////////////////////////////////////////////////

bool JFCListBoxM::OnBeginSelecting(LONG & etat)
{
	// on arrête la sélection des éléments
	return (false);
}

void JFCListBoxM::OnSelectItem(LONG indice, LONG etat)
{
	// on ne fait rien
}

void JFCListBoxM::OnEndSelecting()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les éléments
/////////////////////////////////////////////////////////////////////////////

bool JFCListBoxM::OnBeginDrawing(CDC & dc)
{
	// on teste si on a quelque chose à dessiner
	if (m_Count == 0) return (false);

	// on crée le stylo
	m_PenG.CreatePen(PS_SOLID, 1, CATPColors::GetColorMedium(CATPColors::GREY_MOUSE));

	// on crée le pinceau
	m_BrushB.CreateSolidBrush(CATPColors::GetColorSelect(m_CATPColorsApp));

	// on crée la fonte
	m_FontS.CreatePointFont(CATPColors::GetFontSizeS(), CATPColors::GetFontName(), &dc);
	
	// on continue le dessin des éléments
	return (true);
}

void JFCListBoxM::OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat)
{
	// on récupère la position de l'élément
	LONG x1 = rect.left;
	LONG x2 = rect.left + 7 * ((rect.right - rect.left) / 8);
	LONG x3 = rect.right - 1;
	LONG y1 = rect.top;
	LONG y2 = rect.top + (rect.bottom - rect.top) / 2;
	LONG y3 = rect.bottom - 1;

	// on sélectionne le stylo G et le pinceau B
	CPen*   pOldPen   = dc.SelectObject(&m_PenG);
	CBrush* pOldBrush = dc.SelectObject(&m_BrushB);
	m_ColorText = CATPColors::GetColorBlack(); 

	// on teste l'état
	switch (etat)
	{
	case 1:
		// on remplit le fond de l'élément
		m_ColorText = CATPColors::GetColorWhite(); 
		dc.BeginPath();
		dc.MoveTo(x1, y1);
		dc.LineTo(x3 + 1, y1); dc.LineTo(x3 + 1, y3 + 1);
		dc.LineTo(x1, y3 + 1); dc.LineTo(x1, y1);
		dc.EndPath(); dc.FillPath();
		break;
	case 2:
		// on remplit le fond de l'élément
		m_ColorText = CATPColors::GetColorWhite(); 
		dc.BeginPath();
		dc.MoveTo(x3, y1); dc.LineTo(x3, y3 - 1);
		dc.LineTo(x2, y2); dc.LineTo(x3, y1);
		dc.EndPath(); dc.StrokeAndFillPath();
		break;
	default:
		// on ne fait rien
		break;
	}

	// on restaure le stylo et le pinceau
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
}

void JFCListBoxM::OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j)
{
	RECT cadre;

	// on initialise le cadre
	cadre.left   = rect.left   + 4;
	cadre.top    = rect.top    + 1;
	cadre.right  = rect.right  - 4;
	cadre.bottom = rect.bottom - 2;

	// on sélectionne la fonte S
	CFont* pOldFont = dc.SelectObject(&m_FontS);

	// on modifie la couleur du texte
	LONG OldCol = dc.SetTextColor(CATPColors::GetColorBlack());//m_ColorText);

	// on modifie le mode de transparence
	LONG OldBack = dc.SetBkMode(TRANSPARENT);

	// on récupère le libellé de l'élément
	const TCHAR* pLibelle = this->OnGetItemLibelle(j);

	// on écrit le libellé
	if (pLibelle != NULL) dc.DrawText(pLibelle, &cadre, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	// on restaure le mode de transparence
	dc.SetBkMode(OldBack);

	// on restaure la couleur du texte
	dc.SetTextColor(OldCol);

	// on restaure la fonte
	dc.SelectObject(pOldFont);
}

void JFCListBoxM::OnEndDrawing(CDC & dc)
{
	// on détruit les objets graphiques
	m_FontS.DeleteObject();
	m_BrushB.DeleteObject();
	m_PenG.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour masquer la dimension horizontale
/////////////////////////////////////////////////////////////////////////////

LONG JFCListBoxM::OnGetItemSelection(LONG i, LONG j)
{
	// on renvoie l'état de l'élément
	return (this->OnGetItemSelection(j));
}

void JFCListBoxM::OnSelectItem(LONG i, LONG j, LONG etat)
{
	// on sélectionne l'élément
	this->OnSelectItem(j, etat);
}

/////////////////////////////////////////////////////////////////////////////
// JFCListBoxM message handlers

void JFCListBoxM::PreSubclassWindow() 
{
	// on initialise le nombre d'éléments
	m_Count = 0;

	// on appelle le gestionnaire de base
	this->JFCViewportSelectEx::PreSubclassWindow();

	// on initialise la taille des éléments
	this->SetHorzItemSize( 0, TRUE, FALSE);
	this->SetVertItemSize(16, TRUE, FALSE);
}
