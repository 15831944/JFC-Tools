// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCFlatPane.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCFlatPane::JFCFlatPane(): JFCPane()
{
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCFlatPane::~JFCFlatPane()
{
}

BEGIN_MESSAGE_MAP(JFCFlatPane, JFCPane)
	//{{AFX_MSG_MAP(JFCFlatPane)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void JFCFlatPane::OnPaint() 
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

		// on crée les stylos
		CPen PenF(PS_SOLID, 0, CATPColors::GetColorMedium(m_CATPColorsApp));
		//CPen PenS(PS_SOLID, 0, ::GetSysColor(COLOR_3DSHADOW));
		//CPen PenH(PS_SOLID, 0, ::GetSysColor(COLOR_3DHILIGHT));
		//CPen PenD(PS_SOLID, 0, ::GetSysColor(COLOR_3DDKSHADOW));
		CPen PenS(PS_SOLID, 0, CATPColors::GetColorMedium(m_CATPColorsApp));
		CPen PenH(PS_SOLID, 0, CATPColors::GetColorMedium(m_CATPColorsApp));
		CPen PenD(PS_SOLID, 0, CATPColors::GetColorMedium(m_CATPColorsApp));

		// on sélectionne le stylo H
		CPen* pOldPen = Dc.SelectObject(&PenH);

		// on dessine le contour 3D
		Dc.MoveTo(Rect.left,      Rect.bottom - 1);
		Dc.LineTo(Rect.right - 1, Rect.bottom - 1);
		Dc.LineTo(Rect.right - 1, Rect.top    - 1);

		// on teste si la taille de la fenêtre est suffisante
		if (Haut > 1 && Larg > 1)
		{
			// on sélectionne le stylo S
			Dc.SelectObject(&PenS);
		
			// on dessine le contour 3D	
			Dc.MoveTo(Rect.left,      Rect.bottom - 2);
			Dc.LineTo(Rect.left,      Rect.top);
			Dc.LineTo(Rect.right - 1, Rect.top);

			// on teste si la taille de la fenêtre est suffisante
			if (Haut > 2 && Larg > 2)
			{
				// on sélectionne le stylo F
				Dc.SelectObject(&PenF);

				// on dessine le contour 3D
				Dc.MoveTo(Rect.left  + 1, Rect.bottom - 2);
				Dc.LineTo(Rect.right - 2, Rect.bottom - 2);
				Dc.LineTo(Rect.right - 2, Rect.top);

				// on teste si la taille de la fenêtre est suffisante
				if (Haut > 3 && Larg > 3)
				{
					// on sélectionne le stylo D
					Dc.SelectObject(&PenD);

					// on dessine le contour 3D
					Dc.MoveTo(Rect.left  + 1, Rect.bottom - 3);
					Dc.LineTo(Rect.left  + 1, Rect.top    + 1);
					Dc.LineTo(Rect.right - 2, Rect.top    + 1);

					// on teste si la taille de la fenêtre est suffisante
					if (Haut > 4 && Larg > 4)
					{
						// on crée le pinceau
						CBrush Brush(CATPColors::GetColorMedium(m_CATPColorsApp));

						// on corrige le rectangle
						Rect.left += 2; Rect.top += 2; Rect.right -= 2; Rect.bottom -= 2;

						// on dessine le fond
						Dc.FillRect(&Rect, &Brush);
					}
				}
			}
		}

		// on restaure le stylo initial
		Dc.SelectObject(pOldPen);
	}
}

