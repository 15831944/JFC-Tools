#include "StdAfx.h"
#include ".\jfctruc.h"

JFCTruc::JFCTruc(void)
{
}

JFCTruc::~JFCTruc(void)
{
}

BEGIN_MESSAGE_MAP(JFCTruc, JFControl)
	//{{AFX_MSG_MAP(JFCTruc)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void JFCTruc::OnPaint() 
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

		CBrush Brush;
		Brush.CreateSolidBrush(CATPColors::GetColorWhite());
		// on dessine le fond
		Dc.FillRect(&Rect, &Brush);

		// on crée le stylo pour le tracé
		CPen PenA(PS_SOLID, 0, CATPColors::GetColorSelect(m_CATPColorsApp));

		// on sélectionne le stylo A
		CPen* pOldPen = Dc.SelectObject(&PenA);

		// on dessine le contour 2D
		Dc.MoveTo(Rect.left,      Rect.top );
		Dc.LineTo(Rect.left, Rect.bottom - 1);
		Dc.LineTo(Rect.right, Rect.bottom - 1);
	
		Dc.SelectObject(pOldPen);
		

	}
}
