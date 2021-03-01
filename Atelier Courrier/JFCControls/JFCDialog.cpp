// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// les constructeurs
/////////////////////////////////////////////////////////////////////////////
 
JFCDialog::JFCDialog()
{
	// on initialise la taille
	m_HStretch = m_HSize = m_HCoef = 0;
	m_VStretch = m_VSize = m_VCoef = 0;
}

JFCDialog::JFCDialog(UINT nIDTemplate, CWnd* pParent) : CDialog(nIDTemplate, pParent)
{
	// on initialise la taille
	m_HStretch = m_HSize = m_HCoef = 0;
	m_VStretch = m_VSize = m_VCoef = 0;
}

JFCDialog::JFCDialog(LPCTSTR lpszTemplateName, CWnd* pParent) : CDialog(lpszTemplateName, pParent)
{
	// on initialise la taille
	m_HStretch = m_HSize = m_HCoef = 0;
	m_VStretch = m_VSize = m_VCoef = 0;
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur
/////////////////////////////////////////////////////////////////////////////

JFCDialog::~JFCDialog()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCDialog, CDialog)
	//{{AFX_MSG_MAP(JFCDialog)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////
// la fonction pour mettre à jour les vues
VOID JFCDialog::UpdateAllViews(const JFCUpdate & Message)
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer les modes de déplacement
/////////////////////////////////////////////////////////////////////////////

void JFCDialog::SetMode(CWnd* pWnd, LONG HMode, LONG VMode)
{
	// on teste si on a quelque chose à faire
	if (pWnd != NULL)
	{
		Link Infos;

		// on vérifie la validité de la fenêtre
		ASSERT(::IsWindow(pWnd->m_hWnd));

		// on recherche l'élément
		BOOL trouve = m_Items.Lookup(pWnd, Infos);

		// on teste si on doit insérer un nouvel élément
		if (trouve == FALSE)
		{
			RECT rect;

			// on récupère le rectangle de la fenêtre
			pWnd->GetWindowRect(&rect);

			// on convertit le rectangle dans le repère de la boîte
			this->ScreenToClient(&rect);

			// on initialise les infos
			Infos.m_HPos     = rect.left;
			Infos.m_VPos     = rect.top;
			Infos.m_HSize    = rect.right  - rect.left;
			Infos.m_VSize    = rect.bottom - rect.top;
			Infos.m_HMode    = HMode;
			Infos.m_VMode    = VMode;
			Infos.m_pHScroll = NULL; 
			Infos.m_pVScroll = NULL;
		}
		else
		{
			// on met à jour les indicateurs
			Infos.m_HMode = HMode;
			Infos.m_VMode = VMode;
		}

		// on insère ou on modifie l'élément
		m_Items.SetAt(pWnd, Infos);
	}
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer la gestion des ascenseurs
/////////////////////////////////////////////////////////////////////////////

void JFCDialog::SetScrollBar(CWnd* pWnd, CWnd* pHScroll, CWnd* pVScroll)
{
	// on teste si on a quelque chose à faire
	if (pWnd != NULL)
	{
		Link Infos;

		// on vérifie la validité de la fenêtre
		ASSERT(::IsWindow(pWnd->m_hWnd));

		// on recherche l'élément
		BOOL trouve = m_Items.Lookup(pWnd, Infos);

		// on teste si on doit insérer un nouvel élément
		if (trouve == FALSE)
		{
			RECT rect;

			// on récupère le rectangle de la fenêtre
			pWnd->GetWindowRect(&rect);

			// on convertit le rectangle dans le repère de la boîte
			this->ScreenToClient(&rect);

			// on initialise les infos
			Infos.m_HPos     = rect.left;
			Infos.m_VPos     = rect.top;
			Infos.m_HSize    = rect.right  - rect.left;
			Infos.m_VSize    = rect.bottom - rect.top;
			Infos.m_HMode    = MGER_NONE;
			Infos.m_VMode    = MGER_NONE;
			Infos.m_pHScroll = pHScroll;
			Infos.m_pVScroll = pVScroll;
		}
		else
		{
			// on met à jour les indicateurs
			Infos.m_pHScroll = pHScroll;
			Infos.m_pVScroll = pVScroll;
		}

		// on insère ou on modifie l'élément
		m_Items.SetAt(pWnd, Infos);
	}
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer les fenêtres du stretching
/////////////////////////////////////////////////////////////////////////////

void JFCDialog::SetStretchWnd(CWnd* pHStretch, CWnd* pVStretch)
{
	// on initialise les dimensions
	m_HStretch = m_VStretch = 0;

	// on teste si on a quelque chose à faire
	if (pHStretch != NULL)
	{
		RECT rect;

		// on vérifie la validité de la fenêtre
		ASSERT(::IsWindow(pHStretch->m_hWnd));

		// on récupère le rectangle de la fenêtre
		pHStretch->GetWindowRect(&rect);

		// on mémorise la largeur de la fenêtre
		m_HStretch = rect.right - rect.left;
	}

	// on teste si on a quelque chose à faire
	if (pVStretch != NULL)
	{
		RECT rect;

		// on vérifie la validité de la fenêtre
		ASSERT(::IsWindow(pVStretch->m_hWnd));

		// on récupère le rectangle de la fenêtre
		pVStretch->GetWindowRect(&rect);

		// on mémorise la largeur de la fenêtre
		m_VStretch = rect.bottom - rect.top;
	}
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour modifier les coefficients de stretch
/////////////////////////////////////////////////////////////////////////////

void JFCDialog::SetStretchCoef(LONG HCoef, LONG VCoef)
{
	RECT rect;

	// on récupère le rectangle de la boîte
	this->GetClientRect(&rect);

	// on met à jour les coefficients
	if ((HCoef >= 1) && (HCoef <= 128)) m_HCoef = HCoef - 1;
	if ((VCoef >= 1) && (VCoef <= 128)) m_VCoef = VCoef - 1;

	// on met à jour la fenêtre
	this->OnUpdateWndPos(rect.right - rect.left, rect.bottom - rect.top); 
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour déplacer les éléments
/////////////////////////////////////////////////////////////////////////////

void JFCDialog::OnUpdateWndPos(LONG cx, LONG cy)
{
	// on calcule l'étirement
	LONG dx = cx - m_HSize;
	LONG dy = cy - m_VSize;

	// on calcule le décalage
	LONG sx = m_HCoef * m_HStretch;
	LONG sy = m_VCoef * m_VStretch;

	// on récupère le nombre d'éléments
	LONG taille = m_Items.GetCount();
	
	// on teste si on a quelque chose à faire
	if (taille > 0)
	{
		// on initialise le déplacement des éléments
		HDWP hdwp = ::BeginDeferWindowPos(taille);
		
		// on vérifie la validité du déplacement
		if (hdwp != NULL)
		{
			// on se place sur le premier élément
			POSITION position = m_Items.GetStartPosition();

			// on parcourt tous les éléments
			while (position != NULL)
			{
				Link  Infos;
				CWnd* pWnd;

				// on récupère l'élément courant
				m_Items.GetNextAssoc(position, pWnd, Infos);

				// on vérifie la validité de la fenêtre
				if (::IsWindow(pWnd->m_hWnd) != FALSE)
				{
					// on teste si on doit déplacer la fenêtre
					if (this == pWnd->GetParent())
					{
						// on récupère la position initiale de l'élément
						LONG HPos  = Infos.m_HPos;
						LONG VPos  = Infos.m_VPos;
						LONG HSize = Infos.m_HSize;
						LONG VSize = Infos.m_VSize;

						// on calcule la nouvelle position de l'élément
						this->OnUpdatePosition(dx, sx, Infos.m_HMode, HPos, HSize);
						this->OnUpdatePosition(dy, sy, Infos.m_VMode, VPos, VSize);

						// on met à jour la position de l'élément
						hdwp = ::DeferWindowPos(hdwp, pWnd->m_hWnd, NULL, HPos, VPos, HSize, VSize, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER);
					}
				}
			}

			// on effecture le déplacement
			BOOL succes = ::EndDeferWindowPos(hdwp);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour mettre à jour les positions
/////////////////////////////////////////////////////////////////////////////

void JFCDialog::OnUpdatePosition(LONG dx, LONG sx, LONG mode, LONG & pos, LONG & size)
{
	// on teste le mode
	switch (mode)
	{
	case MGER_NONE:
		// on ne fait rien
		break;
	case MGER_MOVE:
		// on met à jour la fenêtre
		if (dx > 0) pos += dx;
		break;
	case MGER_SIZE:
		// on met à jour la fenêtre
		if (dx > 0) size += dx;
		break;
	case MGER_SHIFT:
		// on met à jour la fenêtre
		if (sx > 0) pos += sx;
		break;
	case MGER_STRETCH:
		// on met à jour la fenêtre
		if (sx > 0) size += sx;
		break;	
	case MGER_SHIFTANDMOVE:
		// on met à jour la fenêtre
		if (sx > 0) { if (dx > sx) pos += dx; else pos += sx; } else { if (dx > 0) pos += dx; }
		break;	
	case MGER_SHIFTANDSIZE:
		// on met à jour la fenêtre
		if (sx > 0) { pos += sx; if (dx > sx) size += dx - sx; } else { if (dx > 0) size += dx; }
		break;	
	case MGER_STRETCHANDSIZE:
		// on met à jour la fenêtre
		if (sx > 0) { if (dx > sx) size += dx; else size += sx; } else { if (dx > 0) size += dx; }
		break;	
	default:
		// on ne fait rien
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour mettre à jour les éléments
/////////////////////////////////////////////////////////////////////////////

void JFCDialog::OnUpdateHorzScroll(CWnd* pScroll)
{
	// on teste si on a quelque chose à faire
	if (pScroll != NULL)
	{
		// on se place sur le premier élément
		POSITION position = m_Items.GetStartPosition();

		// on parcourt tous les éléments
		while (position != NULL)
		{
			Link  Infos;
			CWnd* pWnd;

			// on récupère l'élément courant
			m_Items.GetNextAssoc(position, pWnd, Infos);

			// on met à jour l'élément
			if (Infos.m_pHScroll == pScroll) { pWnd->InvalidateRect(NULL, FALSE); pWnd->UpdateWindow(); }
		}
	}
}
	
void JFCDialog::OnUpdateVertScroll(CWnd* pScroll)
{
	// on teste si on a quelque chose à faire
	if (pScroll != NULL)
	{
		// on se place sur le premier élément
		POSITION position = m_Items.GetStartPosition();

		// on parcourt tous les éléments
		while (position != NULL)
		{	
			Link  Infos;
			CWnd* pWnd;

			// on récupère l'élément courant
			m_Items.GetNextAssoc(position, pWnd, Infos);

			// on met à jour l'élément
			if (Infos.m_pVScroll == pScroll) { pWnd->InvalidateRect(NULL, FALSE); pWnd->UpdateWindow(); }
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// JFCDialog message handlers

void JFCDialog::PreSubclassWindow() 
{
	// on initialise les éléments
	m_Items.RemoveAll();

	// on initialise la taille
	m_HStretch = m_HSize = 0; m_HCoef = 0;
	m_VStretch = m_VSize = 0; m_VCoef = 0;

	// on appelle le gestionnaire de base
	this->CDialog::PreSubclassWindow();
}

BOOL JFCDialog::OnInitDialog() 
{
	RECT rect;

	// on appelle le gestionnaire de base
	this->CDialog::OnInitDialog();
	
	// on récupère le rectangle de la boîte
	this->GetClientRect(&rect);

	// on mémorise la taille initiale de la boîte
	m_HSize = rect.right  - rect.left;
	m_VSize = rect.bottom - rect.top;

	// on quitte	
	return (TRUE);
}

void JFCDialog::OnSize(UINT nType, INT cx, INT cy) 
{
	// on appelle le gestionnaire de base
	this->CDialog::OnSize(nType, cx, cy);
	
	// on met à jour la position des éléments
	if ((m_HSize > 0) && (m_VSize > 0)) this->OnUpdateWndPos(cx, cy);
}

void JFCDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// on vérifie la provenance du message
	if (pScrollBar != NULL)
	{
		// on positionne l'indicateur
		BOOL update = TRUE;

		// on teste si l'ascenseur est actif
		if (pScrollBar->IsWindowEnabled() != FALSE)
		{
			SCROLLINFO infos;
	
			// on initialise les informations
			infos.cbSize = sizeof(infos);
			infos.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;

			// on récupère l'état de l'ascenseur
			pScrollBar->GetScrollInfo(&infos, infos.fMask);

			// on teste le code de notification
			switch(nSBCode)
			{
				case SB_RIGHT:
					// on déplace le curseur à droite au maximum
					infos.nPos = infos.nMax;
					break;
				case SB_PAGERIGHT:
					// on déplace le curseur d'une page à droite
					infos.nPos += (infos.nPage > 1) ? (infos.nPage - 1) : 1;
					break;
				case SB_LINERIGHT:
					// on déplace le curseur d'une ligne à droite
					infos.nPos += 1;
					break;
				case SB_LEFT:
					// on déplace le curseur à gauche au maximum
					infos.nPos  = infos.nMin;
					break;
				case SB_PAGELEFT:
					// on déplace le curseur d'une page à gauche
					infos.nPos -= (infos.nPage > 1) ? (infos.nPage - 1) : 1;
					break;
				case SB_LINELEFT:
					// on déplace le curseur d'une ligne à gauche
					infos.nPos -= 1;
					break;
				case SB_THUMBTRACK:
					// on déplace le curseur à la position courante
					infos.nPos = infos.nTrackPos;
					break;
				case SB_ENDSCROLL:
					// on déplace le curseur
					infos.nPos = infos.nPos;
					break;
				default:
					// on annule l'indicateur
					update = FALSE;
					break;
			}

			// on met à jour les informations
			infos.cbSize = sizeof(infos);
			infos.fMask  = SIF_POS;

			// on met à jour l'ascenseur
			pScrollBar->SetScrollInfo(&infos, TRUE);
		}

		// on met à jour l'affichage
		if (update != FALSE) this->OnUpdateHorzScroll((CWnd*)pScrollBar);
	}

	// on appelle le gestionnaire de base
	this->CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void JFCDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// on vérifie la provenance du message
	if (pScrollBar != NULL)
	{
		// on positionne l'indicateur
		BOOL update = TRUE;

		// on teste si l'ascenseur est actif
		if (pScrollBar->IsWindowEnabled() != FALSE)
		{
			SCROLLINFO infos;

			// on initialise les informations
			infos.cbSize = sizeof(infos);
			infos.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;

			// on récupère l'état de l'ascenseur
			pScrollBar->GetScrollInfo(&infos, infos.fMask);

			// on teste le code de notification
			switch(nSBCode)
			{
				case SB_BOTTOM:
					// on déplace le curseur en bas au maximum
					infos.nPos = infos.nMax;
					break;
				case SB_PAGEDOWN:
					// on déplace le curseur d'une page en bas
					infos.nPos += (infos.nPage > 1) ? (infos.nPage - 1) : 1;
					break;
				case SB_LINEDOWN:
					// on déplace le curseur d'une ligne en bas
					infos.nPos += 1;
					break;
				case SB_TOP:
					// on déplace le curseur en haut au maximum
					infos.nPos  = infos.nMin;
					break;
				case SB_PAGEUP:
					// on déplace le curseur d'une page en haut
					infos.nPos -= (infos.nPage > 1) ? (infos.nPage - 1) : 1;
					break;
				case SB_LINEUP:
					// on déplace le curseur d'une ligne en haut
					infos.nPos -= 1;
					break;
				case SB_THUMBTRACK:
					// on déplace le curseur à la position courante
					infos.nPos = infos.nTrackPos;
					break;
				case SB_ENDSCROLL:
					// on déplace le curseur
					infos.nPos = infos.nPos;
					break;
				default:
					// on annule l'indicateur
					update = FALSE;
					break;
			}

			// on met à jour les informations
			infos.cbSize = sizeof(infos);
			infos.fMask  = SIF_POS;

			// on met à jour l'ascenseur
			pScrollBar->SetScrollInfo(&infos, TRUE);
		}

		// on met à jour l'affichage
		if (update != FALSE) this->OnUpdateVertScroll((CWnd*)pScrollBar);
	}

	// on appelle le gestionnaire de base
	this->CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}
