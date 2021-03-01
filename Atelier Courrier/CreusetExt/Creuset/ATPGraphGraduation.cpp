// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCGraduation.h"
#include "ATPGraphGraduation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPGraphGraduation::CATPGraphGraduation()
{
	// on initialise les maximums
	m_VALMax = 0.0;

	m_bpourcent = false;
}



void CATPGraphGraduation::SetPourcent(bool set)
{
	m_bpourcent = set;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPGraphGraduation::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPGraphGraduation::~CATPGraphGraduation()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPGraphGraduation, JFControl)
	//{{AFX_MSG_MAP(CATPGraphGraduation)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner l'axe de l'histogramme
/////////////////////////////////////////////////////////////////////////////

JBool CATPGraphGraduation::OnBeginDrawing(CDC & dc)
{
	// on crée la fonte et le stylo
	m_Font.CreateFont(-MulDiv(8, dc.GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	m_PenG.CreatePen(PS_SOLID, 0, CATPColors::GetColorDark(CATPColors::GREY_MOUSE));

	// on continue le dessin des éléments
	return (true);
}

JVoid CATPGraphGraduation::OnDrawAxe(CDC & dc, JInt32 x1, JInt32 y1, JInt32 x2, JInt32 y2)
{
	// on sélectionne le stylo
	CPen* pOldPen = (CPen*)dc.SelectObject(&m_PenG);

	// on trace l'axe de l'histogramme
	dc.MoveTo(x1, y1); dc.LineTo(x2, y1); dc.LineTo(x2, y2 - 10);
	dc.MoveTo(x2, y2 - 10); dc.LineTo(x2 - 4, y2 - 5);
	dc.MoveTo(x2, y2 - 10); dc.LineTo(x2 + 4, y2 - 5);

	// on restaure le stylo
	dc.SelectObject(pOldPen);
}

JVoid CATPGraphGraduation::OnDrawGraduation(CDC & dc, JInt32 x1, JInt32 y1, JInt32 x2, JInt32 y2)
{
	// on modifie la transparence
	JInt32 OldBck = dc.SetBkMode(TRANSPARENT);

	// on modifie la couleur du texte
	JInt32 OldCol = dc.SetTextColor(RGB(96,96,96));

	// on sélectionne le stylo
	CPen* pOldPen = (CPen*)dc.SelectObject(&m_PenG);

	// on sélectionne la fonte
	CFont* pOldFont = (CFont*)dc.SelectObject(&m_Font);

	// on construit les graduations
	JFCGraduation Grad; Grad.SetRange(0.0, m_VALMax);

	// on récupère le nombre de graduations
	JInt32 Taille = Grad.GetNbGraduation();

	// on trace toutes les graduations
	for (JInt32 indice = 0; indice < Taille; indice += 1)
	{
		// on récupère la graduation
		JFlt64 Grd = Grad.GetGraduation(indice);

		// on récupère le libellé
		CString Lib = Grad.GetLibGraduation(indice);
		if(m_bpourcent)
			Lib += "%";

		// on récupère la taille du texte
		CSize Size = dc.GetTextExtent(Lib);

		// on calcule l'ordonnée de la graduation
		JInt32 Ord = y1 - (JInt32)JMath::Round((Grd * (JFlt64)(y1 - y2)) / Grad.GetMax());

		// on écrit le libellé
		dc.TextOut(x2 - Size.cx - 5, Ord - (Size.cy / 2), Lib);

		// on trace la graduation
		dc.MoveTo(x2 - 3, Ord); dc.LineTo(x2 + 3, Ord);
	}

	// on restaure la fonte
	dc.SelectObject(pOldFont);

	// on restaure le stylo
	dc.SelectObject(pOldPen);

	// on restaure la couleur du texte
	dc.SetTextColor(OldCol);

	// on restaure la transparence
	dc.SetBkMode(OldBck);
}

JVoid CATPGraphGraduation::OnDrawBorder(CDC & dc, JInt32 x1, JInt32 y1, JInt32 x2, JInt32 y2)
{
	// on récupère les dimensions à dessiner
	LONG Larg = x2 - x1;
	LONG Haut = y2 - y1;

	// on vérifie les dimensions de la fenêtre
	if (Larg > 0 && Haut > 0)
	{
		// on crée le stylo
		CPen PenA(PS_SOLID, 0, CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

		// on sélectionne le stylo
		CPen* pOldPen = (CPen*)dc.SelectObject(&PenA);

		// on trace le trait horizontal
		dc.MoveTo(x1, y1); dc.LineTo(x2, y1);

		// on vérifie les dimensions de la fenêtre
		if (Haut > 1)
		{
			// on trace les traits suivants
			dc.MoveTo(x1, y1); dc.LineTo(x1, y2 - 1); dc.LineTo(x2, y2 - 1);
		}

		// on restaure le stylo
		dc.SelectObject(pOldPen);
	}
}

JVoid CATPGraphGraduation::OnEndDrawing(CDC & dc)
{
	// on détruit les objets graphiques
	m_Font.DeleteObject();
	m_PenG.DeleteObject();
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPGraphGraduation::OnUpdate()
{
	this->InvalidateRect(NULL, FALSE);
}

////////////////////////////////////
// CATPGraphGraduation message handlers

void CATPGraphGraduation::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFControl::PreSubclassWindow();
}

void CATPGraphGraduation::OnPaint() 
{
	CDC DcComp;
	RECT Client;
	CBitmap BmpNew;

	// on crée un contexte de périphérique
	CPaintDC Dc(this);

	// on récupère la zone cliente
	this->GetClientRect(&Client);

	// on récupère la position de la zone à dessiner
	LONG x = Dc.m_ps.rcPaint.left;
	LONG y = Dc.m_ps.rcPaint.top;

	// on calcule la taille de la zone à dessiner
	LONG dx = Dc.m_ps.rcPaint.right  - Dc.m_ps.rcPaint.left;
	LONG dy = Dc.m_ps.rcPaint.bottom - Dc.m_ps.rcPaint.top;

	// on crée un contexte de périphérique compatible
	if (FALSE != DcComp.CreateCompatibleDC(&Dc))
	{
		// on crée une bitmap compatible avec ce contexte de périphérique
		if (FALSE != BmpNew.CreateCompatibleBitmap(&Dc, dx, dy))
		{
			// on sélectionne la bitmap dans le contexte de périphérique
			CBitmap* pBmpOld = (CBitmap*)DcComp.SelectObject(&BmpNew);

			// on vide la bitmap
			DcComp.BitBlt(0, 0, dx, dy, NULL, 0, 0, WHITENESS);

			// on commence le dessin
			if (this->OnBeginDrawing(DcComp))
			{
				// on calcule la position de l'axe
				LONG x1 = Client.right  - x;
				LONG x2 = Client.right  - x -  4;
				LONG y1 = Client.bottom - y -  6;
				LONG y2 = Client.top    - y + 14;

				// on trace l'axe vertical
				this->OnDrawAxe(DcComp, x1, y1, x2, y2);

				// on trace les graduations
				this->OnDrawGraduation(DcComp, x1, y1, x2, y2);

				// on met fin au dessin
				this->OnEndDrawing(DcComp);
			}

			// on trace les bordures
			this->OnDrawBorder(DcComp, -x, -y, Client.right - Client.left - x, Client.bottom - Client.top - y); 

			// on copie la bitmap dans la zone à dessiner de la fenêtre
			Dc.BitBlt(x, y, dx, dy, &DcComp, 0, 0, SRCCOPY);

			// on restaure la bitmap initiale
			pBmpOld = DcComp.SelectObject(pBmpOld);
		}

		// on détruit le contexte de périphérique compatible
		DcComp.DeleteDC();
	}
}

void CATPGraphGraduation::SetMax(JFlt64 max)
{
	m_VALMax = max;
	this->OnUpdate();
}

JFlt64 CATPGraphGraduation::GetMax() const
{
	return m_VALMax;
}
