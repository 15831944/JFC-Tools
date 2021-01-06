// on inclut les d�finitions n�cessaires
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

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPGraphGraduation::Update()
{
	// on met � jour le composant
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
	// on cr�e la fonte et le stylo
	m_Font.CreateFont(-MulDiv(8, dc.GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	m_PenG.CreatePen(PS_SOLID, 0, CATPColors::GetColorDark(CATPColors::GREY_MOUSE));

	// on continue le dessin des �l�ments
	return (true);
}

JVoid CATPGraphGraduation::OnDrawAxe(CDC & dc, JInt32 x1, JInt32 y1, JInt32 x2, JInt32 y2)
{
	// on s�lectionne le stylo
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

	// on s�lectionne le stylo
	CPen* pOldPen = (CPen*)dc.SelectObject(&m_PenG);

	// on s�lectionne la fonte
	CFont* pOldFont = (CFont*)dc.SelectObject(&m_Font);

	// on construit les graduations
	JFCGraduation Grad; Grad.SetRange(0.0, m_VALMax);

	// on r�cup�re le nombre de graduations
	JInt32 Taille = Grad.GetNbGraduation();

	// on trace toutes les graduations
	for (JInt32 indice = 0; indice < Taille; indice += 1)
	{
		// on r�cup�re la graduation
		JFlt64 Grd = Grad.GetGraduation(indice);

		// on r�cup�re le libell�
		CString Lib = Grad.GetLibGraduation(indice);
		if(m_bpourcent)
			Lib += "%";

		// on r�cup�re la taille du texte
		CSize Size = dc.GetTextExtent(Lib);

		// on calcule l'ordonn�e de la graduation
		JInt32 Ord = y1 - (JInt32)JMath::Round((Grd * (JFlt64)(y1 - y2)) / Grad.GetMax());

		// on �crit le libell�
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
	// on r�cup�re les dimensions � dessiner
	LONG Larg = x2 - x1;
	LONG Haut = y2 - y1;

	// on v�rifie les dimensions de la fen�tre
	if (Larg > 0 && Haut > 0)
	{
		// on cr�e le stylo
		CPen PenA(PS_SOLID, 0, CATPColors::GetColorSelect(CATPColors::COLORCREUSET));

		// on s�lectionne le stylo
		CPen* pOldPen = (CPen*)dc.SelectObject(&PenA);

		// on trace le trait horizontal
		dc.MoveTo(x1, y1); dc.LineTo(x2, y1);

		// on v�rifie les dimensions de la fen�tre
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
	// on d�truit les objets graphiques
	m_Font.DeleteObject();
	m_PenG.DeleteObject();
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

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

	// on cr�e un contexte de p�riph�rique
	CPaintDC Dc(this);

	// on r�cup�re la zone cliente
	this->GetClientRect(&Client);

	// on r�cup�re la position de la zone � dessiner
	LONG x = Dc.m_ps.rcPaint.left;
	LONG y = Dc.m_ps.rcPaint.top;

	// on calcule la taille de la zone � dessiner
	LONG dx = Dc.m_ps.rcPaint.right  - Dc.m_ps.rcPaint.left;
	LONG dy = Dc.m_ps.rcPaint.bottom - Dc.m_ps.rcPaint.top;

	// on cr�e un contexte de p�riph�rique compatible
	if (FALSE != DcComp.CreateCompatibleDC(&Dc))
	{
		// on cr�e une bitmap compatible avec ce contexte de p�riph�rique
		if (FALSE != BmpNew.CreateCompatibleBitmap(&Dc, dx, dy))
		{
			// on s�lectionne la bitmap dans le contexte de p�riph�rique
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

			// on copie la bitmap dans la zone � dessiner de la fen�tre
			Dc.BitBlt(x, y, dx, dy, &DcComp, 0, 0, SRCCOPY);

			// on restaure la bitmap initiale
			pBmpOld = DcComp.SelectObject(pBmpOld);
		}

		// on d�truit le contexte de p�riph�rique compatible
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
