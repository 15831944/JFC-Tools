// ValuesDlg.cpp : implementation file
//

#include "stdafx.h"

#include ".\valuesdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CValuesDlg dialog


CValuesDlg::CValuesDlg(CWnd* pParent) : CDialog(CValuesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CValuesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_NbValues = 0;

	m_bPalValues = false;
	
	m_bNumValues = false;
	m_TypeMode   = 0; // Item mode
	m_CATPColorsApp = CATPColors::COLORREPITEMS;

	m_FenDejaVisu = false;

	// Fenêtre bloquée
	m_LockFen = false;

	// Fenêtre volante par défaut
	m_FlightWindow = true;
}

CValuesDlg::~CValuesDlg()
{
	RemoveValues();
}

void CValuesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CValuesDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CValuesDlg, CDialog)
	//{{AFX_MSG_MAP(CValuesDlg)
	ON_BN_CLICKED(IDC_RI_BUTTON, OnButtonValues)
	ON_BN_CLICKED(IDC_RI_BUT_VALUES_QTLE, OnButtonQTLEValues)
	ON_BN_CLICKED(IDC_RI_BUT_VALUES_RAZ, OnButtonRAZValues)
	ON_BN_CLICKED(IDC_RI_BUT_VALUES_SUP1, OnButtonSup1Values)
	ON_LBN_SELCHANGE(IDC_RI_LIST_VALUES, OnSelchangeListValues)
	ON_WM_SHOWWINDOW()
	ON_WM_MOVE()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

void CValuesDlg::SetItemMode()
{
	m_TypeMode   = 0;

	m_bPalValues = false;
	UpdateTitle();
}

void CValuesDlg::SetPaletteMode(JBool bPalValues, const JChar* pLabValues)
{
	// Indique mode palette en cours
	m_TypeMode   = 1;
	m_LabValues  = pLabValues;
	
	if (!m_FenDejaVisu)
	{
		// D'office le mode ajout palette est sélectionné
		//
		// Aout 2013
		// m_bPalValues  = true;
		m_bPalValues  = bPalValues;

		m_FenDejaVisu = true;

		// Mise à jour du titre
		UpdateTitle();
	}

	/*
	else
		// On fait comme avant, reprend la sélection courante
		m_bPalValues = bPalValues;
	*/
	// Mise à jour du titre
	// UpdateTitle();
}

void CValuesDlg::SetQuantileMode()
{
	m_TypeMode   = 2;
	m_bPalValues = false;
	UpdateTitle();
}

void CValuesDlg::UpdateTitle()
{
	if (m_bPalValues)
	{
		CString Text;
		// Text.Format(IDS_RI_DIM_SEL_NEW_INGREDIENT,m_LabValues.AsJCharPtr()); 
		// Text.Format(IDS_RI_DIM_SEL_NEW_INGREDIENT,""); 
		Text.Format(IDS_RI_DIM_SEL_VENTILER,"");
		m_Button.SetWindowText(Text);
		m_Button.SetCheck(1);
	}
	else
	{
		CString Text;
		// Text.Format(IDS_RI_EN,m_LabValues.AsJCharPtr()); 
		Text.Format(IDS_RI_DIM_SEL_NEW_INGREDIENT_NONVENT);
		m_Button.SetWindowText(Text);
		m_Button.SetCheck(0);
	}
	if(m_TypeMode == 1)
	{
		m_Label     .ShowWindow(SW_HIDE);
		m_ButtonQTLE.ShowWindow(SW_HIDE);
		m_Button    .ShowWindow(SW_SHOW);
	}
	else if (m_TypeMode == 2)
	{
		m_Label     .ShowWindow(SW_HIDE);
		m_Button    .ShowWindow(SW_HIDE);
		m_ButtonQTLE.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Button    .ShowWindow(SW_HIDE);
		m_ButtonQTLE.ShowWindow(SW_HIDE);
		m_Label     .ShowWindow(SW_SHOW);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CValuesDlg message handlers

void CValuesDlg::OnButtonValues() 
{
	m_bPalValues = false;
	if (m_TypeMode == 1 && m_Button.GetCheck() != 0)
	{
		if (m_ListValues.GetSelCount() == 0)
		{
			CString Text;
			Text.Format(IDS_RI_VAL_ERROR,m_LabValues.AsJCharPtr()); 
			this->MessageBox(Text, _T(""), MB_OK|MB_ICONEXCLAMATION);
			m_Button.SetCheck(0);
			return;
		}
		m_bPalValues = true;
	}

	
	if (m_bPalValues)
	{
		CString Text;
		// Text.Format(IDS_RI_DIM_SEL_NEW_INGREDIENT,""); 
		Text.Format(IDS_RI_DIM_SEL_VENTILER,""); 
		m_Button.SetWindowText(Text);
		m_Button.SetCheck(1);
	}
	else
	{
		CString Text;
		// Text.Format(IDS_RI_EN,m_LabValues.AsJCharPtr()); 
		Text.Format(IDS_RI_DIM_SEL_NEW_INGREDIENT_NONVENT);

		m_Button.SetWindowText(Text);
		m_Button.SetCheck(0);
	}

	if(m_ListValues.GetSelCount())
		UpdateSelections();
	GetParent()->PostMessage(WM_USER_VALUE_DLG_DIMSEL,0,0);
}

void CValuesDlg::OnButtonQTLEValues()
{
	if (m_ListValues.GetSelCount() > 0)
	{
		CRect rect;
		CMenu menu;
		CString Label;

		m_ButtonQTLE.GetWindowRect(&rect);

		menu.CreatePopupMenu();
		Label.LoadString(IDS_RI_TERCILE);
		menu.AppendMenu(MF_STRING,IDM_RI_TERCILE,Label); 
		Label.LoadString(IDS_RI_QUARTILE);
		menu.AppendMenu(MF_STRING,IDM_RI_QUARTILE,Label); 
		Label.LoadString(IDS_RI_QUINTILE);
		menu.AppendMenu(MF_STRING,IDM_RI_QUINTILE,Label); 
		Label.LoadString(IDS_RI_DECILE);
		menu.AppendMenu(MF_STRING,IDM_RI_DECILE,Label);
		switch(menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD,rect.left,rect.bottom,this))
		{
			case IDM_RI_TERCILE:
				GetParent()->SendMessage(WM_USER_VALUE_DLG_QUANTILE,0,3);
			break;
			case IDM_RI_QUARTILE:
				GetParent()->SendMessage(WM_USER_VALUE_DLG_QUANTILE,0,4);
			break;
			case IDM_RI_QUINTILE:
				GetParent()->SendMessage(WM_USER_VALUE_DLG_QUANTILE,0,5);
			break;
			case IDM_RI_DECILE:
				GetParent()->SendMessage(WM_USER_VALUE_DLG_QUANTILE,0,10);
			break;
		}
	}
	else
	{
		CString Text;
		Text.LoadString(IDS_RI_QUANTILE_ERROR); 
		this->MessageBox(Text, _T(""), MB_OK|MB_ICONEXCLAMATION);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//	Sélection 1ere valeur > 0 avec prise en compte de toutes les suivantes
///////////////////////////////////////////////////////////////////////////////////////
void CValuesDlg::OnButtonSup1Values()
{
	// Curseur d'attente car dans certains cas ça peut être long
	CWaitCursor Wait;

	m_ListValues.SetRedraw(FALSE);
	int FirstVisible = m_ListValues.GetTopIndex();

	if(m_ListValues.GetSelCount())
	{
		// Si déjà une sélection on remet tout à zéro
		long count = m_ListValues.GetCount();
		for (long idx = 0; idx < count; idx += 1) m_ListValues.SetSel(idx, 0);
		ResetAll();
		UpdateSelected();
	}
	else
	{
		// Recherche de la 1ere valeur > 0
		long idx = 0;
		for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
		{
			CValueSel * pVal = m_Values.GetItem();
			if (pVal->GetValue() > 0)
			{
				// On prend cet element comme referebce pour tout ce qui est supérieur à 
				pVal->SetOperator(IBloc::GTE);
				pVal->SetSelected(1);
				m_ListValues.SetSel(idx, 1);
				break;
			}
			idx++;
		}

		// Mise à jour de la sélection
		UpdateSelections(true);
	}

	if (FirstVisible != LB_ERR) m_ListValues.SetTopIndex(FirstVisible);
	m_ListValues.SetRedraw(TRUE);
	m_ListValues.RedrawWindow();
	GetParent()->PostMessage(WM_USER_VALUE_DLG_UPDATED,0,0);
}

void CValuesDlg::OnButtonRAZValues() 
{
	// Curseur d'attente car dans certains cas ça peut être long
	CWaitCursor Wait;

	m_ListValues.SetRedraw(FALSE);
	int FirstVisible = m_ListValues.GetTopIndex();
	if(m_ListValues.GetSelCount())
	{
		long count = m_ListValues.GetCount();
		for (long idx = 0; idx < count; idx += 1) m_ListValues.SetSel(idx, 0);
		// m_bPalValues = false;
		ResetAll();
		UpdateSelected();
		// m_Button.SetCheck(0);
	}
	else
	{
		// Nb Valeurs à sélectionner (toutes)
		long count = m_ListValues.GetCount();
		for (long idx = 0; idx < count; idx += 1) m_ListValues.SetSel(idx, 1);
		
		// Mise à jour de la sélection
		// m_bPalValues = false;
		UpdateSelections(true);
		
		/* ?????? moi pas comprendre !!!!!
		for (long idx = 0; idx < 1; idx += 1) m_ListValues.SetSel(idx, 1);
		UpdateSelections(true);
		*/
	}
	if (FirstVisible != LB_ERR) m_ListValues.SetTopIndex(FirstVisible);
	m_ListValues.SetRedraw(TRUE);
	m_ListValues.RedrawWindow();
	GetParent()->PostMessage(WM_USER_VALUE_DLG_UPDATED,0,0);
}

void CValuesDlg::OnSelchangeListValues() 
{
	// Curseur d'attente car dans certains cas ça peut être long
	CWaitCursor Wait;

	// Bloque mise à jour liste
	m_ListValues.SetRedraw(FALSE);

	if(m_ListValues.GetSelCount())
	{
		UpdateSelections();
	}
	else
	{
		m_bPalValues = false;
		ResetAll();
		UpdateSelected();
		// m_Button.SetCheck(0);
	}

	// Bloque mise à jour liste
	m_ListValues.SetRedraw(TRUE);

	GetParent()->PostMessage(WM_USER_VALUE_DLG_UPDATED,0,0);

}

// Set the quanti values
void CValuesDlg::SetQuantiValues(JVector<JFlt32> &Values,JInt32 Divider)
{
	m_ListValues.EnableWindow(false);
	m_bNumValues = false;
	int Top = 0;
	m_NbValues = Values.GetCount();
	m_Divider = Divider;
	SetupValuesMap(Values);
	CreateValuesVector();
	SetClassSelection();
	SetStrings();
	FillListBox();
	UpdateSelected();
	m_ListValues.EnableWindow(true);
	UpdateWindow();
}

// Set the quanti values
void CValuesDlg::SetQuantiValues(JVector<JFlt32> &Values,CIBClass *pClass,JInt32 Divider)
{
	m_ListValues.EnableWindow(false);
	m_bNumValues = false;
	int Top = 0;
	m_NbValues = Values.GetCount();
	m_Divider = Divider;
	SetupValuesMap(Values);
	SetSelectedValues(pClass);
	CreateValuesVector();
	SetClassSelection();
	SetStrings();
	FillListBox();
	UpdateSelected();
	m_ListValues.EnableWindow(true);
	UpdateWindow();
}

// Set the quanti values
void CValuesDlg::SetQuantiValues(JVector<JFlt32> &Values,CIBNumValue *pNumValue, JInt32 Divider)
{
	m_ListValues.EnableWindow(false);
	m_bNumValues = true;
	int Top = 0;
	m_NbValues = Values.GetCount();
	m_Divider = Divider;
	SetupValuesMap(Values);
	SetSelectedValues(pNumValue);
	CreateValuesVector();
	SetStrings();
	FillListBox();
	UpdateSelected();
	m_ListValues.EnableWindow(true);
	UpdateWindow();
}

void CValuesDlg::RemoveValues()
{
	for(m_ValuesMap.MoveFirst(); m_ValuesMap.IsValid(); m_ValuesMap.MoveNext())
	{
		delete m_ValuesMap.GetItem();
	}
	m_ValuesMap.Reset(); 
}

void CValuesDlg::SetupValuesMap(JVector<JFlt32> &Values)
{
	RemoveValues(); 
	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		CValueSel * pVal = new CValueSel;
		pVal->SetValue(Values.GetItem()); 
		m_ValuesMap.Add(pVal->GetValue()) = pVal;
	}
}

// Set the current selected values
void CValuesDlg::SetSelectedValues(CIBNumValue *pNumValue)
{
	bool SelChange = false;
	JList<JFlt32> Values;

	if(pNumValue)
		pNumValue->GetValues(Values);

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		CValueSel * pVal = 0;
		m_ValuesMap.MoveTo(Values.GetItem());
		if(!m_ValuesMap.IsValid())
		{
			pVal = new CValueSel;
			pVal->SetValue(Values.GetItem()); 
			m_ValuesMap.Add(pVal->GetValue()) = pVal;
		}
		else
		{
			pVal = m_ValuesMap.GetItem();
		}
		if(pVal)
		{
			pVal->SetOperator(IBloc::EQ);
			pVal->SetSelected(1);
		}
	}
}

// Set the current selected values
void CValuesDlg::SetSelectedValues(CIBClass *pClass)
{
	bool SelChange = false;
	JList<JFlt32> Values;
	JList<IBloc::OPERATOR> Ops;

	if(pClass)
	{
		pClass->GetValues(Values);
		pClass->GetOperators(Ops);
	}

	for(Values.MoveFirst(), Ops.MoveFirst(); Values.IsValid(); Values.MoveNext(), Ops.MoveNext())
	{
		CValueSel * pVal = 0;

		m_ValuesMap.MoveTo(Values.GetItem());

		if(!m_ValuesMap.IsValid())
		{
			pVal = new CValueSel;
			pVal->SetValue(Values.GetItem());
			pVal->SetOperator(Ops.GetItem());
			pVal->SetSelected(1); 
			m_ValuesMap.Add(pVal->GetValue()) = pVal;
			SelChange = true;
		}
		else
		{
			pVal = m_ValuesMap.GetItem();
			pVal->SetOperator(Ops.GetItem());
			pVal->SetSelected(1); 
		}
	}
}

// Get the selected values
CIBNumValue * CValuesDlg::GetNumValues()
{
	if(!m_bNumValues)
		return 0;

	CIBNumValue *pValues = 0;
	JList<JFlt32> Values;

	if(m_Values.GetCount())
	{
		for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
		{
			CValueSel * pVal = m_Values.GetItem();
			if(pVal->GetOperator()==IBloc::EQ)
				Values.AddTail() = pVal->GetValue();
		}
		if(Values.GetCount())
		{
			pValues = new CIBNumValue;
			pValues->SetValues(Values);
		}
	}
	return pValues;
}

// Get the selected values
CIBClass * CValuesDlg::GetClass()
{
	if(m_bNumValues)
		return 0;

	CIBClass *pValues = 0;

	JList<IBloc::OPERATOR>	Ops;
	JList<JFlt32>			Values;
		
	if(m_Values.GetCount())
	{
		for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
		{
			CValueSel * pVal = m_Values.GetItem();
			if(pVal->GetOperator()!=IBloc::Nop)
			{
				Values.AddTail()	= pVal->GetValue();
				Ops.AddTail()		= pVal->GetOperator();
			}
		}
		if(Values.GetCount())
		{
			pValues = new CIBClass;
			pValues->SetValues(Values);
			pValues->SetOperators(Ops);
		}
	}
	return pValues;
}

// Get the selected values
JList<JFlt32> & CValuesDlg::GetPalValues(JList<JFlt32> & NumValues)
{
	JList<JFlt32> TmpValues;
	if (m_bPalValues)
	{
		for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
		{
			CValueSel * pVal = m_Values.GetItem();
			if(pVal->GetOperator()!=IBloc::Nop)
			{
				TmpValues.AddTail()	= pVal->GetValue();
			}
		}
	}
	NumValues.Swap(TmpValues);
	return (NumValues);
}

BOOL CValuesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ListValues.SubclassDlgItem(IDC_RI_LIST_VALUES, this);
	m_Label.SubclassDlgItem(IDC_RI_LABEL, this);
	m_Button.SubclassDlgItem(IDC_RI_BUTTON, this);
	m_ButtonQTLE.SubclassDlgItem(IDC_RI_BUT_VALUES_QTLE, this);
	m_ButtonRAZ.SubclassDlgItem(IDC_RI_BUT_VALUES_RAZ, this);
	m_ButtonSup1.SubclassDlgItem(IDC_RI_BUT_VALUES_SUP1, this);
	m_Box.SubclassDlgItem(IDC_RI_STATIC_BOX, this);

	m_Label.SetFontName(CATPColors::GetFontLabelName());
	m_Label.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label.SetBorder(false);


	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_Label);
	m_Box.AddTopWindow(m_Button);
	m_Box.AddTopWindow(m_ButtonQTLE);
	m_Box.AddTopWindow(m_ButtonRAZ);
	m_Box.AddTopWindow(m_ButtonSup1);
	m_Box.AddTopWindow(m_ListValues);

	m_Button.ShowWindow(SW_HIDE);
	m_ButtonQTLE.ShowWindow(SW_HIDE);
	m_Button.SetCheck(0, FALSE);

	SetColors();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CValuesDlg::SetColors()
{
	m_Label.SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 

	m_Button.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp),m_CATPColorsApp,0);
	m_Button.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp);
	m_Button.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorSelect(m_CATPColorsApp),m_CATPColorsApp);
	m_Button.SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorBlack(),m_CATPColorsApp);
	m_Button.SetColor(CButtonST::BTNST_COLOR_FG_OUT,	RGB(255,255,0),m_CATPColorsApp);
	m_Button.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	RGB(255,255,0),m_CATPColorsApp);

	m_ButtonQTLE.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp),m_CATPColorsApp,0);
	m_ButtonQTLE.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp);
	m_ButtonQTLE.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorSelect(m_CATPColorsApp),m_CATPColorsApp);
	m_ButtonQTLE.SetColor(CButtonST::BTNST_COLOR_FG_IN,		CATPColors::GetColorBlack(),m_CATPColorsApp);
	m_ButtonQTLE.SetColor(CButtonST::BTNST_COLOR_FG_OUT,	RGB(255,255,0),m_CATPColorsApp);
	m_ButtonQTLE.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS,	RGB(255,255,0),m_CATPColorsApp);

	m_ButtonRAZ.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp),m_CATPColorsApp,0);
	m_ButtonRAZ.SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp),m_CATPColorsApp,0);
	m_ButtonRAZ.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp);

	m_ButtonSup1.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_CATPColorsApp),m_CATPColorsApp,0);
	m_ButtonSup1.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(m_CATPColorsApp),m_CATPColorsApp,0);
	m_ButtonSup1.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp);

	m_Box.SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);

	m_ListValues.SetSelectionColor(JFC_MultiHideListBox<CValueSel *>::NORMAL ,CATPColors::GetColorSelect(m_CATPColorsApp));
	m_ListValues.SetSelectionColor(JFC_MultiHideListBox<CValueSel *>::PREGNANCE,CATPColors::GetColorSelect(m_CATPColorsApp));

	// Rafraichit liste sélection valeur, sinon reste avec la couleur précédente
	m_ListValues.RedrawWindow(); 
}

void CValuesDlg::FillListBox()
{
	// Fills the List box with the values
	m_ListValues.ResetContent();
	for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
	{
		m_ListValues.AddItem(m_Values.GetItem());
	}
}

void CValuesDlg::SetStrings()
{
	for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
	{
		CValueSel * pVal = m_Values.GetItem();
		SetString(pVal);
	}
}

void CValuesDlg::SetString(CValueSel *pVal)
{
	CString strValue;
	CString strDiv;

	if(pVal)
	{
		if((pVal->GetValue()) < 0.0)
			strDiv.Format("%%s #");
		else
		{
			strDiv.Format("%%s %%0.");
			if(m_Divider>1)
			{
				strDiv+="4";
			}
			strDiv+="f";
		}

		switch(pVal->GetOperator())
		{
			case IBloc::EQ:
			{
				strValue.Format(strDiv,"=",pVal->GetValue());
			}
			break;

			case IBloc::LTE:
			{
				strValue.Format(strDiv,"<=",pVal->GetValue());
			}
			break;

			case IBloc::GTE:
			{
				strValue.Format(strDiv,">=",pVal->GetValue());
			}
			break;

			case IBloc::Nop:
			{
				strValue.Format(strDiv,"",pVal->GetValue());
			}
			break;
		}
		pVal->SetString(strValue);
	}
}

void CValuesDlg::CreateValuesVector()
{
	m_Values.Reset();
	m_Values.SetCount(m_ValuesMap.GetCount());

	for(m_Values.MoveFirst(), m_ValuesMap.MoveFirst(); m_ValuesMap.IsValid(); m_Values.MoveNext(), m_ValuesMap.MoveNext())
	{
		m_Values.GetItem() = m_ValuesMap.GetItem();
		m_Values.GetItem()->SetID(m_Values.GetIndex());
	}
}

void CValuesDlg::UpdateSelections(bool ForceGTE)
{
	m_bNumValues = (m_TypeMode == 1) && m_bPalValues;
	int NbSel = m_ListValues.GetSelCount();
	
	CArray<int,int> Sel;

	Sel.SetSize(NbSel);
	m_ListValues.GetSelItems(NbSel, Sel.GetData());

	if(NbSel==1)
	{
		ResetBefore(Sel.GetAt(0));
		ResetAfter(Sel.GetAt(0));
		m_Values.MoveTo(Sel.GetAt(0));
		if(m_Values.IsValid())
		{
			CValueSel * pVal = m_Values.GetItem();
			if (ForceGTE)
			{
				pVal->SetSelected(3);
				pVal->SetOperator(IBloc::GTE); 
				SelectAfter(Sel.GetAt(0),1);
			}
			else
			{
				if(!m_ListValues.IsRightClick())
				{
					pVal->SetSelected(0);
					pVal->SetOperator(IBloc::Nop);
					m_bNumValues = true;
				}

				switch(pVal->GetSelected())
				{
					case 0:
					{
						pVal->SetSelected(1);
						pVal->SetOperator(IBloc::EQ);
						m_bNumValues = true;
					}
					break;
					case 1:
					{
						pVal->SetSelected(2);
						pVal->SetOperator(IBloc::LTE); 
						SelectBefore(Sel.GetAt(0),1);
					}
					break;
					case 2:
					{
						pVal->SetSelected(3);
						pVal->SetOperator(IBloc::GTE); 
						SelectAfter(Sel.GetAt(0),1);
					}
					break;
					case 3:
					{
						pVal->SetSelected(0);
						pVal->SetOperator(IBloc::Nop);
						m_bNumValues = true;
					}
					break;
				}
			}
			SetString(pVal);
		}
	}
	else
	{
//		bool bRange = true;
		bool bRange = (m_TypeMode != 1) || !m_bPalValues;
		int Pos = -1;
		for(int x=0; x< Sel.GetSize(); x++)
		{
			if(Pos==-1)
			{
				Pos = Sel.GetAt(x);
			}
			else
			{
				if(Pos!= Sel.GetAt(x)-1)
				{
					bRange = false;
					break;
				}
				else
					Pos = Sel.GetAt(x);
			}
		}
		ResetAll();
		if(bRange)
		{
			m_Values.MoveTo(Sel.GetAt(0));
			if(m_Values.IsValid())
			{
				CValueSel * pVal = m_Values.GetItem();
				pVal->SetSelected(3);
				pVal->SetOperator(IBloc::GTE);
				SelectAfter(Sel.GetAt(0),1);
				SetString(pVal);
			}

			m_Values.MoveTo(Sel.GetAt(Sel.GetSize()-1));
			if(m_Values.IsValid())
			{
				CValueSel * pVal = m_Values.GetItem();
				pVal->SetSelected(2);
				pVal->SetOperator(IBloc::LTE);
				SelectAfter(Sel.GetAt(Sel.GetSize()-1),0);
				SetString(pVal);
			}
		}
		else
		{
			m_bNumValues = true;
			for(int x=0; x<Sel.GetSize();x++)
			{
				m_Values.MoveTo(Sel.GetAt(x));
				if(m_Values.IsValid())
				{
					CValueSel * pVal = m_Values.GetItem();
					pVal->SetSelected(1);
					pVal->SetOperator(IBloc::EQ);
					SetString(pVal);
				}
			}
		}
	}
	UpdateSelected();
}

void	CValuesDlg::ResetAll()
{
	for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
	{
		CValueSel * pVal = m_Values.GetItem();
		pVal->SetOperator(IBloc::Nop);
		pVal->SetSelected(0);
		SetString(pVal);
	}
}

void	CValuesDlg::ResetBefore(int Pos)
{
	for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
	{
		if(m_Values.GetIndex() == Pos)
			break;

		CValueSel * pVal = m_Values.GetItem();
		pVal->SetOperator(IBloc::Nop);
		pVal->SetSelected(0);
		SetString(pVal);
	}
}

void	CValuesDlg::ResetAfter(int Pos)
{
	for(m_Values.MoveLast(); m_Values.IsValid(); m_Values.MovePrev())
	{
		if(m_Values.GetIndex() == Pos)
			break;

		CValueSel * pVal = m_Values.GetItem();
		pVal->SetOperator(IBloc::Nop);
		pVal->SetSelected(0);
		SetString(pVal);
	}
}

void	CValuesDlg::SelectBefore(int Pos, int Select)
{
	for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
	{
		if(m_Values.GetIndex() == Pos)
			break;

		CValueSel * pVal = m_Values.GetItem();
		pVal->SetSelected(Select);
		SetString(pVal);
	}
}

void	CValuesDlg::SelectAfter(int Pos, int Select)
{
	for(m_Values.MoveLast(); m_Values.IsValid(); m_Values.MovePrev())
	{
		if(m_Values.GetIndex() == Pos)
			break;

		CValueSel * pVal = m_Values.GetItem();
		pVal->SetSelected(Select);
		SetString(pVal);
	}
}

void CValuesDlg::SetClassSelection()
{
	int Count = 0;
	for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
	{
		CValueSel * pVal = m_Values.GetItem();
		if(pVal->GetOperator()!=IBloc::Nop)
			Count++;
	}
	if(Count==1)
	{
		for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
		{
			CValueSel * pVal = m_Values.GetItem();
			switch(pVal->GetOperator())
			{
				case IBloc::EQ:
				{
					pVal->SetSelected(1);
				}
				break;
				case IBloc::LTE:
				{
					pVal->SetSelected(2);
					SelectBefore(m_Values.GetIndex(),1);
				}
				break;
				case IBloc::GTE:
				{
					pVal->SetSelected(3);
					SelectAfter(m_Values.GetIndex(),1);
				}
				break;
			}
		}
	}
	if(Count==2)
	{
		for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
		{
			CValueSel * pVal = m_Values.GetItem();
			switch(pVal->GetOperator())
			{
				case IBloc::LTE:
				{
					pVal->SetSelected(2);
					SelectAfter(m_Values.GetIndex(),0);
				}
				break;
				case IBloc::GTE:
				{
					pVal->SetSelected(3);
					SelectAfter(m_Values.GetIndex(),1);
				}
				break;
			}
		}
	}
}

void CValuesDlg::UpdateSelected()
{
	JList<JInt32> Sel;
	for(m_Values.MoveFirst(); m_Values.IsValid(); m_Values.MoveNext())
	{
		CValueSel * pVal = m_Values.GetItem();
		if(pVal->GetSelected())
			Sel.AddTail() = pVal->GetID();
	}
	m_ListValues.SetSelectedID(Sel);
}

LRESULT CValuesDlg::OnNcHitTest(CPoint point)
{
    UINT hit = CDialog::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}

// Repositionnement de la fenêtre valeurs quantitatives
void CValuesDlg::UpdateSize()
{
	// Espace bordure
	const int SizeBordure = 4;

	// Resize liste des valeurs
	CRect Rect;
	GetWindowRect(&Rect);

	// Les dimensions controles
	CRect RectBtnRAZ;
	m_ButtonRAZ.GetWindowRect(&RectBtnRAZ);

	CRect RectBtnSup1;
	m_ButtonSup1.GetWindowRect(&RectBtnSup1);

	// Positionne label et bouton tous
	JUnt32 LargLabel = Rect.right - Rect.left - RectBtnSup1.Width() - RectBtnRAZ.Width() - 4 * SizeBordure;
	JUnt32 LargRaz = RectBtnRAZ.Width();
	JUnt32 LargSup1 = RectBtnSup1.Width();

	// m_Label.SetWindowPos(NULL, SizeBordure, SizeBordure, Rect.right - Rect.left - RectBtnRAZ.Width() - SizeBordure, RectBtnRAZ.Height() , SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_Label.SetWindowPos(NULL, SizeBordure, SizeBordure, LargLabel, RectBtnRAZ.Height() , SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// Récupère nvlles coordonnées du label
	CRect RectLabel;
	m_Label.GetWindowRect(&RectLabel);

	// Repositionnement des autres labels
	m_ButtonSup1.SetWindowPos(NULL, RectLabel.Width() + (3 * SizeBordure) / 2 , SizeBordure, RectBtnSup1.Width() , RectBtnSup1.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_ButtonRAZ.SetWindowPos(NULL, RectLabel.Width() + LargSup1 + 2 * SizeBordure, SizeBordure, RectBtnRAZ.Width() , RectBtnRAZ.Height(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	m_ButtonQTLE.SetWindowPos(NULL, SizeBordure, SizeBordure, Rect.right - Rect.left - LargSup1 - LargRaz - 4 * SizeBordure - RectBtnRAZ.Width() , RectBtnRAZ.Height() , SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	m_Button.SetWindowPos(NULL, SizeBordure, SizeBordure, Rect.right - Rect.left - LargSup1 - LargRaz - 4 * SizeBordure , RectBtnRAZ.Height() , SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	CRect RectBtn;
	m_ButtonQTLE.GetWindowRect(&RectBtn);

	// Calcul emplacement liste
	m_ListValues.SetWindowPos(NULL, SizeBordure, RectLabel.Height() + 2 * SizeBordure, Rect.right - Rect.left - 3 * SizeBordure, Rect.bottom - Rect.top - RectLabel.Height() - 5 * SizeBordure,SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// Le fond
	m_Box.SetWindowPos(NULL, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);

	// Fenêtre bloquée
	m_LockFen = true;

}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Visualisation de la boite value
void CValuesDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	UpdateSize();
}


void CValuesDlg::OnWindowPosChanging( WINDOWPOS* lpwndpos )
{
	// In WizardMode the System calls SetWindowPos with the 
	// original size at every activation. This could cause
	// some flicker in certain circumstances. Therefore we lock
	// moving the page and unlock it only if _we_ move the page
	// lpwndpos->flags |= SWP_NOMOVE | SWP_NOSIZE;

	if (m_LockFen)
	{
		lpwndpos->flags |= SWP_NOMOVE;
		CDialog::OnWindowPosChanging(lpwndpos);
	}
}

void CValuesDlg::SetBlockFen(bool Bloque)
{
	m_LockFen = Bloque;
}

// Positionne ou non comme fenêtre volante
void CValuesDlg::SetFlightWindow(bool Flight)
{
	m_FlightWindow = Flight;
}