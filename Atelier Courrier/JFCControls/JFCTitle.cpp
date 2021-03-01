// on inclut les défintions nécessaires
#include "stdafx.h"

#include "JFCTitle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCTitle::JFCTitle()
{
	// on ne fait rien
	m_crBorderSet = false;

	m_CATPColorsApp = CATPColors::COLORCREUSET;
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCTitle::~JFCTitle()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCTitle, JFControl)
	//{{AFX_MSG_MAP(JFCTitle)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour dessiner l'élément
/////////////////////////////////////////////////////////////////////////////

void JFCTitle::OnDrawItem(CDC & dc, RECT rect)
{
	CString Text;

	// on récupère le texte
	this->GetWindowText(Text);

	// on écrit le texte
	if (!Text.IsEmpty()) dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

/////////////////////////////////////////////////////////////////////////////
// JFCTitle message handlers

void JFCTitle::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFControl::PreSubclassWindow();
}

void JFCTitle::OnPaint() 
{
	RECT ClientRect, BorderRect;

	// on récupère la zone cliente
	this->GetClientRect(&ClientRect);

	// on copie le rectangle
	BorderRect = ClientRect;

	// on récupère les dimensions à dessiner
	LONG Larg = ClientRect.right  - ClientRect.left;
	LONG Haut = ClientRect.bottom - ClientRect.top;

	// on vérifie les dimensions de la fenêtre
	if (Larg > 0 && Haut > 0)
	{
		// on initialise le DC
		CPaintDC Dc(this);
		
		// on crée le pinceau
		// CBrush BrushA(CATPColors::GetColorDark(m_CATPColorsApp));

		//////////////////////////////////////////////////
		// TEST COULEUR DEGRADE
		/*
		CBitmap bm;
		bm.LoadBitmap(IDB_BLEU_DARK_DEGRADE);

		CBrush BrushA;
		BrushA.CreatePatternBrush(&bm);
		
		// on dessine le fond
		Dc.FillRect(&ClientRect, &BrushA);
		*/

		// const int NbGrad = 10;
		static COLORREF clr_degrad[CATPColors::m_NbGrad];
		CATPColors::GetColorDarkGraduation(m_CATPColorsApp, clr_degrad, CATPColors::m_NbGrad);
		CATPColors::FillRectGraduateWithSurround(Dc, ClientRect, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD, m_CATPColorsApp);

		// on teste si la taille de la fenêtre est suffisante
		if (Haut > 2 && Larg > 2)
		{
			CFont FontL;

			// on corrige le rectangle
			ClientRect.left += 1; ClientRect.top += 1; ClientRect.right -= 1; ClientRect.bottom -= 1;

			// on crée la fonte
			FontL.CreatePointFont(CATPColors::GetFontSizeL(), CATPColors::GetFontName(), &Dc);

			// on sélectionne la fonte L
			CFont* pOldFont = Dc.SelectObject(&FontL);

			// on modifie le mode de transparence
			LONG OldBack = Dc.SetBkMode(TRANSPARENT);

			// on modifie la couleur du texte
			// LONG OldCol = Dc.SetTextColor(CATPColors::GetColorWhite());
			LONG OldCol = Dc.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp));

			// on dessine l'élément
			this->OnDrawItem(Dc, ClientRect);

			// on dessine le bord
			if (m_crBorderSet) OnDrawBorder(Dc, BorderRect);

			// on restaure la couleur du texte
			Dc.SetTextColor(OldCol);

			// on restaure la transparence
			Dc.SetBkMode(OldBack);

			// on restaure la fonte
			Dc.SelectObject(pOldFont);
		}
	}
}


void JFCTitle::SetBorderColor(COLORREF crBorder)
{
	m_crBorderSet = true;
	m_crBorder = crBorder;

	CRect rc;
	GetWindowRect(rc);
	RedrawWindow();

	GetParent()->ScreenToClient(rc);
	GetParent()->InvalidateRect(rc, true);
	GetParent()->UpdateWindow();
}

/*
void JFCTitle::SetFontName(TCHAR* FontName)
{
	this.SetFont(FontName);
}

void JFCTitle::SetBorder(BOOL bSet)
{
	
}
*/

// la fonction pour indiquer l'application où le controle se place
void JFCTitle::SetAppColor(CATPColors::APPCOLOR AppColor)
{
	m_CATPColorsApp = AppColor;
}


void JFCTitle::OnDrawBorder(CDC & dc, RECT rc)
{
	CPen pen3Border		(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW));	// Border color
	CPen pen3HighLight	(PS_SOLID, 0, m_crBorder);						// High Light

	// on dessine les bords haut et gauche en foncé
	dc.SelectObject(&pen3Border);
	dc.MoveTo(rc.left , rc.bottom - 1);
	dc.LineTo(rc.left , rc.top);
	dc.LineTo(rc.right - 1	, rc.top);

	// on dessine les bords bas et droite en clair
	CPen* pOldPen = dc.SelectObject(&pen3HighLight);
	dc.MoveTo(rc.left +1	, rc.top + 1);
	dc.LineTo(rc.right - 1	, rc.top + 1);
	dc.LineTo(rc.right - 1	, rc.bottom - 1);					
	dc.LineTo(rc.left +1	, rc.bottom - 1);					
	dc.LineTo(rc.left +1	,  rc.top + 1);			

	dc.SelectObject(pOldPen);
}