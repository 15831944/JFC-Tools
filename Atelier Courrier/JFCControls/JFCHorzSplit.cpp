// on inclut les d�finitions n�cessaires
#include "stdafx.h"

#include "JFCHorzSplit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le motif du d�placement
/////////////////////////////////////////////////////////////////////////////

static const BYTE g_Pattern[16] = { 0xAA, 0x00, 0x55, 0x00, 0xAA, 0x00, 0x55, 0x00,
                                    0xAA, 0x00, 0x55, 0x00, 0xAA, 0x00, 0x55, 0x00 };

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCHorzSplit::JFCHorzSplit(UINT Message) : m_Msg(Message)
{
	// on initialise les �l�ments
	m_Track = FALSE; m_TrackPos = m_DeltaPos = m_DispDown = m_DispUp = 0;
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour activer les d�placements
/////////////////////////////////////////////////////////////////////////////

BOOL JFCHorzSplit::SetRange(LONG Down, LONG Up)
{
	// on v�rifie la validit� de la fen�tre
	ASSERT(::IsWindow(this->m_hWnd));

	// on v�rifie la validit� des param�tres
	if (Down < 0 || Up < 0) return (FALSE);

	// on m�morise les param�tres
	m_DispDown = Down; m_DispUp = Up;

	// on renvoie l'indicateur de r�ussite
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

	// on initialise le d�placement
	LONG Disp = 0;

	// on r�cup�re la zone cliente
	this->GetClientRect(&Rect);

	// on la convertit en coordonn�es �cran
	this->ClientToScreen(&Rect);

	// on r�cup�re les dimensions � dessiner
	LONG Larg = Rect.right  - Rect.left;
	LONG Haut = Rect.bottom - Rect.top;

	// on r�cup�re la position du curseur
	::GetCursorPos(&Point);

	// on teste si on a quelque chose � dessiner
	if (Larg > 0 && Haut > 0)
	{
		CDC     Dc;
		CBrush  Brush;
		CBitmap Bitmap;

		// on r�cup�re le HDC de l'�cran
		HDC hdc = ::GetDC(NULL);

		// on attache le HDC de l'�cran au dc
		Dc.Attach(hdc);

		// on modifie le mode de trac�
		int OldRop = Dc.SetROP2(R2_XORPEN);

		// on cr�e la bitmap de remplissage
		Bitmap.CreateBitmap(8, 8, 1, 1, (const void*)g_Pattern);

		// on cr�e le pinceau pour le remplissage
		Brush.CreatePatternBrush(&Bitmap);

		// on s�lectionne le pinceau
		CBrush* pOldBrush = Dc.SelectObject(&Brush);

		// on teste le mode de trac�
		if (Mode == 1)
		{
			// on initialise la position courante
			m_TrackPos = Rect.top; m_DeltaPos = Point.y - Rect.top;

			// on effectue le premier trac�
			Dc.Rectangle(Rect.left - 1, m_TrackPos, Rect.right + 1, m_TrackPos + Haut);

			// on modifie le d�placement
			Disp = m_TrackPos - Rect.top;
		}
		else if (Mode == 2)
		{
			// on calcule la nouvelle position
			LONG NewPos = Point.y - m_DeltaPos;

			// on corrige la position pour la conserver des les limites
			if      (NewPos > Rect.top + m_DispDown) NewPos = Rect.top + m_DispDown;
			else if (NewPos < Rect.top - m_DispUp  ) NewPos = Rect.top - m_DispUp;

			// on teste si on doit faire un nouveau trac�
			if (NewPos != m_TrackPos)
			{
				// on efface le trac� pr�c�dent
				Dc.Rectangle(Rect.left - 1, m_TrackPos, Rect.right + 1, m_TrackPos + Haut);

				// on effectue le nouveau trac�
				Dc.Rectangle(Rect.left - 1, NewPos, Rect.right + 1, NewPos + Haut);

				// on met � jour la position courante
				m_TrackPos = NewPos;
			}

			// on modifie le d�placement
			Disp = m_TrackPos - Rect.top;
		}
		else if (Mode == 3)
		{
			// on modifie le d�placement
			Disp = m_TrackPos - Rect.top;

			// on efface le trac� pr�c�dent
			Dc.Rectangle(Rect.left - 1, m_TrackPos, Rect.right + 1, m_TrackPos + Haut);

			// on annule la position courante
			m_TrackPos = m_DeltaPos = 0;
		}

		// on restaure l'ancien pinceau
		Dc.SelectObject(pOldBrush);

		// on restaure le mode de trac�
		Dc.SetROP2(OldRop);

		// on d�tache le HDC de l'�cran
		Dc.Detach();

		// on lib�re le dc sur l'�cran
		::ReleaseDC(NULL, hdc);
	}

	// on renvoie le d�placement
	return (Disp);
}

/////////////////////////////////////////////////////////////////////////////
// JFCHorzSplit message handlers

void JFCHorzSplit::PreSubclassWindow() 
{
	// on initialise les �l�ments
	m_Track = FALSE; m_TrackPos = m_DeltaPos = m_DispDown = m_DispUp = 0;

	// on appelle le gestionnaire de base
	this->JFControl::PreSubclassWindow();
}

void JFCHorzSplit::OnPaint() 
{
	// on initialise le DC
	CPaintDC Dc(this);

	// on r�cup�re les dimensions � dessiner
	LONG Larg = Dc.m_ps.rcPaint.right  - Dc.m_ps.rcPaint.left;
	LONG Haut = Dc.m_ps.rcPaint.bottom - Dc.m_ps.rcPaint.top;

	// on v�rifie les dimensions
	if (Larg > 0 && Haut > 0)
	{
		// on cr�e le pinceau
		CBrush BrushF(::GetSysColor(COLOR_3DFACE));

		// on dessine le fond
		Dc.FillRect(&Dc.m_ps.rcPaint, &BrushF);
	}
}

void JFCHorzSplit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// on teste si on peut effectuer un d�placement
	if (m_DispDown > 0 || m_DispUp > 0)
	{
		// on modifie le curseur de la souris
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));

		// on teste si on peut lancer un tracking
		if (m_Track == FALSE && NULL == ::GetCapture())
		{
			// on capture la souris
			::SetCapture(this->m_hWnd);

			// on teste si a captur� la souris
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
	// on teste si on peut effectuer un d�placement
	if (m_DispDown > 0 || m_DispUp > 0)
	{
		// on teste si on a un tracking en cours
		if (m_Track != FALSE && this->m_hWnd == ::GetCapture())
		{
			// on lib�re la souris
			::ReleaseCapture();

			// on teste si la lib�ration s'est bien effectu�e
			if (this->m_hWnd != ::GetCapture())
			{
				// on dessine le rectangle
				LONG Disp = this->OnDrawRectangle(3);

				// on teste si on doit envoyer le message
				if (m_Msg >= WM_USER)
				{
					// on r�cup�re la fen�tre parente
					CWnd* pParent = this->GetParent();

					// on envoie le message de mise � jour
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
	// on teste si on peut effectuer un d�placement
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
