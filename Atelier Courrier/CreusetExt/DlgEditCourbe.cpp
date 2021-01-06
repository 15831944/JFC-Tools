// DlgEditCourbe.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "DlgEditCourbe.h"
#include "GridCellNumeric.h"


// Boîte de dialogue CDlgEditCourbe

IMPLEMENT_DYNAMIC(CDlgEditCourbe, CDialog)
CDlgEditCourbe::CDlgEditCourbe(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEditCourbe::IDD, pParent)
	, m_lbl(_T(""))
{
	m_courbes = CCourbes::GetInstance();
	m_idCourbe = -1;
}

CDlgEditCourbe::~CDlgEditCourbe()
{
}

void CDlgEditCourbe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CR_LBL, m_lbl);
}


BEGIN_MESSAGE_MAP(CDlgEditCourbe, CDialog)
//	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgEditCourbe
BOOL CDlgEditCourbe::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	CDialog::OnInitDialog();

	SetupControls();

	for (int i = 0; i<20; i++)
	{
		CString txt;
		txt.Format("%d", i+1);
		m_gridValues.SetItemText(0, i, txt);
		m_gridValues.SetItemState(0, i, m_gridValues.GetItemState(0, i) | GVIS_READONLY);
		m_gridValues.SetItemFormat(0, i, DT_CENTER);
	}

	InitGrid();
	
	if(m_idCourbe>0)
	{
		m_lbl.LoadString(IDS_CR_MODIFCRB);
		SetWindowText(m_lbl);
		m_lbl = m_courbes->GetLabel(m_idCourbe).AsJCharPtr();
	}
	else
	{
		//m_lbl = "Nouvelle courbe";
		m_lbl.LoadString(IDS_CR_NEWCRB);
		SetWindowText(m_lbl);
	}

	UpdateData(FALSE);
	GetDlgItem(IDC_CR_LBL)->SetFocus();
	((CEdit*)GetDlgItem(IDC_CR_LBL))->SetSel(0,-1);

	return TRUE;
}

void CDlgEditCourbe::InitGrid()
{
	//Initialise les valeurs de la grille
	CString txt;
	for (int i = 0; i<20; i++)
	{
		double res = 100.0;
		if(m_idCourbe>0)
			res = 100.0 * m_courbes->GetProba(i+1, m_idCourbe);

		txt.Format("%.0f", res);

		//ecriture du txt
		m_gridValues.SetItemText(1, i, txt);
		m_gridValues.SetItemFormat(1, i, DT_CENTER);
		m_gridValues.SetCellType(1, i, RUNTIME_CLASS(CGridCellNumeric));
	}
	//maj grille
	m_gridValues.Refresh();
}



void CDlgEditCourbe::SetupControls()
{
//subclass 

	m_bkg.SubclassDlgItem(IDC_CR_BK, this);
	m_ttl.SubclassDlgItem(IDC_CR_TTL, this);
	m_Txt.SubclassDlgItem(IDC_CR_TXT, this);

	m_OK.SubclassDlgItem(IDOK, this);
	m_Cancel.SubclassDlgItem(IDCANCEL, this);

	m_bkg.SetTransparent(false);
	m_bkg.SetSunken(true);
	m_bkg.AddTopWindow(m_ttl);
	m_bkg.AddTopWindow(m_Txt);
	m_bkg.AddTopWindow(GetDlgItem(IDC_CR_LBL)->GetSafeHwnd());

////
	m_ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttl.SetBorder(false);

	//grille
	CRect rect;
	GetDlgItem(IDC_CR_STATICGRID)->GetWindowRect(&rect);
	POINT pt1, pt2;
	pt1.x = rect.left, pt1.y = rect.top;
	pt2.x = rect.right, pt2.y = rect.bottom;
	::ScreenToClient(m_hWnd, &pt1);
	::ScreenToClient(m_hWnd, &pt2);
	rect.left = pt1.x, rect.top = pt1.y, rect.right = pt2.x, rect.bottom = pt2.y;
	m_gridValues.Create(rect, this, IDC_CR_STATICGRID, WS_CHILD | WS_TABSTOP| WS_VISIBLE );
	m_gridValues.ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	m_bkg.AddTopWindow(m_gridValues);

	m_gridValues.SetDefCellWidth(m_gridValues.GetDefCellWidth()/2);
	m_gridValues.SetColumnCount(20);
	m_gridValues.SetRowCount(2);

	SetColors();
}

void CDlgEditCourbe::SetColors()
{
	m_bkg.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);

	m_ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_Txt.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Txt.SetTextColor(CATPColors::GetColorBlack());
	
	m_OK.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );
	m_Cancel.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorWhite(),CATPColors::COLORCREUSET );

	m_gridValues.GetDefaultCell(FALSE, FALSE)->SetSelClr(CATPColors::GetColorSelect(CATPColors::COLORCREUSET));
	for (int i = 0; i<20; i++)
		m_gridValues.SetItemBkColour(0, i, CATPColors::GetColorLight(CATPColors::COLORCREUSET));

}

void CDlgEditCourbe::SetIdCourbe(int id)
{
	m_idCourbe = id;
}

int CDlgEditCourbe::GetIdCourbe() const
{
	return m_idCourbe;
}


void CDlgEditCourbe::OnOK()
{
	UpdateData();
	
	CCourbe * courbe = NULL;
	m_courbes->MoveTo(m_idCourbe);
	if(m_idCourbe>0 && m_courbes->IsValid())
	{
		//edition courbe
		courbe = m_courbes->GetItem();
		courbe->SetLabel(m_lbl);
	}
	else
	{
		//nouvelle courbe
		srand( (unsigned)time( NULL ) );
		courbe = new CCourbe(101 + rand(), m_lbl);
		m_courbes->Add(courbe->GetId()) = courbe;
	}

	//recup des valeurs
	for (int i = 0; i<20; i++)
		courbe->Item(i) = atoi(m_gridValues.GetItemText(1, i));

	m_idCourbe = courbe->GetId();

	m_courbes->Save();
	CDialog::OnOK();
}

LRESULT CDlgEditCourbe::OnNcHitTest(CPoint point)
{
    UINT hit = CDialog::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}

