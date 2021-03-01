// on inclut les définitions nécessaires
#include "stdafx.h"

// on inclut les définitions nécessaires
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
// les fonctions pour récupérer les dimensions

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

// Récupération du pas de graduation axe des X
long JFCView::GetPasGraduationX()
{
	// on renvoie le pas de graduation axe  des x
	return (m_PasGraduationX);
}

// Définit le pas de graduation en X  (pour clarté lecture sur axe horizontal)
void JFCView::SetPasGraduationX(LONG PasGraduationX)
{
	m_PasGraduationX = PasGraduationX;
}	

// Récupération du pas de graduation axe des Y
long JFCView::GetPasGraduationY()
{
	// on renvoie le pas de graduation axe  des y
	return (m_PasGraduationY);
}

// Définit le pas de graduation en Y  (pour clarté lecture sur axe horizontal)
void JFCView::SetPasGraduationY(LONG PasGraduationY)
{
	m_PasGraduationY = PasGraduationY;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour convertir les positions

bool JFCView::GetIdxAtPos(LONG x, LONG y, LONG & i, LONG & j)
{
	RECT Rect;

	// on récupère la zone cliente
	this->GetClientRect(&Rect);

	// on récupère les dimensions d'affichage
	LONG HSize = this->GetHorzSize();
	LONG VSize = this->GetVertSize();

	// on vérifie la validité de la position
	if (x < Rect.left || (x + 1) > Rect.right ) return (false);
	if (y < Rect.top  || (y + 1) > Rect.bottom) return (false);

	// on calcule la taille de la fenêtre
	LONG HWnd = Rect.right  - Rect.left;
	LONG VWnd = Rect.bottom - Rect.top;

	// on calcule l'offset d'affichage
	LONG OffX = (HWnd < HSize) ? -((HSize - HWnd) / 2) : (HWnd - HSize) / 2;
	LONG OffY = (VWnd < VSize) ? -((VSize - VWnd) / 2) : (VWnd - VSize) / 2;

	// on calcule les indices réels
	LONG hidx = x - OffX;
	LONG vidx = y - OffY;

	// on vérifie la validité des indices
	if (hidx < 0 || (hidx + 1) > HSize) return (false);
	if (vidx < 0 || (vidx + 1) > VSize) return (false);
	
	// on renvoie les indices
	i = hidx;
	j = vidx;

	// on renvoie l'indicateur de réussite
	return (true);
}

bool JFCView::GetPosAtIdx(LONG i, LONG j, LONG & x, LONG & y)
{
	RECT Rect;

	// on récupère la zone cliente
	this->GetClientRect(&Rect);

	// on récupère les dimensions d'affichage
	LONG HSize = this->GetHorzSize();
	LONG VSize = this->GetVertSize();

	// on vérifie la validité des indices
	if (i < 0 || (i + 1) > HSize) return (false);
	if (j < 0 || (j + 1) > VSize) return (false);
	
	// on calcule la taille de la fenêtre
	LONG HWnd = Rect.right  - Rect.left;
	LONG VWnd = Rect.bottom - Rect.top;

	// on calcule l'offset d'affichage
	LONG OffX = (HWnd < HSize) ? -((HSize - HWnd) / 2) : (HWnd - HSize) / 2;
	LONG OffY = (VWnd < VSize) ? -((VSize - VWnd) / 2) : (VWnd - VSize) / 2;

	// on calcule les positions réelles
	LONG hpos = i + OffX;
	LONG vpos = j + OffY;

	// on vérifie la validité des positions
	if (hpos < Rect.left || (hpos + 1) > Rect.right ) return (false);
	if (vpos < Rect.top  || (vpos + 1) > Rect.bottom) return (false);

	// on renvoie les positions
	x = hpos;
	y = vpos;

	// on renvoie l'indicateur de réussite
	return (true);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour convertir les positions en indices de tracking

void JFCView::GetTrackIdx(LONG x, LONG y, LONG & i, LONG & j)
{
	RECT Rect;

	// on récupère la zone cliente
	this->GetClientRect(&Rect);

	// on récupère les dimensions d'affichage
	LONG HSize = this->GetHorzSize();
	LONG VSize = this->GetVertSize();

	// on calcule la taille de la fenêtre
	LONG HWnd = Rect.right  - Rect.left;
	LONG VWnd = Rect.bottom - Rect.top;

	// on calcule l'offset d'affichage
	LONG OffX = (HWnd < HSize) ? -((HSize - HWnd) / 2) : (HWnd - HSize) / 2;
	LONG OffY = (VWnd < VSize) ? -((VSize - VWnd) / 2) : (VWnd - VSize) / 2;

	// on calcule les indices réels
	i = x - OffX;
	j = y - OffY;
}

void JFCView::GetTrackPos(LONG i, LONG j, LONG & x, LONG & y)
{
	RECT Rect;

	// on récupère la zone cliente
	this->GetClientRect(&Rect);

	// on récupère les dimensions d'affichage
	LONG HSize = this->GetHorzSize();
	LONG VSize = this->GetVertSize();

	// on calcule la taille de la fenêtre
	LONG HWnd = Rect.right  - Rect.left;
	LONG VWnd = Rect.bottom - Rect.top;

	// on calcule l'offset d'affichage
	LONG OffX = (HWnd < HSize) ? -((HSize - HWnd) / 2) : (HWnd - HSize) / 2;
	LONG OffY = (VWnd < VSize) ? -((VSize - VWnd) / 2) : (VWnd - VSize) / 2;

	// on calcule les positions réelles
	x = i + OffX;
	y = j + OffY;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les éléments

bool JFCView::OnBeginDrawing(CDC & dc)
{
	// on arrête le dessin des éléments
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
// la fonction pour mettre à jour les dimensions

void JFCView::OnSetSize()
{
	RECT rect;

	// on récupère la zone cliente
	this->GetClientRect(&rect);

	// on calcule la taille de la fenêtre
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

	// on crée un contexte de périphérique
	CPaintDC Dc(this);

	// on récupère la zone cliente de la fenêtre
	this->GetClientRect(&Client);

	// on récupère la position de la zone à dessiner
	LONG x = Dc.m_ps.rcPaint.left;
	LONG y = Dc.m_ps.rcPaint.top;

	// on calcule la taille de la zone à dessiner
	LONG dx = Dc.m_ps.rcPaint.right  - Dc.m_ps.rcPaint.left;
	LONG dy = Dc.m_ps.rcPaint.bottom - Dc.m_ps.rcPaint.top;

	// on calcule la taille de la fenêtre
	LONG HWnd = Client.right  - Client.left;
	LONG VWnd = Client.bottom - Client.top;

	// on récupère les dimensions d'affichage
	LONG HSize = this->GetHorzSize();
	LONG VSize = this->GetVertSize();

	// on calcule l'offset d'affichage
	LONG OffX = (HWnd < HSize) ? -((HSize - HWnd) / 2) : (HWnd - HSize) / 2;
	LONG OffY = (VWnd < VSize) ? -((VSize - VWnd) / 2) : (VWnd - VSize) / 2;

	// on crée un contexte de périphérique compatible
	if (FALSE != DcComp.CreateCompatibleDC(&Dc))
	{
		// on crée une bitmap compatible avec ce contexte de périphérique
		if (FALSE != NewBmp.CreateCompatibleBitmap(&Dc, dx, dy))
		{
			// on sélectionne la bitmap dans le contexte de périphérique
			CBitmap* pOldBmp = (CBitmap*)DcComp.SelectObject(&NewBmp);

			// on vide la bitmap
			DcComp.BitBlt(0, 0, dx, dy, NULL, 0, 0, WHITENESS);

			// on modifie l'origine de la fenêtre
			CPoint OldWindowOrg = DcComp.SetWindowOrg(x, y);

			// on modifie l'origine du viewport
			CPoint OldViewportOrg = DcComp.SetViewportOrg(OffX, OffY);

			// on prépare le dessin des éléments
			if (this->OnBeginDrawing(DcComp))
			{
				RECT rect;

				// on initialise le rectangle à dessiner
				rect.left = x - OffX; rect.right  = rect.left + dx;
				rect.top  = y - OffY; rect.bottom = rect.top  + dy;

				// on dessine les bordures
				this->OnDrawItems(DcComp, rect);
			}

			// on termine le dessin des éléments
			this->OnEndDrawing(DcComp);

			// on restaure l'origine du viewport
			DcComp.SetViewportOrg(OldViewportOrg);

			// on restaure l'origine de la fenêtre
			DcComp.SetWindowOrg(OldWindowOrg);

			// on crée le stylo A
			CPen PenA(PS_SOLID, 1, JFCScheme::GetChorusA());

			// on sélectionne le stylo A
			CPen* pOldPen = DcComp.SelectObject(&PenA);

			// on trace la bordure de la fenêtre
			DcComp.MoveTo(-x, -y);
			DcComp.LineTo(Client.right - x - 1, -y);
			DcComp.LineTo(Client.right - x - 1, Client.bottom - y - 1);
			DcComp.LineTo(-x, Client.bottom - y - 1);
			DcComp.LineTo(-x, -y);

			// on restaure le stylo
			DcComp.SelectObject(pOldPen);

			// on copie la bitmap dans la zone à dessiner de la fenêtre
			Dc.BitBlt(x, y, dx, dy, &DcComp, 0, 0, SRCCOPY);

			// on restaure la bitmap initiale
			DcComp.SelectObject(pOldBmp);
		}

		// on détruit le contexte de périphérique compatible
		DcComp.DeleteDC();
	}
}

void JFCView::OnSize(UINT nType, int cx, int cy) 
{
	// on met à jour les dimensions
	this->OnSetSize();

	// on appelle le gestionnaire de base
	this->JFControl::OnSize(nType, cx, cy);
}

void JFCView::OnDestroy() 
{
	// on réinitialise les dimensions
	m_HorzSize = m_VertSize = 0;

	// on appelle le gestionnaire de base
	this->JFControl::OnDestroy();
}
