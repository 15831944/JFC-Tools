// SineQuaNonDlg.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "SineQuaNon.h"
#include "SineQuaNonDlg.h"
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
	CString m_AddrLink;

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
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	CString path = theApp.GetProfileString("", "InstallPath", 0);

	// Aout 2013
	path += "\\AtelierCourrier.html";
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

// boîte de dialogue CSineQuaNonDlg

// const CRect CSineQuaNonDlg::s_rectDefault(95, 80, 286, 195);  // statique
const CRect CSineQuaNonDlg::s_rectDefault(95, 80, 310, 195);  // statique
//			+191 +115
//0095 0080 0286 0195

CSineQuaNonDlg::CSineQuaNonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSineQuaNonDlg::IDD, pParent)
{
	m_RepItems  = 0;
	m_Analyseur = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_ATP_MAINFRAME);
	bFisrt = TRUE;
	m_bFirstTime = TRUE;
}

void CSineQuaNonDlg::LoadApps()
{
	m_RepItems  = new CRepItems();
	m_Analyseur = new CAnalyseur();
}


CSineQuaNonDlg::~CSineQuaNonDlg()
{
	delete m_Analyseur;
	delete m_RepItems;
}

void CSineQuaNonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSineQuaNonDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ATP_REPITEM,		OnBnClickedAtpRepitem)
	ON_BN_CLICKED(IDC_ATP_ANALYSEUR,	OnBnClickedAtpAnalyseur)
	ON_COMMAND(IDM_ABOUTBOX,			OnAboutbox)
	ON_COMMAND(IDM_ATP_PATH,			OnAtpPath)

	//Transmission de message
	//ON_MESSAGE(ATPMSG_TEST, OnMessageFromModule)
	//ON_MESSAGE(ATPMSG_TARIF_UPDATED, OnMessageFromModule)
	ON_MESSAGE(ATPMSG_SEUILS_CHANGED, OnMessageFromModule)

	ON_WM_CLOSE()

	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()

	ON_WM_SYSCOLORCHANGE()
	ON_COMMAND(IDM_ATP_SEUILSTAT, OnAtpSeuilstat)
END_MESSAGE_MAP()


// gestionnaires de messages pour CSineQuaNonDlg


void CSineQuaNonDlg::SetCmdInfo(CATPComm *cmdInfo)
{
	m_pcmdInfo = cmdInfo;
}


BOOL CSineQuaNonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	return TRUE;  // retourne TRUE, sauf si vous avez défini le focus sur un contrôle
}

void CSineQuaNonDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CSineQuaNonDlg::OnAboutbox()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


// Si vous ajoutez un bouton Réduire à votre boîte de dialogue, vous devez utiliser le code ci-dessous
//  pour dessiner l'icône. Pour les applications MFC utilisant le modèle Document/Vue,
//  cela est fait automatiquement par l'infrastructure.

void CSineQuaNonDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// Le système appelle cette fonction pour obtenir le curseur à afficher lorsque l'utilisateur fait glisser
//  la fenêtre réduite.
HCURSOR CSineQuaNonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSineQuaNonDlg::SetupControls()
{
	// Définition des sous classes dialogs via les buttons
	m_bk.SubclassDlgItem(IDC_ATP_BK, this);
	m_btnRItem.SubclassDlgItem(IDC_ATP_REPITEM, this);
	m_btnAnalyseur.SubclassDlgItem(IDC_ATP_ANALYSEUR, this);

	// Association icones aux buttons
	m_btnRItem.SetIcon(IDI_ATP_REPITEM,16,16);
	m_btnAnalyseur.SetIcon(IDI_ATP_ANALYSEUR,16,16);

	// Association texte tooltip aux buttons
	m_btnRItem.SetTooltipText(IDS_ATP_STRREPITEM);
	m_btnAnalyseur.SetTooltipText(IDS_ATP_STRANALYSEUR);

	// Association texte tooltip aux buttons
	m_btnRItem.EnableBalloonTooltip();
	m_btnAnalyseur.EnableBalloonTooltip();

	m_bk.SetTransparent(false);
	m_bk.SetSunken(true);
	m_bk.AddTopWindow(m_btnRItem);
	m_bk.AddTopWindow(m_btnAnalyseur);
}

void CSineQuaNonDlg::SetColors()
{
	m_bk.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
	// m_bk.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORLANCEUR));

	m_btnRItem.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR) );
	m_btnRItem.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR) );
	m_btnRItem.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR) );

	m_btnAnalyseur.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR) );
	m_btnAnalyseur.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR) );
	m_btnAnalyseur.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR) );

}

//////////////////////////////////////////////////////////////////////////////////////////
// Lancement Répertoire des items
void CSineQuaNonDlg::OnBnClickedAtpRepitem()
{
	if (!m_RepItems->GetState())
		m_RepItems->Show();
	else
		m_RepItems->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lancement de l'Analyseur - Tris Croisés
void CSineQuaNonDlg::OnBnClickedAtpAnalyseur()
{
	if (!m_Analyseur->GetState())
		m_Analyseur->Show();
	else
		m_Analyseur->Hide();
}

BOOL CSineQuaNonDlg::ContinueModal()
{
	if (bFisrt)
	{

		// Lancement Répertoire Cibles
		if (m_pcmdInfo->m_appToStart == CATPComm::COM1_ITEM)
		{
			m_RepItems->Show();
		}

		// Lancement des Tris Croises
		if (m_pcmdInfo->m_appToStart == CATPComm::COM1_ANALYSEUR)
		{
			m_Analyseur->Show();
			if(m_pcmdInfo->m_strFileName != "")
				m_Analyseur->Open(m_pcmdInfo->m_strFileName);
		}

		bFisrt = FALSE;
	}

	return CDialog::ContinueModal();
}


void CSineQuaNonDlg::OnAtpPath()
{
	CPathDlg pathdlg;
	pathdlg.m_pathSrc = theApp.GetProfileString("Data", "SourcesPath", 0);
	pathdlg.m_pathUser = theApp.GetProfileString("Data", "UserPath", 0);
	
	if (pathdlg.DoModal() == IDOK)
	{
		theApp.WriteProfileString("Data", "SourcesPath", pathdlg.m_pathSrc);
		SetCurrentDirectory(pathdlg.m_pathSrc);
		theApp.WriteProfileString("Data", "UserPath", pathdlg.m_pathUser);
	}
}



//Permet de transmettre un message provenant d'un module
LRESULT CSineQuaNonDlg::OnMessageFromModule(WPARAM wParam, LPARAM lParam)
{
	m_RepItems->ForwardMessage(GetCurrentMessage()->message, wParam, lParam);
	m_Analyseur->ForwardMessage(GetCurrentMessage()->message, wParam, lParam);
	return 0;
}


void CSineQuaNonDlg::OnDestroy()
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
 
	AfxGetApp()->WriteProfileString("Options\\SineQuaNon", "Rect", strText);
    //AfxGetApp()->WriteProfileInt("Options\\ATP", "Icon", bIconic);
    AfxGetApp()->WriteProfileInt("Options\\SineQuaNon", "Max", bMaximized);

	CDialog::OnDestroy();
}

void CSineQuaNonDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CString strText;
    BOOL bIconic, bMaximized;
    UINT flags;
    WINDOWPLACEMENT wndpl;
    CRect rect;

    if (m_bFirstTime)
	{
        m_bFirstTime = FALSE;
        strText = AfxGetApp()->GetProfileString("Options\\Items", "Rect");
        if (!strText.IsEmpty())
		{
            rect.left = atoi((const char*) strText);
            rect.top = atoi((const char*) strText + 5);
            //rect.right = atoi((const char*) strText + 10);
            //rect.bottom = atoi((const char*) strText + 15);
            rect.right = rect.left + 191;
            rect.bottom = rect.top + 115;
        }
        else 
            rect = s_rectDefault;

        bIconic = AfxGetApp()->GetProfileInt("Options\\SineQuaNon", "Icon", 0);
        bMaximized = AfxGetApp()->GetProfileInt("Options\\SineQuaNon", "Max", 0);   
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

	CDialog::OnShowWindow(bShow, nStatus);
}



void CSineQuaNonDlg::OnSysColorChange()
{
	CDialog::OnSysColorChange();
	SetColors();
}

void CSineQuaNonDlg::OnAtpSeuilstat()
{
	CDlgSeuilsStats dlg;
	dlg.SetColors(CATPColors::COLORLANCEUR);
	if(dlg.DoModal()==IDOK)
	{
		// Mise à jour des résultats via modification des seuils
		PostMessage(ATPMSG_SEUILS_CHANGED);
	}
}

void CSineQuaNonDlg::OnClose()
{
	// on ferme l'analyseur
	if (m_Analyseur->CanClose()) m_Analyseur->Close(); else return;

	// on ferme le répertoire des items
	if (m_RepItems->CanClose()) m_RepItems ->Close(); else return;

	// on quitte
	CDialog::OnClose();
}

BOOL CSineQuaNonDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// Uniquement si escape
		long ValParam = pMsg->wParam; 
		if(ValParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}