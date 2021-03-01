// on inclut les définitions nécessaires
#include "stdafx.h"

#include "JFCHorzSplit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le motif du déplacement
/////////////////////////////////////////////////////////////////////////////

static const BYTE g_Pattern[16] = { 0xAA, 0x00, 0x55, 0x00, 0xAA, 0x00, 0x55, 0x00,
                                    0xAA, 0x00, 0x55, 0x00, 0xAA, 0x00, 0x55, 0x00 };

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCHorzSplit::JFCHorzSplit(UINT Message) : m_Msg(Message)
{
	// on initialise les éléments
	m_Track = FALSE; m_TrackPos = m_DeltaPos = m_DispDown = m_DispUp = 0;
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour activer les déplacements
/////////////////////////////////////////////////////////////////////////////

BOOL JFCHorzSplit::SetRange(LONG Down, LONG Up)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on vérifie la validité des paramètres
	if (Down < 0 || Up < 0) return (FALSE);

	// on mémorise les paramètres
	m_DispDown = Down; m_DispUp = Up;

	// on renvoie l'indicateur de réussite
	return (TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCHorzSplit::~JFCHorzSplit()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCHorzSplit, JFControl)
	//{{AFX_MSG_MAP(JFCHorzSplit)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// la fonction pour dessiner le rectangle
/////////////////////////////////////////////////////////////////////////////

LONG JFCHorzSplit::OnDrawRectangle(LONG Mode)
{
	RECT  Rect;
	POINT Point;

	// on initialise le déplacement
	LONG Disp = 0;

	// on récupère la zone cliente
	this->GetClientRect(&Rect);

	// on la convertit en coordonnées écran
	this->ClientToScreen(&Rect);

	// on récupère les dimensions à dessiner
	LONG Larg = Rect.right  - Rect.left;
	LONG Haut = Rect.bottom - Rect.top;

	// on récupère la position du curseur
	::GetCursorPos(&Point);

	// on teste si on a quelque chose à dessiner
	if (Larg > 0 && Haut > 0)
	{
		CDC     Dc;
		CBrush  Brush;
		CBitmap Bitmap;

		// on récupère le HDC de l'écran
		HDC hdc = ::GetDC(NULL);

		// on attache le HDC de l'écran au dc
		Dc.Attach(hdc);

		// on modifie le mode de tracé
		int OldRop = Dc.SetROP2(R2_XORPEN);

		// on crée la bitmap de remplissage
		Bitmap.CreateBitmap(8, 8, 1, 1, (const void*)g_Pattern);

		// on crée le pinceau pour le remplissage
		Brush.CreatePatternBrush(&Bitmap);

		// on sélectionne le pinceau
		CBrush* pOldBrush = Dc.SelectObject(&Brush);

		// on teste le mode de tracé
		if (Mode == 1)
		{
			// on initialise la position courante
			m_TrackPos = Rect.top; m_DeltaPos = Point.y - Rect.top;

			// on effectue le premier tracé
			Dc.Rectangle(Rect.left - 1, m_TrackPos, Rect.right + 1, m_TrackPos + Haut);

			// on modifie le déplacement
			Disp = m_TrackPos - Rect.top;
		}
		else if (Mode == 2)
		{
			// on calcule la nouvelle position
			LONG NewPos = Point.y - m_DeltaPos;

			// on corrige la position pour la conserver des les limites
			if      (NewPos > Rect.top + m_DispDown) NewPos = Rect.top + m_DispDown;
			else if (NewPos < Rect.top - m_DispUp  ) NewPos = Rect.top - m_DispUp;

			// on teste si on doit faire un nouveau tracé
			if (NewPos != m_TrackPos)
			{
				// on efface le tracé précédent
				Dc.Rectangle(Rect.left - 1, m_TrackPos, Rect.right + 1, m_TrackPos + Haut);

				// on effectue le nouveau tracé
				Dc.Rectangle(Rect.left - 1, NewPos, Rect.right + 1, NewPos + Haut);

				// on met à jour la position courante
				m_TrackPos = NewPos;
			}

			// on modifie le déplacement
			Disp = m_TrackPos - Rect.top;
		}
		else if (Mode == 3)
		{
			// on modifie le déplacement
			Disp = m_TrackPos - Rect.top;

			// on efface le tracé précédent
			Dc.Rectangle(Rect.left - 1, m_TrackPos, Rect.right + 1, m_TrackPos + Haut);

			// on annule la position courante
			m_TrackPos = m_DeltaPos = 0;
		}

		// on restaure l'ancien pinceau
		Dc.SelectObject(pOldBrush);

		// on restaure le mode de tracé
		Dc.SetROP2(OldRop);

		// on détache le HDC de l'écran
		Dc.Detach();

		// on libère le dc sur l'écran
		::ReleaseDC(NULL, hdc);
	}

	// on renvoie le déplacement
	return (Disp);
}

/////////////////////////////////////////////////////////////////////////////
// JFCHorzSplit message handlers

void JFCHorzSplit::PreSubclassWindow() 
{
	// on initialise les éléments
	m_Track = FALSE; m_TrackPos = m_DeltaPos = m_DispDown = m_DispUp = 0;

	// on appelle le gestionnaire de base
	this->JFControl::PreSubclassWindow();
}

void JFCHorzSplit::OnPaint() 
{
	// on initialise le DC
	CPaintDC Dc(this);

	// on récupère les dimensions à dessiner
	LONG Larg = Dc.m_ps.rcPaint.right  - Dc.m_ps.rcPaint.left;
	LONG Haut = Dc.m_ps.rcPaint.bottom - Dc.m_ps.rcPaint.top;

	// on vérifie les dimensions
	if (Larg > 0 && Haut > 0)
	{
		// on crée le pinceau
		CBrush BrushF(::GetSysColor(COLOR_3DFACE));

		// on dessine le fond
		Dc.FillRect(&Dc.m_ps.rcPaint, &BrushF);
	}
}

void JFCHorzSplit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// on teste si on peut effectuer un déplacement
	if (m_DispDown > 0 || m_DispUp > 0)
	{
		// on modifie le curseur de la souris
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));

		// on teste si on peut lancer un tracking
		if (m_Track == FALSE && NULL == ::GetCapture())
		{
			// on capture la souris
			::SetCapture(this->m_hWnd);

			// on teste si a capturé la souris
			if (this->m_hWnd == ::GetCapture())
			{
				// on dessine le rectangle
				LONG Disp = this->OnDrawRectangle(1);

				// on modifie l'indicateur de tracking
				m_Track = TRUE;
			}
		}
	}

	// on appelle le gestionnaire de base
	this->JFControl::OnLButtonDown(nFlags, point);
}

void JFCHorzSplit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// on teste si on peut effectuer un déplacement
	if (m_DispDown > 0 || m_DispUp > 0)
	{
		// on teste si on a un tracking en cours
		if (m_Track != FALSE && this->m_hWnd == ::GetCapture())
		{
			// on libère la souris
			::ReleaseCapture();

			// on teste si la libération s'est bien effectuée
			if (this->m_hWnd != ::GetCapture())
			{
				// on dessine le rectangle
				LONG Disp = this->OnDrawRectangle(3);

				// on teste si on doit envoyer le message
				if (m_Msg >= WM_USER)
				{
					// on récupère la fenêtre parente
					CWnd* pParent = this->GetParent();

					// on envoie le message de mise à jour
					if (pParent != NULL) pParent->SendMessage(m_Msg, (WPARAM)Disp, 0);
				}

				// on modifie l'indicateur de tracking
				m_Track = FALSE;
			}
		}
	}
	
	// on appelle le gestionnaire de base
	this->JFControl::OnLButtonUp(nFlags, point);
}

void JFCHorzSplit::OnMouseMove(UINT nFlags, CPoint point) 
{
	// on teste si on peut effectuer un déplacement
	if (m_DispDown > 0 || m_DispUp > 0)
	{
		// on modifie le curseur de la souris
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));

		// on teste si on a un tracking en cours
		if (m_Track != FALSE && this->m_hWnd == ::GetCapture())
		{
			// on dessine le rectangle
			LONG Disp = this->OnDrawRectangle(2);
		}
	}

	// on appelle le gestionnaire de base
	this->JFControl::OnMouseMove(nFlags, point);
}
