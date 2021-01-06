// on inclut les d�fintions n�cessaires
#include "stdafx.h"

#include "JFCStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCStatic::JFCStatic()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCStatic::~JFCStatic()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCStatic, JFControl)
	//{{AFX_MSG_MAP(JFCStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour dessiner l'�l�ment
/////////////////////////////////////////////////////////////////////////////

void JFCStatic::OnDrawItem(CDC & dc, RECT rect)
{
	CString Text;

	// on r�cup�re le texte
	this->GetWindowText(Text);

	// on �crit le texte
	if (!Text.IsEmpty()) dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

/////////////////////////////////////////////////////////////////////////////
// JFCStatic message handlers

void JFCStatic::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFControl::PreSubclassWindow();
}

void JFCStatic::OnPaint() 
{
	RECT Rect;

	// on r�cup�re la zone cliente
	this->GetClientRect(&Rect);

	// on r�cup�re les dimensions � dessiner
	LONG Larg = Rect.right  - Rect.left;
	LONG Haut = Rect.bottom - Rect.top;

	// on v�rifie les dimensions de la fen�tre
	if (Larg > 0 && Haut > 0)
	{
		// on initialise le DC
		CPaintDC Dc(this);

		// on cr�e le stylo pour le trac�
		CPen PenA(PS_SOLID, 0, CATPColors::GetColorSelect(m_CATPColorsApp));

		// on s�lectionne le stylo A
		CPen* pOldPen = Dc.SelectObject(&PenA);

		// on dessine le contour 2D
		Dc.MoveTo(Rect.left,      Rect.bottom - 1);
		Dc.LineTo(Rect.right - 1, Rect.bottom - 1);
		Dc.LineTo(Rect.right - 1, Rect.top    - 1);

		// on teste si la taille de la fen�tre est suffisante
		if (Haut > 1 && Larg > 1)
		{
			// on dessine le contour 2D
			Dc.MoveTo(Rect.left,  Rect.bottom - 1);
			Dc.LineTo(Rect.left,  Rect.top);
			Dc.LineTo(Rect.right, Rect.top);

			// on teste si la taille de la fen�tre est suffisante
			if (Haut > 2 && Larg > 2)
			{
				// on cr�e le pinceau
				CBrush BrushW(CATPColors::GetColorWhite());

				// on corrige le rectangle
				Rect.left += 1; Rect.top += 1; Rect.right -= 1; Rect.bottom -= 1;

				// on dessine le fond
				Dc.FillRect(&Rect, &BrushW);

				// on teste si la taille de la fen�tre est suffisante
				if (Haut > 3 && Larg > 7)
				{
					CFont FontS;

					// on corrige le rectangle
					Rect.right -= 6;

					// on cr�e la fonte
					FontS.CreatePointFont(CATPColors::GetFontSizeS(), CATPColors::GetFontName(), &Dc);

					// on s�lectionne la fonte L
					CFont* pOldFont = Dc.SelectObject(&FontS);

					// on modifie le mode de transparence
					LONG OldBack = Dc.SetBkMode(TRANSPARENT);

					// on modifie la couleur du texte
					LONG OldCol = Dc.SetTextColor(CATPColors::GetColorBlack());

					// on dessine l'�l�ment
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
