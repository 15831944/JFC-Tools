// Atelier PresseDlg.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Atelier Presse.h"
#include "Atelier PresseDlg.h"
#include ".\atelier pressedlg.h"
#include "PathDlg.h"
#include "ATPMessages.h"
#include "BrowseWrapper.h"
#include "DlgSeuilsStats.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// boîte de dialogue CAboutDlg utilisée pour la boîte de dialogue 'À propos de' pour votre application

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Données de boîte de dialogue
	enum { IDD = IDD_ABOUTBOX };

protected:

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ReSize();

// Implémentation
	DECLARE_MESSAGE_MAP()
	CBrowseWrapper m_browser;

	CString m_AddrLink;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	CString path = theApp.GetProfileString("", "InstallPath", 0);

	path += "\\AtelierPresse.html";
	m_AddrLink = path;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ATP_EXPLORER, m_browser);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_SIZE()

END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(AfxGetApp()->LoadIcon(IDR_ATP_MAINFRAME), TRUE);		// Définir une grande icône
	SetIcon(AfxGetApp()->LoadIcon(IDR_ATP_MAINFRAME), FALSE);		// Définir une petite icône
	COleVariant vtEmpty;
	m_browser.Navigate(m_AddrLink, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);
	ReSize();
	return true;
}


void CAboutDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	ReSize();
}

void CAboutDlg::ReSize()
{
	if (::IsWindow(m_browser.m_hWnd))
	{
		CRect rect;
		GetClientRect(rect);
		m_browser.MoveWindow(rect);
	}
}

// boîte de dialogue CAtelierPresseDlg

const CRect CAtelierPresseDlg::s_rectDefault(95, 80, 286, 325);  // statique
//			+191 +245
//0096 0080 0285 0325

CAtelierPresseDlg::CAtelierPresseDlg(CWnd* pParent /*=NULL*/)
	: ETSLayoutDialog(CAtelierPresseDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_ATP_MAINFRAME);
	bFisrt = TRUE;
	m_bFirstTime = TRUE;
	m_ChoixSupport = NULL;
	m_Analyseur = NULL;
	m_RepItems = NULL;
	m_RepSupports = NULL;

	m_bGripper = false;
}

void CAtelierPresseDlg::LoadApps()
{
	m_ChoixSupport = new CChoixSupport();
	m_Analyseur = new CAnalyseur();
	m_RepItems = new CRepItems();
	m_RepSupports = new CRepSupports();
}


CAtelierPresseDlg::~CAtelierPresseDlg()
{
	if(m_ChoixSupport)
		delete	m_ChoixSupport;
	if(m_Analyseur)
		delete		m_Analyseur;
	if(m_RepItems)
		delete		m_RepItems;
	if(m_RepSupports)
		delete	m_RepSupports;

	if (m_Creuset.CanClose())
		m_Creuset.Close();
}

void CAtelierPresseDlg::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAtelierPresseDlg, ETSLayoutDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ATP_REPITEM,		OnBnClickedAtpRepitem)
	ON_BN_CLICKED(IDC_ATP_REPSUPPORT,	OnBnClickedAtpRepsupport)
	// ON_BN_CLICKED(IDC_ATP_NOMENCL,		OnBnClickedAtpNomencl)
	ON_BN_CLICKED(IDC_ATP_ANALYSEUR,	OnBnClickedAtpAnalyseur)
	ON_BN_CLICKED(IDC_ATP_CREUSET,		OnBnClickedAtpCreuset)
	ON_BN_CLICKED(IDC_ATP_PLAN,			OnBnClickedAtpPlan)
	ON_BN_CLICKED(IDC_ATP_CHOIXSUPPORT, OnBnClickedChoixsupport)
	// ON_BN_CLICKED(IDC_ATP_QUICKPLAN,    OnBnClickedQuickPlan)
	
	ON_COMMAND(IDM_ABOUTBOX,			OnAboutbox)
	ON_COMMAND(IDM_ATP_PATH,			OnAtpPath)

	//Transmission de message
	ON_MESSAGE(ATPMSG_TEST, OnMessageFromModule)
	ON_MESSAGE(ATPMSG_TARIF_UPDATED, OnMessageFromModule)
	ON_MESSAGE(ATPMSG_SUPPORT_TREE_NEEEDUPD, OnMessageFromModule)
	ON_MESSAGE(ATPMSG_SEUILS_CHANGED, OnMessageFromModule)
	
	ON_WM_CLOSE()

	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()

	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(IDM_ATP_SEUILSTAT, OnAtpSeuilstat)
END_MESSAGE_MAP()


// gestionnaires de messages pour CAtelierPresseDlg


void CAtelierPresseDlg::SetCmdInfo(CATPComm *cmdInfo)
{
	m_pcmdInfo = cmdInfo;
}


BOOL CAtelierPresseDlg::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();

	// Ajouter l'élément de menu "À propos de..." au menu Système.

	// IDM_ABOUTBOX doit se trouver dans la plage des commandes système.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// Définir l'icône de cette boîte de dialogue. L'infrastructure effectue cela automatiquement
	//  lorsque la fenêtre principale de l'application n'est pas une boîte de dialogue
	SetIcon(m_hIcon, TRUE);			// Définir une grande icône
	SetIcon(m_hIcon, FALSE);		// Définir une petite icône

	SetupControls();
	SetColors();

	//valid le bouton nomenclature
	ValidNomenc();

	UpdateLayout(	paneCtrl(IDC_ATP_BK, VERTICAL, GREEDY, 4,4,0,4)
								<<item (IDC_ATP_REPITEM, ABSOLUTE_VERT)
								<<item (IDC_ATP_CHOIXSUPPORT, ABSOLUTE_VERT)
								// <<item (IDC_ATP_QUICKPLAN, ABSOLUTE_VERT)
								<<item (IDC_ATP_PLAN, ABSOLUTE_VERT)
								<<item (IDC_ATP_CREUSET, ABSOLUTE_VERT)
								<<item (IDC_ATP_ANALYSEUR, ABSOLUTE_VERT)
								<<item (IDC_ATP_REPSUPPORT, ABSOLUTE_VERT)
								// <<item (IDC_ATP_NOMENCL, ABSOLUTE_VERT)
				);
	
	return TRUE;  // retourne TRUE, sauf si vous avez défini le focus sur un contrôle
}

void CAtelierPresseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		
		dlgAbout.DoModal();
	}
	else
	{
		ETSLayoutDialog::OnSysCommand(nID, lParam);
	}
}

void CAtelierPresseDlg::OnAboutbox()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


// Si vous ajoutez un bouton Réduire à votre boîte de dialogue, vous devez utiliser le code ci-dessous
//  pour dessiner l'icône. Pour les applications MFC utilisant le modèle Document/Vue,
//  cela est fait automatiquement par l'infrastructure.

void CAtelierPresseDlg::OnPaint() 
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
HCURSOR CAtelierPresseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAtelierPresseDlg::SetupControls()
{
	// Définition des sous classes dialogs via les buttons
	m_bk.SubclassDlgItem(IDC_ATP_BK, this);
	m_btnCreuset.SubclassDlgItem(IDC_ATP_CREUSET, this);
	m_btnPlan.SubclassDlgItem(IDC_ATP_PLAN, this);
	m_btnRItem.SubclassDlgItem(IDC_ATP_REPITEM, this);
	m_btnRSupport.SubclassDlgItem(IDC_ATP_REPSUPPORT, this);
	// m_btnNomencl.SubclassDlgItem(IDC_ATP_NOMENCL, this);
	m_btnAnalyseur.SubclassDlgItem(IDC_ATP_ANALYSEUR, this);
	m_btnChoixSupport.SubclassDlgItem(IDC_ATP_CHOIXSUPPORT, this);
	// m_btnQuickPlan.SubclassDlgItem(IDC_ATP_QUICKPLAN, this);

	// Association icones aux buttons
	m_btnCreuset.SetIcon(IDI_ATP_CREUSET,16,16);
	m_btnPlan.SetIcon(IDI_ATP_CREUSET,16,16);
	m_btnRItem.SetIcon(IDI_ATP_REPITEM,16,16);
	m_btnRSupport.SetIcon(IDI_ATP_REPSUPPORT,16,16);
	// m_btnNomencl.SetIcon(IDI_ATP_NOMENCL,16,16);
	m_btnAnalyseur.SetIcon(IDI_ATP_ANALYSEUR,16,16);
	m_btnChoixSupport.SetIcon(IDI_ATP_CHOIXSUPPORT,16,16);
	// m_btnQuickPlan.SetIcon(IDR_ATP_MAINFRAME,16,16);

	// Association texte tooltip aux buttons
	m_btnCreuset.SetTooltipText(IDS_ATP_STRCREUSET);
	m_btnPlan.SetTooltipText(IDS_ATP_STRPLAN);
	m_btnRItem.SetTooltipText(IDS_ATP_STRREPITEM);
	m_btnRSupport.SetTooltipText(IDS_ATP_STRREPSUPPORT);
	// m_btnNomencl.SetTooltipText(IDS_ATP_STRNOMENCL);
	m_btnAnalyseur.SetTooltipText(IDS_ATP_STRANALYSEUR);
	m_btnChoixSupport.SetTooltipText(IDS_ATP_STRCHOIXSUPPORT);
	// m_btnQuickPlan.SetTooltipText(IDS_ATP_STRCHOIXQUICKPLAN);

	// Association texte tooltip aux buttons
	m_btnCreuset.EnableBalloonTooltip();
	m_btnPlan.EnableBalloonTooltip();
	m_btnRItem.EnableBalloonTooltip();
	m_btnRSupport.EnableBalloonTooltip();
	// m_btnNomencl.EnableBalloonTooltip();
	m_btnAnalyseur.EnableBalloonTooltip();
	m_btnChoixSupport.EnableBalloonTooltip();
	// m_btnQuickPlan.EnableBalloonTooltip();

	m_bk.SetTransparent(false);
	m_bk.SetSunken(true);
	m_bk.AddTopWindow(m_btnCreuset);
	m_bk.AddTopWindow(m_btnPlan);
	m_bk.AddTopWindow(m_btnRItem);
	m_bk.AddTopWindow(m_btnRSupport);
	// m_bk.AddTopWindow(m_btnNomencl);
	m_bk.AddTopWindow(m_btnAnalyseur);
	m_bk.AddTopWindow(m_btnChoixSupport);
	// m_bk.AddTopWindow(m_btnQuickPlan);
	
}

void CAtelierPresseDlg::SetColors()
{
	m_bk.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORNOAPP,CATPColors::DARK_DEGRAD);

	m_btnCreuset.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnCreuset.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnCreuset.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );

	m_btnPlan.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnPlan.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnPlan.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );

	m_btnRItem.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnRItem.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnRItem.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );

	m_btnRSupport.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnRSupport.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnRSupport.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );

	/* Nomenclature non implémentée pour le moment
	m_btnNomencl.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR) );
	m_btnNomencl.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR) );
	m_btnNomencl.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR) );
	*/

	m_btnAnalyseur.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnAnalyseur.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnAnalyseur.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );

	m_btnChoixSupport.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnChoixSupport.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnChoixSupport.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );

	/*
	m_btnQuickPlan.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR) );
	m_btnQuickPlan.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR) );
	m_btnQuickPlan.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR) );
	*/
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lancement du Creuset - Plans Calendaires
void CAtelierPresseDlg::OnBnClickedAtpCreuset()
{
	/*
	if (m_Creuset.GetPlanState())
		m_Creuset.HidePlan();
	else
	{
		m_Creuset.ShowPlan();
		m_Creuset.HideHypo();
		this->ShowWindow(SW_MINIMIZE);
	}
	*/

	if (m_Creuset.GetPlanState())
		// m_Creuset.HidePlan();
		m_Creuset.ShowPlan();
	else
	{
		m_Creuset.ShowPlan();
		m_Creuset.HideHypo();
		// this->ShowWindow(SW_MINIMIZE);
	}

	// Met le lanceur juste derrière
	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

}

//////////////////////////////////////////////////////////////////////////////////////////
// Lancement du Creuset - Test de Plans
void CAtelierPresseDlg::OnBnClickedAtpPlan()
{
	if (m_Creuset.GetHypoState())
		// m_Creuset.HideHypo();
		m_Creuset.ShowHypo();
	else
	{
		m_Creuset.ShowHypo();
		m_Creuset.HidePlan();
		// this->ShowWindow(SW_MINIMIZE);
	}

	// Met le lanceur juste derrière
	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lancement Répertoire des items
void CAtelierPresseDlg::OnBnClickedAtpRepitem()
{
	/*
	if (!m_RepItems->GetState())
	{
		m_RepItems->Show();
		this->ShowWindow(SW_MINIMIZE);
	}
	else
		m_RepItems->Hide();
	*/
	m_RepItems->Show();
	// this->ShowWindow(SW_MINIMIZE);

	// Met le lanceur juste derrière
	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lancement dRépertoire des supports
void CAtelierPresseDlg::OnBnClickedAtpRepsupport()
{
	/*
	if (!m_RepSupports->GetState())
	{
		m_RepSupports->Show();
		this->ShowWindow(SW_MINIMIZE);
	}
	else
		m_RepSupports->Hide();
	*/
	m_RepSupports->Show();
	// this->ShowWindow(SW_MINIMIZE);

	// Met le lanceur juste derrière
	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lancement Nomenclature
void CAtelierPresseDlg::OnBnClickedAtpNomencl()
{
	// Déclaration fenêtre 
	STARTUPINFO StartupInfo;
	
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb			= sizeof(STARTUPINFO);
	StartupInfo.dwFlags		= STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_SHOW;

	// Lancement application Nomenclature
	CString path;
	path = m_nomencPath + "Nomenclature.exe";
	if (!CreateProcess( path, "", NULL, NULL, FALSE,
						CREATE_NEW_CONSOLE, 
						NULL, 
						NULL,
						&StartupInfo,
						&m_ProcessInfo))
	{
		AfxMessageBox("Impossible de lancer gestion des annonceurs",MB_OKCANCEL);
	}
	this->ShowWindow(SW_MINIMIZE);

	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

}

//////////////////////////////////////////////////////////////////////////////////////////
// Lancement de l'Analyseur - Tris Croisés
void CAtelierPresseDlg::OnBnClickedAtpAnalyseur()
{
	/*
	if (!m_Analyseur->GetState())
	{
		m_Analyseur->Show();
		this->ShowWindow(SW_MINIMIZE);
	}
	else
	{
		m_Analyseur->Hide();
	}
	*/
	m_Analyseur->Show();
	// this->ShowWindow(SW_MINIMIZE);

	// Met le lanceur juste derrière
	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lancement Choix Supports (Echelles / Mapping / Cumulativité / Duplication
void CAtelierPresseDlg::OnBnClickedChoixsupport()
{
	/*
	if (!m_ChoixSupport->GetState())
	{
		m_ChoixSupport->Show();
		this->ShowWindow(SW_MINIMIZE);
	}
	else
		m_ChoixSupport->Hide();
	*/

	m_ChoixSupport->Show();
	// this->ShowWindow(SW_MINIMIZE);

	// Met le lanceur juste derrière
	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
}

BOOL CAtelierPresseDlg::ContinueModal()
{
	if (bFisrt)
	{
		// Lancement Choix Supports
		if (m_pcmdInfo->m_appToStart == CATPComm::COM_CHOIXSUPPORT)
		{
			m_ChoixSupport->Show();
			if(m_pcmdInfo->m_strFileName != "")
				m_ChoixSupport->Open(m_pcmdInfo->m_strFileName);
		}

		// Lancement des Plans calendaires
		if (m_pcmdInfo->m_appToStart == CATPComm::COM_CREUSET)
		{
			m_Creuset.ShowPlan();
			if(m_pcmdInfo->m_strFileName != "")
				m_Creuset.Open(m_pcmdInfo->m_strFileName);
		}

		// Lancement des Tests de Plan
		if (m_pcmdInfo->m_appToStart == CATPComm::COM_PLAN)
		{
			m_Creuset.ShowHypo();
		}

		// Lancement Répertoire Cibles
		if (m_pcmdInfo->m_appToStart == CATPComm::COM_ITEM)
		{
			m_RepItems->Show();
		}

		// Lancement Répertoire Supports
		if (m_pcmdInfo->m_appToStart == CATPComm::COM_SUPPORT)
		{
			m_RepSupports->Show();
		}

		// Lancement Nomenclature
		if (m_pcmdInfo->m_appToStart == CATPComm::COM_NOMENCLATURE)
		{
			// Déclaration fenêtre 
			STARTUPINFO StartupInfo;
			PROCESS_INFORMATION ProcessInfo;
			
			memset(&StartupInfo, 0, sizeof(StartupInfo));
			StartupInfo.cb			= sizeof(STARTUPINFO);
			StartupInfo.dwFlags		= STARTF_USESHOWWINDOW;
			StartupInfo.wShowWindow = SW_SHOW;

			// Lancement application Nomenclature
			CString path;
			path = m_nomencPath + "Nomencl.exe";
			if (!CreateProcess( path, "", NULL, NULL, FALSE,
								CREATE_NEW_CONSOLE, 
								NULL, 
								NULL,
								&StartupInfo,
								&ProcessInfo))
				// Problème lancement Executable Nomenclature
				AfxMessageBox("Impossible de lancer gestion des annonceurs",MB_OKCANCEL);
		}

		// Lancement des Tris Croises
		if (m_pcmdInfo->m_appToStart == CATPComm::COM_ANALYSEUR)
		{
			m_Analyseur->Show();
			if(m_pcmdInfo->m_strFileName != "")
				m_Analyseur->Open(m_pcmdInfo->m_strFileName);
		}

		bFisrt = FALSE;
	}

	return ETSLayoutDialog::ContinueModal();
}


void CAtelierPresseDlg::OnAtpPath()
{
	CPathDlg pathdlg;
	pathdlg.m_pathSrc = theApp.GetProfileString("Data", "SourcesPath", 0);
	pathdlg.m_pathTrf = theApp.GetProfileString("Data", "TarifsPath", 0);
	pathdlg.m_pathUser = theApp.GetProfileString("Data", "UserPath", 0);
	
	if (pathdlg.DoModal() == IDOK)
	{
		theApp.WriteProfileString("Data", "SourcesPath", pathdlg.m_pathSrc);
		SetCurrentDirectory(pathdlg.m_pathSrc);
		theApp.WriteProfileString("Data", "TarifsPath", pathdlg.m_pathTrf);
		theApp.WriteProfileString("Data", "UserPath", pathdlg.m_pathUser);
	}
}




void CAtelierPresseDlg::ValidNomenc()
{
	HKEY hKey;
	LONG lRet;
	m_nomencPath = "";
	lRet = RegOpenKeyEx( HKEY_CURRENT_USER,"Software\\JFC\\Nomenclature\\Configuration",0, KEY_QUERY_VALUE, &hKey );
	if( lRet == ERROR_SUCCESS )
	{
		char szPath[1024];
		DWORD dwBufLen=1024;
		lRet = RegQueryValueEx( hKey, "NomenclPath", NULL, NULL,(LPBYTE) szPath, &dwBufLen);
		if(lRet == ERROR_SUCCESS)
		{
			//on valide le bouton
			m_nomencPath = szPath;
			// m_btnNomencl.EnableWindow();
		}
		RegCloseKey( hKey );
	}
	else
	{
		//le produit n'est pas installé
		// m_btnNomencl.EnableWindow(FALSE);
	}
}

BOOL CAtelierPresseDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// Uniquement si escape
		long ValParam = pMsg->wParam; 
		if(ValParam == VK_ESCAPE)
			return TRUE;
	}

	return ETSLayoutDialog::PreTranslateMessage(pMsg);
}

void CAtelierPresseDlg::OnClose()
{
#ifndef _DEBUG
	if(AfxMessageBox("Voulez vous quitter l'Atelier Presse ?", MB_YESNO) == IDYES)
#endif
	{
		// Fermeture avec enrgt étude
		m_Creuset.Close();
		
		m_QuickPlan.Close();
		
		// on ferme l'analyseur
		if (m_Analyseur->CanClose()) m_Analyseur->Close(); else return;

		// on ferme l'analyse de l'offre
		if (m_ChoixSupport->CanClose()) m_ChoixSupport->Close(); else return;

		// on ferme le répertoire des items
		if (m_RepItems->CanClose()) m_RepItems ->Close(); else return;

		// on ferme aussi le présentoir
		// if (

		// on quitte
		ETSLayoutDialog::OnClose();
	}
}

//Permet de transmettre un message provenant d'un module
LRESULT CAtelierPresseDlg::OnMessageFromModule(WPARAM wParam, LPARAM lParam)
{
	m_Creuset.ForwardMessage(GetCurrentMessage()->message, wParam, lParam);
	m_RepItems->ForwardMessage(GetCurrentMessage()->message, wParam, lParam);
	m_RepSupports->ForwardMessage(GetCurrentMessage()->message, wParam, lParam);
	m_Analyseur->ForwardMessage(GetCurrentMessage()->message, wParam, lParam);
	m_ChoixSupport->ForwardMessage(GetCurrentMessage()->message, wParam, lParam);
	return 0;
}


void CAtelierPresseDlg::OnDestroy()
{
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
 
	AfxGetApp()->WriteProfileString("Options\\ATP", "Rect", strText);
    //AfxGetApp()->WriteProfileInt("Options\\ATP", "Icon", bIconic);
    AfxGetApp()->WriteProfileInt("Options\\ATP", "Max", bMaximized);

	ETSLayoutDialog::OnDestroy();
}

void CAtelierPresseDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CString strText;
    BOOL bIconic, bMaximized;
    UINT flags;
    WINDOWPLACEMENT wndpl;
    CRect rect;

    if (m_bFirstTime)
	{
        m_bFirstTime = FALSE;
        strText = AfxGetApp()->GetProfileString("Options\\ATP", "Rect");
        if (!strText.IsEmpty())
		{
            rect.left = atoi((const char*) strText);
            rect.top = atoi((const char*) strText + 5);
			//0002 0372 0195 0645         //rect.right = atoi((const char*) strText + 10);
            //rect.bottom = atoi((const char*) strText + 15);
            rect.right = rect.left + 191;
            rect.bottom = rect.top + 265;
        }
        else 
            rect = s_rectDefault;

        bIconic = AfxGetApp()->GetProfileInt("Options\\ATP", "Icon", 0);
        bMaximized = AfxGetApp()->GetProfileInt("Options\\ATP", "Max", 0);   
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



void CAtelierPresseDlg::OnSysColorChange()
{
	CDialog::OnSysColorChange();
	SetColors();
}

void CAtelierPresseDlg::OnAtpSeuilstat()
{
	CDlgSeuilsStats dlg;
	dlg.SetColors(CATPColors::COLORLANCEUR);
	if(dlg.DoModal()==IDOK)
	{	
		// Mise à jour des résultats via modification des seuils
		PostMessage(ATPMSG_SEUILS_CHANGED);
	}
}
