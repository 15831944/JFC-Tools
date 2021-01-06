// DlgSurlignage.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSurlignage.h"
#include ".\dlgsurlignage.h"
#include "ExcelPrinter.h"


// CDlgSurlignage dialog

IMPLEMENT_DYNAMIC(CDlgSurlignage, CDialog)
CDlgSurlignage::CDlgSurlignage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSurlignage::IDD, pParent)
{
	// Init mode base 100
	m_Base100			= FALSE;

	m_pBut_Surlignage	= 0;

	m_pIItemTable		= 0;
	m_pDoc				= 0;
}

CDlgSurlignage::~CDlgSurlignage()
{
}

void CDlgSurlignage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// Message Maps
BEGIN_MESSAGE_MAP(CDlgSurlignage, CDialog)
	ON_BN_CLICKED(IDC_AN_BUT_EXIT1, OnBnClickedAnButExit)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_QUICKLIST_GETLISTITEMDATA, OnGetListItem)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_AN_BUT_EXPORT, OnBnClickedAnButExport)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_AN_LIST_ITEMS1, OnLvnColumnclickAnListItems1)
END_MESSAGE_MAP()


// CDlgSurlignage message handlers

void CDlgSurlignage::OnBnClickedAnButExit()
{
	// Dévalide le bouton surlignage de la fenêtre dialogue tris croisés
	UINT ClickMode = m_pDoc->Get_ClickMode();
	if(ClickMode == CMODE_ORDONNER)
	{
		m_pDoc->Set_ClickMode(CMODE_TRI);
		if (m_pBut_Surlignage) m_pBut_Surlignage->SetCheck(0);

		m_Base100 = FALSE;
	}
	else
		ShowWindow(SW_HIDE);
}

BOOL CDlgSurlignage::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetupControls();
	m_List.SetItemCount(0); 

	m_InfoText.FlashBackground(FALSE);
	m_InfoText.FlashText(TRUE);
	m_InfoText.ShowWindow(SW_SHOW);
	m_InfoText.EnableWindow(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgSurlignage::OnSysColorChange()
{
	CDialog::OnSysColorChange();
	SetColors(); 
}

void CDlgSurlignage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// Retaille les colonnes
	RetailleColonne();
}

void CDlgSurlignage::SetupControls()
{
	m_Box.				SubclassDlgItem(IDC_AN_STATIC_BOX1, this);
	m_Label.			SubclassDlgItem(IDC_AN_LABEL1, this);
	m_List.				SubclassDlgItem(IDC_AN_LIST_ITEMS1, this);
	m_InfoText.			SubclassDlgItem(IDC_INFO_TEXT_FILIGRAMME, this);
	m_But_Exit.			SubclassDlgItem(IDC_AN_BUT_EXIT1, this);
	m_But_Export.		SubclassDlgItem(IDC_AN_BUT_EXPORT, this);

	CRect rect;
	m_List.GetClientRect(&rect);
	int nColInterval = rect.Width()/24;
	m_List.InsertColumn(0, _T("Pos"),		LVCFMT_LEFT,	nColInterval);
	m_List.InsertColumn(1, _T("Ligne"),		LVCFMT_LEFT,	nColInterval*6);
	m_List.InsertColumn(2, _T("Col"),		LVCFMT_LEFT,	nColInterval*6);
	m_List.InsertColumn(3, _T("Page"),		LVCFMT_LEFT,	nColInterval*6);
	m_List.InsertColumn(4, _T("Valeur"),	LVCFMT_RIGHT,	rect.Width()-(19*nColInterval));
	m_List.EnableToolTips();
	m_List.EnableColumnNavigation(false);

//	m_List.SetToolTips(&m_ToolTip);

	m_Label.SetFontName(CATPColors::GetFontLabelName());
	m_Label.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label.SetFontBold(true);
	m_Label.SetBorder(false);

	m_InfoText.SetFontName(CATPColors::GetFontLabelName());
	m_InfoText.SetFontSize(CATPColors::GetFontBigLabelSize());
	m_InfoText.SetBorder(false);

	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_Label);
	m_Box.AddTopWindow(m_But_Export);
	m_Box.AddTopWindow(m_List);
	m_Box.AddTopWindow(m_InfoText);
	m_Box.AddTopWindow(m_But_Exit);
	
	// Set the windows colors
	SetColors();

	// Set window relations and positions for resizing
	SetWindowRelations();
}

void CDlgSurlignage::SetColors()
{
	m_Box.			SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORNOAPP,CATPColors::MEDIUM_DEGRAD);

	// Bouton export composition item
	m_But_Export.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Export.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Export.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Export.		SetBitmaps(IDB_AN_EXPORT_SURLIGN,CATPColors::GetColorMedium(CATPColors::GREY_MOUSE)); 

	// BackColor of list
	m_List.SetBkColor(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR));

	// Bouton sortie fenêtre surlignage
	m_But_Exit.			SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,0);
	m_But_Exit.			SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);
	m_But_Exit.			SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR);

	m_Label.			SetBkColor(CATPColors::GetColorDark(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::DARK_DEGRAD);
	m_Label.			SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORANALYSEUR)); 

	m_InfoText.			SetBkColor(CATPColors::GetColorLight(CATPColors::COLORANALYSEUR),CATPColors::COLORANALYSEUR,CATPColors::LIGHT_DEGRAD);
	m_InfoText.			SetTextColor(CATPColors::GetColorRed()); 
}

void CDlgSurlignage::SetWindowRelations()
{
	// Set all widow relation positions
	static CResizer::CBorderInfo s_bi[] = {

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  CONFIG BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_AN_STATIC_BOX1,		{CResizer::eFixed,			IDC_MAIN, CResizer::eLeft}, //Left side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eTop},  //Top side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eRight}, //Right side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eBottom}}, //Bottom side

	{IDC_AN_LABEL1,				{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eLeft},	//Left side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eTop},	//Top side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eRight},	//Right side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eTop}},	//Bottom side

	{IDC_AN_BUT_EXPORT ,		{CResizer::eFixed,			IDC_AN_LABEL1, CResizer::eRight},		//Left side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eTop},	//Top side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eRight},	//Right side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eTop}},	//Bottom side	

	{IDC_AN_BUT_EXIT1 ,			{CResizer::eFixed,			IDC_AN_BUT_EXPORT, CResizer::eRight},	//Left side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eTop},	//Top side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eRight},	//Right side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eTop}},	//Bottom side

	{IDC_AN_LIST_ITEMS1,		{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eLeft},	//Left side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eTop},	//Top side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eRight},	//Right side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eBottom}},//Bottom side

	{IDC_INFO_FILIGRAMME,		{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eLeft},	//Left side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eTop},	//Top side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eRight},	//Right side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eBottom}},//Bottom side

	{IDC_INFO_TEXT_FILIGRAMME,	{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eLeft},	//Left side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eTop},	//Top side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eRight},	//Right side
								{CResizer::eFixed,			IDC_AN_STATIC_BOX1, CResizer::eBottom}},//Bottom side
	};

	const int nSize = (int)(sizeof(s_bi)/sizeof(s_bi[0]));
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);
}

LRESULT CDlgSurlignage::OnGetListItem(WPARAM wParam, LPARAM lParam)
{
    //wParam is a handler to the list
    //Make sure message comes from list box
    ASSERT( (HWND)wParam == m_List.GetSafeHwnd() );

    //lParam is a pointer to the data that 
    //is needed for the element
	CQuickList::CListItemData * pdata = 
        (CQuickList::CListItemData *) lParam;

    //Get which item and subitem that is asked for.
    int item = pdata->GetItem();
    int subItem = pdata->GetSubItem();

	m_Items.MoveTo(item);
	if(m_Items.IsValid())
	{
		// Hide & stop Flash Info Filigramme
		m_InfoText.ShowWindow(SW_HIDE);
		m_InfoText.EnableWindow(FALSE);
		m_InfoText.FlashBackground(FALSE);
		m_InfoText.FlashText(FALSE);

		CString St, St2;

		CTableKey & Item = m_Items.GetItem();

		pdata->m_allowEdit = false;
		pdata->m_noSelection = false;

		pdata->m_colors.m_textColor = CATPColors::GetColorBlack();
		pdata->m_colors.m_backColor = CATPColors::GetColorLight(CATPColors::COLORANALYSEUR);
		if(item%2 != 0)
			pdata->m_colors.m_backColor = CATPColors::GetColorMedium(CATPColors::COLORANALYSEUR);

		pdata->m_colors.m_selectedTextColor	= CATPColors::GetColorWhite();
		pdata->m_colors.m_selectedBackColor	= CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR);
		pdata->m_colors.m_selectedBackColorNoFocus	= CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR);

//		pdata->m_textStyle.m_bold = true;
		switch(subItem)
		{
			case 0:
			{
				pdata->m_text.Format("%d",item+1); 
				pdata->m_tooltip = pdata->m_text;
			}
			break;

			case 1:
			{
				// Row 
				if(m_pIItemTable && m_pDoc)
				{
					m_pIItemTable->GetRowCellString(St, CTYPE_HEADER2, Item.m_Universe, Item.m_Row, m_SurlignageCalc);
					pdata->m_text = St; 
					pdata->m_tooltip = pdata->m_text;
					
				}
			}
			break;
			case 2:
			{
				// Col
				if(m_pIItemTable && m_pDoc)
				{
					m_pIItemTable->GetColCellString(St, CTYPE_HEADER2, Item.m_Universe, Item.m_Col, m_SurlignageCalc);
					pdata->m_text = St; 
					pdata->m_tooltip = pdata->m_text;
				}
			}
			break;
			case 3:
			{
				// Page
				if(m_pIItemTable && m_pDoc)
				{
					m_pIItemTable->GetPageCellString(St,St2,Item.m_Page);  
					pdata->m_text = St2; 
					pdata->m_tooltip = pdata->m_text;
				}
			}
			break;
			case 4:
			{
				if(m_pIItemTable && m_pDoc)
				{
					double Result	= 0.0;
					
					// Flag si division par zéro
					bool DivZero			= false;

					// Flag si marge d'erreur atteinte
					bool MargeErreurDepasse = false;

					// Indication nombre de cas
					double NbCas = 0.0;

					// Marge d'erreur
					double MargeErreur = 0.0;

					// Intervalle confiance
					double IntervalleConfiance = 0.0;

					if(m_pIItemTable->GetCellValue(Result, m_SurlignageCalc , Item.m_Row, Item.m_Col, Item.m_Universe, m_SurlignageStyle , Item.m_Page, m_pDoc->Get_BaseColItem(), m_pDoc->Get_BaseRowItem(), DivZero, NbCas, MargeErreurDepasse, MargeErreur,IntervalleConfiance))
						m_pIItemTable->GetFormattedResult(St, Result, m_SurlignageStyle, m_SurlignageCalc, Item.m_Row, Item.m_Col, m_pDoc->Get_BaseRowItem(), m_pDoc->Get_BaseColItem(), DivZero, NbCas, MargeErreurDepasse);

					pdata->m_text = St; 
					pdata->m_tooltip = pdata->m_text;
				}
//					pdata->m_progressBar.m_edge		= 0;
//					pdata->m_progressBar.m_maxvalue	= 100;
//					pdata->m_progressBar.m_value	= (int)((Item.m_Effective/Item.m_EffectiveTotal)*100.0);
//					pdata->m_progressBar.m_fillTextColor	= CATPColors::GetColorBlack();
//					pdata->m_progressBar.m_fillColor		= RGB(255,255,0);//CATPColors::GetColorSelect(CATPColors::COLORANALYSEUR);//CATPColors::GetColorRed();
			}
			break;
		}
	}
	else
	{
		pdata->m_allowEdit = false;
//		pdata->m_noSelection = false;
		pdata->m_text = _T("Empty");

		// Visible & active Flash Info Filigramme
		m_InfoText.ShowWindow(SW_SHOW);
		m_InfoText.EnableWindow(TRUE);
		m_InfoText.FlashBackground(FALSE);
		m_InfoText.FlashText(TRUE);

	}
    return 0;
}

LRESULT CDlgSurlignage::OnNcHitTest(CPoint point)
{
    UINT hit = CDialog::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}

// void CDlgSurlignage::SetButtons(CButtonST * pBut_Surlignage, CButtonST * pBut_Base100)
void CDlgSurlignage::SetButtons(CButtonST * pBut_Surlignage, BOOL Base100)
{
	m_Base100			= Base100;	
	m_pBut_Surlignage	= pBut_Surlignage;
}

void CDlgSurlignage::SetData(JVector<CTableKey> &Items, IItemTable * pIItemTable, RESULTTYPE SurlignageCalc, RESULTSTYLE SurlignageStyle, CAnalyseurDoc * pDoc)
{
	m_pDoc = pDoc;

	m_Items.Swap(Items);
	m_pIItemTable = pIItemTable;
	m_SurlignageCalc = SurlignageCalc;
	m_SurlignageStyle = SurlignageStyle;

	m_InfoText.FlashBackground(FALSE);
	m_InfoText.FlashText(TRUE);
	m_InfoText.ShowWindow(SW_SHOW);
	m_InfoText.EnableWindow(TRUE);

	m_List.SetItemCount(m_Items.GetCount()); 

	// Modifie entete colonne type de calcul
	CString TxtGrandeur = GrandeurCalcul();
	LVCOLUMN Colonne;
	Colonne.mask = LVCF_TEXT;
	Colonne.pszText = TxtGrandeur.GetBuffer();
	m_List.SetColumn(4,&Colonne); 

	// Mise à jour entete surlignage
	UpdateTitre();
}

///////////////////////////////////////////////////////////////////////////
// Mise à jour titre surlignage si fenêtre surlignage ouverte
void CDlgSurlignage::UpdateTitre()
{
	// Style global
	CString StyleGlobal = GetStrStyleCalcul();

	// Indicateur de calcul
	CString Indicateur = GetStrIndicateur();

	// Titre entete surlignage
	CString TxtTitre;

	/*
	if (StyleGlobal != "")
		TxtTitre.Format("STYLE GLOBAL : %s   -  INDICATEUR : %s",StyleGlobal, Indicateur);
	else
		TxtTitre.Format("INDICATEUR : %s",Indicateur);
	*/
	TxtTitre.Format("INDICATEUR : %s",Indicateur);

	m_Label.SetWindowText(TxtTitre);
}

////////////////////////////////////////////////////////////////////////////////////////////
// Procédure de tri ascendant et descendant
static int CALLBACK MyCompareProcUp(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
   CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
   CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
   CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);

   // Retour comparaison libellé
   return strcmp(strItem2, strItem1);
}

static int CALLBACK MyCompareProcDown(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
   CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
   CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
   CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);

   // Retour comparaison libellé
   return strcmp(strItem1, strItem2);
}

////////////////////////////////////////////////////////////////////////////////////
// 
void CDlgSurlignage::OnLvnColumnclickAnListItems1(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	if(m_Sort) 
		// m_List.SortItems(MyCompareProcUp,	(LPARAM) &m_List);
		m_List.SortItems(MyCompareProcUp,	(LPARAM) NULL);
		
		// ((CListCtrl *)GetDlgItem(IDC_AN_LIST_ITEMS1))->SortItems(MyCompareProcUp, (LPARAM)((CListCtrl *)GetDlgItem(IDC_AN_LIST_ITEMS1)));
	else
		m_List.SortItems(MyCompareProcDown,	(LPARAM) &m_List);
		// ((CListCtrl *)GetDlgItem(IDC_AN_LIST_ITEMS1))->SortItems(MyCompareProcDown, (LPARAM)((CListCtrl *)GetDlgItem(IDC_AN_LIST_ITEMS1)));
	*/

	// Inversion pour tri ultérieur
	m_Sort = !m_Sort;  
	
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	pResult = 0;
}


//////////////////////////////////////////////////////////////////////////////
// Renvoi la grandeur utilisée pour affichage lignes surlignages
CString CDlgSurlignage::GrandeurCalcul()
{
	// Type de calcul
	CString StrTypeCalc = "Valeur";
	switch (m_SurlignageCalc)
	{
		case RTYPE_EFFECTIVE_000:
			m_pDoc->GetGrandeurString(StrTypeCalc,RTYPE_EFFECTIVE_000);;
			break;
		case RTYPE_NBCAS:
			m_pDoc->GetGrandeurString(StrTypeCalc,RTYPE_NBCAS);
			break;
		case RTYPE_QUANTITY_000:
			m_pDoc->GetGrandeurString(StrTypeCalc,RTYPE_QUANTITY_000);
			break;
		case RTYPE_AVERAGEQUANTUM:
			m_pDoc->GetGrandeurString(StrTypeCalc,RTYPE_AVERAGEQUANTUM);
			break;
		case RTYPE_PENETRATION:
			StrTypeCalc = "Pénétration";
			break;
		case RTYPE_CENTRAGE:
			break;
		case RTYPE_EFFECTIVE_CIBLE:
			m_pDoc->GetGrandeurString(StrTypeCalc,RTYPE_EFFECTIVE_CIBLE);
			break;
		case RTYPE_CONCOMITENCE:
			break;
		case RTYPE_COEFFYULE:
			break;
		case RTYPE_EFFECTIVE_NONCIBLE:
			break;
		case RTYPE_AVERAGEQUANTUM_P00:
			m_pDoc->GetGrandeurString(StrTypeCalc,RTYPE_AVERAGEQUANTUM_P00);
			break;
		case RTYPE_ZSCORE:
			break;
	};

	// Retaillage des colonnes via largeur fenêtre
	RetailleColonne();
	
	return StrTypeCalc;
}


//////////////////////////////////////////////////////////////////////////////
// Indicateur de calcul
CString CDlgSurlignage::GetStrIndicateur()
{
	// Indicateur de calcul
	CString Indicateur = "";

	switch (m_SurlignageStyle) 
	{
		case RSTYLE_NOSTYLE:
		case RSTYLE_RESULT:
			// "Grandeur";
			Indicateur.LoadString(IDS_AN_LABEL_GRANDEUR);
			break;
		case RSTYLE_HORZ_STRUCTURE:
			// "H% Structure"
			Indicateur.LoadString(IDS_AN_LABEL_H_STRUCTURE);
			break;
		case RSTYLE_HORZ_CORRELATION:
			// "H% Corrélation"
			Indicateur.LoadString(IDS_AN_LABEL_H_CORRELATION);
			break;
		case RSTYLE_HORIZONTAL:
			// "H%"
			Indicateur.LoadString(IDS_AN_ABR_HORIZ);
			break;
		case RSTYLE_VERTICAL:
			// "V%"
			Indicateur.LoadString(IDS_AN_ABR_VERT);
			break;
		case RSTYLE_GBL_STRUCTURE:
			// "Indice structure"
			Indicateur.LoadString(IDS_AN_LABEL_INDICE_STRUCTURE);
			break;
		case RSTYLE_GBL_CORRELATION:
			// "Indice corrélation"
			Indicateur.LoadString(IDS_AN_LABEL_INDICE_CORRELATION);
			break;
		case RSTYLE_GBL_PROXIMITY:
			// "Indice proximité"
			Indicateur.LoadString(IDS_AN_LABEL_INDICE_PROXIMITE);
			break;
		case RSTYLE_GBL_ZSCORE:
			// "Zscore"
			Indicateur.LoadString(IDS_AN_ZSCORE);
			break;
		case RSTYLE_GLOBAL:
			// "Global"
			Indicateur.LoadString(IDS_AN_LABEL_GLOBAL);
			break;
		case RSTYLE_CIBLES:
			// "Cibles";
			Indicateur.LoadString(IDS_AN_LABEL_CIBLES);
			break;
	}
	return Indicateur;
}

//////////////////////////////////////////////////////////////////////////////
// Renvoi le style utilisé pour affichage lignes surlignages
CString CDlgSurlignage::GetStrStyleCalcul()
{
	// Indicateur de calcul
	CString StrStyleCalc = "";

	if (m_pDoc)
	{
		// Mode calcul global standard
		if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_CORRELATION)
		{
			StrStyleCalc.LoadString(IDS_AN_CORRELATION);
		}
		else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_PROXIMITY)
		{
			StrStyleCalc.LoadString(IDS_AN_PROXIMITY);
		}
		else if(m_pDoc->Get_ResultStyle() & RSTYLE_GBL_ZSCORE)
		{
			StrStyleCalc.LoadString(IDS_AN_ZSCORE);
		}
	}

	return StrStyleCalc;
}

////////////////////////////////////////////////////////////////////////////////
// // Retaillage des colonnes via largeur fenêtre
void CDlgSurlignage::RetailleColonne()
{
	m_Resizer.Move();
	if(IsWindow(m_List))
	{
		CRect rect;
		m_List.GetClientRect(&rect);
		int nColInterval = rect.Width()/24;
		m_List.SetColumnWidth(0,nColInterval);
		m_List.SetColumnWidth(1,nColInterval*6);
		m_List.SetColumnWidth(2,nColInterval*6);
		m_List.SetColumnWidth(3,nColInterval*6);
		m_List.SetColumnWidth(4,rect.Width()-(19*nColInterval));
	}
	Invalidate();
	UpdateWindow();
}

////////////////////////////////////////////////////////////////////////////////////////
// Export de la grille surlignage
void CDlgSurlignage::OnBnClickedAnButExport()
{
	CString txt;

	// Entete surlignage
	CString Descriptif = "";
	Descriptif = m_pDoc->HeaderTrisCroises();

	// Saut de ligne après entête
	CString Txt;
	Txt.Format(_T("\r\n\r\n"));
	Descriptif += Txt;

	// Balaye toutes les lignes surlignage
	for (JUnt32 InxLst = 0; InxLst < m_List.GetItemCount(); InxLst++)
	{
		// m_List.SetItemData(InxLst,InxLst);

		// Puis toutes les colonnes (0/ Pos 1/lib ligne, 2/lib col, 3/lib page 4/résultat)
		for (JUnt32 NoCol = 0; NoCol < 5; NoCol++)
		{
			Txt.Format(_T("%s\t"),m_List.GetItemText(InxLst, NoCol));
			Descriptif += Txt;
		}
		
		// Passe à la ligne suivante
		Txt.Format(_T("\r\n"));
		Descriptif += Txt;
	}

	// Ecriture dans clipboard pour prévisu et impression
	// Fichier temporaire clipboard
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	// Entete Information
	CString ClipString		= Descriptif;
	clipb.Write(ClipString.GetBuffer(),ClipString.GetLength());

	// Test opération Clipboard
	VERIFY(OpenClipboard());
	BOOL bEmpty = EmptyClipboard();
	VERIFY(CloseClipboard());

	// Lance la copie clipboard
	HGLOBAL hData = 0;
	HANDLE hResult = 0;
	hData = clipb.Detach();

	// Now, hand over memory block to clipboard
	if (bEmpty && hData)
	{
		VERIFY(OpenClipboard());
		hResult = SetClipboardData(CF_TEXT, hData);
		VERIFY(CloseClipboard());
	}
	
	/* Ecriture dans un fichier
	// Boite de dialogue demande nom fichier
	CFileDialog dlgFile(
		FALSE,
		_T(".xls"),
		NULL,
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Fichier Excel (*.xls)|*.xls|Fichier Texte(*.txt)|*.txt|"),this);

	if (dlgFile.DoModal( ) == IDCANCEL)
		return;

	CString Txt = dlgFile.GetFileExt();

	if(dlgFile.GetFileExt().CompareNoCase("xls") == 0)
	{
		// Ecriture dans clipboard pour prévisu et impression
		//if (Clipboard(DescriptifItem) && Impression == true)
		//{
		//	CExcelPrinter excelPrinter(CATPColors::COLORREPITEMS, this);
		// CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
		//
		//	// Compléter le MacroPresse pour affichage item
		//	path += "\\MacroPresse.xla";
		//	excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
		//}

		// Appel au clipboard grille surlignage
		if(CopyAllToClipboard())
		{
			CExcelPrinter excelPrinter(CATPColors::COLORANALYSEUR, this);
			CString path = AfxGetApp()->GetProfileString("", "InstallPath", 0);
			path += "\\MacroPresse.xla";
			excelPrinter.SaveAsFile(dlgFile.GetPathName( ), path, "DefinirTypeExportPresse");
		}
	}
	else
	{
		CWaitCursor wait;
		CStdioFile txtFile;

		try
		{
			// Export sous format texte les infos surilgnage
			txtFile.Open(dlgFile.GetPathName( ), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
			ExportFormatTexte(txtFile);
		}
		catch (CFileException *e)
		{
			CString txt;
			txt.Format("Fichier %s invalide!", dlgFile.GetFileName( ));
			MessageBox(txt);	
			return;
		}

		txtFile.Close();
	}
	*/
}

/////////////////////////////////////////////////////////////////////////////
// Export format texte
void CDlgSurlignage::ExportFormatTexte(CFile & exportFile)
{
	// Export de l'entête
	m_pDoc->ExportHeader((CFile *) &exportFile);

	// Export des lignes surlignages
	ExportLignes((CFile *) &exportFile);
}

/////////////////////////////////////////////////////////////////////////////
// Export des lignes dans fichier texte
void CDlgSurlignage::ExportLignes(CFile * pFile)
{
	CString txt;

	// Saut de ligne après entête
	txt.Format(_T("\r\n\r\n"));
	pFile->Write(txt.GetBuffer(),txt.GetLength());

	// Balaye toutes les lignes surlignage
	for (JUnt32 InxLst = 0; InxLst < m_List.GetItemCount(); InxLst++)
	{
		// Puis toutes les colonnes (0/ Pos 1/lib ligne, 2/lib col, 3/lib page 4/résultat)
		for (JUnt32 NoCol = 0; NoCol < 5; NoCol++)
		{
			txt.Format(_T("%s\t"),m_List.GetItemText(InxLst, NoCol));
			pFile->Write(txt.GetBuffer(),txt.GetLength());
		}
		
		// Passe à la ligne suivante
		txt.Format(_T("\r\n"));
		pFile->Write(txt.GetBuffer(),txt.GetLength());
	}
}

/////////////////////////////////////////////////////////////////////////////
// Export dans clipboard
bool CDlgSurlignage::CopyAllToClipboard()
{
	/*
	// Préparation du clipboard
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	if(Export(clipb))
	{

		VERIFY(m_pGrid->OpenClipboard());
		BOOL bEmpty = EmptyClipboard();
		VERIFY(CloseClipboard());

		HGLOBAL hData = 0;
		HANDLE hResult = 0;
		hData = clipb.Detach();

		// Now, hand over memory block to clipboard
		if (bEmpty && hData)
		{
			VERIFY(m_pGrid->OpenClipboard());
			hResult = SetClipboardData(CF_TEXT, hData);
			VERIFY(CloseClipboard());
			return true;
		}
	}
	*/

	return false;
}

// Exports the table to the given file
bool CDlgSurlignage::Export(CFile & exportFile)
{
	/*
	CProgressCtrl & Prog = m_pDoc->GetProgressCtrl(); 
	int Universe		= 0;
	CString TString		= _T(" ");
	UINT TableStyle		= m_pDoc->Get_TableStyle();
	bool bTerrainCol	= (TableStyle&TSTYLE_TERRAINCOL);
	bool bGrandeurCol	= (TableStyle&TSTYLE_GRANDEURCOL);
	int BaseColItem		= m_pDoc->Get_BaseColItem();
	int BaseRowItem		= m_pDoc->Get_BaseRowItem();
	const JList<RESULTTYPE> & CalcTypes   = m_pDoc->Get_Grandeurs();
	UINT ResultStyle	= m_pDoc->Get_ResultStyle(); 


	// Pour être sûr d'avoir les dernieres largeurs et hauteurs du grid
	if(m_pGrid->GetRowCount() >= 2)
	{
		// Hauteur des headers lignes
		m_pDoc->SetRowHdrHeight(0,m_pGrid->GetRowHeight(0));
		m_pDoc->SetRowHdrHeight(1,m_pGrid->GetRowHeight(1));
	}
	if(m_pGrid->GetColCount() >= 2)
	{
		// Largeur des headers colonnes
		m_pDoc->SetColHdrWidth(0,m_pGrid->GetColWidth(0));
		m_pDoc->SetColHdrWidth(1,m_pGrid->GetColWidth(1));
	}
	
	m_pDoc->ExportHeader((CFile *) &exportFile);

	// ???? Ici on utilisera cet espace inutilisé pour afficher titre du tableau
	TString.Format(_T("@TABLEAU@\t%s\r\n"), m_pDoc->GetTitle()); 

	exportFile.Write(TString.GetBuffer(), TString.GetLength());
	TString.Format(_T("@VERSION@\t1\r\n")); 
	exportFile.Write(TString.GetBuffer(), TString.GetLength());

	Prog.SetRange32(0,m_NbPages*m_NbCols*m_NbRows);
	Prog.SetStep(1);
	Prog.SetPos(0);
	for(int nPage = 0; nPage < m_NbPages; ++nPage)
	{
		int Pi = GetPageItem(nPage);
		CString PString; CString PString2;
		GetPageCellString(PString,PString2,Pi);

		TString.Format(_T("@PAGE@\t%s\t%s\n"),PString,PString2);
		exportFile.Write(TString.GetBuffer(), TString.GetLength());

		for(int nRow = 0; nRow < m_NbRows; ++nRow)
		{
			int Ri,Ru,Rc,Rv;
			CELLTYPE Rt			= GetRowCoordinates(nRow, Ri, Ru, Rc, Rv);
			bool bRtItemCell	= (Rt&CTYPE_ITEMCELL);
			CStringArray  RowString;
			CBitArray		bValidRow;
			CString ColString;

			if(!bGrandeurCol)
				CalcTypes.MoveTo(Rc);
			if(!bTerrainCol)
				Universe = Ru;

			if(bRtItemCell)
			{
				RowString.SetSize(m_Result_Col_Count);
				bValidRow.SetSize(m_Result_Col_Count);
				bValidRow.SetAll(true);
			}
			else
			{
				RowString.SetSize(1);
				bValidRow.SetSize(1);
				bValidRow.SetAll(true);
			}

			int CntFirstCell = RowString.GetSize();
			int vRow = 0;
			for(int nCol = 0; nCol < m_NbCols; ++nCol, ++vRow)
			{
				Prog.StepIt();
				int Ci,Cu,Cc,Cv;
				CELLTYPE Ct				= GetColCoordinates(nCol, Ci, Cu, Cc, Cv);
				bool bCtItemCell		= (Ct&CTYPE_ITEMCELL);

				if(bGrandeurCol)
					CalcTypes.MoveTo(Cc);
				if(bTerrainCol)
					Universe = Cu;


				ColString	= _T(" ");

				if(!bRtItemCell)
				{
					if(vRow >= m_Result_Col_Count)
					// Reset the virtual row
						vRow = 0;
					// Not a row item
					if(bCtItemCell)
					{
						if(CntFirstCell != 0)
						{
							// Row item cell
							ColString = _T("Indicateur");
							ColString.Append(_T("\t"));
							RowString[0].Append(ColString);
							--CntFirstCell;
							ColString	= _T(" ");
						}
						// Cell item
						if(vRow != 0)
							continue;

						GetColCellString(ColString, Rt, Universe, Ci, Cc);
					}
					else
					{
						if((Rt&CTYPE_HEADER1) != 0)
							ColString = PString;
					}

					ColString.Append(_T("\t"));
					RowString[0].Append(ColString);
				}
				else
				{
					// Row Item Cell
					if(bCtItemCell)
					{
						RESULTSTYLE ResStyle	= GetResultStyle(Rv, Cv);

						// Reset the virtual row
						if(vRow >= RowString.GetSize())
							vRow = 0;

						if(CntFirstCell != 0)
						{
							// Row header cell
							switch(ResStyle)
							{
								case RSTYLE_RESULT:
									ColString.Format("Grandeur");
								break;

								case RSTYLE_HORIZONTAL:
									// Spécif Advision (IC = IA / Indice affinité)
									if(m_pDoc->Get_ResultStyle()&RSTYLE_HORZ_CORRELATION)
										ColString.LoadString(IDS_AN_ABR_INDAFFINITE);
                                    else
										ColString.Format("H%%");
								break;

								case RSTYLE_VERTICAL:
									ColString.Format("V%%");
								break;

								case RSTYLE_GLOBAL:
								{
									if(m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_CORRELATION|RSTYLE_GBL_PROXIMITY|RSTYLE_GBL_ZSCORE))
									{
										// Affichage des abréviations grandeurs utilisées en Global
										if((m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_CORRELATION)) == RSTYLE_GBL_CORRELATION)
											ColString.LoadString(IDS_AN_ABR_CORRELATION);
										else if ((m_pDoc->Get_ResultStyle()&(RSTYLE_GBL_PROXIMITY)) == RSTYLE_GBL_PROXIMITY)
											ColString.LoadString(IDS_AN_ABR_PROXIMITY);
										else
											ColString.LoadString(IDS_AN_ABR_ZSCORE);
									}
									else
										ColString.LoadString(IDS_AN_ABR_GLOBAL);
								}
								break;
							}
							ColString.Append(_T("\t"));
							RowString[vRow].Append(ColString);
							--CntFirstCell;
							ColString	= _T(" ");
						}
						// Column item cell
						if((ResultStyle&ResStyle) != 0 && GetCellResults(Universe) != 0 && CalcTypes.IsValid())
						{
							RESULTTYPE Type	= CalcTypes.GetItem();
							double Result	= 0.0;

							// Flag si division par zéro
							bool DivZero			= false;

							// Flag si marge d'erreur atteinte
							bool MargeErreurDepasse = false;

							// Indication nombre de cas
							double NbCas = 0.0;

							// Marge d'erreur
							double MargeErreur = 0.0;

							// Intervalle confiance
							double IntervalleConfiance = 0.0;
							
							if(GetCellValue(Result, Type, Ri, Ci, Universe, ResStyle, Pi, BaseColItem, BaseRowItem, DivZero, NbCas, MargeErreurDepasse,MargeErreur,IntervalleConfiance))
								GetFormattedResult(ColString, Result, ResStyle, Type, Ri, Ci, BaseRowItem, BaseColItem, DivZero, NbCas, MargeErreurDepasse);
						}
						else
							bValidRow.SetAt(vRow,false);

						ColString.Append(_T("\t"));
						RowString[vRow].Append(ColString);
					}
					else
					{
						GetRowCellString(ColString, Ct, Universe, Ri, Rc);
						ColString.Append(_T("\t"));
						for(int x = 0; x < RowString.GetSize(); ++x) 
							RowString[x].Append(ColString);
					}
				}
			}
			for(int x = 0; x < RowString.GetSize(); ++x) 
			{
				if(bValidRow.GetAt(x))
				{
					RowString[x].Append(_T("\n")); 
					exportFile.Write(RowString[x].GetBuffer(), RowString[x].GetLength());
				}
			}
		}
		TString = _T("\n");
		exportFile.Write(TString.GetBuffer(), TString.GetLength());
	}
	Prog.SetPos(0);
	*/

	return true;
}


