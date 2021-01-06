// RepSupportsDlg.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Resource.h"
#include "RepSupportsDlg.h"
#include ".\RepSupportsdlg.h"
#include "ATPMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// boîte de dialogue CAboutDlg utilisée pour la boîte de dialogue 'À propos de' pour votre application

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Données de boîte de dialogue
	enum { IDD = IDD_RS_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

// Implémentation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// boîte de dialogue CRepSupportsDlg


const CRect CRepSupportsDlg::s_rectDefault(355, 170, 1000, 740);  // statique

CRepSupportsDlg::CRepSupportsDlg(CWnd* pParent /*=NULL*/)
	: ETSLayoutDialog(CRepSupportsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_RS_MAINFRAME);
	m_pActivePage = NULL;
	m_pPageFamille = NULL;
	m_pPageLot = NULL;
	m_pPageHS = NULL;
	m_pPageTarif = NULL;
	m_pPageCouplage = NULL;
	m_pPageApparie = NULL;
	m_pPageDataExo = NULL;

	m_bFirstTime = TRUE;

	m_bGripper = false;
}

CRepSupportsDlg::~CRepSupportsDlg()
{
}

void CRepSupportsDlg::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRepSupportsDlg, ETSLayoutDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RS_RADIO_FAM, OnBnClickedRadio_FAM)
	ON_BN_CLICKED(IDC_RS_RADIO_LOT, OnBnClickedRadio_LOT)
	ON_BN_CLICKED(IDC_RS_RADIO_CPL, OnBnClickedRadio_CPL)
	ON_BN_CLICKED(IDC_RS_RADIO_HS,  OnBnClickedRadio_HS)
	ON_BN_CLICKED(IDC_RS_RADIO_APP, OnBnClickedRadio_APP)
	ON_BN_CLICKED(IDC_RS_RADIO_TAR, OnBnClickedRadio_TAR)
	ON_BN_CLICKED(IDC_RS_RADIO_EXO, OnBnClickedRadio_EXO)
	ON_WM_DESTROY()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(IDM_RS_PALETTE_1, OnPalette1)
	ON_COMMAND(IDM_RS_PALETTE_2, OnPalette2)
	ON_COMMAND(IDM_RS_PALETTE_3, OnPalette3)
	ON_COMMAND(IDM_RS_PALETTE_4, OnPalette4)
	ON_COMMAND(IDM_RS_PALETTE_5, OnPalette5)
	ON_COMMAND(IDM_RS_PALETTE_6, OnPalette6)
	ON_COMMAND(IDM_RS_PALETTE_7, OnPalette7)
	ON_COMMAND(IDM_RS_PALETTE_8, OnPalette8)
	ON_COMMAND(IDM_RS_PALETTE_9, OnPalette9)
	ON_COMMAND(IDM_RS_PALETTE_10, OnPalette10)
	ON_COMMAND(IDM_RS_PALETTE_11, OnPalette11)
	ON_COMMAND(IDM_RS_PALETTE_12, OnPalette12)
	ON_COMMAND(IDM_RS_PALETTE_13, OnPalette13)
	ON_COMMAND(IDM_RS_PALETTE_14, OnPalette14)
	ON_COMMAND(IDM_RS_PALETTE_15, OnPalette15)

	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_1,									OnUpdatePalette1)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_2,									OnUpdatePalette2)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_3,									OnUpdatePalette3)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_4,									OnUpdatePalette4)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_5,									OnUpdatePalette5)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_6,									OnUpdatePalette6)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_7,									OnUpdatePalette7)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_8,									OnUpdatePalette8)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_9,									OnUpdatePalette9)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_10,									OnUpdatePalette10)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_11,									OnUpdatePalette11)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_12,									OnUpdatePalette12)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_13,									OnUpdatePalette13)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_14,									OnUpdatePalette14)
	ON_UPDATE_COMMAND_UI(IDM_RS_PALETTE_15,									OnUpdatePalette15)

	
	ON_COMMAND(IDM_RS_ABOUTBOX, OnAppAbout)
	ON_WM_SHOWWINDOW()
	ON_WM_SYSCOLORCHANGE()
	ON_MESSAGE(ATPMSG_SUPPORT_TREE_NEEEDUPD, OnSupportTreeNeedUpd)
END_MESSAGE_MAP()


// gestionnaires de messages pour CRepSupportsDlg

BOOL CRepSupportsDlg::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();

	// Ajouter l'élément de menu "À propos de..." au menu Système.

	// IDM_RS_ABOUTBOX doit se trouver dans la plage des commandes système.
	//ASSERT((IDM_RS_ABOUTBOX & 0xFFF0) == IDM_RS_ABOUTBOX);
	//ASSERT(IDM_RS_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_RS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_RS_ABOUTBOX, strAboutMenu);
		}
	}

	// Définir l'icône de cette boîte de dialogue. L'infrastructure effectue cela automatiquement
	//  lorsque la fenêtre principale de l'application n'est pas une boîte de dialogue
	SetIcon(m_hIcon, TRUE);			// Définir une grande icône
	SetIcon(m_hIcon, FALSE);		// Définir une petite icône

	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, AfxGetApp()->GetProfileInt("Options\\RepSupport", "ColorSupport", CATPColors::GREY_MOUSE) );

	SetupControls();
	SetWindowRelations();

	m_btnDataExo.ShowWindow(SW_HIDE);


	//Création de la page Familles
	m_pPageFamille = new CDlgFamille(this);
	if(m_pPageFamille->Create(IDD_RS_DLGFAMILLE, this) == FALSE)
		return FALSE;
	
	//Création de la page Lots
	m_pPageLot = new CDlgLot(this);
	if(m_pPageLot->Create(IDD_RS_DLGLOT, this) == FALSE)
		return FALSE;

	//Création de la page Lots
	m_pPageHS = new CDlgApparente(this);
	if(m_pPageHS->Create(IDD_RS_DLGAPPARENTE, this) == FALSE)
		return FALSE;

	//Création de la page Tarifs
	m_pPageTarif = new CDlgTarif(this);
	if(m_pPageTarif->Create(IDD_RS_DLGTARIF, this) == FALSE)
		return FALSE;

	//Création de la page Couplage
	m_pPageCouplage = new CDlgCouplage(this);
	if(m_pPageCouplage->Create(IDD_RS_DLGCOUPLAGE, this) == FALSE)
		return FALSE;

	//Création de la page Apparie
	m_pPageApparie = new CDlgApparie(this);
	if(m_pPageApparie->Create(IDD_RS_DLGAPPARIE, this) == FALSE)
		return FALSE;

	m_pPageDataExo = new CDlgDataExo(this);
	if(m_pPageDataExo->Create(CDlgDataExo::IDD, this) == FALSE)
		return FALSE;

	//Activation de la page Famille comme page par défaut
	switch( AfxGetApp()->GetProfileInt("Options\\RepSupport","ActivePageSupport",IDC_RS_RADIO_FAM) )
	{
	case IDC_RS_RADIO_FAM:
		OnBnClickedRadio_FAM();
		break;
	case IDC_RS_RADIO_LOT:
		OnBnClickedRadio_LOT();
		break;
	case IDC_RS_RADIO_CPL:
		OnBnClickedRadio_CPL();
		break;
	case IDC_RS_RADIO_HS:
		OnBnClickedRadio_HS();
		break;
	case IDC_RS_RADIO_APP:
		OnBnClickedRadio_APP();
		break;
	case IDC_RS_RADIO_TAR:
		OnBnClickedRadio_TAR();
		break;
	case IDC_RS_RADIO_EXO:
		OnBnClickedRadio_EXO();
		break;
	default:
		OnBnClickedRadio_FAM();
	}

	//Taille nominale de la boite 650x570
	SetWindowPos(NULL, 0,0, 650, 570, SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOREDRAW );

	return TRUE;  // retourne TRUE, sauf si vous avez défini le focus sur un contrôle
}

void CRepSupportsDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	//Taille minimale de la boite 540x400
	lpMMI->ptMinTrackSize.x = 540;
	lpMMI->ptMinTrackSize.y = 400;
	ETSLayoutDialog::OnGetMinMaxInfo(lpMMI);
}

void CRepSupportsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_RS_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		ETSLayoutDialog::OnSysCommand(nID, lParam);
	}
}

// Si vous ajoutez un bouton Réduire à votre boîte de dialogue, vous devez utiliser le code ci-dessous
//  pour dessiner l'icône. Pour les applications MFC utilisant le modèle Document/Vue,
//  cela est fait automatiquement par l'infrastructure.

void CRepSupportsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // contexte de périphérique pour la peinture

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrer l'icône dans le rectangle client
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dessiner l'icône
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		ETSLayoutDialog::OnPaint();
	}
}

// Le système appelle cette fonction pour obtenir le curseur à afficher lorsque l'utilisateur fait glisser
//  la fenêtre réduite.
HCURSOR CRepSupportsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRepSupportsDlg::OnBnClickedRadio_FAM()
{
	CheckRadioButton(IDC_RS_RADIO_APP, IDC_RS_RADIO_EXO, IDC_RS_RADIO_FAM);
	if (m_pActivePage)
		m_pActivePage->ShowWindow(SW_HIDE);
	m_pActivePage = m_pPageFamille;
	UpdateDlgPos(TRUE);
}

void CRepSupportsDlg::OnBnClickedRadio_LOT()
{
	CheckRadioButton(IDC_RS_RADIO_APP, IDC_RS_RADIO_EXO, IDC_RS_RADIO_LOT);
	if (m_pActivePage)
		m_pActivePage->ShowWindow(SW_HIDE);
	m_pActivePage = m_pPageLot;
	UpdateDlgPos(TRUE);
}

void CRepSupportsDlg::OnBnClickedRadio_CPL()
{
	CheckRadioButton(IDC_RS_RADIO_APP, IDC_RS_RADIO_EXO, IDC_RS_RADIO_CPL);
	if (m_pActivePage)
		m_pActivePage->ShowWindow(SW_HIDE);
	m_pActivePage = m_pPageCouplage;

	
	UpdateDlgPos(TRUE);
}
void CRepSupportsDlg::OnBnClickedRadio_HS()
{
	CheckRadioButton(IDC_RS_RADIO_APP, IDC_RS_RADIO_EXO, IDC_RS_RADIO_HS);
	if (m_pActivePage)
		m_pActivePage->ShowWindow(SW_HIDE);
	m_pActivePage = m_pPageHS;
	UpdateDlgPos(TRUE);
}
void CRepSupportsDlg::OnBnClickedRadio_APP()
{
	CheckRadioButton(IDC_RS_RADIO_APP, IDC_RS_RADIO_EXO, IDC_RS_RADIO_APP);
	if (m_pActivePage)
		m_pActivePage->ShowWindow(SW_HIDE);
	m_pActivePage = m_pPageApparie;
	UpdateDlgPos(TRUE);
}
void CRepSupportsDlg::OnBnClickedRadio_TAR()
{
	CheckRadioButton(IDC_RS_RADIO_APP, IDC_RS_RADIO_EXO, IDC_RS_RADIO_TAR);
	if (m_pActivePage)
		m_pActivePage->ShowWindow(SW_HIDE);
	m_pActivePage = m_pPageTarif;
	UpdateDlgPos(TRUE);
}

void CRepSupportsDlg::OnBnClickedRadio_EXO()
{
	CheckRadioButton(IDC_RS_RADIO_APP, IDC_RS_RADIO_EXO, IDC_RS_RADIO_EXO);
	if (m_pActivePage)
		m_pActivePage->ShowWindow(SW_HIDE);
	m_pActivePage = m_pPageDataExo;
	UpdateDlgPos(TRUE);
}

void CRepSupportsDlg::OnSize(UINT nType, int cx, int cy) 
{
	ETSLayoutDialog::OnSize(nType, cx, cy);
 //   m_Resize.Move();
	if (m_pActivePage)
		UpdateDlgPos(TRUE);
	//Invalidate();
	//UpdateWindow();
}

void CRepSupportsDlg::UpdateDlgPos(BOOL b_show/*=FALSE*/) 
{
	CRect rect;
	GetDlgItem(IDC_RS_DLGSTATIC)->GetWindowRect(&rect);
	POINT pt1, pt2;
	pt1.x = rect.left, pt1.y = rect.top;
	pt2.x = rect.right, pt2.y = rect.bottom;
	::ScreenToClient(m_hWnd, &pt1);
	::ScreenToClient(m_hWnd, &pt2);
	rect.left = pt1.x, rect.top = pt1.y, rect.right = pt2.x, rect.bottom = pt2.y;
	rect.InflateRect(3,3);
	if (m_pActivePage)
	{
		if (b_show)
		{
			// m_pActivePage->SetWindowPos(NULL, rect.TopLeft().x, rect.TopLeft().y, rect.Width(), rect.Height(), SWP_NOZORDER);			
			// Modif alain 
			m_pActivePage->ShowWindow(SW_SHOW);
			m_pActivePage->SetWindowPos(NULL, rect.TopLeft().x, rect.TopLeft().y, rect.Width(), rect.Height(), SWP_NOZORDER);
		}
		else
			m_pActivePage->SetWindowPos(NULL, rect.TopLeft().x, rect.TopLeft().y, rect.Width(), rect.Height(), SWP_HIDEWINDOW|SWP_NOZORDER);

		//m_pActivePage->SetWindowPos(NULL, rect.TopLeft().x, rect.TopLeft().y, rect.Width(), rect.Height(), SWP_NOZORDER );
		//if (b_show)
	}

}




void CRepSupportsDlg::SetWindowRelations()
{
	UpdateLayout(
		pane(VERTICAL)		<<item (IDC_RS_DLGSTATIC, GREEDY)
							<< itemFixed(HORIZONTAL, 4)
							<<(paneCtrl(IDC_RS_BKGBTN, HORIZONTAL, GREEDY, 4,4,0,4)
								<<item (IDC_RS_RADIO_FAM, ABSOLUTE_VERT)
								<<item (IDC_RS_RADIO_CPL, ABSOLUTE_VERT)
								<<item (IDC_RS_RADIO_HS, ABSOLUTE_VERT)
								<<item (IDC_RS_RADIO_APP, ABSOLUTE_VERT)
								<<item (IDC_RS_RADIO_TAR, ABSOLUTE_VERT)
								//<<item (IDC_RS_RADIO_EXO, ABSOLUTE_VERT)
								)
							);
}

void CRepSupportsDlg::SetupControls()
{
	m_bkgBtn.SubclassDlgItem(IDC_RS_BKGBTN, this);

	m_btnFamille.SubclassDlgItem(IDC_RS_RADIO_FAM, this);
	m_btnLot.SubclassDlgItem(IDC_RS_RADIO_LOT, this);
	m_btnHS.SubclassDlgItem(IDC_RS_RADIO_HS, this);
	m_btnTarif.SubclassDlgItem(IDC_RS_RADIO_TAR, this);
	m_btnCouplage.SubclassDlgItem(IDC_RS_RADIO_CPL, this);
	m_btnApparie.SubclassDlgItem(IDC_RS_RADIO_APP, this);
	m_btnDataExo.SubclassDlgItem(IDC_RS_RADIO_EXO, this);

	m_bkgBtn.AddTopWindow(m_btnFamille);
	m_bkgBtn.AddTopWindow(m_btnLot);
	m_bkgBtn.AddTopWindow(m_btnHS);
	m_bkgBtn.AddTopWindow(m_btnTarif);
	m_bkgBtn.AddTopWindow(m_btnCouplage);
	m_bkgBtn.AddTopWindow(m_btnApparie);
	m_bkgBtn.AddTopWindow(m_btnDataExo);

	SetColors();
}

void CRepSupportsDlg::SetColors()
{
	m_bkgBtn.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_bkgBtn.SetTransparent(false);
	m_bkgBtn.SetSunken(true);

	m_btnFamille.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnFamille.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnFamille.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnLot.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnLot.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnLot.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnHS.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnHS.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnHS.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnTarif.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnTarif.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnTarif.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnCouplage.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnCouplage.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnCouplage.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnApparie.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnApparie.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnApparie.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnDataExo.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnDataExo.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnDataExo.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
}

void CRepSupportsDlg::OnUpdatePalette1(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 1) 
		pCmdUI->SetCheck(TRUE); 
	else 
		pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette1()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 0 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnPalette2()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 1 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette2(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 2) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette3()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 2 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette3(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 3) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette4()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 3 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette4(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 4) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette5()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 4 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette5(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 5) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette6()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 5 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette6(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 6) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette7()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 6 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette7(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 7) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette8()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 7 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette8(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 8) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette9()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 8 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette9(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 9) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette10()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 9 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette10(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 10) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette11()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 10 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette11(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 11) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette12()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 11 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette12(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 12) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette13()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 12 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette13(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 13) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette14()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 13 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette14(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 14) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnPalette15()
{
	CATPColors::SetAppColor(CATPColors::COLORREPSUPPORTS, 14 );
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	Invalidate();
}

void CRepSupportsDlg::OnUpdatePalette15(CCmdUI *pCmdUI)
{
	if (CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS) == 15) pCmdUI->SetCheck(TRUE); else pCmdUI->SetCheck(FALSE);
}

void CRepSupportsDlg::OnAppAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CRepSupportsDlg::OnDestroy()
{
	AfxGetApp()->WriteProfileInt("Options\\RepSupport","ActivePageSupport", GetCheckedRadioButton(IDC_RS_RADIO_APP, IDC_RS_RADIO_EXO) );
	AfxGetApp()->WriteProfileInt("Options\\RepSupport","ColorSupport",CATPColors::GetAppColor(CATPColors::COLORREPSUPPORTS));

	if(m_pPageFamille)
	{
		m_pPageFamille->DestroyWindow();
		delete m_pPageFamille;
	}
	if(m_pPageLot)
	{
		m_pPageLot->DestroyWindow();
		delete m_pPageLot;
	}
	if(m_pPageHS)
	{
		m_pPageHS->DestroyWindow();
		delete m_pPageHS;
	}
	if(m_pPageTarif)
	{
		m_pPageTarif->DestroyWindow();
		delete m_pPageTarif;
	}
	if(m_pPageCouplage)
	{
		m_pPageCouplage->DestroyWindow();
		delete m_pPageCouplage;
	}
	if(m_pPageApparie)
	{
		m_pPageApparie->DestroyWindow();
		delete m_pPageApparie;
	}
	if(m_pPageDataExo)
	{
		m_pPageDataExo->DestroyWindow();
		delete m_pPageDataExo;
	}


	
	CString strText;
    BOOL bIconic, bMaximized;

    WINDOWPLACEMENT wndpl;
    wndpl.length = sizeof(WINDOWPLACEMENT);
    // lit la position courante de la fenêtre et
    // l'état (zoomé/réduit)
    BOOL bRet = GetWindowPlacement(&wndpl);
    if (wndpl.showCmd == SW_SHOWNORMAL)
	{
        bIconic = FALSE;
        bMaximized = FALSE;
    }
    else if (wndpl.showCmd == SW_SHOWMAXIMIZED)
	{
        bIconic = FALSE;
        bMaximized = TRUE;
    } 
    else if (wndpl.showCmd == SW_SHOWMINIMIZED)
	{
        bIconic = TRUE;
        if (wndpl.flags)
            bMaximized = TRUE;
        else
            bMaximized = FALSE;
    }
 
	strText.Format("%04d %04d %04d %04d",
                   wndpl.rcNormalPosition.left,
                   wndpl.rcNormalPosition.top,
                   wndpl.rcNormalPosition.right,
                   wndpl.rcNormalPosition.bottom);
 
	AfxGetApp()->WriteProfileString("Options\\RepSupport", "Rect", strText);
    //AfxGetApp()->WriteProfileInt("Options\\RepSupport", "Icon", bIconic);
    AfxGetApp()->WriteProfileInt("Options\\RepSupport", "Max", bMaximized);

	ETSLayoutDialog::OnDestroy();
}

void CRepSupportsDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CString strText;
    BOOL bIconic, bMaximized;
    UINT flags;
    WINDOWPLACEMENT wndpl;
    CRect rect;

    if (m_bFirstTime)
	{
        m_bFirstTime = FALSE;
        strText = AfxGetApp()->GetProfileString("Options\\RepSupport", "Rect");
        if (!strText.IsEmpty())
		{
            rect.left = atoi((const char*) strText);
            rect.top = atoi((const char*) strText + 5);
            rect.right = atoi((const char*) strText + 10);
            rect.bottom = atoi((const char*) strText + 15);
        }
        else 
            rect = s_rectDefault;

        bIconic = AfxGetApp()->GetProfileInt("Options\\RepSupport", "Icon", 0);
        bMaximized = AfxGetApp()->GetProfileInt("Options\\RepSupport", "Max", 0);   
        if (bIconic)
		{
            nStatus = SW_SHOWMINNOACTIVE;
            if (bMaximized)
                flags = WPF_RESTORETOMAXIMIZED;
            else
                flags = WPF_SETMINPOSITION;
        }
        else
		{
            if (bMaximized)
			{
                nStatus = SW_SHOWMAXIMIZED;
                flags = WPF_RESTORETOMAXIMIZED;
            }
            else
			{
                nStatus = SW_NORMAL;
                flags = WPF_SETMINPOSITION;
            }
        }
        wndpl.length = sizeof(WINDOWPLACEMENT);
        wndpl.showCmd = nStatus;
        wndpl.flags = flags;
        wndpl.ptMinPosition = CPoint(0, 0);
        wndpl.ptMaxPosition =  CPoint(-::GetSystemMetrics(SM_CXBORDER), -::GetSystemMetrics(SM_CYBORDER));
        wndpl.rcNormalPosition = rect;

		// définit la position de la fenêtre 
		// et l'état (zoomé/réduit)
        BOOL bRet = SetWindowPlacement(&wndpl);
    }

	ETSLayoutDialog::OnShowWindow(bShow, nStatus);
}


void CRepSupportsDlg::OnSysColorChange()
{
	ETSLayoutDialog::OnSysColorChange();
	SetColors();
	m_pPageFamille->SetColors();
	m_pPageLot->SetColors();
	m_pPageHS->SetColors();
	m_pPageTarif->SetColors();
	m_pPageCouplage->SetColors();
	m_pPageApparie->SetColors();
	m_pPageDataExo->SetColors();
	
}

LRESULT CRepSupportsDlg::OnSupportTreeNeedUpd(WPARAM wParam, LPARAM lParam)
{
	m_pPageFamille->RefillTree();
	return 0;
}

BOOL CRepSupportsDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// Uniquement si escape
		WPARAM ValParam = pMsg->wParam; 
		if(ValParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
