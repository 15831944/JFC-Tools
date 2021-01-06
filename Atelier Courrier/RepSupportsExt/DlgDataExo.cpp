// DlgDataExo.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DlgDataExo.h"


// Boîte de dialogue CDlgDataExo

IMPLEMENT_DYNAMIC(CDlgDataExo, ETSLayoutDialog)
CDlgDataExo::CDlgDataExo(CWnd* pParent /*=NULL*/)
	: ETSLayoutDialog(CDlgDataExo::IDD, pParent)
{
	m_bGripper = false;
}

CDlgDataExo::~CDlgDataExo()
{
}

void CDlgDataExo::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RS_MAJFORM, m_btnNewForm);
	DDX_Control(pDX, IDC_RS_IMPORTFORM, m_btnImport);
	DDX_Control(pDX, IDC_RS_EXPORTFORM, m_btnExport);
	
	DDX_Control(pDX, IDC_RS_STATICLSTDATA, m_staticLstData);
	DDX_Control(pDX, IDC_RS_STATICTTLLSTDATA, m_ttlLstData);

	DDX_Control(pDX, IDC_RS_STATICDATA, m_staticData);
	DDX_Control(pDX, IDC_RS_STATICTTLDATA, m_ttlData);
	DDX_Control(pDX, IDC_RS_STATICTXTNB, m_lblNbDecimales);

	DDX_Control(pDX, IDC_RS_STATICBTN, m_staticBtn);
	DDX_Control(pDX, IDC_RS_STATICTTLBTN, m_ttlBtn);

	DDX_Control(pDX, IDC_RS_LISTDATA, m_list);
	DDX_Control(pDX, IDC_RS_COMBO, m_combo);


//	DDX_GridControl(pDX, IDC_RS_STATICGRID, m_gridValeur);
}


BEGIN_MESSAGE_MAP(CDlgDataExo, ETSLayoutDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// Gestionnaires de messages CDlgDataExo
void CDlgDataExo::OnSize(UINT nType, int cx, int cy)
{
	ETSLayoutDialog::OnSize(nType, cx, cy);
	m_gridValeur.ExpandColumnsToFit();
}

BOOL CDlgDataExo::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();

	CRect rect;
	GetDlgItem(IDC_RS_STATICGRID)->GetWindowRect(&rect);
	POINT pt1, pt2;
	pt1.x = rect.left, pt1.y = rect.top;
	pt2.x = rect.right, pt2.y = rect.bottom;
	::ScreenToClient(m_hWnd, &pt1);
	::ScreenToClient(m_hWnd, &pt2);
	rect.left = pt1.x, rect.top = pt1.y, rect.right = pt2.x, rect.bottom = pt2.y;
	m_gridValeur.Create(rect, this, IDC_RS_STATICGRID, WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_VSCROLL);

	m_staticLstData.SetTransparent(false);
	m_staticLstData.SetSunken(true);
	m_staticLstData.AddTopWindow(m_ttlLstData);
	m_staticLstData.AddTopWindow(m_list);

	m_staticData.SetTransparent(false);
	m_staticData.SetSunken(true);
	m_staticData.AddTopWindow(m_ttlData);
	m_staticData.AddTopWindow(m_gridValeur);

	m_staticBtn.SetTransparent(false);
	m_staticBtn.SetSunken(true);
	m_staticBtn.AddTopWindow(m_ttlBtn);
	m_staticBtn.AddTopWindow(m_btnExport);
	m_staticBtn.AddTopWindow(m_btnImport);
	m_staticBtn.AddTopWindow(m_btnNewForm);
	m_staticBtn.AddTopWindow(m_lblNbDecimales);
	m_staticBtn.AddTopWindow(m_combo);

	UpdateLayout(
		pane(HORIZONTAL)<<(
							pane(VERTICAL)
										<<(paneCtrl(IDC_RS_STATICLSTDATA, VERTICAL, GREEDY, 4,4,0,4)
										//<<(pane(VERTICAL)
												<<item(IDC_RS_STATICTTLLSTDATA, ABSOLUTE_VERT)
												<<item(IDC_RS_LISTDATA)
												)
										<<(paneCtrl(IDC_RS_STATICDATA, VERTICAL, GREEDY, 4,4,0,4)
										//<<(pane(VERTICAL)
												<<item(IDC_RS_STATICTTLDATA, ABSOLUTE_VERT)
												<<item(&m_gridValeur)
												)
									)
					 <<(paneCtrl(IDC_RS_STATICBTN, VERTICAL, GREEDY, 4,4,0,4)
						<<item(IDC_RS_STATICTTLBTN,NORESIZE)
						<<item(IDC_RS_CREATEFORM,NORESIZE)
						<<item(IDC_RS_EXPORTFORM,NORESIZE)
						<<item(IDC_RS_IMPORTFORM,NORESIZE)
						<<itemGrowing(VERTICAL)
						<<item(IDC_RS_STATICTXTNB,NORESIZE)
						<<item(IDC_RS_COMBO,NORESIZE)
						)

		);

	SetColors();


	m_gridValeur.SetColumnCount(2);
	m_gridValeur.SetRowCount(10);
	m_gridValeur.SetFixedRowCount(1);
	m_gridValeur.SetItemText(0,0,"Titres");
	m_gridValeur.SetItemText(0,1,"OJD 03");
	m_gridValeur.ExpandColumnsToFit();

	int nR = m_list.AddString("Diffusion OJD 2003");
	m_list.SetSel(nR);
	m_list.AddString("Indice de préférence");
	m_list.AddString("Coupons réponse Campagne 1");
	m_list.AddString("Coupons réponse Campagne 2");

	m_combo.SetCurSel(0);

	return 	TRUE;
}

void CDlgDataExo::SetColors()
{
	m_staticLstData.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticData.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);
	m_staticBtn.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::MEDIUM_DEGRAD);

	m_ttlLstData.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_ttlLstData.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));

	m_ttlData.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_ttlData.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));

	m_ttlBtn.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_ttlBtn.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));

	m_lblNbDecimales.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS, CATPColors::DARK_DEGRAD);
	m_lblNbDecimales.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPSUPPORTS));

	m_btnExport.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS);
	m_btnExport.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnExport.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnImport.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnImport.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnImport.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );

	m_btnNewForm.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnNewForm.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
	m_btnNewForm.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORREPSUPPORTS),CATPColors::COLORREPSUPPORTS );
}

