// PreviewBar.cpp : implementation file
//

#include "stdafx.h"
#include "afxcmn.h"
#include "ATPrintPDF.h"
#include "PreviewBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static UINT BASED_CODE ButtonsID[] =
{
	// same order as in the bitmap 'styles.bmp'
	ID_ABOUTBOX,			// A propos
	ID_SEPARATOR,	
	ID_PRINTCURRENTPAGE,	// bouton impression de la page courante
	ID_PRINTALLPAGE,		// bouton impression de toutes les pages
	ID_SEPARATOR,			// 	
	ID_FIRSTPAGE,			// bouton "aller à la première page"
	ID_PREVPAGE,			// bouton "aller à la page précédente"
	ID_SEPARATOR,			// combo box numéro de la page
	ID_NEXTPAGE,			// bouton "aller à la page suivante"
	ID_LASTPAGE,			// bouton "aller à la dernière page"
	ID_SEPARATOR,			// bouton zoom -
	ID_ZOOMOUT,				// 
	ID_SEPARATOR,			// slider zoom
	ID_ZOOMIN,				// 
	ID_SEPARATOR,			// bouton zoom +
	ID_EXPORTPDF,			// export PDF
	ID_EXPORTEXCEL,			// export Excel
	ID_EXPORTEXCEL03,       // autre .... 
	ID_SEPARATOR,			// PDF
	ID_CONTRASTE,			// Bouton contraste (desactivé)
	IDW_SLIDE_NUANCIER,		// ruban nuancier de gris
	ID_SEPARATOR
};

BEGIN_MESSAGE_MAP(CPreviewBar, CToolBar)
	//{{AFX_MSG_MAP(CPreviewBar)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewBar

CPreviewBar::CPreviewBar()
{	

}
//////////////////////////////////////////////////
// CPreviewBar message handlers
//////////////////////////////////////////////////

int CPreviewBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// on charge la bitmap qui contient les dessins des boutons
	if (!LoadBitmap(IDB_PREVIEW_BUTTONS)) return (-1);

	// on associe les boutons à leurs ID
	if (!SetButtons(ButtonsID, sizeof(ButtonsID)/sizeof(UINT))) return (-1);

	// on crée la combobox
	this->SetButtonInfo(7, IDW_COMBO, TBBS_SEPARATOR, 100);
	// on calcule le rectangle de la combo
	CRect comborect; this->GetItemRect(7, &comborect);
	comborect.top = 0; comborect.bottom = comborect.top + 100;
	if (!this->m_ComboPage.Create(CBS_DROPDOWNLIST|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL, comborect, this, IDW_COMBO)) return (-1);	
		
	// on crée le nuancier de gris
	this->SetButtonInfo(18, IDW_SLIDE_NUANCIER, TBBS_SEPARATOR, 100);
	// on calcule le rectangle du nuancier de gris
	CRect sliderrect; this->GetItemRect(18, &sliderrect);
	sliderrect.right = sliderrect.left + 120;
	sliderrect.top = 0; sliderrect.bottom = sliderrect.top + 20;
	if (!this->m_SliderNuancier.Create(WS_VISIBLE | TBS_AUTOTICKS  | TBS_ENABLESELRANGE | TBS_BOTH  , sliderrect, this, IDW_SLIDE_NUANCIER)) return (-1);
	// Gestion de la taille et de la position du slider
	m_SliderNuancier.SetRange(GestionCouleurs::GetMinContraste() ,GestionCouleurs::GetMaxContraste(), FALSE);
	m_SliderNuancier.SetPos(GestionCouleurs::GetContraste());
	
	// on remplit les champs de la combobox	
	return 0;
}

void CPreviewBar::SetZoomValue(long zoomvalue)
{
	//on ne fait rien
}

// recuperation de la valeur du nuancier
long CPreviewBar::GetValeurGris()
{
	return ( this-> m_SliderNuancier.GetPos());
}

// on fixe la position du slider
void CPreviewBar::SetValeurGris(long valeurGris)
{
	this->m_SliderNuancier.SetPos(valeurGris);
}

// Evenement sur le nuancier
void CPreviewBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Changement de contraste lors du deplacement du slider
	if (nSBCode == SB_ENDSCROLL)
	{
		long pos = m_SliderNuancier.GetPos();
		GestionCouleurs::SetContraste (pos);
		m_pDoc->UpdateAllViews(NULL, UPDATE_PAGE_CHANGED, NULL);
	}
}


// =======================================================
// Destructeur
// =======================================================
CPreviewBar::~CPreviewBar()
{
}


