// on inclut les d�finitions n�cessaires
#include "stdafx.h"

// on inclut les d�finitions n�cessaires
#include "JFCView.h"
#include "JFCScheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur

JFCView::JFCView()
{
	// on initialise les dimensions
	m_HorzSize = m_VertSize = 0;

	// on initialise le pas graduation en X
	m_PasGraduationX = 1;

	// on initialise le pas graduation en Y
	m_PasGraduationY = 1;
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur

JFCView::~JFCView()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour r�cup�rer les dimensions

long JFCView::GetHorzSize()
{
	// on renvoie la taille horizontale
	return (m_HorzSize);
}

long JFCView::GetVertSize()
{
	// on renvoie la taille verticale
	return (m_VertSize);
}

// R�cup�ration du pas de graduation axe des X
long JFCView::GetPasGraduationX()
{
	// on renvoie le pas de graduation axe  des x
	return (m_PasGraduationX);
}

// D�finit le pas de graduation en X  (pour clart� lecture sur axe horizontal)
void JFCView::SetPasGraduationX(LONG PasGraduationX)
{
	m_PasGraduationX = PasGraduationX;
}	

// R�cup�ration du pas de graduation axe des Y
long JFCView::GetPasGraduationY()
{
	// on renvoie le pas de graduation axe  des y
	return (m_PasGraduationY);
}

// D�finit le pas de graduation en Y  (pour clart� lecture sur axe horizontal)
void JFCView::SetPasGraduationY(LONG PasGraduationY)
{
	m_PasGraduationY = PasGraduationY;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour convertir les positions

bool JFCView::GetIdxAtPos(LONG x, LONG y, LONG & i, LONG & j)
{
	RECT Rect;

	// on r�cup�re la zone cliente
	this->GetClientRect(&Rect);

	// on r�cup�re les dimensions d'affichage
	LONG HSize = this->GetHorzSize();
	LONG VSize = this->GetVertSize();

	// on v�rifie la validit� de la position
	if (x < Rect.left || (x + 1) > Rect.right ) return (false);
	if (y < Rect.top  || (y + 1) > Rect.bottom) return (false);

	// on calcule la taille de la fen�tre
	LONG HWnd = Rect.right  - Rect.left;
	LONG VWnd = Rect.bottom - Rect.top;

	// on calcule l'offset d'affichage
	LONG OffX = (HWnd < HSize) ? -((HSize - HWnd) / 2) : (HWnd - HSize) / 2;
	LONG OffY = (VWnd < VSize) ? -((VSize - VWnd) / 2) : (VWnd - VSize) / 2;

	// on calcule les indices r�els
	LONG hidx = x - OffX;
	LONG vidx = y - OffY;

	// on v�rifie la validit� des indices
	if (hidx < 0 || (hidx + 1) > HSize) return (false);
	if (vidx < 0 || (vidx + 1) > VSize) return (false);
	
	// on renvoie les indices
	i = hidx;
	j = vidx;

	// on renvoie l'indicateur de r�ussite
	return (true);
}

bool JFCView::GetPosAtIdx(LONG i, LONG j, LONG & x, LONG & y)
{
	RECT Rect;

	// on r�cup�re la zone cliente
	this->GetClientRect(&Rect);

	// on r�cup�re les dimensions d'affichage
	LONG HSize = this->GetHorzSize();
	LONG VSize = this->GetVertSize();

	// on v�rifie la validit� des indices
	if (i < 0 || (i + 1) > HSize) return (false);
	if (j < 0 || (j + 1) > VSize) return (false);
	
	// on calcule la taille de la fen�tre
	LONG HWnd = Rect.right  - Rect.left;
	LONG VWnd = Rect.bottom - Rect.top;

	// on calcule l'offset d'affichage
	LONG OffX = (HWnd < HSize) ? -((HSize - HWnd) / 2) : (HWnd - HSize) / 2;
	LONG OffY = (VWnd < VSize) ? -((VSize - VWnd) / 2) : (VWnd - VSize) / 2;

	// on calcule les positions r�elles
	LONG hpos = i + OffX;
	LONG vpos = j + OffY;

	// on v�rifie la validit� des positions
	if (hpos < Rect.left || (hpos + 1) > Rect.right ) return (false);
	if (vpos < Rect.top  || (vpos + 1) > Rect.bottom) return (false);

	// on renvoie les positions
	x = hpos;
	y = vpos;

	// on renvoie l'indicateur de r�ussite
	return (true);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour convertir les positions en indices de tracking

void JFCView::GetTrackIdx(LONG x, LONG y, LONG & i, LONG & j)
{
	RECT Rect;

	// on r�cup�re la zone cliente
	this->GetClientRect(&Rect);

	// on r�cup�re les dimensions d'affichage
	LONG HSize = this->GetHorzSize();
	LONG VSize = this->GetVertSize();

	// on calcule la taille de la fen�tre
	LONG HWnd = Rect.right  - Rect.left;
	LONG VWnd = Rect.bottom - Rect.top;

	// on calcule l'offset d'affichage
	LONG OffX = (HWnd < HSize) ? -((HSize - HWnd) / 2) : (HWnd - HSize) / 2;
	LONG OffY = (VWnd < VSize) ? -((VSize - VWnd) / 2) : (VWnd - VSize) / 2;

	// on calcule les indices r�els
	i = x - OffX;
	j = y - OffY;
}

void JFCView::GetTrackPos(LONG i, LONG j, LONG & x, LONG & y)
{
	RECT Rect;

	// on r�cup�re la zone cliente
	this->GetClientRect(&Rect);

	// on r�cup�re les dimensions d'affichage
	LONG HSize = this->GetHorzSize();
	LONG VSize = this->GetVertSize();

	// on calcule la taille de la fen�tre
	LONG HWnd = Rect.right  - Rect.left;
	LONG VWnd = Rect.bottom - Rect.top;

	// on calcule l'offset d'affichage
	LONG OffX = (HWnd < HSize) ? -((HSize - HWnd) / 2) : (HWnd - HSize) / 2;
	LONG OffY = (VWnd < VSize) ? -((VSize - VWnd) / 2) : (VWnd - VSize) / 2;

	// on calcule les positions r�elles
	x = i + OffX;
	y = j + OffY;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les �l�ments

bool JFCView::OnBeginDrawing(CDC & dc)
{
	// on arr�te le dessin des �l�ments
	return (false);
}

void JFCView::OnDrawItems(CDC & dc, RECT rect)
{
	// on ne fait rien
}

void JFCView::OnEndDrawing(CDC & dc)
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour mettre � jour les dimensions

void JFCView::OnSetSize()
{
	RECT rect;

	// on r�cup�re la zone cliente
	this->GetClientRect(&rect);

	// on calcule la taille de la fen�tre
	LONG HWnd = rect.right  - rect.left;
	LONG VWnd = rect.bottom - rect.top;

	// on calcule la taille de l'affichage
	m_HorzSize = ((HWnd * 76) + 38) / 100;
	m_VertSize = ((VWnd * 76) + 38) / 100;
}


BEGIN_MESSAGE_MAP(JFCView, JFControl)
	//{{AFX_MSG_MAP(JFCView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// JFCView message handlers

void JFCView::PreSubclassWindow() 
{
	// on initialise les dimensions
	this->OnSetSize();

	// on appelle le gestionnaire de base
	JFControl::PreSubclassWindow();
}

void JFCView::OnPaint() 
{
	CDC DcComp;
	RECT Client;
	CPen Pen;
	CBitmap NewBmp;

	// on cr�e un contexte de p�riph�rique
	CPaintDC Dc(this);

	// on r�cup�re la zone cliente de la fen�tre
	this->GetClientRect(&Client);

	// on r�cup�re la position de la zone � dessiner
	LONG x = Dc.m_ps.rcPaint.left;
	LONG y = Dc.m_ps.rcPaint.top;

	// on calcule la taille de la zone � dessiner
	LONG dx = Dc.m_ps.rcPaint.right  - Dc.m_ps.rcPaint.left;
	LONG dy = Dc.m_ps.rcPaint.bottom - Dc.m_ps.rcPaint.top;

	// on calcule la taille de la fen�tre
	LONG HWnd = Client.right  - Client.left;
	LONG VWnd = Client.bottom - Client.top;

	// on r�cup�re les dimensions d'affichage
	LONG HSize = this->GetHorzSize();
	LONG VSize = this->GetVertSize();

	// on calcule l'offset d'affichage
	LONG OffX = (HWnd < HSize) ? -((HSize - HWnd) / 2) : (HWnd - HSize) / 2;
	LONG OffY = (VWnd < VSize) ? -((VSize - VWnd) / 2) : (VWnd - VSize) / 2;

	// on cr�e un contexte de p�riph�rique compatible
	if (FALSE != DcComp.CreateCompatibleDC(&Dc))
	{
		// on cr�e une bitmap compatible avec ce contexte de p�riph�rique
		if (FALSE != NewBmp.CreateCompatibleBitmap(&Dc, dx, dy))
		{
			// on s�lectionne la bitmap dans le contexte de p�riph�rique
			CBitmap* pOldBmp = (CBitmap*)DcComp.SelectObject(&NewBmp);

			// on vide la bitmap
			DcComp.BitBlt(0, 0, dx, dy, NULL, 0, 0, WHITENESS);

			// on modifie l'origine de la fen�tre
			CPoint OldWindowOrg = DcComp.SetWindowOrg(x, y);

			// on modifie l'origine du viewport
			CPoint OldViewportOrg = DcComp.SetViewportOrg(OffX, OffY);

			// on pr�pare le dessin des �l�ments
			if (this->OnBeginDrawing(DcComp))
			{
				RECT rect;

				// on initialise le rectangle � dessiner
				rect.left = x - OffX; rect.right  = rect.left + dx;
				rect.top  = y - OffY; rect.bottom = rect.top  + dy;

				// on dessine les bordures
				this->OnDrawItems(DcComp, rect);
			}

			// on termine le dessin des �l�ments
			this->OnEndDrawing(DcComp);

			// on restaure l'origine du viewport
			DcComp.SetViewportOrg(OldViewportOrg);

			// on restaure l'origine de la fen�tre
			DcComp.SetWindowOrg(OldWindowOrg);

			// on cr�e le stylo A
			CPen PenA(PS_SOLID, 1, JFCScheme::GetChorusA());

			// on s�lectionne le stylo A
			CPen* pOldPen = DcComp.SelectObject(&PenA);

			// on trace la bordure de la fen�tre
			DcComp.MoveTo(-x, -y);
			DcComp.LineTo(Client.right - x - 1, -y);
			DcComp.LineTo(Client.right - x - 1, Client.bottom - y - 1);
			DcComp.LineTo(-x, Client.bottom - y - 1);
			DcComp.LineTo(-x, -y);

			// on restaure le stylo
			DcComp.SelectObject(pOldPen);

			// on copie la bitmap dans la zone � dessiner de la fen�tre
			Dc.BitBlt(x, y, dx, dy, &DcComp, 0, 0, SRCCOPY);

			// on restaure la bitmap initiale
			DcComp.SelectObject(pOldBmp);
		}

		// on d�truit le contexte de p�riph�rique compatible
		DcComp.DeleteDC();
	}
}

void JFCView::OnSize(UINT nType, int cx, int cy) 
{
	// on met � jour les dimensions
	this->OnSetSize();

	// on appelle le gestionnaire de base
	this->JFControl::OnSize(nType, cx, cy);
}

void JFCView::OnDestroy() 
{
	// on r�initialise les dimensions
	m_HorzSize = m_VertSize = 0;

	// on appelle le gestionnaire de base
	this->JFControl::OnDestroy();
}
