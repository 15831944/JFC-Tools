// DimensionDlg.cpp : implementation file
//

#include "stdafx.h"

#include ".\dimensiondlg.h"
#include "DlgItems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDimensionDlg dialog

CDimensionDlg::CDimensionDlg(bool bIcones, bool bOperators, CWnd* pParent /*=NULL*/)
	: CDialog(CDimensionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDimensionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bDimMode = false;
	m_DimModeIndex = -1;
	m_Type = IQuestion::NODIM;
	m_Index = 0;
	m_CATPColorsApp = CATPColors::COLORREPITEMS;
	m_bIcones = bIcones;
	m_bOperators = bOperators;
	//m_pTree_Support = NULL;
}


void CDimensionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDimensionDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CDimensionDlg, CDialog)
	//{{AFX_MSG_MAP(CDimensionDlg)
	ON_BN_CLICKED(IDC_RI_RAD_DIMENSION1, OnChange_Dim_1_PaletteSel)
	ON_BN_CLICKED(IDC_RI_RAD_DIM1_ET, OnDim_1_Et)
	ON_BN_CLICKED(IDC_RI_RAD_DIM1_NI, OnDim_1_Ni)
	ON_BN_CLICKED(IDC_RI_RAD_DIM1_OU, OnDim_1_Ou)
	ON_BN_CLICKED(IDC_RI_RAD_DIM1_XOR, OnDim_1_XOr)
	ON_BN_CLICKED(IDC_RI_BUT_DIM1_RAZ, OnDim_1_Raz)
	ON_LBN_SELCHANGE(IDC_RI_LIST_DIMENSION1, OnSelchangeListDimension)
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_RI_BUT_MENU_OPE_MODA2, OnBnClickedRiButMenuOpeModa2)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDimensionDlg message handlers
BOOL CDimensionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString ToolTip;
	
	m_List_Dimension.	SubclassDlgItem(IDC_RI_LIST_DIMENSION1,this);
	m_Button_Dimension. SubclassDlgItem(IDC_RI_RAD_DIMENSION1,this);
	//	m_Button_Dimension.SetBitmaps(IDB_RI_PALETTE,RGB(192,192,192),IDB_RI_TUBE,RGB(192,192,192));

	// Bouton sélecteur opération
	m_Button_Menu_Ope_Moda2.SubclassDlgItem(IDC_RI_BUT_MENU_OPE_MODA2,this);
	m_Button_Menu_Ope_Moda2. SetTypeBtnMenu(TRUE,TRUE); 

	// Bouton ET 
	m_Button_Dim1_ET.			SubclassDlgItem(IDC_RI_RAD_DIM1_ET,this);
	LOADSTRING(ToolTip, IDS_RI_TT_ET_M); 
	m_Button_Dim1_ET.SetTooltipText(ToolTip);

	// Bouton OU
	m_Button_Dim1_OU.			SubclassDlgItem(IDC_RI_RAD_DIM1_OU,this);
	LOADSTRING(ToolTip, IDS_RI_TT_OU_M); 
	m_Button_Dim1_OU.SetTooltipText(ToolTip);

	// Bouton NI
	m_Button_Dim1_NI.			SubclassDlgItem(IDC_RI_RAD_DIM1_NI,this);
	LOADSTRING(ToolTip, IDS_RI_TT_NI_M); 
	m_Button_Dim1_NI.SetTooltipText(ToolTip);

	// Bouton XOR
	m_Button_Dim1_XOR.			SubclassDlgItem(IDC_RI_RAD_DIM1_XOR,this);
	LOADSTRING(ToolTip, IDS_RI_TT_XOR_M); 
	m_Button_Dim1_XOR.SetTooltipText(ToolTip);

	m_Button_Dim1_RAZ.		SubclassDlgItem(IDC_RI_BUT_DIM1_RAZ, this);
	m_Label.			SubclassDlgItem(IDC_RI_LABEL, this);
	m_Box.				SubclassDlgItem(IDC_RI_STATIC_BOX2, this);

	m_Label.SetFontName(CATPColors::GetFontLabelName());
	m_Label.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label.SetBorder(false);

	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_Label);
	m_Box.AddTopWindow(m_List_Dimension);
	m_Box.AddTopWindow(m_Button_Dimension);

	// Ajout Menu Opérateurs
	m_Box.AddTopWindow(*GetDlgItem(IDC_RI_BUT_MENU_OPE_MODA2));
	m_Box.AddTopWindow(*GetDlgItem(IDC_RI_RAD_DIM1_ET));
	m_Box.AddTopWindow(*GetDlgItem(IDC_RI_RAD_DIM1_OU));
	m_Box.AddTopWindow(*GetDlgItem(IDC_RI_RAD_DIM1_NI));
	m_Box.AddTopWindow(*GetDlgItem(IDC_RI_RAD_DIM1_XOR));

	m_Box.AddTopWindow(*GetDlgItem(IDC_RI_BUT_DIM1_RAZ));

	m_Button_Dimension.	ShowWindow(SW_HIDE);
	m_Button_Dimension.	SetCheck(false);

	SetColors();
	SetIcones(m_bIcones);
	EnableOperators(m_bOperators);

	// Par défaut OU
	OnDim_1_Ou();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDimensionDlg::SetColors()
{
	m_Button_Dimension.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp) ,m_CATPColorsApp,0);
	m_Button_Dimension.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorDark(m_CATPColorsApp) ,m_CATPColorsApp);
	m_Button_Dimension.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorSelect(m_CATPColorsApp) ,m_CATPColorsApp);
	m_Button_Dimension.	SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorBlack() ,m_CATPColorsApp);
	m_Button_Dimension.	SetColor(CButtonST::BTNST_COLOR_FG_OUT,		RGB(255,255,0) ,m_CATPColorsApp);
	m_Button_Dimension.	SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	RGB(255,255,0) ,m_CATPColorsApp);


	// Bouton sélection opérateur pour modalité 2
	m_Button_Menu_Ope_Moda2.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp) ,m_CATPColorsApp,0);
	m_Button_Menu_Ope_Moda2.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp) ,m_CATPColorsApp);
	m_Button_Menu_Ope_Moda2.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp) ,m_CATPColorsApp);
	m_Button_Menu_Ope_Moda2.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp) ,m_CATPColorsApp);
	m_Button_Menu_Ope_Moda2.	SetColor(CButtonST::BTNST_COLOR_SEL,		CATPColors::GetColorSelect(m_CATPColorsApp) ,m_CATPColorsApp);

	// Les boutons opérations (sous menu)
	m_Button_Dim1_ET.			SetColor(CButtonST::BTNST_COLOR_BK_IN,CATPColors::GetColorLight(m_CATPColorsApp) ,m_CATPColorsApp,0);
	m_Button_Dim1_ET.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,CATPColors::GetColorMedium(m_CATPColorsApp) ,m_CATPColorsApp);
	m_Button_Dim1_ET.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp) ,m_CATPColorsApp);

	m_Button_Dim1_OU.			SetColor(CButtonST::BTNST_COLOR_BK_IN,CATPColors::GetColorLight(m_CATPColorsApp) ,m_CATPColorsApp,0);
	m_Button_Dim1_OU.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,CATPColors::GetColorMedium(m_CATPColorsApp) ,m_CATPColorsApp);
	m_Button_Dim1_OU.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp) ,m_CATPColorsApp);

	m_Button_Dim1_NI.			SetColor(CButtonST::BTNST_COLOR_BK_IN,CATPColors::GetColorLight(m_CATPColorsApp) ,m_CATPColorsApp,0);
	m_Button_Dim1_NI.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,CATPColors::GetColorMedium(m_CATPColorsApp) ,m_CATPColorsApp);
	m_Button_Dim1_NI.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp) ,m_CATPColorsApp);

	m_Button_Dim1_XOR.			SetColor(CButtonST::BTNST_COLOR_BK_IN,CATPColors::GetColorLight(m_CATPColorsApp) ,m_CATPColorsApp,0);
	m_Button_Dim1_XOR.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,CATPColors::GetColorMedium(m_CATPColorsApp) ,m_CATPColorsApp);
	m_Button_Dim1_XOR.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp) ,m_CATPColorsApp);

	m_Button_Dim1_RAZ.			SetColor(CButtonST::BTNST_COLOR_BK_IN,CATPColors::GetColorLight(m_CATPColorsApp) ,m_CATPColorsApp,0);
	m_Button_Dim1_RAZ.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,CATPColors::GetColorMedium(m_CATPColorsApp) ,m_CATPColorsApp,0);
	m_Button_Dim1_RAZ.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,CATPColors::GetColorDark(m_CATPColorsApp) ,m_CATPColorsApp);

	// La liste des modalités
	m_List_Dimension.	SetSelectionColor(NORMAL,CATPColors::GetColorSelect(m_CATPColorsApp));

	// Titre
	m_Label.			SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label.			SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 

	// Boite contenant les objets
	m_Box.				SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
}

void CDimensionDlg::EnableOperators(BOOL bOperators)
{
	m_bOperators = bOperators;
	m_Button_Dim1_OU .EnableWindow(m_bOperators);
	m_Button_Dim1_ET .EnableWindow(m_bOperators);
	m_Button_Dim1_NI .EnableWindow(m_bOperators);
	m_Button_Dim1_XOR.EnableWindow(m_bOperators);
}

void CDimensionDlg::SetIcones(bool bIcones)
{
	m_bIcones = bIcones;

	if(m_bIcones)
	{
		CString text;
		text = "";

		// puis bouton sélection opération sur modalité 2
		if (m_Button_Dim1_ET.GetCheck())
			m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
		else if (m_Button_Dim1_OU.GetCheck())
			m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
		else if (m_Button_Dim1_NI.GetCheck())
			m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI1,RGB(192,192,192));
		else if (m_Button_Dim1_XOR.GetCheck())
			m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR1,RGB(192,192,192));

		m_Button_Menu_Ope_Moda2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_Moda2.SetWindowText(text);
		
		m_Button_Dim1_ET.SetWindowText(text); 
		m_Button_Dim1_ET.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET,RGB(192,192,192)); 
		m_Button_Dim1_ET.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Dim1_OU.SetWindowText(text); 
		m_Button_Dim1_OU.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU,RGB(192,192,192)); 
		m_Button_Dim1_OU.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Dim1_NI.SetWindowText(text); 
		m_Button_Dim1_NI.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI,RGB(192,192,192)); 
		m_Button_Dim1_NI.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Dim1_XOR.SetWindowText(text); 
		m_Button_Dim1_XOR.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR,RGB(192,192,192)); 
		m_Button_Dim1_XOR.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
	}
	else
	{
		CString text;

		LOADSTRING(text, IDS_RI_ET);
		m_Button_Dim1_ET.SetWindowText(text); 
		m_Button_Dim1_ET.SetBitmaps(0,0);

		LOADSTRING(text, IDS_RI_OU);
		m_Button_Dim1_OU.SetWindowText(text); 
		m_Button_Dim1_OU.SetBitmaps(0,0);

		LOADSTRING(text, IDS_RI_NI);
		m_Button_Dim1_NI.SetWindowText(text); 
		m_Button_Dim1_NI.SetBitmaps(0,0);

		LOADSTRING(text, IDS_RI_XOR);
		m_Button_Dim1_XOR.SetWindowText(text); 
		m_Button_Dim1_XOR.SetBitmaps(0,0);

		// puis récupère texte de l'opérateur sélectionné sur modalité 2 en cours
		int Val = m_Button_Dim1_ET.GetCheck();
		Val = m_Button_Dim1_OU.GetCheck();
		Val = m_Button_Dim1_NI.GetCheck();
		Val = m_Button_Dim1_XOR.GetCheck();

		if (m_Button_Dim1_ET.GetCheck())
			m_Button_Dim1_ET.GetWindowText(text);
		else if (m_Button_Dim1_OU.GetCheck())
			m_Button_Dim1_OU.GetWindowText(text);
		else if (m_Button_Dim1_NI.GetCheck())
			m_Button_Dim1_NI.GetWindowText(text);
		else if (m_Button_Dim1_XOR.GetCheck())
			m_Button_Dim1_XOR.GetWindowText(text);
		
		m_Button_Menu_Ope_Moda2.SetBitmaps(0,0);
		m_Button_Menu_Ope_Moda2.SetWindowText(text);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Choix opérateur ET
void CDimensionDlg::OnDim_1_Et() 
{
	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_ET);
	GetParent()->PostMessage(WM_USER_DIMENSION_X_DLG_UPDATED,m_Type,m_Index);
	
	CString Txt = "";
	if (m_bIcones)
	{
		// Affichage opérateur en mode icone
		m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
		m_Button_Menu_Ope_Moda2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}
	else
	{
		// Affichage en mode texte
		m_Button_Menu_Ope_Moda2.SetBitmaps(0,0);
		m_Button_Dim1_ET.GetWindowText(Txt);
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeModa2(FALSE);
	
	// Rend actif l'arrière plan
	m_List_Dimension.EnableWindow(TRUE); 
}

/////////////////////////////////////////////////////////////////////////////////
// Choix opérateur NI
void CDimensionDlg::OnDim_1_Ni() 
{
	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_NI);
	GetParent()->PostMessage(WM_USER_DIMENSION_X_DLG_UPDATED,m_Type,m_Index);
	
	CString Txt = "";
	if (m_bIcones)
	{
		// Affichage opérateur en mode icone
		m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI1,RGB(192,192,192)); 
		m_Button_Menu_Ope_Moda2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}
	else
	{
		// Affichage en mode texte
		CString Txt;
		m_Button_Menu_Ope_Moda2.SetBitmaps(0,0);
		m_Button_Dim1_NI.GetWindowText(Txt);
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeModa2(FALSE);

	// Rend actif l'arrière plan
	m_List_Dimension.EnableWindow(TRUE); 
}

/////////////////////////////////////////////////////////////////////////////////
// Choix opérateur OR
void CDimensionDlg::OnDim_1_Ou() 
{
	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_OU);
	GetParent()->PostMessage(WM_USER_DIMENSION_X_DLG_UPDATED,m_Type,m_Index);

	CString Txt = "";
	if (m_bIcones)
	{
		// Affichage opérateur en mode icone
		m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
		m_Button_Menu_Ope_Moda2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}
	else
	{
		// Affichage en mode texte
		CString Txt;
		m_Button_Menu_Ope_Moda2.SetBitmaps(0,0);
		m_Button_Dim1_OU.GetWindowText(Txt);
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeModa2(FALSE);

	// Rend actif l'arrière plan
	m_List_Dimension.EnableWindow(TRUE); 
}

/////////////////////////////////////////////////////////////////////////////////
// Choix opérateur XOR
void CDimensionDlg::OnDim_1_XOr() 
{
	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_XOR);
	GetParent()->PostMessage(WM_USER_DIMENSION_X_DLG_UPDATED,m_Type,m_Index);

	CString Txt = "";
	if (m_bIcones)
	{
		// Affichage opérateur en mode icone
		m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR1,RGB(192,192,192)); 
		m_Button_Menu_Ope_Moda2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}
	else
	{
		// Affichage en mode texte
		CString Txt;
		m_Button_Menu_Ope_Moda2.SetBitmaps(0,0);
		m_Button_Dim1_XOR.GetWindowText(Txt);
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}

	// rend invisible tous les boutons opérateurs
	SetVisibleOpeModa2(FALSE);
	
	// Rend actif l'arrière plan
	m_List_Dimension.EnableWindow(TRUE); 
}

void CDimensionDlg::OnDim_1_Raz() 
{
	if(m_List_Dimension.ToggleSelection())
		GetParent()->PostMessage(WM_USER_DIMENSION_X_DLG_UPDATED,m_Type,m_Index);
}

void CDimensionDlg::OnChange_Dim_1_PaletteSel()
{
	if(m_Button_Dimension.GetCheck())
	{
		if(m_List_Dimension.GetSelCount()==0)
		{
			m_Button_Dimension.SetCheck(BST_UNCHECKED,1);
			CString Text;
			Text.Format(GETSTRING(IDS_RI_DIM_ERROR), m_DimText); 
			MessageBox(Text,_T(""),MB_OK|MB_ICONEXCLAMATION);
			return;
		}
	}
	GetParent()->PostMessage(WM_USER_DIMENSION_X_DLG_DIMSEL,m_Type,m_Index);
	GetParent()->PostMessage(WM_USER_DIMENSION_X_DLG_UPDATED,m_Type,m_Index);
}

void CDimensionDlg::SetDimMode( bool bDimMode, int DimModeIndex, CString & DimText)
{
	m_bDimMode		=	bDimMode;
	m_DimModeIndex	=	DimModeIndex;
	m_DimText		=	DimText;

	if(m_bDimMode && DimModeIndex>=0)
		m_Button_Dimension.SetCheck(true);
	else
		m_Button_Dimension.SetCheck(false);
	UpdateTitle();
}

void CDimensionDlg::OnSelchangeListDimension() 
{
	GetParent()->PostMessage(WM_USER_DIMENSION_X_DLG_UPDATED,m_Type,m_Index);
}

// Set the data for this dialog
void CDimensionDlg::SetData(IQuestion::DIMTYPE  Type, int Index, JVector<const IModalite *> & Dimension)
{
	m_Type			= Type;
	m_Index			= Index;

	m_List_Dimension.ResetContent();
	m_List_Dimension.AddItems(Dimension);

	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_OU);

	CString Txt = "";
	if (m_bIcones)
	{
		// Affichage opérateur en mode icone
		m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
		m_Button_Menu_Ope_Moda2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}
	else
	{
		// Affichage en mode texte
		m_Button_Menu_Ope_Moda2.SetBitmaps(0,0);
		m_Button_Menu_Ope_Moda2.GetWindowText(Txt);
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}

	// Bouton opératuer uniquement visible en mode répertoire items
	CDlgItems *pDlg = static_cast<CDlgItems *>(GetParent()); 
	if (pDlg->GetTypeRepItem() != true)
	{
		m_Button_Menu_Ope_Moda2.ShowWindow(SW_HIDE); 

		// Retaille header label et bouton caché derrière
		CRect RectLabel;
		CRect RectMenu;
		m_Label.GetWindowRect(&RectLabel);
		m_Button_Menu_Ope_Moda2.GetWindowRect(&RectMenu);

		// Bouton opérateur absent
		RectLabel.left  = RectMenu.left;
		this->ScreenToClient(&RectLabel);
		
		// Placement des boutons label
		m_Label.SetWindowPos(NULL,RectLabel.left, RectLabel.top, RectLabel.Width(), RectLabel.Height() ,SWP_NOZORDER);
		m_Button_Dimension.SetWindowPos(NULL,RectLabel.left, RectLabel.top, RectLabel.Width(), RectLabel.Height() ,SWP_NOZORDER);
	}

	// Si mode Répertoire Items, mais pas mode Items on cache opérateur
	if (pDlg->ValCurMode() != Mode_Items || pDlg->GetTypeRepItem() == false)
		SetOpeDimension3(false,pDlg->GetAffichageIcones());
		//m_Button_Menu_Ope_Moda2.EnableWindow(false); 
	else
		SetOpeDimension3(true, pDlg->GetAffichageIcones());
		//m_Button_Menu_Ope_Moda2.EnableWindow(true); 

	UpdateTitle();
}

///////////////////////////////////////////////////////////////////
// Rend actif ou pas l'opérateur dimension 3
void CDimensionDlg::SetOpeDimension3(bool Actif, bool ModeIcone)
{
	if (Actif)
	{
		// Activation en mode autre que palette
		m_Button_Menu_Ope_Moda2.ShowWindow(SW_SHOW); 

		// puis bouton sélection opération sur opérateur en cours
		if(ModeIcone)
		{
			// puis bouton sélection opération sur dimension21 en cours
			if (m_Button_Dim1_ET.GetCheck())
				m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_ET1,RGB(192,192,192),IDB_RI_CIRCLES_ET1,RGB(192,192,192)); 
			else if (m_Button_Dim1_OU.GetCheck())
				m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
			else if (m_Button_Dim1_NI.GetCheck())
				m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_NI1,RGB(192,192,192),IDB_RI_CIRCLES_NI1,RGB(192,192,192));
			else if (m_Button_Dim1_XOR.GetCheck())
				m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_XOR1,RGB(192,192,192),IDB_RI_CIRCLES_XOR1,RGB(192,192,192));

			m_Button_Menu_Ope_Moda2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
			m_Button_Menu_Ope_Moda2.SetWindowText("");
		}
		else
		{
			CString Txt;
			if (m_Button_Dim1_ET.GetCheck())
				m_Button_Dim1_ET.GetWindowText(Txt);
			else if (m_Button_Dim1_OU.GetCheck())
				m_Button_Dim1_OU.GetWindowText(Txt);
			else if (m_Button_Dim1_NI.GetCheck())
				m_Button_Dim1_NI.GetWindowText(Txt);
			else if (m_Button_Dim1_XOR.GetCheck())
				m_Button_Dim1_XOR.GetWindowText(Txt);
			
			m_Button_Menu_Ope_Moda2.SetBitmaps(0,0);
			m_Button_Menu_Ope_Moda2.SetWindowText(Txt);
		}

		// Remet les back color d'origine
		m_Button_Menu_Ope_Moda2.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp),m_CATPColorsApp,0);
		m_Button_Menu_Ope_Moda2.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(m_CATPColorsApp),m_CATPColorsApp);

		// Désactivation en mode items
		m_Button_Menu_Ope_Moda2.EnableWindow(true); 
	}
	else
	{			
		// Désactivation en mode palette
		m_Button_Menu_Ope_Moda2.EnableWindow(false); 

		// Efface le texte
		m_Button_Menu_Ope_Moda2.SetBitmaps(0,0);
		m_Button_Menu_Ope_Moda2.SetWindowText(""); 

		// Remettre Fond Back Color inactif
		m_Button_Menu_Ope_Moda2.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp,0);
		m_Button_Menu_Ope_Moda2.SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp,0);
	}
}

void CDimensionDlg::SetData(IQuestion::DIMTYPE  Type, int Index, JList<const IModalite *> & Dimension)
{
	m_Type			= Type;
	m_Index			= Index;

	m_List_Dimension.ResetContent();
	m_List_Dimension.AddItems(Dimension);

	CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_OU);

	CString Txt = "";
	if (m_bIcones)
	{
		// Affichage opérateur en mode icone
		m_Button_Menu_Ope_Moda2.SetBitmaps(IDB_RI_CIRCLES_OU1,RGB(192,192,192),IDB_RI_CIRCLES_OU1,RGB(192,192,192)); 
		m_Button_Menu_Ope_Moda2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);  
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}
	else
	{
		// Affichage en mode texte
		m_Button_Menu_Ope_Moda2.SetBitmaps(0,0);
		m_Button_Menu_Ope_Moda2.GetWindowText(Txt);
		m_Button_Menu_Ope_Moda2.SetWindowText(Txt); 
	}

	UpdateTitle();
}

void CDimensionDlg::UpdateTitle()
{
	CString text;
	CString ToolTip;

	switch(m_Type)
	{
		case IQuestion::MODALITE:
		{
			LOADSTRING(ToolTip, IDS_RI_TT_ET_M); 
			m_Button_Dim1_ET.SetTooltipText(ToolTip);
			LOADSTRING(ToolTip, IDS_RI_TT_OU_M); 
			m_Button_Dim1_OU.SetTooltipText(ToolTip);
			LOADSTRING(ToolTip, IDS_RI_TT_NI_M); 
			m_Button_Dim1_NI.SetTooltipText(ToolTip);
			LOADSTRING(ToolTip, IDS_RI_TT_XOR_M); 
			m_Button_Dim1_XOR.SetTooltipText(ToolTip);
			LOADSTRING(ToolTip, IDS_RI_TT_T_M); 
			m_Button_Dim1_RAZ.SetTooltipText(ToolTip);

			if(m_Button_Dimension.GetCheck())
				// text.Format(IDS_RI_DIM_SEL,m_DimText, m_DimModeIndex);
				text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT), "");
			else
			{
				text.Format(GETSTRING(IDS_RI_DIM_DIMENSION), m_Index+1);
				if(m_bDimMode)
					// text.Format(IDS_RI_EN,m_DimText);
					text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT_NONVENT));
			}
		}
		break;
		
		case IQuestion::ELEMENT:
		{
			LOADSTRING(ToolTip, IDS_RI_TT_ET_E); 
			m_Button_Dim1_ET.SetTooltipText(ToolTip);
			LOADSTRING(ToolTip, IDS_RI_TT_OU_E); 
			m_Button_Dim1_OU.SetTooltipText(ToolTip);
			LOADSTRING(ToolTip, IDS_RI_TT_NI_E); 
			m_Button_Dim1_NI.SetTooltipText(ToolTip);
			LOADSTRING(ToolTip, IDS_RI_TT_XOR_E); 
			m_Button_Dim1_XOR.SetTooltipText(ToolTip);
			LOADSTRING(ToolTip, IDS_RI_TT_T_E); 
			m_Button_Dim1_RAZ.SetTooltipText(ToolTip);

			if(m_Button_Dimension.GetCheck())
				text.Format(GETSTRING(IDS_RI_DIM_SEL), m_DimText, m_DimModeIndex);
			else
			{
				text.Format(GETSTRING(IDS_RI_DIM_DIMENSION), m_Index+1);
				if(m_bDimMode)
					text.Format(GETSTRING(IDS_RI_EN), m_DimText);
			}
		}
		break;
	}
	m_Button_Dimension.SetWindowText(text);
	m_Label.SetWindowText(text);

	if(m_bDimMode)
	{
		m_Button_Dimension.	ShowWindow(SW_SHOW);
		m_Label.			ShowWindow(SW_HIDE);
	}
	else
	{
		m_Button_Dimension.	ShowWindow(SW_HIDE);
		m_Label.			ShowWindow(SW_SHOW);
	}

}

void CDimensionDlg::SetDimensionOperator(IBloc::OPERATOR Operator)
{
	switch(Operator)
	{
		case IBloc::And :
			CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_ET);
		break;

		case IBloc::Or :
			CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_OU);
		break;

		case IBloc::Not :
			CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_NI);
		break;
		case IBloc::Xor :
			CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_XOR);
		break;
	}
}

IBloc::OPERATOR CDimensionDlg::GetDimensionOperator()
{
	switch(GetCheckedRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR))
	{
		case IDC_RI_RAD_DIM1_ET:
			return IBloc::And;
		break;

		case IDC_RI_RAD_DIM1_OU:
			return IBloc::Or;
		break;

		case IDC_RI_RAD_DIM1_NI:
			return IBloc::Not;
		break;

		case IDC_RI_RAD_DIM1_XOR:
			return IBloc::Xor;
		break;
	}
	return IBloc::Nop;
}


LRESULT CDimensionDlg::OnNcHitTest(CPoint point)
{
    UINT hit = CDialog::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}

// Déclenchement menu opérateur pour 2ème modalité
void CDimensionDlg::OnBnClickedRiButMenuOpeModa2()
{
	// Rend visible pour la sélection des opérateurs
	SetVisibleOpeModa2(TRUE);

	// Rend inactif l'arrière plan liste modalités
	m_List_Dimension.EnableWindow(FALSE); 
}

// Visibilité sélection des opérateurs
void CDimensionDlg::SetVisibleOpeModa2(bool Visible)
{
	// Concernant les opérateurs sur la dimension modalité 2
	m_Button_Dim1_ET.ShowWindow(Visible);
	if (Visible==false) m_Button_Dim1_ET.ActivateTopParent(); 
	m_Button_Dim1_OU.ShowWindow(Visible);
	if (Visible==false) m_Button_Dim1_OU.ActivateTopParent(); 
	m_Button_Dim1_XOR.ShowWindow(Visible);
	if (Visible==false) m_Button_Dim1_XOR.ActivateTopParent(); 
	m_Button_Dim1_NI.ShowWindow(Visible);
	if (Visible==false) m_Button_Dim1_NI.ActivateTopParent(); 
}

void CDimensionDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);

	// Y a t-il un opérateur sur modalité 2 actif
	if (m_Button_Dim1_ET.IsWindowVisible()  || 
		m_Button_Dim1_OU.IsWindowVisible()  ||
		m_Button_Dim1_XOR.IsWindowVisible() ||
		m_Button_Dim1_NI.IsWindowVisible())
	{
		// Rend invisible tous les menus opérateurs
		SetVisibleOpeModa2(false);
	}
}

