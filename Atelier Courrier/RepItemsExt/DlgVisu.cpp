// DlgVisu.cpp : implementation file
//
#include "stdafx.h"
#include "DlgVisu.h"
#include "FormatStr.h"
#include "DlgItems.h"
#include "AbstractMode.h"
#include "ExcelPrinter.h"
#include <afxadv.h>            // For CSharedFile

#include "JFCConfig.h"

// CDlgVisu dialog
IMPLEMENT_DYNAMIC(CDlgVisu, CDialog)

//CDlgVisu::CDlgVisu(int ShowResults, CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgVisu::IDD, pParent)
//{
//	m_ShowResults = ShowResults;
//	m_pDlgItems		= pParent;
//}

CDlgVisu::CDlgVisu(int ShowResults, CDlgItems * pParent /*=NULL*/)
	: CDialog(CDlgVisu::IDD, (CWnd*)pParent)
{
	m_ShowResults   = ShowResults;
	m_pDlgItems		= pParent;
}

CDlgVisu::~CDlgVisu()
{
}

void CDlgVisu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// Message Maps
BEGIN_MESSAGE_MAP(CDlgVisu, CDialog)
	ON_BN_CLICKED(IDC_RI_BUT_EXIT, OnExit)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_QUICKLIST_GETLISTITEMDATA, OnGetListItem)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_RI_BTN_IMPITEM, OnBnClickedRiBtnImpitem)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_RI_LIST_ITEMS, OnLvnItemchangedRiListItems)
END_MESSAGE_MAP()


// CDlgVisu message handlers

void CDlgVisu::OnExit()
{
	ShowWindow(SW_HIDE);
	GetParent()->PostMessage(WM_USER_VISU_CLOSE,0,0);
}

BOOL CDlgVisu::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetupControls();
	m_List.SetItemCount(0); 

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgVisu::OnSysColorChange()
{
	CDialog::OnSysColorChange();
	SetColors(); 
}

void CDlgVisu::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	m_Resizer.Move();
	if(IsWindow(m_List))
	{
		CRect rect;
		m_List.GetClientRect(&rect);
		int nColInterval = rect.Width()/9;
		m_List.SetColumnWidth(0,nColInterval*5);
		m_List.SetColumnWidth(1,nColInterval*2);
		m_List.SetColumnWidth(2,rect.Width()-(7*nColInterval));
	}
	Invalidate();
	UpdateWindow();
}

void CDlgVisu::SetupControls()
{
	m_Box.				SubclassDlgItem(IDC_RI_STATIC_BOX, this);
	m_Label.			SubclassDlgItem(IDC_RI_LABEL, this);
	m_List.				SubclassDlgItem(IDC_RI_LIST_ITEMS, this);
	m_But_Exit.			SubclassDlgItem(IDC_RI_BUT_EXIT, this);
	m_But_Imp.			SubclassDlgItem(IDC_RI_BTN_IMPITEM, this);

	CRect rect;
	m_List.GetClientRect(&rect);
	int nColInterval = rect.Width()/9;
	m_List.InsertColumn(0, _T("Items composants"), LVCFMT_LEFT, nColInterval*5);
	m_List.InsertColumn(1, _T("Nb Cas"), LVCFMT_RIGHT, nColInterval*2);
	m_List.InsertColumn(2, _T("Eff(000)"), LVCFMT_RIGHT, rect.Width()-(7*nColInterval));
	m_List.EnableToolTips(true);
//	m_List.SetToolTips(&m_ToolTip);

	m_Label.SetFontName(CATPColors::GetFontLabelName());
	m_Label.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label.SetBorder(false);

	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_Label);
	m_Box.AddTopWindow(m_List);
	m_Box.AddTopWindow(m_But_Exit);
	m_Box.AddTopWindow(m_But_Imp);
	
	// Set the windows colors
	SetColors();
	// Set window relations and positions for resizing
	SetWindowRelations();
}

void CDlgVisu::SetColors()
{
	m_Box.			SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORREPITEMS),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_But_Exit.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORREPITEMS),CATPColors::COLORREPITEMS, 0);
	m_But_Exit.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORREPITEMS),CATPColors::COLORREPITEMS);
	m_But_Exit.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORREPITEMS),CATPColors::COLORREPITEMS);

	m_But_Imp.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(CATPColors::COLORREPITEMS),CATPColors::COLORREPITEMS,0);
	m_But_Imp.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(CATPColors::COLORREPITEMS),CATPColors::COLORREPITEMS);
	m_But_Imp.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORREPITEMS),CATPColors::COLORREPITEMS);

	m_Label.		SetBkColor(CATPColors::GetColorDark(CATPColors::COLORREPITEMS),CATPColors::COLORREPITEMS, CATPColors::DARK_DEGRAD);
	m_Label.		SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORREPITEMS)); 
}

void CDlgVisu::SetWindowRelations()
{
	// Set all widow relation positions
	static CResizer::CBorderInfo s_bi[] = {

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////  CONFIG BOX 
//////////////////////////////////////////////////////////////////////////////////////////////////////

	{IDC_RI_STATIC_BOX,			{CResizer::eFixed,			IDC_MAIN, CResizer::eLeft},				//Left side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eTop},				//Top side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eRight},			//Right side
								{CResizer::eFixed,			IDC_MAIN, CResizer::eBottom}},			//Bottom side

	{IDC_RI_LABEL,				{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eLeft},	//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop},		//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},	//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop}},	//Bottom side

	{IDC_RI_BUT_EXIT  ,			{CResizer::eFixed,			IDC_RI_LABEL, CResizer::eRight},		//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop},		//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},	//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop}},	//Bottom side

	{IDC_RI_LIST_ITEMS,			{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eLeft},	//Left side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eTop},		//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},	//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eBottom}}, //Bottom side


	{IDC_RI_BTN_IMPITEM,		{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eLeft},	//Left side
								{CResizer::eFixed,			IDC_RI_LIST_ITEMS, CResizer::eBottom},		//Top side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eRight},	//Right side
								{CResizer::eFixed,			IDC_RI_STATIC_BOX, CResizer::eBottom}}, //Bottom side

	};

	const int nSize = sizeof(s_bi)/sizeof(s_bi[0]);
	m_Resizer.Init(m_hWnd, NULL, s_bi, nSize);
}

LRESULT CDlgVisu::OnGetListItem(WPARAM wParam, LPARAM lParam)
{
	// En attente >> Impression possible ou pas (à revoir car dans certains cas item en construction)
	if (ImpressionPossible())
		m_But_Imp.EnableWindow(TRUE);
	else
		m_But_Imp.EnableWindow(FALSE);

	CString Fmt = _T("### ### ### ### ###");
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
		CVisuItems & Item = m_Items.GetItem();

		pdata->m_allowEdit = false;
		pdata->m_noSelection = true;

		BYTE red	= 0;
		BYTE green	= 0;
		BYTE blue	= 0;

		switch(Item.m_Level)
		{
			case 0:
			{
				COLORREF clr = CATPColors::GetColorWhite();
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
			}
			break;

			case 1:
			{
				COLORREF clr = CATPColors::GetColorLight(CATPColors::COLORREPITEMS);
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
			}
			break;

			case 2:
			{
				pdata->m_textStyle.m_bold = true;
				pdata->m_colors.m_textColor = CATPColors::GetColorWhite();
				COLORREF clr = CATPColors::GetColorSelect(CATPColors::COLORREPITEMS);
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
			}
			break;

			case 3:
			{
				pdata->m_textStyle.m_bold = true;
				pdata->m_colors.m_textColor = CATPColors::GetColorWhite();
				COLORREF clr = CATPColors::GetColorDark(CATPColors::COLORREPITEMS);
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
			}
            break;

			case 4:
			{
				pdata->m_colors.m_textColor = CATPColors::GetColorWhite();
				COLORREF clr = CATPColors::GetColorMedium(CATPColors::COLORREPITEMS);
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
			}
			break;

			default:
			{
				pdata->m_colors.m_textColor = CATPColors::GetColorWhite();
				COLORREF clr = CATPColors::GetColorMedium(CATPColors::COLORREPITEMS);
				red		= GetRValue(clr);
				green	= GetGValue(clr);
				blue	= GetBValue(clr);
				red		+= (Item.m_Level-4)*15 ;
				green	+= (Item.m_Level-4)*15; 
				blue	+= (Item.m_Level-4)*15; 
			}
			break;
		}

		switch(subItem)
		{
			case 1:
			{
				if(Item.m_NbCas >= 0)
				{
					if (m_ShowResults & SHOW_NBCAS)
						pdata->m_text.Format("%d",Item.m_NbCas);
					else
						pdata->m_text.Format("n/a");
					CFormatStr::FormatNum(Fmt,pdata->m_text);  
					pdata->m_text+= _T("  ");
					if (m_ShowResults & SHOW_NBCAS)
						pdata->m_tooltip.Format("%d",Item.m_NbCas);
					else
						pdata->m_tooltip.Format("n/a");
					CFormatStr::FormatNum(Fmt,pdata->m_tooltip);  
				}
			}
			break;

			case 2:
			{
				if(Item.m_NbCas >= 0)
				{

					if (m_ShowResults & SHOW_EFFECTIVE)
						pdata->m_text.Format("%0.0f",Item.m_Effective/1000.0);
					else
						pdata->m_text.Format("n/a");
					CFormatStr::FormatNum(Fmt,pdata->m_text);  
					pdata->m_text+= _T("  ");
					if (m_ShowResults & SHOW_EFFECTIVE)
						pdata->m_tooltip.Format("%0.0f",Item.m_Effective);
					else
						pdata->m_tooltip.Format("n/a");
					CFormatStr::FormatNum(Fmt,pdata->m_tooltip);  
					pdata->m_progressBar.m_edge		= 0;
					pdata->m_progressBar.m_maxvalue	= 100;
					pdata->m_progressBar.m_value	= (int)((Item.m_Effective/Item.m_EffectiveTotal)*100.0);
					pdata->m_progressBar.m_fillTextColor	= CATPColors::GetColorBlack();
					pdata->m_progressBar.m_fillColor		= RGB(255,255,0);//CATPColors::GetColorSelect(CATPColors::COLORREPITEMS);//CATPColors::GetColorRed();
				}
			}
			break;

			default:
			{
				pdata->m_text = Item.m_Label;
				pdata->m_tooltip = Item.m_Label;
			}
			break;
		}
		pdata->m_colors.m_backColor = RGB(red,green,blue);
	}
	else
	{
		pdata->m_allowEdit = false;
		pdata->m_noSelection = true;
		pdata->m_text = _T("Empty");
	}
    return 0;
}

LRESULT CDlgVisu::OnNcHitTest(CPoint point)
{
    UINT hit = CDialog::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}

void CDlgVisu::SetData(JList<CVisuItems> &Items)
{
	m_Items.Swap(Items);
	m_List.SetItemCount(m_Items.GetCount()); 
}

/////////////////////////////////// EXPORT DANS UN FICHIER ////////////////////////////////////
//
// Export Construction Item Echelle dans un fichier
void CDlgVisu::OnExportItem(JList<JInt32> &Values)
{
	// Texte final descriptif item
	CString DescriptifItem = "";

	// Export de l'entete info
	ExportEntete(Values,DescriptifItem);

	// Export des composants items
	ExportItem(DescriptifItem);

	// A VOIR si on le laisse !!!
	// Puis on inscrit les informations dans un fichier texte
	CStdioFile ExportFile;
	CFileException Error = 0;
	CString path = GETPROFILESTRINGD(SECTION_DEFAULT, KEY_INSTALL_PATH, 0);
	path += "\\TempTarget.txt";
	if(!ExportFile.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::typeText, &Error))
		return;
	ExportFile.WriteString(DescriptifItem);
	ExportFile.Close();

	// Ecriture dans clipboard pour prévisu et impression
	if (Clipboard(DescriptifItem))
	{
		CExcelPrinter excelPrinter(CATPColors::COLORREPITEMS, this);
		CString path = GETPROFILESTRINGD(SECTION_DEFAULT, KEY_INSTALL_PATH, 0);

		// Compléter le MacroPresse pour affichage item
		path += "\\MacroPresse.xla";
		excelPrinter.PrintPreview( path, "DefinirTypeExportPresse");
	}
}

// Passe le descriptif item dans le clipboard
bool CDlgVisu::Clipboard(CString DescriptifItem)
{
	// Fichier temporaire clipboard
	CSharedFile clipb (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

	// Entete Information
	CString ClipString		= DescriptifItem;
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
	return true;
}

// Impression possible ou pas 
bool CDlgVisu::ImpressionPossible()
{
	// Les valeurs
	JList<JInt32> Values;
	m_pDlgItems->m_List_Items.GetSelectedID(Values);

	// Sont-elles valides
	Values.MoveFirst();
	if (!Values.IsValid()) return false; 

	return true;
}

// Bouton impression structure de l'item courant
void CDlgVisu::OnBnClickedRiBtnImpitem()
{
	// A FAIRE
	JList<CVisuItems> Item;
	SetData(Item);

	JList<JInt32> Values;
	m_pDlgItems->m_List_Items.GetSelectedID(Values);
	JList<JInt32> ItemValues;

	// for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	Values.MoveFirst();
	if (!Values.IsValid()) return; 
	

	IItem *pItem = m_pDlgItems->GetItems()->GetIItemByID(Values.GetItem());
	if(pItem && pItem->IsItemConstruit())
		ItemValues.AddTail() = Values.GetItem();
	else
	{
		JList<JInt32> Items;
		if(pItem->IsPalette())
			pItem->AsPalette()->GetItems(Items);
		else if(pItem->IsCritereModifie())
			pItem->AsCritereModifie()->GetItems(Items);
		else if(pItem->IsCritereQuantile())
			pItem->AsCritereQuantile()->GetItems(Items);
		else if(pItem->IsQuestionInduite())
			pItem->AsQuestionInduite()->GetItems(Items);
		
		for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
			ItemValues.AddTail() = Items.GetItem();
	}

	// Export descriptif item
	OnExportItem(ItemValues);
}

// Export de l'entete création d'un item
void CDlgVisu::ExportEntete(JList<JInt32> &Values, CString &TxtMacro)
{
	// Init texte 
	TxtMacro = "";
	CString Text;

	// Titre de la macro
	Text.Format("%s\n","EXPORT ITEM");
	TxtMacro += Text;

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		// Récupère l'item
		CItemConstruit * pItem = m_pDlgItems->GetItems()->GetItemConstruit(Values.GetItem());

		// Item non valide
		if(!pItem || !pItem->IsValid(m_pDlgItems->GetTerrain()))
			continue;

		// Source
		CString Tmp;
		m_pDlgItems->m_Label_Source.GetWindowText(Tmp);
		Text.Format("%s\t%s\n","Source : ", Tmp);
		TxtMacro += Text;

		// Poids
		const JSRCTables & srcTables = JSRCTables::GetInstance(); 

		// All segments
		const JVector<JInt32> & Segments= m_pDlgItems->GetSegments();
		for(Segments.MoveFirst(); Segments.IsValid(); Segments.MoveNext())
		{
			const JSegment* Seg = srcTables.m_TBLSegments.GetItem(Segments.GetItem());
			Text.Format("%s\t%s\n","Segment de population : ",Seg->m_Libelle.AsJCharPtr()); 
			TxtMacro += Text;
		}
		
		// Type de Poids
		JChar * pLibelle = srcTables.m_TBLPoids.GetLabelByID(m_pDlgItems->GetTypePoids());
		Text.Format("%s\t%s\n","Type de poids : ",pLibelle);
		TxtMacro += Text;		

		// Nom composant
		Text.Format("%s\t%s\n","Libellé de l'item : ",pItem->GetLabel().AsJCharPtr());
		TxtMacro += Text;
		
		// Calcul pour la cible
		CCalcItemParams Params;
		Params.SetTerrain(m_pDlgItems->GetTerrain());
		Params.SetEchantillonID(1);
		Params.SetUniversID(m_pDlgItems->GetTerrain()->GetDefaultUnivers());
		Params.SetTypePoidsID(m_pDlgItems->GetTypePoids());
		Params.SetPopulationSegment(m_pDlgItems->GetPopulationSegment());
		Params.SetSegmentsIDs(m_pDlgItems->GetSegments());
		pItem->CalculateEffectif(&Params); 
		
		// Effectif Cible
		if(m_pDlgItems->GetShowResults()&SHOW_EFFECTIVE)
			Text.Format("%s\t%0.f\n","Effectif : ",Params.GetEffectif());
		else
			Text = "\n";
		TxtMacro += Text;

		// Pourcentage cibleXML percent tag
		if(m_pDlgItems->GetShowResults()&SHOW_PERCENT)
			Text.Format("%s\t%0.2f%%\n","Pourcentage : ",(Params.GetEffectif()/Params.GetEffectifTot())*100.0);
		else
			Text = "\n";
		TxtMacro += Text;
		
		// Nb Cas Cible
		if(m_pDlgItems->GetShowResults()&SHOW_NBCAS)
			Text.Format("%s\t%d\n","Nombre de cas : ",Params.GetNbCas());
		else
			Text = "\n";
		TxtMacro += Text;
	}

	// Saut de paragraphe
	TxtMacro += "\n";
}

// Export des composants de  l'item
void CDlgVisu::ExportItem(CString &TxtMacro)
{
	// Récupération des élmts constituant l'item sous forme de texte
	TxtMacro += m_pDlgItems->GetCurMode()->GetVisualisation();  

	// Puis on remet la visualisation normale
	m_pDlgItems->GetCurMode()->DoVisualisation();  
}

void CDlgVisu::OnLvnItemchangedRiListItems(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	*pResult = 0;
}
