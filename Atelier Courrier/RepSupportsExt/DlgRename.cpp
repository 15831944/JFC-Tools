// DlgRename.cpp : implementation file
//

#include "stdafx.h"
#include "DlgRename.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDlgRename dialog
CDlgRename::CDlgRename(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRename::IDD, pParent)
{
	m_Libelle = _T("");
}

BOOL CDlgRename::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Label.SubclassDlgItem(IDC_RS_TTLREN,this);
	m_Box.SubclassDlgItem(IDC_RS_BKREN,this);

	m_Label.SetFontName(CATPColors::GetFontLabelName());
	m_Label.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_Label.SetBorder(false);
	m_Label.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS)); 

	m_Box.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_Label);
	m_Box.AddTopWindow(*GetDlgItem(IDC_RS_LIBELLE));
	// m_Box.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS));

	return true;
}

void CDlgRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RS_LIBELLE, m_Libelle);
}


BEGIN_MESSAGE_MAP(CDlgRename, CDialog)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

LRESULT CDlgRename::OnNcHitTest(CPoint point)
{
    UINT hit = CDialog::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}
