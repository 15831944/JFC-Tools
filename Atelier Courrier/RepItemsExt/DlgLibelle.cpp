// DlgLibelle.cpp : implementation file
//

#include "stdafx.h"

#include "DlgLibelle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLibelle dialog

CDlgLibelle::CDlgLibelle(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLibelle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLibelle)
	m_Libelle = _T("");
	//}}AFX_DATA_INIT
}

BOOL CDlgLibelle::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Button_OK.SubclassDlgItem(IDOK, this);
	m_Button_Cancel.SubclassDlgItem(IDCANCEL , this);
	m_Label.SubclassDlgItem(IDC_RI_LABEL_ITEMS,this);
	m_Box.SubclassDlgItem(IDC_RI_STATIC_BOX1,this);

	m_Label.SetFontName(CATPColors::GetFontLabelName());
	m_Label.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label.SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label.SetBorder(false);
	m_Label.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 

	m_Box.SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),m_CATPColorsApp, CATPColors::MEDIUM_DEGRAD);

	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_Label);
	m_Box.AddTopWindow(*GetDlgItem(IDC_RI_LIBELLE));

	return true;
}

void CDlgLibelle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLibelle)
	DDX_Text(pDX, IDC_RI_LIBELLE, m_Libelle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgLibelle, CDialog)
	//{{AFX_MSG_MAP(CDlgLibelle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLibelle message handlers

void CDlgLibelle::OnOK()
{
	this->UpdateData(TRUE);
	if(m_Libelle.GetLength()>0)
		CDialog::OnOK();
}
