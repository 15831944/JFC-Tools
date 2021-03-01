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

		CBrush Brush;
		Brush.CreateSolidBrush(CATPColors::GetColorWhite());
		// on dessine le fond
		Dc.FillRect(&Rect, &Brush);

		// on cr�e le stylo pour le trac�
		CPen PenA(PS_SOLID, 0, CATPColors::GetColorSelect(m_CATPColorsApp));

		// on s�lectionne le stylo A
		CPen* pOldPen = Dc.SelectObject(&PenA);

		// on dessine le contour 2D
		Dc.MoveTo(Rect.left,      Rect.top );
		Dc.LineTo(Rect.left, Rect.bottom - 1);
		Dc.LineTo(Rect.right, Rect.bottom - 1);
	
		Dc.SelectObject(pOldPen);
		

	}
}
