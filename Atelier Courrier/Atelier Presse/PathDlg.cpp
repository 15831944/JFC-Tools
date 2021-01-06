// PathDlg.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "PathDlg.h"
#include ".\pathdlg.h"


// Boîte de dialogue CPathDlg

IMPLEMENT_DYNAMIC(CPathDlg, CDialog)
CPathDlg::CPathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPathDlg::IDD, pParent)
{
}

CPathDlg::~CPathDlg()
{
}

void CPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CPathDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OleInitialize (NULL);

	SetupControls();
	SetColors();

	m_edtSrc.SetText(m_pathSrc);
	m_edtTrf.SetText(m_pathTrf);
	m_edtUser.SetText(m_pathUser);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}


BEGIN_MESSAGE_MAP(CPathDlg, CDialog)
	ON_BN_CLICKED(IDC_ATP_BTNPATHSRC, OnBnClickedBtnPathSrc)
	ON_BN_CLICKED(IDC_ATP_BTNPATHTRF, OnBnClickedBtnPathTrf)
	ON_BN_CLICKED(IDC_ATP_BTNPATHUSER, OnBnClickedBtnPathUser)
END_MESSAGE_MAP()



void CPathDlg::OnBnClickedBtnPathSrc()
{
	CString ttl;
	ttl.LoadString(IDS_ATP_TTLSRCPATH);
	CFolderDialog dlgFolder(m_hWnd, m_pathSrc, ttl, true, true);

	if(dlgFolder.DoModal() == IDCANCEL)
		return;

	m_pathSrc = dlgFolder.GetPath();
	m_edtSrc.SetText(m_pathSrc);
}

void CPathDlg::OnBnClickedBtnPathTrf()
{
	CString ttl;
	ttl.LoadString(IDS_ATP_TTLTRFPATH);
	CFolderDialog dlgFolder(m_hWnd, m_pathTrf, ttl, true, true);

	if(dlgFolder.DoModal() == IDCANCEL)
		return;

	m_pathTrf = dlgFolder.GetPath();
	m_edtTrf.SetText(m_pathTrf);
}

void CPathDlg::OnBnClickedBtnPathUser()
{
	CString ttl;
	ttl.LoadString(IDS_ATP_TTLUSERPATH);
	CFolderDialog dlgFolder(m_hWnd, m_pathUser, ttl, false, true);

	if(dlgFolder.DoModal() == IDCANCEL)
		return;

	m_pathUser = dlgFolder.GetPath();
	m_edtUser.SetText(m_pathUser);
}

void CPathDlg::SetupControls()
{
	//subclass 
	m_bk.SubclassDlgItem(IDC_ATP_BK, this);
	m_ttl.SubclassDlgItem(IDC_ATP_TTL, this);
	m_lblSrc.SubclassDlgItem(IDC_ATP_STATICPATHSRC, this);
	m_lblTrf.SubclassDlgItem(IDC_ATP_STATICPATHTRF, this);
	m_lblUser.SubclassDlgItem(IDC_ATP_STATICPATHUSER, this);
	
	m_edtSrc.SubclassDlgItem(IDC_ATP_EDITPATHSRC, this);
	m_edtTrf.SubclassDlgItem(IDC_ATP_EDITPATHTRF, this);
	m_edtUser.SubclassDlgItem(IDC_ATP_EDITPATHUSER, this);
	
	m_btnPathSrc.SubclassDlgItem(IDC_ATP_BTNPATHSRC, this);
	m_btnPathTrf.SubclassDlgItem(IDC_ATP_BTNPATHTRF, this);
	m_btnPathUser.SubclassDlgItem(IDC_ATP_BTNPATHUSER, this);
	m_btnOK.SubclassDlgItem(IDOK, this);
	m_btnCancel.SubclassDlgItem(IDCANCEL, this);

	m_bk.SetTransparent(false);
	m_bk.SetSunken(true);

	m_bk.AddTopWindow(m_lblSrc);
	m_bk.AddTopWindow(m_lblTrf);
	m_bk.AddTopWindow(m_lblUser);

	m_bk.AddTopWindow(m_edtSrc);
	m_bk.AddTopWindow(m_edtTrf);
	m_bk.AddTopWindow(m_edtUser);

	m_bk.AddTopWindow(m_btnPathSrc);
	m_bk.AddTopWindow(m_btnPathTrf);
	m_bk.AddTopWindow(m_btnPathUser);

	m_bk.AddTopWindow(m_ttl);

	m_ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttl.SetBorder(false);

}

void CPathDlg::SetColors()
{
	m_bk.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);
	m_ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR,CATPColors::DARK_DEGRAD);
	m_ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORLANCEUR));
	
	m_lblSrc.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);
	
	m_lblTrf.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);
	
	m_lblUser.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);
	

	m_edtSrc.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD);
	m_edtSrc.SetFontName(CATPColors::GetFontStaticName());
	m_edtSrc.SetFontSize(CATPColors::GetFontStaticSize());
	m_edtSrc.SetBorder(true);

	m_edtTrf.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD);
	m_edtTrf.SetFontName(CATPColors::GetFontStaticName());
	m_edtTrf.SetFontSize(CATPColors::GetFontStaticSize());
	m_edtTrf.SetBorder(true);

	m_edtUser.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORNOAPP,CATPColors::LIGHT_DEGRAD);
	m_edtUser.SetFontName(CATPColors::GetFontStaticName());
	m_edtUser.SetFontSize(CATPColors::GetFontStaticSize());
	m_edtUser.SetBorder(true);


	m_btnPathSrc.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnPathSrc.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnPathSrc.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnPathSrc.SetIcon(IDI_ATP_FOLDER);

	m_btnPathTrf.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnPathTrf.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnPathTrf.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnPathTrf.SetIcon(IDI_ATP_FOLDER);

	m_btnPathUser.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnPathUser.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnPathUser.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORLANCEUR),CATPColors::COLORLANCEUR );
	m_btnPathUser.SetIcon(IDI_ATP_FOLDER);
}