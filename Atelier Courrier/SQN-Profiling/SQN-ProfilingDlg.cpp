// SQN-ProfilingDlg.cpp : fichier d'impl�mentation
//

#include "stdafx.h"
#include "SQN-Profiling.h"
#include "SQN-ProfilingDlg.h"
#include "PathDlg.h"
#include "ATPMessages.h"
#include "BrowseWrapper.h"
#include ".\SQN-ProfilingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// bo�te de dialogue CAboutDlg utilis�e pour la bo�te de dialogue '� propos de' pour votre application

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	CString m_AddrLink;

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ReSize();

// Impl�mentation
	DECLARE_MESSAGE_MAP()
	CBrowseWrapper m_browser;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	// AfxMessageBox("Verif Profiling / Step B1", MB_ICONINFORMATION);

	CString path = theApp.GetProfileString("", "InstallPath", 0);

	path += "\\SQN-Profiling.html";
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
	SetIcon(AfxGetApp()->LoadIcon(IDR_ATP_MAINFRAME), TRUE);		// D�finir une grande ic�ne
	SetIcon(AfxGetApp()->LoadIcon(IDR_ATP_MAINFRAME), FALSE);		// D�finir une petite ic�ne
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

// bo�te de dialogue CSQNProfilingDlg

const CRect CSQNProfilingDlg::s_rectDefault(95, 80, 286, 195);  // statique
//			+191 +115
//0095 0080 0286 0195

CSQNProfilingDlg::CSQNProfilingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSQNProfilingDlg::IDD, pParent)
{
	m_RepItems  = 0;
	m_Profiling = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_ATP_MAINFRAME);
	bFisrt = TRUE;
	m_bFirstTime = TRUE;
}

void CSQNProfilingDlg::LoadApps()
{
	m_RepItems  = new CRepItems(SHOW_PERCENT,SHOWMODE_STANDARD,SHOW_SITES);
	m_Profiling = new CProfiling();
}

CSQNProfilingDlg::~CSQNProfilingDlg()
{
	delete m_Profiling;
	delete m_RepItems;
}

void CSQNProfilingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSQNProfilingDlg, CDialog)
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
	//ON_MESSAGE(ATPMSG_SEUILS_CHANGED, OnMessageFromModule)
	
	ON_WM_CLOSE()

	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()

	ON_WM_SYSCOLORCHANGE()
	//ON_COMMAND(IDM_ATP_SEUILSTAT, OnAtpSeuilstat)
END_MESSAGE_MAP()


// gestionnaires de messages pour CSQNProfilingDlg


void CSQNProfilingDlg::SetCmdInfo(CATPComm *cmdInfo)
{
	m_pcmdInfo = cmdInfo;
}


BOOL CSQNProfilingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Ajouter l'�l�ment de menu "� propos de..." au menu Syst�me.

	// IDM_ABOUTBOX doit se trouver dans la plage des commandes syst�me.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// D�finir l'ic�ne de cette bo�te de dialogue. L'infrastructure effectue cela automatiquement
	//  lorsque la fen�tre principale de l'application n'est pas une bo�te de dialogue
	SetIcon(m_hIcon, TRUE);			// D�finir une grande ic�ne
	SetIcon(m_hIcon, FALSE);		// D�finir une petite ic�ne

	SetupControls();
	SetColors();

	return TRUE;  // retourne TRUE, sauf si vous avez d�fini le focus sur un contr�le
}

void CSQNProfilingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSQNProfilingDlg::OnAboutbox()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


// Si vous ajoutez un bouton R�duire � votre bo�te de dialogue, vous devez utiliser le code ci-dessous
//  pour dessiner l'ic�ne. Pour les applications MFC utilisant le mod�le Document/Vue,
//  cela est fait automatiquement par l'infrastructure.

void CSQNProfilingDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // contexte de p�riph�rique pour la peinture

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrer l'ic�ne dans le rectangle client
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Dessiner l'ic�ne
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Le syst�me appelle cette fonction pour obtenir le curseur � afficher lorsque l'utilisateur fait glisser
//  la fen�tre r�duite.
HCURSOR CSQNProfilingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSQNProfilingDlg::SetupControls()
{
	// D�finition des sous classes dialogs via les buttons
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

void CSQNProfilingDlg::SetColors()
{
	m_bk.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORLANCEUR), CATPColors::COLORNOAPP, CATPColors::LIGHT_DEGRAD);
	// m_bk.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORLANCEUR));

	m_btnRItem.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR) );
	m_btnRItem.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR) );
	m_btnRItem.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR) );

	m_btnAnalyseur.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR) );
	m_btnAnalyseur.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR) );
	m_btnAnalyseur.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR) );

}

//////////////////////////////////////////////////////////////////////////////////////////
// Lancement R�pertoire des items
void CSQNProfilingDlg::OnBnClickedAtpRepitem()
{
	if (!m_RepItems->GetState())
		m_RepItems->Show();
	else
		m_RepItems->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Lancement de l'Analyseur - Tris Crois�s
void CSQNProfilingDlg::OnBnClickedAtpAnalyseur()
{
	if (!m_Profiling->GetState())
		m_Profiling->Show();
	else
		m_Profiling->Hide();
}

BOOL CSQNProfilingDlg::ContinueModal()
{
	if (bFisrt)
	{

		// Lancement R�pertoire Cibles
		if (m_pcmdInfo->m_appToStart == CATPComm::COM2_ITEM)
		{
			m_RepItems->Show();
		}

		// Lancement des Tris Croises
		if (m_pcmdInfo->m_appToStart == CATPComm::COM2_ANALYSEUR)
		{
			m_Profiling->Show();
			if(m_pcmdInfo->m_strFileName != "")
				m_Profiling->Open(m_pcmdInfo->m_strFileName);
		}

		bFisrt = FALSE;
	}

	return CDialog::ContinueModal();
}


void CSQNProfilingDlg::OnAtpPath()
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
LRESULT CSQNProfilingDlg::OnMessageFromModule(WPARAM wParam, LPARAM lParam)
{
	m_RepItems->ForwardMessage(GetCurrentMessage()->message, wParam, lParam);
	m_Profiling->ForwardMessage(GetCurrentMessage()->message, wParam, lParam);
	return 0;
}


void CSQNProfilingDlg::OnDestroy()
{
    CString strText;
    BOOL bIconic, bMaximized;

    WINDOWPLACEMENT wndpl;
    wndpl.length = sizeof(WINDOWPLACEMENT);
    // lit la position courante de la fen�tre et
    // l'�tat (zoom�/r�duit)
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
 
	AfxGetApp()->WriteProfileString("Options\\SQN-Profiling", "Rect", strText);
    //AfxGetApp()->WriteProfileInt("Options\\ATP", "Icon", bIconic);
    AfxGetApp()->WriteProfileInt("Options\\SQN-Profiling", "Max", bMaximized);

	CDialog::OnDestroy();
}

void CSQNProfilingDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CString strText;
    BOOL bIconic, bMaximized;
    UINT flags;
    WINDOWPLACEMENT wndpl;
    CRect rect;

    if (m_bFirstTime)
	{
        m_bFirstTime = FALSE;
        strText = AfxGetApp()->GetProfileString("Options\\SQN-Profiling", "Rect");
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

        bIconic = AfxGetApp()->GetProfileInt("Options\\SQN-Profiling", "Icon", 0);
        bMaximized = AfxGetApp()->GetProfileInt("Options\\SQN-Profiling", "Max", 0);   
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

		// d�finit la position de la fen�tre 
		// et l'�tat (zoom�/r�duit)
        BOOL bRet = SetWindowPlacement(&wndpl);
    }

	CDialog::OnShowWindow(bShow, nStatus);
}



void CSQNProfilingDlg::OnSysColorChange()
{
	CDialog::OnSysColorChange();
	SetColors();
}

void CSQNProfilingDlg::OnClose()
{
	// on ferme profiling
	if (m_Profiling->CanClose()) m_Profiling->Close(); else return;

	// on ferme le r�pertoire des items
	if (m_RepItems->CanClose()) m_RepItems ->Close(); else return;

	// on quitte
	CDialog::OnClose();
}

/*
void CSQNProfilingDlg::OnAtpSeuilstat()
{
	CDlgSeuilsStats dlg;
	dlg.SetColors(CATPColors::COLORLANCEUR);
	if(dlg.DoModal()==IDOK)
		PostMessage(ATPMSG_SEUILS_CHANGED);
}
*/

BOOL CSQNProfilingDlg::PreTranslateMessage(MSG* pMsg)
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