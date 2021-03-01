// on inclut les définitions nécessaires
#include "stdafx.h"

#include "JFCViewport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// le constructeur
/////////////////////////////////////////////////////////////////////////////

JFCViewport::JFCViewport()
{
	// on initialise les bordures
	m_HorzBorder = m_VertBorder = FALSE;

	// on initialise les ascenseurs
	m_HorzMaster  = m_VertMaster  = FALSE;
	m_pHorzScroll = m_pVertScroll = NULL;

	// on initialise les éléments
	m_HorzFirst = m_HorzLast = m_HorzSize = 0;
	m_VertFirst = m_VertLast = m_VertSize = 0;
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour définir les ascenseurs externes
/////////////////////////////////////////////////////////////////////////////

void JFCViewport::SetHorzScroll(JFCScroll* pScroll, BOOL Master, BOOL update)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on réinitialise l'ascenseur courant
	if ((m_HorzMaster != FALSE) && (m_pHorzScroll != NULL))	m_pHorzScroll->SetRange(0, 0, 0);

	// on mémorise l'indicateur de maître
	m_HorzMaster = ((Master != FALSE) && (pScroll != NULL)) ? TRUE : FALSE;

	// on mémorise l'ascenseur
	m_pHorzScroll = pScroll;

	// on met à jour les ascenseurs
	if (update != FALSE) this->OnSetScrollBar(TRUE);
}

void JFCViewport::SetVertScroll(JFCScroll* pScroll, BOOL Master, BOOL update)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on réinitialise l'ascenseur courant
	if ((m_VertMaster != FALSE) && (m_pVertScroll != NULL)) m_pVertScroll->SetRange(0, 0, 0);

	// on mémorise l'indicateur de maître
	m_VertMaster = ((Master != FALSE) && (pScroll != NULL)) ? TRUE : FALSE;

	// on mémorise l'ascenseur
	m_pVertScroll = pScroll;

	// on met à jour les ascenseurs
	if (update != FALSE) this->OnSetScrollBar(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour récupérer les ascenseurs externes
/////////////////////////////////////////////////////////////////////////////

JFCScroll* JFCViewport::GetHorzScroll()
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on renvoie l'ascenseur externe
	return (m_pHorzScroll);
}

JFCScroll* JFCViewport::GetVertScroll()
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on renvoie l'ascenseur externe
	return (m_pVertScroll);
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCViewport::~JFCViewport()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour définir la taille des éléments
/////////////////////////////////////////////////////////////////////////////

void JFCViewport::SetHorzItemSize(LONG size, BOOL border, BOOL update)
{
	// on mémorise la taille des éléments
	m_HorzSize = (size > 0) ? size : 0;

	// on mémorise l'indicateur des bordures
	m_HorzBorder = (border != FALSE) ? TRUE : FALSE;

	// on met à jour les ascenseurs
	if (update != FALSE) this->OnSetScrollBar(TRUE);
}

void JFCViewport::SetVertItemSize(LONG size, BOOL border, BOOL update)
{
	// on mémorise la taille des éléments
	m_VertSize = (size > 0) ? size : 0;

	// on mémorise l'indicateur des bordures
	m_VertBorder = (border != FALSE) ? TRUE : FALSE;

	// on met à jour les ascenseurs
	if (update != FALSE) this->OnSetScrollBar(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour définir l'intervalle des éléments
/////////////////////////////////////////////////////////////////////////////

void JFCViewport::SetHorzItemRange(LONG first, LONG last, BOOL update)
{
	// on mémorise l'intervalle
	m_HorzFirst = (first <= last) ? first : last;
	m_HorzLast  = (first >= last) ? first : last;

	// on met à jour les ascenseurs
	if (update != FALSE) this->OnSetScrollBar(TRUE);
}

void JFCViewport::SetVertItemRange(LONG first, LONG last, BOOL update)
{
	// on mémorise l'intervalle
	m_VertFirst = (first <= last) ? first : last;
	m_VertLast  = (first >= last) ? first : last;

	// on met à jour les ascenseurs
	if (update != FALSE) this->OnSetScrollBar(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour rendre visible un élément
/////////////////////////////////////////////////////////////////////////////

bool JFCViewport::EnsureVisible(LONG i, LONG j)
{
	RECT rect;

	// on récupère la zone cliente de la fenêtre
	this->GetClientRect(&rect);

	// on vérifie la validité des indices
	if ((i < m_HorzFirst) || (i > m_HorzLast)) return (false);
	if ((j < m_VertFirst) || (j > m_VertLast)) return (false);

	// on récupère la taille de la fenêtre
	LONG hwnd = rect.right  - rect.left;
	LONG vwnd = rect.bottom - rect.top;

	// on récupère la taille des bordures
	LONG hbrd = (m_HorzBorder == FALSE) ? 0 : 1;
	LONG vbrd = (m_VertBorder == FALSE) ? 0 : 1;

	// on récupère la taille réelle des éléments
	LONG hsize = (m_HorzSize > 0) ? m_HorzSize : (hwnd > hbrd) ? (hwnd - (2 * hbrd)) : 0;
	LONG vsize = (m_VertSize > 0) ? m_VertSize : (vwnd > vbrd) ? (vwnd - (2 * vbrd)) : 0;

	// on récupère la position des ascenseurs
	LONG hpos = ((m_HorzSize == 0) && (m_HorzFirst == m_HorzLast)) ? m_HorzFirst : this->OnGetHorzPosition();
	LONG vpos = ((m_VertSize == 0) && (m_VertFirst == m_VertLast)) ? m_VertFirst : this->OnGetVertPosition();

	// on récupère le nombre d'éléments visibles
	LONG hdta = ((hwnd > hbrd) && (hsize > 0)) ? ((hwnd - (2 * hbrd)) / hsize) : 0;
	LONG vdta = ((vwnd > vbrd) && (vsize > 0)) ? ((vwnd - (2 * vbrd)) / vsize) : 0;

	// on calcule les nouvelles positions des ascenseurs
	LONG nhps = (hdta > 0) ? (((hpos + hdta) <= i) ? (i - hdta + 1) : (hpos <= i) ? hpos : i) : i;
	LONG nvps = (vdta > 0) ? (((vpos + vdta) <= j) ? (j - vdta + 1) : (vpos <= j) ? vpos : j) : j;

	// on modifie les positions d'affichage
	if (this->OnSetHorzPosition(nhps)) this->OnHorzScrollPosition(); else return (false);
	if (this->OnSetVertPosition(nvps)) this->OnVertScrollPosition(); else return (false);

	// on renvoie l'indicateur de réussite
	return (true);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour convertir les indices et les positions
/////////////////////////////////////////////////////////////////////////////

bool JFCViewport::GetIdxAtPos(LONG x, LONG y, LONG & i, LONG & j)
{
	RECT rect;

	// on récupère la zone cliente de la fenêtre
	this->GetClientRect(&rect);

	// on vérifie la validité de la position
	if ((x < rect.left) || ((x + 1) > rect.right )) return (false);
	if ((y < rect.top ) || ((y + 1) > rect.bottom)) return (false);

	// on récupère la taille de la fenêtre
	LONG hwnd = rect.right  - rect.left;
	LONG vwnd = rect.bottom - rect.top;

	// on récupère la taille des bordures
	LONG hbrd = (m_HorzBorder == FALSE) ? 0 : 1;
	LONG vbrd = (m_VertBorder == FALSE) ? 0 : 1;

	// on récupère la taille réelle des éléments
	LONG hsize = (m_HorzSize > 0) ? m_HorzSize : (hwnd > hbrd) ? (hwnd - (2 * hbrd)) : 0;
	LONG vsize = (m_VertSize > 0) ? m_VertSize : (vwnd > vbrd) ? (vwnd - (2 * vbrd)) : 0;

	// on récupère la position des ascenseurs
	LONG hpos = ((m_HorzSize == 0) && (m_HorzFirst == m_HorzLast)) ? m_HorzFirst : this->OnGetHorzPosition();
	LONG vpos = ((m_VertSize == 0) && (m_VertFirst == m_VertLast)) ? m_VertFirst : this->OnGetVertPosition();

	// on calcule les indices de l'élément
	LONG hidx = hpos + (((x > hbrd) && (hsize > 0)) ? ((x - hbrd) / hsize) : (0));
	LONG vidx = vpos + (((y > vbrd) && (vsize > 0)) ? ((y - vbrd) / vsize) : (0));

	// cas particulier : l'élément est caché par la bordure droite ou basse
	if (((x + hbrd + 1) > rect.right ) && (x == ((hidx - hpos) * hsize + hbrd))) return (false);
	if (((y + vbrd + 1) > rect.bottom) && (y == ((vidx - vpos) * vsize + vbrd))) return (false);

	// on vérifie la validité des indices
	if ((hidx < m_HorzFirst) || (hidx > m_HorzLast)) return (false);
	if ((vidx < m_VertFirst) || (vidx > m_VertLast)) return (false);

	// on renvoie les indices
	i = hidx;
	j = vidx;

	// on renvoie l'indicateur de réussite
	return (true);
}

bool JFCViewport::GetPosAtIdx(LONG i, LONG j, LONG & x1, LONG & y1, LONG & x2, LONG & y2)
{
	RECT rect;

	// on récupère la zone cliente de la fenêtre
	this->GetClientRect(&rect);

	// on vérifie la validité des indices
	if ((i < m_HorzFirst) || (i > m_HorzLast)) return (false);
	if ((j < m_VertFirst) || (j > m_VertLast)) return (false);

	// on récupère la taille de la fenêtre
	LONG hwnd = rect.right  - rect.left;
	LONG vwnd = rect.bottom - rect.top;

	// on récupère la taille des bordures
	LONG hbrd = (m_HorzBorder == FALSE) ? 0 : 1;
	LONG vbrd = (m_VertBorder == FALSE) ? 0 : 1;

	// on récupère la taille réelle des éléments
	LONG hsize = (m_HorzSize > 0) ? m_HorzSize : (hwnd > hbrd) ? (hwnd - (2 * hbrd)) : 0;
	LONG vsize = (m_VertSize > 0) ? m_VertSize : (vwnd > vbrd) ? (vwnd - (2 * vbrd)) : 0;

	// on récupère la position des ascenseurs
	LONG hpos = ((m_HorzSize == 0) && (m_HorzFirst == m_HorzLast)) ? m_HorzFirst : this->OnGetHorzPosition();
	LONG vpos = ((m_VertSize == 0) && (m_VertFirst == m_VertLast)) ? m_VertFirst : this->OnGetVertPosition();

	// on calcule la position de l'élément
	LONG hpos1 = (i - hpos) * hsize + hbrd; LONG hpos2 = hpos1 + hsize - 1;
	LONG vpos1 = (j - vpos) * vsize + vbrd; LONG vpos2 = vpos1 + vsize - 1;

	// on vérifie la validité des positions
	if ((hpos1 > hpos2) || ((hpos1 + 1) > rect.right )) return (false);
	if ((vpos1 > vpos2) || ((vpos1 + 1) > rect.bottom)) return (false);

	// on renvoie la position
	x1 = hpos1; y1 = vpos1;
	x2 = hpos2; y2 = vpos2;

	// on renvoie l'indicateur de réussite
	return (true);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour convertir les positions en indices de tracking
/////////////////////////////////////////////////////////////////////////////

void JFCViewport::GetTrackIdx(LONG x, LONG y, LONG & i,  LONG & j, BOOL scroll)
{
	RECT rect;

	// on récupère la zone cliente de la fenêtre
	this->GetClientRect(&rect);

	// on ramène la position à l'intérieur de la zone cliente
	if ((x)     < rect.left  ) { x = rect.left;       if (scroll != FALSE) this->OnHorzScrollItems(FALSE); }
	if ((y)     < rect.top   ) { y = rect.top;        if (scroll != FALSE) this->OnVertScrollItems(FALSE); }
	if ((x + 1) > rect.right ) { x = rect.right  - 1; if (scroll != FALSE) this->OnHorzScrollItems(TRUE ); }
	if ((y + 1) > rect.bottom) { y = rect.bottom - 1; if (scroll != FALSE) this->OnVertScrollItems(TRUE ); }

	// on récupère la taille de la fenêtre
	LONG hwnd = rect.right  - rect.left;
	LONG vwnd = rect.bottom - rect.top;

	// on récupère la taille des bordures
	LONG hbrd = (m_HorzBorder == FALSE) ? 0 : 1;
	LONG vbrd = (m_VertBorder == FALSE) ? 0 : 1;

	// on récupère la taille réelle des éléments
	LONG hsize = (m_HorzSize > 0) ? m_HorzSize : (hwnd > hbrd) ? (hwnd - (2 * hbrd)) : 0;
	LONG vsize = (m_VertSize > 0) ? m_VertSize : (vwnd > vbrd) ? (vwnd - (2 * vbrd)) : 0;

	// on récupère la position des ascenseurs
	LONG hpos = ((m_HorzSize == 0) && (m_HorzFirst == m_HorzLast)) ? m_HorzFirst : this->OnGetHorzPosition();
	LONG vpos = ((m_VertSize == 0) && (m_VertFirst == m_VertLast)) ? m_VertFirst : this->OnGetVertPosition();

	// on calcule les indices de l'élément
	LONG hidx = hpos + (((x > hbrd) && (hsize > 0)) ? ((x - hbrd) / hsize) : (0));
	LONG vidx = vpos + (((y > vbrd) && (vsize > 0)) ? ((y - vbrd) / vsize) : (0));

	// on renvoie les indices
	i = (hidx < m_HorzFirst) ? m_HorzFirst : (hidx > m_HorzLast) ? m_HorzLast : hidx;
	j = (vidx < m_VertFirst) ? m_VertFirst : (vidx > m_VertLast) ? m_VertLast : vidx;
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer l'état des éléments
/////////////////////////////////////////////////////////////////////////////

LONG JFCViewport::OnGetItemState(LONG i, LONG j)
{
	// on renvoie l'état de l'élément
	return (0);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les éléments
/////////////////////////////////////////////////////////////////////////////

bool JFCViewport::OnBeginDrawing(CDC & dc)
{
	// on arrête le dessin des éléments
	return (false);
}

void JFCViewport::OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat)
{
	// on ne fait rien
}

void JFCViewport::OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j)
{
	// on ne fait rien
}

void JFCViewport::OnEndDrawing(CDC & dc)
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCViewport, JFControl)
	//{{AFX_MSG_MAP(JFCViewport)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer les ascenseurs actifs
////////////////////////////////////////////////////////////////////////////

void JFCViewport::OnSetScrollBar(BOOL update)
{
	RECT rect;

	// on met à jour l'affichage des ascenseurs
	this->ShowScrollBar(SB_HORZ, FALSE);
	this->ShowScrollBar(SB_VERT, FALSE);

	// on récupère la zone cliente de la fenêtre
	this->GetClientRect(&rect);

	// on teste si on doit configurer l'ascenseur horizontal
	if ((m_pHorzScroll != NULL) && (m_HorzMaster != FALSE))
	{
		// on récupère la largeur de la fenêtre
		LONG wnd = rect.right - rect.left;

		// on récupère la largeur des bordures
		LONG brd = (m_HorzBorder == FALSE) ? 0 : 1;

		// on récupère la largeur réelle des éléments
		LONG size = (m_HorzSize > 0) ? m_HorzSize : (wnd > brd) ? (wnd - (2 * brd)) : 0;

		// on récupère la taille de la page
		LONG page = ((size > 0) && (size < wnd)) ? (wnd / size) : 0;

		// on configure l'ascenseur externe
		m_pHorzScroll->SetRange(m_HorzFirst, m_HorzLast, (page > 0) ? page : 1);
	}

	// on teste si on doit configurer l'ascenseur vertical
	if ((m_pVertScroll != NULL) && (m_VertMaster != FALSE))
	{
		// on récupère la hauteur de la fenêtre
		LONG wnd = rect.bottom - rect.top;

		// on récupère la hauteur des bordures
		LONG brd = (m_VertBorder == FALSE) ? 0 : 1;

		// on récupère la hauteur réelle des éléments
		LONG size = (m_VertSize > 0) ? m_VertSize : (wnd > brd) ? (wnd - (2 * brd)) : 0;

		// on calcule la taille de la page
		LONG page = ((size > 0) && (size < wnd)) ? (wnd / size) : 0;

		// on configure l'ascenseur externe
		m_pVertScroll->SetRange(m_VertFirst, m_VertLast, (page > 0) ? page : 1);
	}

	// on met à jour la fenêtre
	if (update != FALSE) this->InvalidateRect(NULL, FALSE);
}

////////////////////////////////////////////////////////////////////////////
// les fonctions pour modifier la position des ascenseurs actifs
////////////////////////////////////////////////////////////////////////////

bool JFCViewport::OnSetHorzPosition(LONG position)
{
	// on vérifie la validité et la maitrise de l'ascenseur
	if ((m_pHorzScroll == NULL) || (m_HorzMaster == FALSE)) return (false);

	// on modifie la position de l'ascenseur
	m_pHorzScroll->SetPosition(position);

	// on renvoie l'indicateur de réussite
	return (true);
}

bool JFCViewport::OnSetVertPosition(LONG position)
{
	// on vérifie la validité et la maitrise de l'ascenseur
	if ((m_pVertScroll == NULL) || (m_VertMaster == FALSE)) return (false);

	// on modifie la position de l'ascenseur
	m_pVertScroll->SetPosition(position);

	// on renvoie l'indicateur de réussite
	return (true);
}

////////////////////////////////////////////////////////////////////////////
// les fonctions pour récupérer la position des ascenseurs actifs
////////////////////////////////////////////////////////////////////////////

LONG JFCViewport::OnGetHorzPosition()
{
	// on initialise la position
	LONG position = m_HorzFirst;

	// on récupère la position de l'ascenseur
	if (m_pHorzScroll != NULL) position = m_pHorzScroll->GetPosition();

	// on renvoie la position
	return (position);
}

LONG JFCViewport::OnGetVertPosition()
{
	// on initialise la position
	LONG position = m_VertFirst;

	// on récupère la position de l'ascenseur
	if (m_pVertScroll != NULL) position = m_pVertScroll->GetPosition();

	// on renvoie la position
	return (position);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour faire défiler l'affichage
/////////////////////////////////////////////////////////////////////////////

void JFCViewport::OnHorzScrollItems(BOOL forward)
{
	// on vérifie la validité de l'ascenceur
	if (m_pHorzScroll != NULL)
	{
		// on récupère la destination du message
		CWnd* pDest = this->GetParent();

		// on teste la validité du destinataire
		if (pDest != NULL)
		{
			// on récupère la source du message
			HWND hSrc = m_pHorzScroll->m_hWnd;

			// on récupère le paramètre du message
			LONG Param = MAKELONG((forward != FALSE) ? SB_LINERIGHT : SB_LINELEFT, 0);

			// on envoie le message
			pDest->SendMessage(WM_HSCROLL, (LPARAM)Param, (WPARAM)hSrc);
		}
	}
}

void JFCViewport::OnVertScrollItems(BOOL forward)
{
	// on vérifie la validité de l'ascenceur
	if (m_pVertScroll != NULL)
	{
		// on récupère la destination du message
		CWnd* pDest = this->GetParent();

		// on teste la validité du destinataire
		if (pDest != NULL)
		{
			// on récupère la source du message
			HWND hSrc = m_pVertScroll->m_hWnd;

			// on récupère le paramètre du message
			LONG Param = MAKELONG((forward != FALSE) ? SB_LINEDOWN : SB_LINEUP, 0);

			// on envoie le message
			pDest->SendMessage(WM_VSCROLL, (LPARAM)Param, (WPARAM)hSrc);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour actualiser la position d'affichage
/////////////////////////////////////////////////////////////////////////////

void JFCViewport::OnHorzScrollPosition()
{
	// on vérifie la validité de l'ascenceur
	if (m_pHorzScroll != NULL)
	{
		// on récupère la destination du message
		CWnd* pDest = this->GetParent();

		// on teste la validité du destinataire
		if (pDest != NULL)
		{
			// on récupère la source du message
			HWND hSrc = m_pHorzScroll->m_hWnd;

			// on récupère le paramètre du message
			LONG Param = MAKELONG(SB_ENDSCROLL, 0);

			// on envoie le message
			pDest->SendMessage(WM_HSCROLL, (LPARAM)Param, (WPARAM)hSrc);
		}
	}
}

void JFCViewport::OnVertScrollPosition()
{
	// on vérifie la validité de l'ascenceur
	if (m_pVertScroll != NULL)
	{
		// on récupère la destination du message
		CWnd* pDest = this->GetParent();

		// on teste la validité du destinataire
		if (pDest != NULL)
		{
			// on récupère la source du message
			HWND hSrc = m_pVertScroll->m_hWnd;

			// on récupère le paramètre du message
			LONG Param = MAKELONG(SB_ENDSCROLL, 0);

			// on envoie le message
			pDest->SendMessage(WM_VSCROLL, (LPARAM)Param, (WPARAM)hSrc);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les cellules des éléments
/////////////////////////////////////////////////////////////////////////////

void JFCViewport::OnDrawStates(CDC & dc, LONG x, LONG y, LONG cx, LONG cy, LONG i, LONG j, LONG ci, LONG cj)
{
	RECT rect;

	// on initialise les coordonnées verticales
	rect.top = y; rect.bottom = y + cy;

	// on parcourt tous les éléments verticaux à dessiner
	for (LONG jdx = 0; jdx < cj; jdx += 1)
	{
		// on initialise les coordonnées horizontales
		rect.left = x; rect.right = x + cx;

		// on parcourt tous les éléments horizontaux à dessiner
		for (LONG idx = 0; idx < ci; idx += 1)
		{
			// on récupère l'état de l'élément
			LONG etat = this->OnGetItemState(i + idx, j + jdx);

			// on dessine l'élément courant
			this->OnDrawState(dc, rect, i + idx, j + jdx, etat);

			// on met à jour les coordonnées horizontales
			rect.left += cx; rect.right += cx;
		}

		// on met à jour les coordonnées verticales
		rect.top += cy; rect.bottom += cy;
	}
}

void JFCViewport::OnDrawItems(CDC & dc, LONG x, LONG y, LONG cx, LONG cy, LONG i, LONG j, LONG ci, LONG cj)
{
	RECT rect;

	// on initialise les coordonnées verticales
	rect.top = y; rect.bottom = y + cy;

	// on parcourt tous les éléments verticaux à dessiner
	for (LONG jdx = 0; jdx < cj; jdx += 1)
	{
		// on initialise les coordonnées horizontales
		rect.left = x; rect.right = x + cx;

		// on parcourt tous les éléments horizontaux à dessiner
		for (LONG idx = 0; idx < ci; idx += 1)
		{
			// on dessine l'élément courant
			this->OnDrawItem(dc, rect, i + idx, j + jdx);

			// on met à jour les coordonnées horizontales
			rect.left += cx; rect.right += cx;
		}

		// on met à jour les coordonnées verticales
		rect.top += cy; rect.bottom += cy;
	}
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner la fenêtre
/////////////////////////////////////////////////////////////////////////////

void JFCViewport::OnDrawWindow(CDC & dc, LONG x, LONG y, LONG dx, LONG dy)
{
	// on teste si on a quelque chose à dessiner
	if ((dx > 0) && (dy > 0))
	{
		LONG i1, j1, i2, j2;
		LONG x1, y1, x2, y2;

		// on récupère les indices des éléments à dessiner
		this->GetTrackIdx(x,          y,          i1, j1, FALSE);
		this->GetTrackIdx(x + dx - 1, y + dy - 1, i2, j2, FALSE);

		// on récupère la position du premier élément
		if (this->GetPosAtIdx(i1, j1, x1, y1, x2, y2))
		{
			// on calcule la position
			LONG x3 = x1 - x;
			LONG y3 = y1 - y;

			// on calcule le nombre de cellules
			LONG ci = i2 - i1 + 1;
			LONG cj = j2 - j1 + 1;

			// on calcule la taille des cellules
			LONG cx = x2 - x1 + 1;
			LONG cy = y2 - y1 + 1;

			// on prépare le dessin des éléments
			if (this->OnBeginDrawing(dc))
			{
				// on dessine le fond des éléments
				this->OnDrawStates(dc, x3, y3, cx, cy, i1, j1, ci, cj);

				// on dessine les cellules des éléments
				this->OnDrawItems(dc, x3, y3, cx, cy, i1, j1, ci, cj);
			}

			// on termine le dessin des éléments
			this->OnEndDrawing(dc);
		}
	}
}

void JFCViewport::OnDrawBorder(CDC & dc, RECT client, LONG x, LONG y, LONG dx, LONG dy)
{
	// on teste si on a des bordures a dessiner
	if ((m_HorzBorder != FALSE) || (m_VertBorder != FALSE))
	{
		// on teste si on doit dessiner le cadre
		if ((x <= client.left) || (y <= client.top) || ((x + dx) >= client.right) || ((y + dy) >= client.bottom))
		{
			// on crée le stylo A
			CPen PenA(PS_SOLID, 1, CATPColors::GetColorSelect(m_CATPColorsApp));

			// on sélectionne le stylo A
			CPen* pOldPen = dc.SelectObject(&PenA);

			// on récupère la position de la bordure
			LONG x1 = client.left   - x;
			LONG x2 = client.right  - x - 1;
			LONG y1 = client.top    - y;
			LONG y2 = client.bottom - y - 1;

			// on teste si on doit tracer les bordures horizontales
			if (m_HorzBorder != FALSE)
			{
				// on trace les bordures horizontales
				dc.MoveTo(x1, y1); dc.LineTo(x2 + 1, y1);
				dc.MoveTo(x1, y2); dc.LineTo(x2 + 1, y2);
			}

			// on teste si on doit tracer les bordures verticales
			if (m_VertBorder != FALSE)
			{
				// on trace les bordures verticales
				dc.MoveTo(x1, y1); dc.LineTo(x1, y2 + 1);
				dc.MoveTo(x2, y1); dc.LineTo(x2, y2 + 1);
			}

			// on restaure le stylo
			dc.SelectObject(pOldPen);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// JFCViewport message handlers

void JFCViewport::PreSubclassWindow() 
{
	// on initialise les bordures
	m_HorzBorder = m_VertBorder = FALSE;

	// on initialise les ascenseurs
	m_HorzMaster  = m_VertMaster  = FALSE;
	m_pHorzScroll = m_pVertScroll = NULL;

	// on initialise les éléments
	m_HorzFirst = m_HorzLast = m_HorzSize = 0;
	m_VertFirst = m_VertLast = m_VertSize = 0;

	// on appelle le gestionnaire de base
	this->JFControl::PreSubclassWindow();

	// on configure les ascenseurs
	this->OnSetScrollBar(FALSE);
}

void JFCViewport::OnPaint() 
{
	CDC DcComp;
	RECT Client;
	CBitmap BmpNew;

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

			// on dessine la fenêtre
			this->OnDrawWindow(DcComp, x, y, dx, dy);

			// on dessine les bordures
			this->OnDrawBorder(DcComp, Client, x, y, dx, dy);

			// on copie la bitmap dans la zone à dessiner de la fenêtre
			Dc.BitBlt(x, y, dx, dy, &DcComp, 0, 0, SRCCOPY);

			// on restaure la bitmap initiale
			DcComp.SelectObject(pBmpOld);
		}

		// on détruit le contexte de périphérique compatible
		DcComp.DeleteDC();
	}
}

void JFCViewport::OnSize(UINT nType, INT cx, INT cy) 
{
	// on appelle le gestionnaire de base
	this->JFControl::OnSize(nType, cx, cy);
	
	// on met à jour les ascenseurs
	this->OnSetScrollBar(FALSE);
}
