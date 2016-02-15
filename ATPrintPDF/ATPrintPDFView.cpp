// ATPrintPDFView.cpp : implementation of the CATPrintPDFView class
//

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "ATPrintPDF.h"
#include "ATPrintPDFDoc.h"
#include "ATPrintPDFView.h"
#include "./Draw/JFCGDIDraw.h"
#include "./Draw/JFCPDFDraw.h"
#include "atPrDico.h"
#include "pdflib\\utzadico.h"
#include "ExcelProgressSplash.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CATPrintPDFApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CATPrintPDFView

IMPLEMENT_DYNCREATE(CATPrintPDFView, CView)

BEGIN_MESSAGE_MAP(CATPrintPDFView, CView)
	//{{AFX_MSG_MAP(CATPrintPDFView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()	
	ON_WM_MOUSEWHEEL()
	ON_WM_PARENTNOTIFY()
	ON_COMMAND(ID_PRINTCURRENTPAGE, OnPrintcurrentpage)
	ON_COMMAND(ID_PRINTALLPAGE, OnPrintallpage)
	ON_COMMAND(ID_FIRSTPAGE, OnViewFirstpage)
	ON_COMMAND(ID_PREVPAGE, OnViewPrevpage)
	ON_COMMAND(ID_NEXTPAGE, OnViewNextpage)
	ON_COMMAND(ID_LASTPAGE, OnViewLastpage)
	ON_COMMAND(ID_ZOOMIN, OnViewZoomin)	
	ON_COMMAND(ID_ZOOMOUT, OnViewZoomout)
	ON_COMMAND(ID_EXPORTPDF, OnFileExportpdf)
	ON_COMMAND(ID_EXPORTEXCEL, OnFileExportExcel)	
	ON_COMMAND(ID_EXPORTEXCEL03, OnFileExportExcel03)	
	ON_CBN_SELCHANGE(IDW_COMBO, OnSelChangePage)
	ON_UPDATE_COMMAND_UI(ID_FIRSTPAGE, OnUpdateViewFirstpage)
	ON_UPDATE_COMMAND_UI(ID_PREVPAGE, OnUpdateViewPrevpage)
	ON_UPDATE_COMMAND_UI(ID_NEXTPAGE, OnUpdateViewNextpage)
	ON_UPDATE_COMMAND_UI(ID_LASTPAGE, OnUpdateViewLastpage)
	ON_UPDATE_COMMAND_UI(ID_EXPORTPDF, OnUpdateFileExportpdf)
	ON_UPDATE_COMMAND_UI(ID_EXPORTEXCEL, OnUpdateFileExportExcel)
	ON_UPDATE_COMMAND_UI(ID_EXPORTEXCEL03, OnUpdateFileExportExcel)// new excel
	ON_UPDATE_COMMAND_UI(ID_ZOOMIN, OnUpdateZoomin)
	ON_UPDATE_COMMAND_UI(ID_ZOOMOUT, OnUpdateZoomout)

	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

//=========================================================================
// le constructeur:
//=========================================================================
CATPrintPDFView::CATPrintPDFView()
{
	// on récupère la résolution de l'écran
	HDC hdc = (::GetDC(::GetDesktopWindow()));

	// on récupère la résolution de l'écran
	long ResX = (::GetDeviceCaps(hdc, HORZRES));
	long ResY = (::GetDeviceCaps(hdc, VERTRES));

	// on récupère les dimensions de l'écran
	long SizeX = (::GetDeviceCaps(hdc, HORZSIZE));
	long SizeY = (::GetDeviceCaps(hdc, VERTSIZE));

	m_PageInfo = new CPageInfo(ResX, ResY, SizeX, SizeY);
	
	if (theApp.m_ModePaysage == FALSE)
	{
		// on fixe les paramètres
		m_PageInfo->SetPageHeight(2970);
		m_PageInfo->SetPageWidth(2100);
		m_PageInfo->SetZoom(100);
		m_PageInfo->SetCurrentPage(1);
	}
	else
	{
		// on fixe les paramètres
		m_PageInfo->SetPageHeight(2100);
		m_PageInfo->SetPageWidth(2970);
		m_PageInfo->SetZoom(80);
		m_PageInfo->SetCurrentPage(1);
	}

	
	m_IsCurrentPage = FALSE;

	// on initialise le pointeur sur la fenêtre d'aperçu
	m_pWindow = 0;
	m_PreviewBar = 0;
}

BOOL CATPrintPDFView::PreCreateWindow(CREATESTRUCT& cs)
{
	// on renvoie le traitement par défaut
	return CView::PreCreateWindow(cs);
}

//============================================================================
// la fonction de création:
//============================================================================
int CATPrintPDFView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_PreviewBar = new CPreviewBar();

	// on crée la toolbar
	m_PreviewBar->CreateEx(this->GetParent(), TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(0, 0, 0, 0), IDW_PREVIEWBAR);
	
	// on récupère le document
	CATPrintPDFDoc* pDoc = GetDocument();

	// on fixe les paramètre de la toolbar
	m_PreviewBar->m_pDoc = pDoc;
	m_PreviewBar->m_pPageInfo = m_PageInfo;
	
	// on initialise le pointeur sur le document
	m_PageInfo->SetDrawDocument(&pDoc->m_pDrawDoc);

	// on crée la fenêtre membre
	m_pWindow = new CViewportWindow(m_PageInfo);

	// on récupère le rectangle client
	RECT ClientRect; this->GetClientRect(&ClientRect);

	// on calcule la taille de la scrollbar horizontale
	RECT HorzScrollBarRect;
	HorzScrollBarRect.top = 0; HorzScrollBarRect.bottom = 0; HorzScrollBarRect.left = 0; HorzScrollBarRect.right = 0;

	// on calcule la taille de la scrollbar verticale
	RECT VertScrollBarRect;
	VertScrollBarRect.top = 0; VertScrollBarRect.bottom = 0; VertScrollBarRect.left   = 0; VertScrollBarRect.right  = 0;

	// on crée la fenêtre
	if (m_pWindow) m_pWindow->Create(NULL, "Clipboard", WS_CHILD|WS_VISIBLE, ClientRect, this, ID_WNDCLIPBOARD);

	// on crée les scrollbars
	m_HorzScrollBar.Create(WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_TOPALIGN, HorzScrollBarRect, this, ID_HORZSCROLLBAR);
	m_VertScrollBar.Create(WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_LEFTALIGN , VertScrollBarRect, this, ID_VERTSCROLLBAR);

	return 0;
}

//============================================================================
// la fonction pour dessiner la vue:
//============================================================================
void CATPrintPDFView::OnDraw(CDC* pDC)
{
	CATPrintPDFDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (pDC->IsPrinting())
	{
		
	}
	else
	{

		// on crée le pinceau
		CBrush brush; 
		if (!m_HorzScrollBar.IsWindowEnabled() || !this->m_VertScrollBar.IsWindowEnabled()) brush.CreateSolidBrush(::GetSysColor(COLOR_APPWORKSPACE));
		else brush.CreateSolidBrush(RGB(255, 255, 255));

		// on récupère le rectangle client
		RECT ClientRect; GetClientRect(&ClientRect);

		// on remplit le rectangle avec le pinceau
		pDC->FillRect(&ClientRect, &brush);

		// on récupère le rectangle en bas à droite
		RECT PetitRectangleEnBasADroite = ClientRect;
		PetitRectangleEnBasADroite.top  = PetitRectangleEnBasADroite.bottom - (::GetSystemMetrics(SM_CYHSCROLL));
		PetitRectangleEnBasADroite.left = PetitRectangleEnBasADroite.right  - (::GetSystemMetrics(SM_CXVSCROLL));

		// on change la couleur du pinceau
		brush.DeleteObject();
		brush.CreateSolidBrush(::GetSysColor(COLOR_ACTIVEBORDER));

		// on remplit le petit rectangle
		pDC->FillRect(&PetitRectangleEnBasADroite, &brush);
	}
}

//============================================================================
// les fonctions d'aide au débuggage:
//============================================================================
#ifdef _DEBUG
void CATPrintPDFView::AssertValid() const
{
	CView::AssertValid();
}

void CATPrintPDFView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CATPrintPDFDoc* CATPrintPDFView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CATPrintPDFDoc)));
	return (CATPrintPDFDoc*)m_pDocument;
}
#endif //_DEBUG

//============================================================================
// la fonction de gestion lors du retaillage:
//============================================================================
void CATPrintPDFView::OnSize(UINT nType, int cx, int cy) 
{
	// on met à jour la vue enfant
	UpdateChild();
}

//=========================================================================
// les fonctions de gestion des scrollbars:
//=========================================================================
void CATPrintPDFView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	{
		// on positionne le flag
		BOOL update = TRUE;
		
		// on teste si l'ascenseur est actif
		if (pScrollBar->IsWindowEnabled() != FALSE)
		{
			// la structure 
			SCROLLINFO infos;

			// on initialise les informations
			infos.cbSize = sizeof(infos);
			infos.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;

			// on récupère l'état de l'ascenseur
			pScrollBar->GetScrollInfo(&infos, infos.fMask);

			// teste le code de notification
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
				infos.nPos += 5;
				break;
			case SB_LEFT:
				// on déplace le curseur à gauche au maximum
				infos.nPos = infos.nMin;
				break;
			case SB_PAGELEFT:
				// on déplace le curseur d'une page à gauche
				infos.nPos -= (infos.nPage > 1) ? (infos.nPage - 1) : 1;
				break;
			case SB_LINELEFT:
				// on déplace le curseur d'une ligne à gauche
				infos.nPos -= 5;
				break;
			case SB_THUMBTRACK:
				// on déplace le curseur à la position courante
				infos.nPos = infos.nTrackPos;
				break;
			case SB_ENDSCROLL:
				infos.nPos = infos.nPos;
				break;
			default:
				// on annule l'indicateur
				update = FALSE;
				break;
			}
			// on met à jour les informations
			infos.cbSize = sizeof(infos);
			infos.fMask = SIF_POS;

			// on met à jour l'ascenseur
			pScrollBar->SetScrollInfo(&infos, TRUE);
			pScrollBar->GetScrollInfo(&infos, SIF_ALL);
		
			// on met à jour l'affichage
			if (m_pWindow!=0)
			{
				m_PageInfo->SetOrgX_px(infos.nPos);
				m_pWindow->InvalidateRect(NULL, TRUE);				
			}			
		}
	}	
}

void CATPrintPDFView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// on teste le contrôle
	if (pScrollBar != NULL)
	{
		// on positionne le flag
		BOOL update = TRUE;

		// on teste si l'ascenseur est actif
		if (pScrollBar->IsWindowEnabled() != FALSE)
		{
			SCROLLINFO infos;

			// on initialise les informations
			infos.cbSize = sizeof(infos);
			infos.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;

			// on récupère l'état de l'ascenseur
			pScrollBar->GetScrollInfo(&infos, infos.fMask);

			// on teste le code de notification
			switch(nSBCode)
			{
			case SB_BOTTOM:
				// on déplace le curseur en bas au maximum
				infos.nPage = infos.nMax;
				break;
			case SB_PAGEDOWN:
				// on déplace le curseur d'une page en bas
				infos.nPos += (infos.nPage > 1) ? (infos.nPage - 1) : 1;
				break;
			case SB_LINEDOWN:
				// on déplace le curseur d'une ligne en bas
				infos.nPos += 5;
				break;
			case SB_TOP:
				// on déplace le curseur en haut au maximum
				infos.nPos = infos.nMin;
				break;
			case SB_PAGEUP:
				// on déplace le curseur d'une page en haut
				infos.nPos -= (infos.nPage > 1) ? (infos.nPage - 1) : 1;
				break;
			case SB_LINEUP:
				// on déplace le curseur d'une ligne en haut
				infos.nPos -= 5;
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
			infos.fMask = SIF_POS;

			// on met à jour l'ascenseur
			pScrollBar->SetScrollInfo(&infos, TRUE);
			pScrollBar->GetScrollInfo(&infos, SIF_ALL);
		
			// on positionne l'origine
			if (m_pWindow!= 0) 
			{
				m_PageInfo->SetOrgY_px(infos.nPos);
				m_pWindow->InvalidateRect(NULL, TRUE);
			}
		}
	}
	// on appelle le gestionnaire de base
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

//==============================================================================
// la fonction pour mettre à jour l'affichage de la fenêtre enfant:
//==============================================================================
void CATPrintPDFView::UpdateChild()
{
	// on récupère le rectangle client
	RECT ClientRect; this->GetClientRect(&ClientRect);

	// les rectangles dont on aura besoin
	RECT ClipboardRect;
	ClipboardRect.top = ClientRect.top;
	ClipboardRect.left = ClientRect.left;
	ClipboardRect.right = ClientRect.right - (::GetSystemMetrics(SM_CXVSCROLL));
	ClipboardRect.bottom = ClientRect.bottom - (::GetSystemMetrics(SM_CYHSCROLL));

	// on récupère les informations de la scrollbar horizontale
	SCROLLINFO HorzScrollInfo;
	m_HorzScrollBar.GetScrollInfo(&HorzScrollInfo, SIF_ALL);

	// on met à jour les paramètres
	HorzScrollInfo.nMax = m_PageInfo->GetSizeX_px();
	HorzScrollInfo.nMin = 0;
	HorzScrollInfo.nPage = ClipboardRect.right - ClipboardRect.left;

	// on teste si il faut afficher la scrollbar
	if ((UINT)(HorzScrollInfo.nMax)<=HorzScrollInfo.nPage) 
	{
		m_HorzScrollBar.EnableScrollBar(ESB_DISABLE_BOTH);
		m_PageInfo->SetOrgX_px(0);
	}
	else 
	{
		m_HorzScrollBar.EnableScrollBar(ESB_ENABLE_BOTH);
		m_HorzScrollBar.SetScrollInfo(&HorzScrollInfo, TRUE);
		if ((int)(HorzScrollInfo.nPos + HorzScrollInfo.nPage) <= m_PageInfo->GetSizeX_px()) m_PageInfo->SetOrgX_px(HorzScrollInfo.nPos);
		else m_PageInfo->SetOrgX_px(m_PageInfo->GetSizeX_px() - HorzScrollInfo.nPage);
	}

	// on récupère les infos de la scrollbar verticale
	SCROLLINFO VertScrollInfo;
	m_VertScrollBar.GetScrollInfo(&VertScrollInfo, SIF_ALL);
	
	// on met à jour les paramètres
	VertScrollInfo.nMax = m_PageInfo->GetSizeY_px();
	VertScrollInfo.nMin = 0;
	VertScrollInfo.nPage = ClipboardRect.bottom - ClipboardRect.top;

	// on teste si il faut afficher la scrollbar
	if ((UINT)(VertScrollInfo.nMax) <= VertScrollInfo.nPage)
	{
		m_VertScrollBar.EnableScrollBar(ESB_DISABLE_BOTH);
		m_PageInfo->SetOrgY_px(0);
	}
	else
	{
		m_VertScrollBar.EnableScrollBar(ESB_ENABLE_BOTH);
		m_VertScrollBar.SetScrollInfo(&VertScrollInfo, TRUE);
		if ((int)(VertScrollInfo.nPos + VertScrollInfo.nPage) <= m_PageInfo->GetSizeY_px()) m_PageInfo->SetOrgY_px(VertScrollInfo.nPos);
		else m_PageInfo->SetOrgY_px(m_PageInfo->GetSizeY_px()-VertScrollInfo.nPage);
	}

	// on fixe les taille et position des fenêtres
	if (m_pWindow) m_pWindow->SetWindowPos(NULL, ClipboardRect.left, ClipboardRect.top, ClipboardRect.right, ClipboardRect.bottom, SWP_NOZORDER);
	m_HorzScrollBar.SetWindowPos(NULL, ClipboardRect.left, ClipboardRect.bottom, ClipboardRect.right, (::GetSystemMetrics(SM_CYHSCROLL)), SWP_NOZORDER);
	m_VertScrollBar.SetWindowPos(NULL, ClipboardRect.right, ClipboardRect.top, (::GetSystemMetrics(SM_CXVSCROLL)), ClipboardRect.bottom, SWP_NOZORDER);
}

BOOL CATPrintPDFView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// on gere le deplacement de la VscrollBar
	long currentPos =m_VertScrollBar.GetScrollPos();
	m_VertScrollBar.SetScrollPos(currentPos - zDelta/6);
	this->OnVScroll(SB_ENDSCROLL, 0, &m_VertScrollBar);

	CView::OnMouseWheel(nFlags, (zDelta), pt);
	return true;
}

void CATPrintPDFView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// on récupère la position
	this->m_StartGrabX = point.x;
	this->m_StartGrabY = point.y;
	this->SetCapture();
	
	CView::OnLButtonDown(nFlags, point);
}

void CATPrintPDFView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// on teste la capture
	if (this == this->GetCapture())
	{
		(::ReleaseCapture());
	}
	// 
	CView::OnLButtonUp(nFlags, point);
}

void CATPrintPDFView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (this == this->GetCapture())
	{
		// on calcule le déplacement
		long moveX = point.x - m_StartGrabX;
		long moveY = point.y - m_StartGrabY;
		
		// on déplace l'origine si possible
		if (m_HorzScrollBar.IsWindowEnabled()) m_HorzScrollBar.SetScrollPos(m_OrgGrabX - moveX);
		if (m_VertScrollBar.IsWindowEnabled()) m_VertScrollBar.SetScrollPos(m_OrgGrabY - moveY);
		this->UpdateChild();
		this->m_pWindow->InvalidateRect(NULL);
	}
	// 
	CView::OnMouseMove(nFlags, point);
	
}

void CATPrintPDFView::OnParentNotify( UINT message, LPARAM lParam )
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		// on récupère la position
		this->m_StartGrabX = LOWORD(lParam);
		this->m_StartGrabY = HIWORD(lParam);
		// on récupère l'origine au moment du grab
		m_OrgGrabX = m_HorzScrollBar.GetScrollPos();
		m_OrgGrabY = m_VertScrollBar.GetScrollPos();
		this->SetCapture();
		break;
	case WM_MOUSEMOVE:
		if (this == this->GetCapture())
		{
			// on calcule le déplacement
			long moveX = LOWORD(lParam)- m_StartGrabX;
			long moveY = HIWORD(lParam) - m_StartGrabY;

			// on déplace l'origine si possible
			m_PageInfo->SetOrgX_px(m_PageInfo->GetOrgX_px() + moveX);
			m_PageInfo->SetOrgY_px(m_PageInfo->GetOrgY_px() + moveY);
			this->UpdateChild();
		}		
		break;
	case WM_LBUTTONUP:
		// on teste la capture
		if (this == this->GetCapture())
		{
			(::ReleaseCapture());
		}
		break;
	}
}

//============================================================================
// les fonctions de gestion de l'impression:
//============================================================================

BOOL CATPrintPDFView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// on recupere le document a imprimer
	DrawDocument * pDrawDoc = m_PageInfo->GetDrawDocument();

	if (!m_IsCurrentPage)
	{
		if (pDrawDoc != 0) 
		{
			// on affiche la boite de dialogue d'impression
			pInfo->m_bDirect = FALSE;


			// on imprime toutes les pages
			pInfo->SetMaxPage(pDrawDoc->GetNbPage());
		}
		else return (FALSE);
	}
	else
	{
		if (pDrawDoc != 0) 
		{
			// on empeche l'affichage de la boite de dialogue d'impression
			pInfo->m_bDirect = TRUE;

			// on initialise le nombre de pages
			pInfo->m_nCurPage = m_PageInfo->GetCurrentPage();

			// on recupere la page en cours
			pInfo->SetMinPage(pInfo->m_nCurPage);
			pInfo->SetMaxPage(pInfo->m_nCurPage);
		}
		else return (FALSE);
	}

	// on récupère les marges
	long margeTop, margeBottom, margeLeft, margeRight;
	m_PageInfo->GetMarges_px(margeTop, margeBottom, margeLeft, margeRight);

	// on fixe les marges
	pInfo->m_rectDraw.top = margeTop;
	pInfo->m_rectDraw.bottom = margeBottom;
	pInfo->m_rectDraw.left = margeLeft;
	pInfo->m_rectDraw.right = margeRight;

	// default preparation
 	return DoPreparePrinting(pInfo);
}

void CATPrintPDFView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

BOOL CALLBACK CATPrintPDFView::OnAbortProc(HDC hdc, int nCode)
{
	// on continue l'impression
	return (TRUE);
}

void CATPrintPDFView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	pDC->SetAbortProc(CATPrintPDFView::OnAbortProc);
	
	CView::OnPrepareDC(pDC, pInfo);
}

void CATPrintPDFView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// on récupère les données graphiques du document
	DrawDocument * pDrawDoc = m_PageInfo->GetDrawDocument();

	if (pDrawDoc != 0)
	{
		long printhorz = pDC->GetDeviceCaps(HORZSIZE);
		long printvert = pDC->GetDeviceCaps(VERTSIZE);

		long orgx = ((printhorz*10) - pDrawDoc->GetPageWidth()) / 2;
		long orgy = ((printvert*10) - pDrawDoc->GetPageHeight()) / 2;

		try
		{
			// on crée un jfcGDIDraw
			JFCGDIDraw MyDraw(pDC->m_hDC, orgx, orgy, 100, 0, 0);

			// on dessine le document
			pDrawDoc->Draw(&MyDraw, pInfo->m_nCurPage, pInfo->m_nCurPage);
		}
		catch(long)
		{
			return;
		}
	}
	else return;
}

void CATPrintPDFView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

//=========================================================================
// les fonctions pour gérer le zoom:
//=========================================================================
void CATPrintPDFView::Zoom(long zoom)
{
	if (zoom >= 25 && zoom <= 400)
	{
		m_PageInfo->SetZoom(zoom);
		m_PreviewBar->SetZoomValue((int)(sqrt((double)m_PageInfo->GetZoom())));
		
		this->UpdateChild();
	}
}

//==============================================================================
// la gestion des actions bouton
//==============================================================================

void CATPrintPDFView::OnPrintcurrentpage() 
{
	// on enleve la boite de dialogue
	m_IsCurrentPage = TRUE;	
	CView::OnFilePrint();
}
void CATPrintPDFView::OnPrintallpage() 
{
	// on remet la boite de dialogue
	m_IsCurrentPage = FALSE;
	OnFilePrint();
}
void CATPrintPDFView::OnViewFirstpage() 
{
	// on change la page courante
	m_PageInfo->SetCurrentPage(1);

	// on fixe la page courante dans la combo
	m_PreviewBar->m_ComboPage.SetCurSel(0);
	this->OnUpdate(NULL, UPDATE_FIRST_PAGE, NULL);
}

void CATPrintPDFView::OnViewPrevpage()
{
	// on change la page courante
	if (m_PageInfo->GetCurrentPage() > 1) m_PageInfo->SetCurrentPage(m_PageInfo->GetCurrentPage() - 1);

	// on fixe la page courante dans la combo
	m_PreviewBar->m_ComboPage.SetCurSel(m_PageInfo->GetCurrentPage()-1);
	this->OnUpdate(NULL, UPDATE_PREV_PAGE, NULL);	
}

void CATPrintPDFView::OnViewNextpage() 
{
	// on change la page courante
	if (m_PageInfo->GetCurrentPage() < m_PageInfo->GetNbPage()) m_PageInfo->SetCurrentPage(m_PageInfo->GetCurrentPage() + 1);

	// on fixe la page courante dans la combo
	m_PreviewBar->m_ComboPage.SetCurSel(m_PageInfo->GetCurrentPage()-1);
	this->OnUpdate(NULL, UPDATE_NEXT_PAGE, NULL);
}

void CATPrintPDFView::OnViewLastpage() 
{
	// on change la page courante
	m_PageInfo->SetCurrentPage(m_PageInfo->GetNbPage());

	// on fixe la page courante dans la combo
	m_PreviewBar->m_ComboPage.SetCurSel(m_PageInfo->GetNbPage()-1);
	this->OnUpdate(NULL, UPDATE_LAST_PAGE, NULL);
}

void CATPrintPDFView::OnViewZoomin() 
{
	m_PageInfo->SetZoom((long)((m_PageInfo->GetZoom()*5)/4));
	this->OnUpdate(NULL, UPDATE_ZOOM_IN, NULL);
}

void CATPrintPDFView::OnViewZoomout() 
{
	m_PageInfo->SetZoom((long)((m_PageInfo->GetZoom()/5)*4));
	this->OnUpdate(NULL, UPDATE_ZOOM_OUT, NULL);
}

void CATPrintPDFView::OnFileExportpdf()
{
	// on récupère les données graphiques du document
	DrawDocument * pDrawDoc = m_PageInfo->GetDrawDocument();

	if (pDrawDoc != 0)
	{
		// la chaine qui contient le nom du fichier
		CString FileName;
	
		// on crée une boite de dialogue d'enregistrement du fichier
		CFileDialog Dialog(FALSE, "pdf", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR
			, "pdf files (*.pdf)|*.pdf|All Files (*.*)|*.*||");
 	
		// on teste la réponse
		if (Dialog.DoModal() == IDOK)
		{
			// on récupère le nom du fichier
			FileName = Dialog.GetPathName();
		}
		// on calcule le décalage d'origine
		long offsetX = 0;
		long offsetY = 0;
			
		if (pDrawDoc != 0)
		{
			offsetX = (m_PageInfo->GetSizeX_mm() - (pDrawDoc->GetPageWidth()*m_PageInfo->GetZoom() / 100)) / 2;
			offsetY = (m_PageInfo->GetSizeY_mm() - (pDrawDoc->GetPageHeight()*m_PageInfo->GetZoom() / 100)) / 2;
		}

		try
		{
			// on crée un jfcGDIDraw
			JFCPDFDraw MyDraw(FileName, theApp.m_ModePaysage != FALSE, m_PageInfo->GetZoom(), m_PageInfo->GetOrgX_mm(), m_PageInfo->GetOrgY_mm(), offsetX, offsetY);	
			// on dessine le document
			pDrawDoc->Draw(&MyDraw, 1, m_PageInfo->GetNbPage());
		}
		catch (long)
		{
			AfxMessageBox ("Erreur lors de l'export PDF !");
		}


	}
	else return;
}

// Evenement sur la combo page
void CATPrintPDFView::OnSelChangePage()
{
	m_PageInfo->SetCurrentPage(m_PreviewBar->m_ComboPage.GetCurSel() +1);
	this->OnUpdate(NULL, UPDATE_PAGE_CHANGED, NULL);
}


//==============================================================================
// la gestion de mise a jour des boutons:
//==============================================================================


void CATPrintPDFView::OnUpdateViewFirstpage(CCmdUI* pCmdUI) 
{
	// on teste la page courante
	if (m_PageInfo->GetCurrentPage() == 1) pCmdUI->Enable(FALSE);
}

void CATPrintPDFView::OnUpdateViewPrevpage(CCmdUI* pCmdUI)  
{
	// on teste la page courante
	if (m_PageInfo->GetCurrentPage() == 1) pCmdUI->Enable(FALSE);	
}

void CATPrintPDFView::OnUpdateViewNextpage(CCmdUI* pCmdUI)
{
	// on teste la page courante
	if (m_PageInfo->GetCurrentPage() == m_PageInfo->GetNbPage()) pCmdUI->Enable(FALSE);	
}

void CATPrintPDFView::OnUpdateViewLastpage(CCmdUI* pCmdUI)
{
	// on met à jour la page courante
	if (m_PageInfo->GetCurrentPage() == m_PageInfo->GetNbPage()) pCmdUI->Enable(FALSE);	
}

void CATPrintPDFView::OnUpdateZoomin(CCmdUI* pCmdUI)
{
	if (m_PageInfo->GetZoom() >= 400) pCmdUI->Enable(FALSE);
}

void CATPrintPDFView::OnUpdateZoomout(CCmdUI* pCmdUI) 
{
	if (m_PageInfo->GetZoom() <= 25) pCmdUI->Enable(FALSE);
}

void CATPrintPDFView::OnUpdateFileExportpdf(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

//============================================================================
// la fonction pour gérer la mise à jour
//============================================================================

void CATPrintPDFView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	switch (lHint)
	{
	case UPDATE_DOC_CHANGED:					// ok
	{
		// on se positionne sur la première page
		OnViewFirstpage();

		// on réinitialise le zoom
		Zoom(100);

		// on réinitialise le nombre de page
		m_PreviewBar->m_ComboPage.ResetContent();
		for (long i = 1, max = m_PageInfo->GetNbPage(); i <= max; i++)
		{
			CString str;str.Format("%d", i);
			m_PreviewBar->m_ComboPage.AddString(str);
		}
		m_PreviewBar->m_ComboPage.SetCurSel(0);
	}
		break;
	// traitement impression page courante
	case UPDATE_PRINT_CURRENT_PAGE:				// ok
		m_pWindow->InvalidateRect(NULL, TRUE);
		break;
	
	// traitement impression toutes pages 
	case UPDATE_PRINT_ALL_PAGE:					// ok
		m_pWindow->InvalidateRect(NULL, TRUE);
		break;
	
	case UPDATE_FIRST_PAGE:
		m_pWindow->InvalidateRect(NULL, TRUE);	// ok
		break;

	case UPDATE_PREV_PAGE:
		m_pWindow->InvalidateRect(NULL, TRUE);	// ok
		break;

	case UPDATE_PAGE_CHANGED:					// ok 
		m_pWindow->InvalidateRect(NULL, TRUE);
		break;	

	case UPDATE_NEXT_PAGE:
		m_pWindow->InvalidateRect(NULL, TRUE);	// ok
		break;

	case UPDATE_LAST_PAGE:
		m_pWindow->InvalidateRect(NULL, TRUE);	// ok
		break;

	case UPDATE_ZOOM_OUT:
		this->Zoom(m_PageInfo->GetZoom());
		m_pWindow->InvalidateRect(NULL, TRUE);
		break;
	
	case UPDATE_ZOOM_IN:						// ok
		this->Zoom(m_PageInfo->GetZoom());
		m_pWindow->InvalidateRect(NULL, TRUE);
		break;

	case UPDATE_PDF_CHANGED:					// ok
		break;

	case UPDATE_CONTRASTE_CHANGED:				// ok
		m_pWindow->InvalidateRect(NULL, TRUE);
		break;

	default:
		CView::OnUpdate(pSender, lHint, pHint);
		break;
	}
}

//=========================================================================
// le destructeur:
//=========================================================================
CATPrintPDFView::~CATPrintPDFView()
{
	// on libère les infos de la page
	delete m_PageInfo;

	delete m_PreviewBar;

	// on libère la fenêtre membre
	delete (m_pWindow);
}


/*
void CATPrintPDFView::OnFileExportExcel()
{
	char buffer[256];
	int i;

	CATPrintPDFDoc* pDoc = GetDocument();

	sprintf(buffer,"MacroXls.exe %s %s",pDoc->GetPathName(),theApp.m_ModePaysage?"/P160":"");
	if ((i=WinExec((LPCSTR)buffer,SW_HIDE))<32) {
		AfxMessageBox("Erreur : CATPrintPDFView::OnFileExportExcel");
	}
}
*/


void CATPrintPDFView::OnFileExportExcel()
{

	CATPrintPDFDoc* pDoc = GetDocument();
	CString fileName = pDoc->GetPathName();

	long length = fileName.GetLength();
	char *buffer = new char[2*length+100];

	int pos = fileName.ReverseFind('.');
	if (pos >= 0)
		fileName = fileName.Left(pos);
	fileName += ".XLS";

	// Lancer avec Exel visible
	sprintf(buffer,"MacroXls.exe %s %s",pDoc->GetPathName(),theApp.m_ModePaysage?"/P160":"");
	// Lancer avec Exel non visible
	//sprintf(buffer,"MacroXls.exe %s %s %s",pDoc->GetPathName(),fileName,theApp.m_ModePaysage?"/P160":"");

	
	// CreateProcess API initialization
     STARTUPINFO siStartupInfo;
     PROCESS_INFORMATION piProcessInfo;
     memset(&siStartupInfo, 0, sizeof(siStartupInfo));
     memset(&piProcessInfo, 0, sizeof(piProcessInfo));
     siStartupInfo.cb = sizeof(siStartupInfo);

     if (CreateProcess(	NULL,
						  buffer, 0, 0, false,
						  CREATE_DEFAULT_ERROR_MODE | CREATE_NO_WINDOW, 0, 0,
						  &siStartupInfo, &piProcessInfo) != false)
     {
		// CString sTitle="Merci de patienter pendant la durée de l'export et ne pas fermer Excel OK";

//		CString sTitle[256];
		char sTitle[256];
		sprintf(sTitle,"%s",GetDico(theApp.m_NumDico,SD_EXCELACTIF));
		// crer la bo_ite de dialogue d'attente Excel
		CExcelProgressSplash Splash(CString(sTitle),piProcessInfo.hProcess, INFINITE, NULL);
		Splash.DoModal();
	 }
     else
     {
          // CreateProcess failed 
          DWORD iReturnVal = GetLastError();
     }


     // Release handles 
     CloseHandle(piProcessInfo.hProcess);
     CloseHandle(piProcessInfo.hThread);

	 delete buffer;
}

void CATPrintPDFView::OnFileExportExcel03()
{
	CATPrintPDFDoc* pDoc = GetDocument();
	CString fileName = pDoc->GetPathName();

	long length = fileName.GetLength();
	char *buffer = new char[2*length+100];

	int pos = fileName.ReverseFind('.');
	if (pos >= 0)
		fileName = fileName.Left(pos);
	fileName += ".xlsx";

	//WORK

	sprintf(buffer,"ExportXLS.exe %s",pDoc->GetPathName());


	// CreateProcess API initialization
	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);

	CreateProcess(	NULL,
		buffer, 0, 0, false,
		CREATE_DEFAULT_ERROR_MODE | CREATE_NO_WINDOW, 0, 0,
		&siStartupInfo, &piProcessInfo);

	// Release handles 
	CloseHandle(piProcessInfo.hProcess);
	CloseHandle(piProcessInfo.hThread);

	delete buffer;
}

void CATPrintPDFView::OnUpdateFileExportExcel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
