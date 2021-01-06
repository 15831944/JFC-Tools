// on inclut les définitions nécessaires
#include "stdafx.h"

#include "JFCButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCButton::JFCButton()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCButton::~JFCButton()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCButton, JFControl)
	//{{AFX_MSG_MAP(JFCButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer l'état de l'élément
/////////////////////////////////////////////////////////////////////////////

LONG JFCButton::OnGetItemState()
{
	// on renvoie l'état de l'élément
	return (0);
}
	
/////////////////////////////////////////////////////////////////////////////
// les fonctions pour sélectionner l'élément
/////////////////////////////////////////////////////////////////////////////

bool JFCButton::OnBeginSelecting(LONG & etat)
{
	// on arrête la sélection de l'élément
	return (false);
}

void JFCButton::OnSelectItem(LONG etat)
{
	// on ne fait rien
}

void JFCButton::OnEndSelecting()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour dessiner l'élément
/////////////////////////////////////////////////////////////////////////////

void JFCButton::OnDrawItem(CDC & dc, RECT rect)
{
	CString Text;

	// on récupère le texte
	this->GetWindowText(Text);

	// on écrit le texte
	if (!Text.IsEmpty()) 
		dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

/////////////////////////////////////////////////////////////////////////////
// JFCButton message handlers

void JFCButton::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFControl::PreSubclassWindow();
}

void JFCButton::OnPaint() 
{
	RECT Rect;

	// on récupère la zone cliente
	this->GetClientRect(&Rect);

	// on récupère les dimensions à dessiner
	LONG Larg = Rect.right  - Rect.left;
	LONG Haut = Rect.bottom - Rect.top;

	// on vérifie les dimensions de la fenêtre
	if (Larg > 0 && Haut > 0)
	{
		// on initialise le DC
		CPaintDC Dc(this);

		// on crée le stylo pour le tracé
		CPen PenA(PS_SOLID, 0, CATPColors::GetColorBlack());

		// on sélectionne le stylo A
		CPen* pOldPen = Dc.SelectObject(&PenA);

		// on dessine le contour 2D
		Dc.MoveTo(Rect.left,      Rect.bottom - 1);
		Dc.LineTo(Rect.right - 1, Rect.bottom - 1);
		Dc.LineTo(Rect.right - 1, Rect.top    - 1);

		// on teste si la taille de la fenêtre est suffisante
		if (Haut > 1 && Larg > 1)
		{
			// on dessine le contour 2D
			Dc.MoveTo(Rect.left,  Rect.bottom - 1);
			Dc.LineTo(Rect.left,  Rect.top);
			Dc.LineTo(Rect.right, Rect.top);

			// on teste si la taille de la fenêtre est suffisante
			if (Haut > 2 && Larg > 2)
			{
				CBrush Brush;
				
				// on récupère l'état du bouton
				long State = this->OnGetItemState();

				/*
				CBitmap bm;
				bm.LoadBitmap(IDB_BLEU_DEGRADE);

				// Create a pattern brush from the bitmap.
				CBrush brush;
				brush.CreatePatternBrush(&bm);
				*/

				// on crée le pinceau
				if  (State == 0) 
					Brush.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
				else if (State == 1) 
					Brush.CreateSolidBrush(CATPColors::GetColorLight(m_CATPColorsApp));
				else if (State > 1)
					// pas d'effet validation bouton 
					Brush.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));
				else
					Brush.CreateSolidBrush(CATPColors::GetColorMedium(m_CATPColorsApp));

				// on corrige le rectangle
				Rect.left += 1; Rect.top += 1; Rect.right -= 1; Rect.bottom -= 1;

				// on dessine le fond
				Dc.FillRect(&Rect, &Brush);

				// on teste si la taille de la fenêtre est suffisante
				if (Haut > 3 && Larg > 3)
				{
					CFont FontS;

					// on corrige le rectangle
					Rect.left += 1; Rect.top += 1; Rect.right -= 1; Rect.bottom -= 1;

					// on crée la fonte
					FontS.CreatePointFont(CATPColors::GetFontSizeS(), CATPColors::GetFontName(), &Dc);

					// on sélectionne la fonte L
					CFont* pOldFont = Dc.SelectObject(&FontS);

					// on modifie le mode de transparence
					LONG OldBack = Dc.SetBkMode(TRANSPARENT);

					// on modifie la couleur du texte
					LONG OldCol = Dc.SetTextColor(CATPColors::GetColorBlack());

					// on dessine l'élément
					this->OnDrawItem(Dc, Rect);

					// on restaure la couleur du texte
					Dc.SetTextColor(OldCol);

					// on restaure la transparence
					Dc.SetBkMode(OldBack);

					// on restaure la fonte
					Dc.SelectObject(pOldFont);
				}
			}
		}

		// on restaure le stylo initial
		Dc.SelectObject(pOldPen);
	}
}

void JFCButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// on récupère l'état du bouton
	long State = this->OnGetItemState();

	// on appelle le gestionnaire de base
	this->JFControl::OnLButtonDown(nFlags, point);

	// on commence la sélection de l'élément
	if (this->OnBeginSelecting(State))
	{
		// on sélectionne l'élément
		this->OnSelectItem(State);

		// on termine la sélection de l'élément
		this->OnEndSelecting();

		// on redessine la fenêtre
		this->InvalidateRect(NULL, FALSE);
	}
}
