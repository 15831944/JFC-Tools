#include "StdAfx.h"
#include "PaletteMode.h"

#include "DlgItems.h"

#include "SerialisePaletteBuilder.h"
#include "Convert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPaletteMode::CPaletteMode(CDlgItems *pDlgItems) : CItemMode(pDlgItems)
{
	m_CurMode = 0;
	m_Map_PaletteBuilder.Reset();
	m_CurPaletteBuilder .Reset();
}

CPaletteMode::~CPaletteMode(void)
{
}

void CPaletteMode::StartMode()
{
	EnableOperators(TRUE);

	// Validatiion et validité des différents boutons dialog Items
	m_pDlgItem->m_Button_Segment_OU		.EnableWindow(FALSE);
	m_pDlgItem->m_Button_Segment_ET		.EnableWindow(FALSE);
	// m_pDlgItem->m_Button_Filtre			.EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_UP		.EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_DOWN		.EnableWindow(FALSE);
	m_pDlgItem->m_Tab_Segment			.EnableWindow(FALSE);
	m_pDlgItem->m_Button_Genese			.EnableWindow(FALSE);
	m_pDlgItem->m_Button_List2			.EnableWindow(TRUE);
	m_pDlgItem->m_Button_Accept			.EnableWindow(FALSE);
	m_pDlgItem->m_Button_CalcSegment	.EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_OK       .ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Dimension1    .ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Dimension2    .ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Questions_Tous.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Header_Dimension1    .ShowWindow(SW_SHOW);
	m_pDlgItem->m_Header_Dimension2    .ShowWindow(SW_SHOW);
	m_pDlgItem->m_Button_Genese        .ShowWindow(SW_SHOW);
	m_pDlgItem->m_Button_List2         .ShowWindow(SW_SHOW);

	CString Label;
	CString ToolTip;

	SetModeTitle();

	LOADSTRING(Label, IDS_RI_ITEMS); 
	LOADSTRING(ToolTip, IDS_RI_TT_ITEMS); 
	m_pDlgItem->m_Button_List1.SetWindowText(Label);
	m_pDlgItem->m_Button_List1.SetTooltipText(ToolTip);

	LOADSTRING(Label, IDS_RI_PALETTES); 
	LOADSTRING(ToolTip, IDS_RI_TT_PALETTE); 
	m_pDlgItem->m_Button_List2.SetWindowText(Label);
	m_pDlgItem->m_Button_List2.SetTooltipText(ToolTip);

	LOADSTRING(ToolTip, IDS_RI_TT_ET_Q_PALETTE); 
	m_pDlgItem->m_Button_SAS_ET.SetTooltipText(ToolTip);

	LOADSTRING(ToolTip, IDS_RI_TT_OU_Q_PALETTE); 
	m_pDlgItem->m_Button_SAS_OU.SetTooltipText(ToolTip);

	LOADSTRING(ToolTip, IDS_RI_TT_VALID_PALETTE); 
	m_pDlgItem->m_Button_Accept.SetTooltipText(ToolTip);

	m_Map_PaletteBuilder.Reset();
	m_CurPaletteBuilder .Reset();

	m_Dim_Sel.Reset();

	m_pDlgItem->CheckRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2,IDC_RI_RAD_QUESTIONS);
	
	UpdateTheme(IDC_RI_RAD_QUESTIONS);
	UpdateListItems();

	m_IBuilder.Reset();
	m_IBuilder.SetOperator(IBloc::OPERATOR ::Or);
	SetSegmentOperator(IBloc::OPERATOR ::Or);
	FindSet_IBsegment(0);
	SetQuestionOperator(IBloc::Or);  
	m_pIBsegment->SetOperator(IBloc::OPERATOR ::Or);
	m_pDlgItem->m_List_Questions.SetCurSel(LB_ERR); 
	OnSelchange_ListQuestions();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Récupération du mode en cours
void CPaletteMode::RecupMode()
{

	EnableOperators(TRUE);

	m_pDlgItem->m_Button_Segment_OU.EnableWindow(FALSE);
	m_pDlgItem->m_Button_Segment_ET.EnableWindow(FALSE);

	// m_pDlgItem->m_Button_Filtre			.EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_UP		.EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_DOWN		.EnableWindow(FALSE);
	m_pDlgItem->m_Tab_Segment			.EnableWindow(FALSE);
	m_pDlgItem->m_Button_Genese			.EnableWindow(FALSE);
	m_pDlgItem->m_Button_List2			.EnableWindow(TRUE);
	m_pDlgItem->m_Button_Accept			.EnableWindow(FALSE);
	m_pDlgItem->m_Button_CalcSegment	.EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_OK		.ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Dimension1		.ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Dimension2		.ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Questions_Tous	.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Header_Dimension1		.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Header_Dimension2		.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Button_Genese			.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Button_List2			.ShowWindow(SW_SHOW);

	CString Label;
	CString ToolTip;

	SetModeTitle();

	LOADSTRING(Label, IDS_RI_ITEMS); 
	LOADSTRING(ToolTip, IDS_RI_TT_ITEMS); 
	m_pDlgItem->m_Button_List1.SetWindowText(Label);
	m_pDlgItem->m_Button_List1.SetTooltipText(ToolTip);

	LOADSTRING(Label, IDS_RI_PALETTES); 
	LOADSTRING(ToolTip, IDS_RI_TT_PALETTE); 
	m_pDlgItem->m_Button_List2.SetWindowText(Label);
	m_pDlgItem->m_Button_List2.SetTooltipText(ToolTip);

	LOADSTRING(ToolTip, IDS_RI_TT_ET_Q_PALETTE); 
	m_pDlgItem->m_Button_SAS_ET.SetTooltipText(ToolTip);

	LOADSTRING(ToolTip, IDS_RI_TT_OU_Q_PALETTE); 
	m_pDlgItem->m_Button_SAS_OU.SetTooltipText(ToolTip);

	LOADSTRING(ToolTip, IDS_RI_TT_VALID_PALETTE); 
	m_pDlgItem->m_Button_Accept.SetTooltipText(ToolTip);

	m_pDlgItem->CheckRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2,IDC_RI_RAD_QUESTIONS);

	UpdateTheme(IDC_RI_RAD_QUESTIONS);
	UpdateListItems();

	//////////////////////////////////// Remise en place des opérateurs ///////////////////////////
	// Positionne le 1er segment
	FindSet_IBsegment(0, false, false);
}

// Displays the selected theme
void CPaletteMode::UpdateTheme(int ID)
{
	CString Label;

	// Récup texte en cours
	CString ExLabel;
	m_pDlgItem->m_Label_Theme.GetWindowText(ExLabel);

	switch(ID)
	{
		case IDC_RI_RAD_QUESTIONS :
			LOADSTRING(Label, IDS_RI_INDEX_THEMATIQUE);
			m_pDlgItem->m_Tree_Rubrique_Items	.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE);
			m_pDlgItem->m_List_Items			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_Genese			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_RAZ_search		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Edit_Search			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Theme			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Button_QFilter		.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Button_QFilterRaz		.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Edit_QFilter			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Tree_Theme			.Invalidate();
		break;

		case IDC_RI_RAD_LIST_1:
			LOADSTRING(Label, IDS_RI_ITEMS_CONST);
			m_pDlgItem->m_Tree_Theme			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilter		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilterRaz		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Edit_QFilter			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Items	.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE);
			m_pDlgItem->ResizeListItems(true);
			m_pDlgItem->m_List_Items			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Button_Genese			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Button_RAZ_search		.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Edit_Search			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Tree_Rubrique_Items	.Invalidate();
			m_pDlgItem->m_Tree_Rubrique_Palettes.Invalidate();
			m_pDlgItem->m_List_Items			.Invalidate();
			UpdateListItems();
		break;

		case IDC_RI_RAD_LIST_2:
			LOADSTRING(Label, IDS_RI_PALETTES);
			m_pDlgItem->m_Tree_Theme			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilter		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilterRaz		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Edit_QFilter			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Items	.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Palettes.ShowWindow(SW_SHOW);
			// m_pDlgItem->ResizeListItems(false);
			m_pDlgItem->ResizeListItems(true);
			m_pDlgItem->m_List_Items			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Button_Genese			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Button_RAZ_search		.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Edit_Search			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Tree_Rubrique_Items	.Invalidate();
			m_pDlgItem->m_Tree_Rubrique_Palettes.Invalidate();
			m_pDlgItem->m_List_Items			.Invalidate();
			UpdateListItems();
		break;
	}

	if (Label != ExLabel)
	{
		m_pDlgItem->m_Label_Theme.SetWindowText(Label);
		m_pDlgItem->m_Label_Theme.Invalidate();
	}
}

void CPaletteMode::UpdateListItems()
{
	switch(m_pDlgItem->GetCheckedRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2))
	{
		case IDC_RI_RAD_LIST_1:
		{
			CItemMode::UpdateListItems();
		}
		break;

		case IDC_RI_RAD_LIST_2:
		{
			JList<IItem const *> vItems;
			JList<IQuestion const *> vInduite;
			m_pDlgItem->m_List_Items.ResetContent(); 
			m_pDlgItem->GetItems()->GetPalettes(vItems, m_pDlgItem->GetTerrain());
			m_pDlgItem->GetItems()->GetQuestionsInduiteByBlocID(vInduite, 0, m_pDlgItem->GetTerrain());
			for (vInduite.MoveFirst(); vInduite.IsValid(); vInduite.MoveNext())
			{
				if(vInduite.GetItem()->GetBlocID() == 0)
					vItems.AddTail() = vInduite.GetItem()->AsQuestionInduite()->AsIItem();  
			}
			// Get only those valid for this terrain
			for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
			{
				if (IsSearched(vItems.GetItem()))
				{
					// Ident de la palette
					JInt32 IdPalette = vItems.GetItem()->GetID();

					// Il faut de plus que la palette soit visualisable (sélectionné dans une rubrique palette)
					if (m_pDlgItem->ShowPalette(IdPalette))
					{
						// On peut l'ajouter à la liste des palettes
						long Pos = m_pDlgItem->m_List_Items.AddItem(vItems.GetItem());
						m_Selected_UserItems.MoveTo(vItems.GetItem()->GetID());
						if(m_Selected_UserItems.IsValid())
							m_pDlgItem->m_List_Items.SetSel(Pos, TRUE);
					}
				}
			}
			m_pDlgItem->m_Button_Genese.EnableWindow(FALSE);
			m_pDlgItem->m_List_Items.Invalidate();
		}
		break;
	}
}

// List Items selection changed
void CPaletteMode::OnSelchange_ListItems()
{
	switch(m_pDlgItem->GetCheckedRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2))
	{
		case IDC_RI_RAD_LIST_1:
		{
			CItemMode::OnSelchange_ListItems();
		}
		break;
		case IDC_RI_RAD_LIST_2:
		{
			m_pDlgItem->m_Button_Genese.EnableWindow(FALSE);
			// Get selected IDs
			JList<JInt32> Values;
			m_pDlgItem->m_List_Items.GetSelectedID(Values);
			// Get the Palettes
			JList<JInt32> NewValues;
			JMap<JInt32, JBool> Selected_UserItems;
			for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
			{
				m_Selected_UserItems.MoveTo(Values.GetItem());
				if(!m_Selected_UserItems.IsValid())
					NewValues.AddTail() = Values.GetItem();
				Selected_UserItems.MoveTo(Values.GetItem());
				if(!Selected_UserItems.IsValid())
					Selected_UserItems.Add(Values.GetItem()) = true;
			}
			m_Selected_UserItems.Swap(Selected_UserItems);
			if(m_pIBsegment)
			{
				JList<JInt32> vItems;
                m_pIBsegment->GetItemsID(vItems);
				for(vItems.MoveFirst();vItems.IsValid();vItems.MoveNext()) {
					if(m_pDlgItem->GetItems()->GetPalette(vItems.GetItem()))
					{
						m_Selected_UserItems.MoveTo(vItems.GetItem());
						if(!m_Selected_UserItems.IsValid())
						{
							m_Map_PaletteBuilder.MoveTo(vItems.GetItem());
							if(m_Map_PaletteBuilder.IsValid())
								m_Map_PaletteBuilder.Remove();
                            CIBItem* pIBItem = m_pIBsegment->FindItem(vItems.GetItem());
							if(m_pIBitem == pIBItem)
								m_pIBitem = 0;
							m_pIBsegment->DeleteItem(pIBItem);
						}
					}
				}
			}
			UpdateQuestionList();
			JInt32 ID = m_pDlgItem->m_List_Questions.GetSelectedID();
			NewValues.MoveFirst();
			if(ID!=LB_ERR) {
				if(NewValues.IsValid() && !m_pDlgItem->GetItems()->GetPalette(ID))
					m_pDlgItem->m_List_Questions.SetSelectedID(NewValues.GetItem());
			}
			else {
				if(NewValues.IsValid())
					m_pDlgItem->m_List_Questions.SetSelectedID(NewValues.GetItem());
			}
			SetActiveSegmentQuestions();
			OnSelchange_ListQuestions();
			UpdateIBuilder(true, false);
		}
		break;
	}
}

void CPaletteMode::StopMode()
{
	m_Map_PaletteBuilder.Reset();
	m_CurPaletteBuilder .Reset();
	CItemMode::StopMode();
}

void CPaletteMode::UpdateFiltreDimension1List()
{
	CItemMode::UpdateFiltreDimension1List();
}

/// Update the Modalite list box
void CPaletteMode::UpdateDimensionsList()
{
	m_pDlgItem->m_Button_Dimension1.ShowWindow(SW_HIDE);
	m_pDlgItem->m_Header_Dimension1.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Button_Dimension2.ShowWindow(SW_HIDE);
	m_pDlgItem->m_Header_Dimension2.ShowWindow(SW_SHOW);

	CItemMode::UpdateDimensionsList();

	for(int Dim=0; Dim < m_DimType_New.GetCount() ; ++Dim)
	{
		if(Dim==0)
		{
			// Get the first dimension
			m_pDlgItem->m_Button_Dimension1.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Header_Dimension1.ShowWindow(SW_HIDE);
		}
		else if(Dim==1)
		{
			// Get the second dimension
			m_pDlgItem->m_Button_Dimension2.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Header_Dimension2.ShowWindow(SW_HIDE);
			m_DimDlg.MoveFirst();
		}
		else
		{
			CDimensionDlg & Dlg = m_DimDlg.GetItem();
			Dlg.SetDimMode(true,-1,GetDimensionModeString(m_CurMode));
			m_DimDlg.MoveNext();
		}
	}
}

// Updates the Values from values dlg
bool CPaletteMode::UpdateItemValues(bool bValues)
{
	bool Ret = CItemMode::UpdateItemValues(bValues);

	if (m_pValuesDlg)
	{
		JUnt32 NbVal  = m_CurPaletteBuilder.GetNumValues().GetCount();
		m_pValuesDlg->SetPaletteMode(m_CurPaletteBuilder.GetNumValues().GetCount() > 0, GetDimensionModeString(m_CurMode));
	}

	return (Ret);
}

void CPaletteMode::OnSelchange_ListValue()
{
	JList<JFlt32> NumValues;
	if (m_pValuesDlg && m_pValuesDlg->HasPalValue())
		m_pValuesDlg->GetPalValues(NumValues);
	m_CurPaletteBuilder.SetNumValues(NumValues);
	CItemMode::OnSelchange_ListValue();
}

void CPaletteMode::OnChange_Value_PaletteSel()
{
	JList<JFlt32> NumValues;
	if (m_pValuesDlg && m_pValuesDlg->HasPalValue())
		m_pValuesDlg->GetPalValues(NumValues);
	m_CurPaletteBuilder.SetNumValues(NumValues);
	CItemMode::OnChange_Value_PaletteSel();
}

void CPaletteMode::QuestionChanged(IQuestion * pCurQuestion)
{
	m_CurPaletteBuilder.Reset();

	// Update selected question
	if(pCurQuestion)
	{
		// Si question déjà dals le map palette construite, on récupère toutre les infos dans m_CurPaletteBuider
		m_Map_PaletteBuilder.MoveTo(pCurQuestion->GetID());
		if(m_Map_PaletteBuilder.IsValid())
			m_CurPaletteBuilder = m_Map_PaletteBuilder.GetItem();
	}


	CItemMode::QuestionChanged(pCurQuestion);
}

/// Button Dimension2 pushed
void CPaletteMode::OnChange_Dim_2_PaletteSel()
{
	if(m_pDlgItem->m_Button_Dimension2.GetCheck())
	{
		if(m_pDlgItem->m_List_Dimension2.GetSelCount()!=0) 
			SetDimSelection(1,true);
		else
		{
			SetDimSelection(1,false);
			// m_pDlgItem->m_Button_Dimension2.SetCheck(0,TRUE);
		}
	}
	else
		SetDimSelection(1,false);
}

/// Button Dimension1 actif d'office
void CPaletteMode::OnSetDimSelection2()
{
	SetDimSelection(1,true);
}

/// Button Dimension1 pushed
void CPaletteMode::OnChange_Dim_1_PaletteSel()
{
	if(m_pDlgItem->m_Button_Dimension1.GetCheck())
	{
		// Si au moins 1 sélection dans dimension 1, on la valide en entete
		if(m_pDlgItem->m_List_Dimension1.GetSelCount()!=0) 
		{
			// Valide la sélection pour la mise en palette
			SetDimSelection(0,true);
		}
		else
		{
			SetDimSelection(0,false);
		}
	}
	else
		SetDimSelection(0,false);
}

/// Button Dimension1 actif d'office
void CPaletteMode::OnSetDimSelection1()
{
	SetDimSelection(0,true);
}


LRESULT CPaletteMode::OnChange_Dim_x_PaletteSel(WPARAM wParam, LPARAM lParam)
{
	m_DimDlg.MoveTo(lParam-2);
	if(m_DimDlg.IsValid())
		SetDimSelection(lParam,m_DimDlg.GetItem().IsDimSelected());
	return CItemMode::OnChange_Dim_x_PaletteSel(wParam, lParam);
}

LRESULT CPaletteMode::OnSelChange_ListDimension_x(WPARAM wParam, LPARAM lParam)
{
	m_DimDlg.MoveTo(lParam-2);
	if(m_DimDlg.IsValid())
		SetDimSelection(lParam,m_DimDlg.GetItem().IsDimSelected());
	return CItemMode::OnSelChange_ListDimension_x(wParam, lParam);
}

void CPaletteMode::ResetDimSelection()
{
	m_Map_PaletteBuilder.Reset(); 
	m_Map_PaletteBuilderLigne.Reset(); 
	m_Map_PaletteBuilderColonne.Reset(); 
	m_Map_PaletteBuilderPage.Reset(); 
	m_CurPaletteBuilder.ResetDimSelection();
	UpdateDisplay();
}

/// Returns a string to the current dimension mode
CString CPaletteMode::GetDimensionModeString(int Mode)
{
	CString St;
	LOADSTRING(St, IDS_RI_PALETTE_DIM);
	return St;
}

void CPaletteMode::SetDimSelection(int Index, bool Add)
{
	m_CurPaletteBuilder.SetDimSelection(Index, Add, m_CurMode);
	UpdateDisplay();
}

void CPaletteMode::UpdateDisplay()
{
	SetModeTitle();

	if(m_pSelectedQuestion)
	{
		GetPaletteValues(m_CurPaletteBuilder);

		if(m_bItemValid)
		{
			m_Map_PaletteBuilder.MoveTo(m_pSelectedQuestion->GetID());
			if(m_Map_PaletteBuilder.IsValid())
				m_Map_PaletteBuilder.GetItem() = m_CurPaletteBuilder;
			else
				m_Map_PaletteBuilder.Add(m_pSelectedQuestion->GetID()) = m_CurPaletteBuilder;
		}
		else
		{
			// Attention !!!!!
			m_Map_PaletteBuilder.MoveTo(m_pSelectedQuestion->GetID());
			if(m_Map_PaletteBuilder.IsValid())
				m_Map_PaletteBuilder.Remove();
		}
	}

	BOOL State = FALSE;
	if(m_bItemValid && m_Map_PaletteBuilder.GetCount() > 0) {
		State = TRUE;
		for(m_Map_PaletteBuilder.MoveFirst(); m_Map_PaletteBuilder.IsValid(); m_Map_PaletteBuilder.MoveNext()) {
			if(m_Map_PaletteBuilder.GetItem().m_List_DimSel.GetCount() == 0 && m_Map_PaletteBuilder.GetItem().m_List_NumValue.GetCount() == 0)
				State = FALSE;
		}
	}
	m_pDlgItem->m_Button_Accept.EnableWindow(State);

	// Enables or disables certain windows according the selected properties
	// also changes certain button texts
	if(m_CurPaletteBuilder.m_Map_DimType.GetCount())
	{
		for(int x=0; x < m_CurPaletteBuilder.m_Map_DimType.GetCount(); x++)
		{
			CString text = "";
			if(x==0)
			{
				// Récup texte courant dimension 1
				CString ExTextDim1; 
				m_pDlgItem->m_Button_Dimension1.GetWindowText(ExTextDim1);

				// Sélection palette reste en position
				if(m_CurPaletteBuilder.IsDimensionSelected(x))
				{
					// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT), GetDimensionModeString(m_CurPaletteBuilder.GetSelectedDimensionMode(x)));
					if (m_pDlgItem->m_Button_Dimension2.IsWindowVisible()) 
					{
						// Les 2 fenêtres dimensions visibles
						// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT_VENT));
						// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT),"");
						text.Format(GETSTRING(IDS_RI_DIM_SEL_VENTILER),"");
					}
					else
						// Seule la dimension 1 visible
						text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT),"");

					// Active le bouton dimension 1
					if (m_pDlgItem->m_Button_Dimension1.GetCheck() != 1) 
						m_pDlgItem->m_Button_Dimension1.SetCheck(1);
				}
				else
				{
					// En mode répertoire des items
					if (m_pDlgItem->GetSelDimActive(1))
					{
						// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT), GetDimensionModeString(m_CurPaletteBuilder.GetSelectedDimensionMode(x)));
						if (m_pDlgItem->m_Button_Dimension2.IsWindowVisible()) 
						{
							// Les 2 fenêtres dimensions visibles
							// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT_VENT));
							// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT),"");
							text.Format(GETSTRING(IDS_RI_DIM_SEL_VENTILER),"");
						}
						else
							// Seule la dimension 1 visible
							text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT),"");
						
						// Valide entete mode actif ssi au moins 1 élmt sélectionné
						if(m_pDlgItem->m_List_Dimension1.GetSelCount()!=0)
							m_pDlgItem->m_Button_Dimension1.SetCheck(1);

						// Redessine le bouton dimension 1
						m_pDlgItem->m_Button_Dimension1.Invalidate();
						m_pDlgItem->m_Button_Dimension1.RedrawWindow();
					}
					else
					{
						// text.Format(GETSTRING(IDS_RI_EN), GetDimensionModeString(m_CurMode));
						if (m_pDlgItem->m_Button_Dimension2.IsWindowVisible()) 
						{
							// Les 2 fenêtres dimensions visibles
							// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT_NONVENT));
							text.Format(GETSTRING(IDS_RI_DIM_SEL_REGROUPER),"");
							
							// Bouton dimension 1 non actif
							m_pDlgItem->m_Button_Dimension1.SetCheck(0);
						}
						else
						{
							// Seule la dimension 1 visible
							text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT),"");

							// Bouton dimension 1 non actif
							m_pDlgItem->m_Button_Dimension1.SetCheck(0);
						}

						// Redessine le bouton dimension 1
						m_pDlgItem->m_Button_Dimension1.Invalidate();
						m_pDlgItem->m_Button_Dimension1.RedrawWindow();
					}
				}

				// Remplace texte ssi changé
				if (text != ExTextDim1) 
					m_pDlgItem->m_Button_Dimension1.SetWindowText(text);

			}
			else if(x==1)
			{
				// Récup texte courant dimension 1
				CString ExTextDim2; 
				m_pDlgItem->m_Button_Dimension2.GetWindowText(ExTextDim2);

				if(m_CurPaletteBuilder.IsDimensionSelected(x))
				{
					// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT), GetDimensionModeString(m_CurPaletteBuilder.GetSelectedDimensionMode(x)));
					// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT_VENT));
					// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT),"");
					text.Format(GETSTRING(IDS_RI_DIM_SEL_VENTILER),"");
					m_pDlgItem->m_Button_Dimension2.SetCheck(1);
				}
				else
				{	
					// En mode répertoire des items
					if (m_pDlgItem->GetSelDimActive(2))
					{
						// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT),"");
						text.Format(GETSTRING(IDS_RI_DIM_SEL_VENTILER),"");
						m_pDlgItem->m_Button_Dimension2.SetCheck(1);
					}
					else
					{
						// text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT_NONVENT));
						text.Format(GETSTRING(IDS_RI_DIM_SEL_REGROUPER),"");
						m_pDlgItem->m_Button_Dimension2.SetCheck(0);
					}
				}

				// Remplace texte ssi changé
				if (text != ExTextDim2) 
					m_pDlgItem->m_Button_Dimension2.SetWindowText(text);
			}
			else
			{
				m_DimDlg.MoveTo(x-2);
				if(m_DimDlg.IsValid())
				{
					if(m_CurPaletteBuilder.IsDimensionSelected(x))
						m_DimDlg.GetItem().SetDimMode(true, m_CurPaletteBuilder.GetSelectedDimensionIndex(x)+1,GetDimensionModeString(m_CurPaletteBuilder.GetSelectedDimensionMode(x)));
					else
						m_DimDlg.GetItem().SetDimMode(true, -1,GetDimensionModeString(m_CurMode));
				}
			}
		}

		// Si on n'est en mode pupitre
		if (m_pDlgItem->GetTypeRepItem() == false)
		{
			UpdateMapModePupitre();
		}
	}
	else
	{
		JInt32 IdCurMode = m_pDlgItem->ValCurMode(); 

		// Check if we are on a valid question
		CString text = "";

		// text.Format(GETSTRING(IDS_RI_EN), GetDimensionModeString(m_CurMode));
		text.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT), "");

		m_pDlgItem->m_Button_Dimension1.SetWindowText(text);
		m_pDlgItem->m_Button_Dimension1.SetCheck(0);
		m_pDlgItem->m_Button_Dimension2.SetWindowText(text);
		m_pDlgItem->m_Button_Dimension2.SetCheck(0);

		for(m_DimDlg.MoveFirst(); m_DimDlg.IsValid(); m_DimDlg.MoveNext())
			m_DimDlg.GetItem().SetDimMode(true, -1, GetDimensionModeString(m_CurMode));
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des maps ligne colonne page lié au map builder courant
void CPaletteMode::UpdateMapModePupitre()
{
	// Mise à jour du map palette builder du mode en cours
	if (m_pDlgItem->ValCurMode() == Line)
	{
		// Mode ligne en cours
		m_Map_PaletteBuilderLigne.Reset();

		// Recopie les infos map builder courant dans map builder lignes
		for (m_Map_PaletteBuilder.MoveFirst(); m_Map_PaletteBuilder.IsValid(); m_Map_PaletteBuilder.MoveNext())
		{
			m_Map_PaletteBuilderLigne.Add(m_Map_PaletteBuilder.GetKey()) = m_Map_PaletteBuilder.GetItem();   
		}
		// m_Map_PaletteBuilderLigne = m_Map_PaletteBuilder;
	}
	else if (m_pDlgItem->ValCurMode() == Column)
	{
		// Mode colonne en cours
		m_Map_PaletteBuilderColonne.Reset();

		// Recopie les infos map builder courant dans map builder colonnes
		for (m_Map_PaletteBuilder.MoveFirst(); m_Map_PaletteBuilder.IsValid(); m_Map_PaletteBuilder.MoveNext())
		{
			m_Map_PaletteBuilderColonne.Add(m_Map_PaletteBuilder.GetKey()) = m_Map_PaletteBuilder.GetItem();   
		}
		// m_Map_PaletteBuilderColonne = m_Map_PaletteBuilder;
	}
	else if (m_pDlgItem->ValCurMode() == Page)
	{
		// Mode page en cours
		m_Map_PaletteBuilderPage.Reset();

		// Recopie les infos map builder courant dans map builder pages
		for (m_Map_PaletteBuilder.MoveFirst(); m_Map_PaletteBuilder.IsValid(); m_Map_PaletteBuilder.MoveNext())
		{
			m_Map_PaletteBuilderPage.Add(m_Map_PaletteBuilder.GetKey()) = m_Map_PaletteBuilder.GetItem();   
		}
		// m_Map_PaletteBuiderPage = m_Map_PaletteBuilder;
	}
}
void CPaletteMode::UpdateSegmentTabs(int Sel)
{
	m_pDlgItem->m_Tab_Segment.DeleteAllItems();
	for(int x = 0; x < m_IBuilder.GetSegmentCount() ; x++)
	{
		CString Text;
		// Text.Format("%d",x+1);
		CString label; LOADSTRING(label, IDS_RI_BUT_SEGEMENT);
		Text.Format("%s%d", label, x+1);
		m_pDlgItem->m_Tab_Segment.InsertItem(TCIF_TEXT|TCIF_PARAM,x,Text,0,x);
	}
	m_pDlgItem->m_Tab_Segment.SetCurSel(Sel);
	m_pDlgItem->m_Tab_Segment.UpdateWindow();
}

void CPaletteMode::UpdateIBuilder(bool Changed, bool bValues)
{
	for(m_Map_PaletteBuilder.MoveFirst(); m_Map_PaletteBuilder.IsValid(); m_Map_PaletteBuilder.MoveNext())
	{
		if(m_Map_PaletteBuilder.GetItem().m_List_DimSel.GetCount())
		{
			Changed = false;
			break;
		}
	}
	CItemMode::UpdateIBuilder(Changed, bValues);
	UpdateDisplay();
}

////////////////////////////////////////////////////
/// Button SAS Raz pushed
void CPaletteMode::OnSAS_Raz()
{
	ResetDimSelection();
	CItemMode::OnSAS_Raz();
	SetQuestionOperator(IBloc::Or);  
}

///////////////////////////////////////////////////
// Suppression d'un segment
void CPaletteMode::OnSegment_Raz()
{
	// Idem Supprression SAS
	OnSAS_Raz();

	// ++ Mise à jour segment actif
}

bool CPaletteMode::OnAccept(CString &Name) 
{
	if(!m_bItemValid)
		return false;

	if(!m_pIBquestion && !m_pIBitem)
		return false;

	// If the current question is not valid remove it
	if(m_pIBquestion && !m_pIBquestion->IsValid())
		return false;
	// Remove any Invalide items
	if(m_pIBitem && !m_pIBitem->IsValid())
		return false;

	CString FieldName = "";
	JList<IItem *> IItems;
	bool Mode = (GetQuestionOperator()==IBloc::Or);
	// Get first palette
	m_Map_PaletteBuilder.MoveFirst();
	// Move to first dimension
	m_Map_PaletteBuilder.GetItem().m_List_DimSel.MoveFirst(); 
	if(BuildPalette(IItems, m_Map_PaletteBuilder, FieldName, Mode)&& IItems.GetCount())
	{
		// Create new Palette
		CPalettes * pPalette = m_pDlgItem->GetItems()->GetNewPalette();
		for(IItems.MoveFirst(); IItems.IsValid(); IItems.MoveNext())
			pPalette->AddItem(IItems.GetItem()->AsItemConstruit());
		JLabel Lbl = Name;
		if(!pPalette->SetLabel(Lbl))
		{
			m_pDlgItem->MessageBox("Le libellé de la palette est invalide !",m_pDlgItem->GetTitle(),MB_OK);
			delete pPalette;
			return false;
		}
		const CPalettes *pData = m_pDlgItem->GetItems()->RegisterPalette(pPalette); 
		JUnt32 IdPalette = pData->GetID();
		
		// Ajoute item dans la rubrique divers du tree rubrique palettes
		// m_pDlgItem->m_Tree_Rubrique_Palettes.AddItemRubriqueDivers(IdPalette, pPalette->GetLabel().AsJCharPtr());

		// Ajoute item dans la rubrique defaut ou autre rubrique du tree rubrique palettes
		HTREEITEM HRubriqueAjout;
		HRubriqueAjout = m_pDlgItem->GetRubriquePaletteDefaut();
		if (HRubriqueAjout)
		{
			// On archive le nouvel item dans la rubrique par défaut
			m_pDlgItem->m_Tree_Rubrique_Palettes.AddItemRubrique(HRubriqueAjout, IdPalette, pPalette->GetLabel().AsJCharPtr());
		}
		else
		{
			// En attente @@@@ il faudra demander la rubrique où sauvegarder l'item
			m_pDlgItem->m_Tree_Rubrique_Palettes.AddItemRubriqueDivers(IdPalette, pPalette->GetLabel().AsJCharPtr());

			m_pDlgItem->AddItemRubrique(IdPalette, pPalette->GetLabel().AsJCharPtr());					
		}

		// TRAC : TICKET 35 : on sauve le fichier des rubriques
		m_pDlgItem->m_Tree_Rubrique_Palettes.SaveFileRubrique();

		UpdateListItems();
		Name.Empty();
		delete pPalette;

		return true;
	}
	return false;
}

void CPaletteMode::GetPaletteValues(CPaletteBuilder &PBuilder)
{
	PBuilder.m_pIBitem = 0;
	PBuilder.m_pIBquestion = 0;
	PBuilder.m_Map_pIBModalite.Reset();
	PBuilder.m_Map_pIModalite.Reset();
	PBuilder.m_Map_DimType.Reset();  

	// Check if we have a segment 
	if(m_pIBsegment)
	{
		// Get and set the question operator
 		m_pIBsegment->SetOperator(GetQuestionOperator());

		// For Questions
		if(m_pIBquestion)
		{
			int x = 0;
			PBuilder.m_pIBquestion = m_pIBquestion;
			// Get and set the selected modalities
			JList< CIBModalite *> pIModalite;
			m_pIBquestion->GetModalites(pIModalite);
			for(x = 0, pIModalite.MoveFirst(); pIModalite.IsValid() ; pIModalite.MoveNext(), x++)
			{
				// Get the modalities used
				CIBModalite * pModalite = pIModalite.GetItem();
				PBuilder.m_Map_pIBModalite.Add(x) = pModalite;
				PBuilder.m_Map_DimType.Add(x) = m_pSelectedQuestion->DimensionType(x);
				JList<JInt32> Values;
				JList<const IModalite *> & Items = PBuilder.m_Map_pIModalite.Add(x);
				if(x==0)
				{
					// Set the operator 
					pModalite->SetOperator(GetDimension1Operator());
					// Get the selected ID in the list box
					m_pDlgItem->m_List_Dimension1.GetSelectedID(Values);
					m_pDlgItem->m_List_Dimension1.GetSelectedItems(Items);
					pModalite->SetValues(Values);
					if(Values.GetCount() == 0)
					{
						PBuilder.SetDimSelection(x, false, m_CurMode);
					}
				}
				else if(x==1)
				{
					// Set the operator 
					pModalite->SetOperator(GetDimension2Operator());
					// Get the selected ID in the list box
					m_pDlgItem->m_List_Dimension2.GetSelectedID(Values);
					m_pDlgItem->m_List_Dimension2.GetSelectedItems(Items);
					pModalite->SetValues(Values);
					if(Values.GetCount() == 0)
					{
						PBuilder.SetDimSelection(x, false, m_CurMode);
					}

					// Move to first modeless modalites dialog
					m_DimDlg.MoveFirst(); 
				}
				else
				{
					// Update the data from the floating modality dialogs
					if(m_DimDlg.IsValid())
					{
						CDimensionDlg & Dlg = m_DimDlg.GetItem();
						pModalite->SetOperator(Dlg.GetDimensionOperator());
						// Get the selected ID in the list box
						Dlg.GetSelectedID(Values);
						Dlg.GetSelectedItems(Items); 
						pModalite->SetValues(Values);
						if(Values.GetCount() == 0)
						{
							PBuilder.SetDimSelection(x, false, m_CurMode);
						}

						// Move to next modeless modalites dialog
						m_DimDlg.MoveNext();
					}
				}
			}
		}

		// For items
		if(m_pIBitem)
		{
			PBuilder.m_pIBitem = m_pIBitem;
			// Is Pseudo ou Critere modifié ou Critere Quantile etc..
			if(!m_pSelectedQuestion->IsQuestion())
			{
				if(m_pSelectedQuestion->HasDimensions())
				{
					PBuilder.m_Map_pIBModalite.Add(0) = m_pIBitem;
					JList<JInt32> Values;
					PBuilder.m_Map_DimType.Add(0) = m_pSelectedQuestion->DimensionType(0);
					JList<const IModalite *> & Items = PBuilder.m_Map_pIModalite.Add(0);
					// Set the operator 
					m_pIBitem->SetOperator(GetDimension1Operator());
					// Get the selected ID in the list box
					m_pDlgItem->m_List_Dimension1.GetSelectedID(Values);
					m_pDlgItem->m_List_Dimension1.GetSelectedItems(Items);
					m_pIBitem->SetValues(Values);
					if(Values.GetCount() == 0)
					{
						PBuilder.SetDimSelection(0, false, m_CurMode);
					}
				}
			}
		}
	}
}

void CPaletteMode::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	switch(pWnd->GetDlgCtrlID())
	{
		case IDC_RI_LIST_IITEMS:
		{
			CItemMode::OnContextMenu(pWnd, point);
		}
		break;

		case IDC_RI_LIST_DIMENSION1:
		{
			CItemMode::OnContextMenu(pWnd, point);
		}
		break;
	}
}

// Rename modalites
void CPaletteMode::DeleteDimension1()
{
	// Must have a selected question
	if(!m_pSelectedQuestion)
		return;

	// Check if question is a critere modifie
	if(m_pSelectedQuestion->CanModify() && m_pSelectedQuestion->IsPalette())
	{
		JList<JInt32> Values;
		// Set the selected ID in the list box
		m_pDlgItem->m_List_Dimension1.GetSelectedID(Values);
		if(Values.GetCount()==1)
		{
			Values.MoveFirst();
			CPalettes * pPalette =  const_cast< IQuestion * >( m_pSelectedQuestion )->AsPalette(); 
			if(!pPalette || !pPalette->RemoveItem(Values.GetItem()))
				m_pDlgItem->MessageBox("La modalité ne peut pas être supprimée !",m_pDlgItem->GetTitle(),MB_OK);
			else
			{
				UpdateDimensionsList();
				m_pDlgItem->m_List_Dimension1.SetSelectedID(Values.GetItem());
				m_pDlgItem->GetItems()->Save(); 
			}
		}
	}
}

/// Questions to palette
void CPaletteMode::OnQuestions_Tous()
{
	// Convert the search string 
	CString strFilter = m_pDlgItem->m_QFilter;
	Convert::ToSearchString(strFilter); 

	// Remove all existing palette questions
	m_Map_PaletteBuilder.Reset();

	// Remove all questions from the segment
	if(!m_pIBsegment)
		FindSet_IBsegment(0);

	// Reset the segment
	m_pIBsegment->Reset();
	// Set the question operator
	m_pIBsegment->SetOperator(IBloc::Or);


	// go through each question in the list of questions
	// and add them to the palette
	int NbQuestn = m_pDlgItem->m_List_Questions.GetCount();
	if(NbQuestn != 0)
	{
		for(int x = 0; x < NbQuestn; x++)
		{
			m_pIBquestion	= 0;
			m_pIBitem		= 0;

			// Get the Question
			m_pSelectedQuestion = (IQuestion *)m_pDlgItem->m_List_Questions.GetItemData(x);

			// If this is not a real question skip it
			if(!m_pSelectedQuestion || !m_pSelectedQuestion->IsQuestion())
				continue;
			// If this is an agregat ou numeric data type skip it
			if(m_pSelectedQuestion->AsQuestion()->IsAgregatDataType() || m_pSelectedQuestion->AsQuestion()->IsNumericDataType())
				continue;

			// Reset the current palette builder question
			m_CurPaletteBuilder.Reset();
			// Set the current question
			FindSet_IBcontainerByType(m_pSelectedQuestion); 
			// Get and set the question operator
			m_pIBsegment->SetOperator(IBloc::Or);

			// If IQuestions
			if(m_pIBquestion)
			{
				// Reset the question 
				m_pIBquestion->Reset();
				// Set the question Id
				m_pIBquestion->SetID(m_pSelectedQuestion->GetID());
				// Add the question to the palette builder
				m_CurPaletteBuilder.m_pIBquestion = m_pIBquestion;
				// Go through each dimension adding the modalities
				for(int y = 0; y < m_pSelectedQuestion->HasDimensions(); y++)
				{
					// Create new modalite dimension
					CIBModalite * pModalite = new CIBModalite();
					// Add the modality to the question
					m_pIBquestion->AddModalite(pModalite);
					// Set the operator 
					pModalite->SetOperator(IBloc::Or);
					// Add this dimensions modalites
					m_CurPaletteBuilder.m_Map_pIBModalite.Add(y) = pModalite;
					// Set this dimension as the selected dimension
					m_CurPaletteBuilder.SetDimSelection(y,true,m_CurMode);
					// Set the dimension type
					m_CurPaletteBuilder.m_Map_DimType.Add(y) = m_pSelectedQuestion->DimensionType(y);
					// Get reference to the modality list
					JList<const IModalite *> & Mods = m_CurPaletteBuilder.m_Map_pIModalite.Add(y);

					// Get the modalities used
					JList<JInt32> Ids;
					JVector<const IModalite *> vMods;
					m_pSelectedQuestion->GetIModalites(vMods,0,y,m_pDlgItem->GetContingentMask());
					for(vMods.MoveFirst(); vMods.IsValid(); vMods.MoveNext())
					{
						const IModalite * pMod = vMods.GetItem();
						// Check if we are using a search filter and filter out the 
						// unwanted modalities
						if(m_pDlgItem->m_iModality == 4 && Convert::ToSearchString(pMod->GetLabel().AsJCharPtr()).Find(strFilter,0) == -1)
								continue;

						// Add the modality id to the Id list
						Ids.AddTail() = pMod->GetID();
						// add the modality to the modality list
						Mods.AddTail() = pMod;
					}
					if(Ids.GetCount() == 0)
					{
						for(vMods.MoveFirst(); vMods.IsValid(); vMods.MoveNext())
						{
							const IModalite * pMod = vMods.GetItem();
							// Add the modality id to the Id list
							Ids.AddTail() = pMod->GetID();
							// add the modality to the modality list
							Mods.AddTail() = pMod;
						}
					}
					// Set the Ids
					pModalite->SetValues(Ids);
				}
				// Add the question to the palette builder
				m_Map_PaletteBuilder.Add(m_pSelectedQuestion->GetID()) = m_CurPaletteBuilder;
			}

			// If Iitem
			if(m_pIBitem)
			{
				m_pIBitem->Reset();
				// Set the operator 
				m_pIBitem->SetOperator(IBloc::Or);
				// Set the question Id
				m_pIBitem->SetID(m_pSelectedQuestion->GetID());
				// Add the question to the palette builder
				m_CurPaletteBuilder.m_pIBitem = m_pIBitem;
				// Is Pseudo ou Critere modifié ou Critere Quantile
				if(!m_pSelectedQuestion->IsQuestion()&& m_pSelectedQuestion->HasDimensions())
				{
					// Add the IItem
					m_CurPaletteBuilder.m_Map_pIBModalite.Add(0) = m_pIBitem;
					// Set this dimension as the selected dimension
					m_CurPaletteBuilder.SetDimSelection(0,true,m_CurMode);
					// Set the dimension type
					m_CurPaletteBuilder.m_Map_DimType.Add(0) = m_pSelectedQuestion->DimensionType(0);
					// Get reference to the modality list
					JList<const IModalite *> & Mods = m_CurPaletteBuilder.m_Map_pIModalite.Add(0);

					// Get the modalities used
					JList<JInt32> Ids;
					JVector<const IModalite *> vMods;
					m_pSelectedQuestion->GetIModalites(vMods,0,0,m_pDlgItem->GetContingentMask());
					for(vMods.MoveFirst(); vMods.IsValid(); vMods.MoveNext())
					{
						// Add the modality id to the Id list
						Ids.AddTail() = vMods.GetItem()->GetID();
						// add the modality to the modality list
						Mods.AddTail() = vMods.GetItem();
					}
					// Set the Ids
					m_pIBitem->SetValues(Ids);
					// Add the question to the palette builder
					m_Map_PaletteBuilder.Add(m_pSelectedQuestion->GetID()) = m_CurPaletteBuilder;
				}
			}
		}
	}

	// Si on n'est en mode pupitre
	if (m_pDlgItem->GetTypeRepItem() == false)
	{
		UpdateMapModePupitre();
	}

	// Set the first segment
	FindSet_IBsegment(0);
	// Update the selection list
	if(m_pDlgItem->m_List_SAS.GetCount())
	{
		m_pDlgItem->m_List_SAS.SetCurSel(0);
		OnSelchange_ListSas();
	}
	else
	{
		m_pDlgItem->m_List_Questions.SetCurSel(LB_ERR); 
		OnSelchange_ListQuestions();
	}
	UpdateIBuilder(true, false);
}

// Validate the label
void CPaletteMode::ValidateLabel(CString & Label)
{
	// Check Label length
	if (Label.GetLength() >= JLABEL_MAX)
	{
		// Truncate Label
		Label = Label.Left(JLABEL_MAX - 1);

		// Set Ellipsis
		Label.SetAt(JLABEL_MAX - 4, '.');
		Label.SetAt(JLABEL_MAX - 3, '.');
		Label.SetAt(JLABEL_MAX - 2, '.');
	}
}

bool CPaletteMode::BuildPalette(JList<IItem *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label, bool Mode)
{
	if(!PBuilders.IsValid())
		return false;

	if(Mode)
	{
		for(PBuilders.MoveFirst(); PBuilders.IsValid(); PBuilders.MoveNext())
		{
			// Set the questions selection to the beginning
			PBuilders.GetItem().m_List_DimSel.MoveFirst();
			// Build Palette linear
			BuildPaletteL(IItems,PBuilders,Label);
		}
		return true;
	}
	else
		// Build Palette compiled
		return BuildPaletteX(IItems,PBuilders,Label);
}

bool CPaletteMode::BuildPaletteL(JList<IItem *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label)
{
	if(!PBuilders.IsValid())
		return false;

	bool bValidDimSel		= false;
	bool bHasDimSel			= false;
	int Sel					= 0; 
	CPaletteBuilder & PB	= PBuilders.GetItem();

	if(PB.m_List_DimSel.GetCount()>0)
	{
		bHasDimSel = true;
		// Check for Dimension selection
		if(PB.m_List_DimSel.IsValid())
		{
			// Get current selection
			Sel = PB.m_List_DimSel.GetItem();
			// Step over current selection
			PB.m_List_DimSel.MoveNext();
			// Position all dimension data
			PB.m_Map_pIBModalite	.MoveTo(Sel);
			PB.m_Map_pIModalite		.MoveTo(Sel);
			PB.m_Map_DimType		.MoveTo(Sel);
			// This Question has dimensions
			bValidDimSel = true;
		}
	}

	if(!bHasDimSel || !bValidDimSel)
	{
		// Mode linear add the Item
		if(PB.m_pIBitem || PB.m_pIBquestion)
		{
			const IQuestion * pQ = 0;
			if(PB.m_pIBquestion)
				pQ = (const IQuestion *)m_pDlgItem->GetTerrain()->GetQuestionByID(PB.m_pIBquestion->GetID());
			else
				pQ = (const IQuestion *)m_pDlgItem->GetItems()->GetQuestionByID(PB.m_pIBitem->GetID());
			if(Label.IsEmpty())
			{
				if (PB.GetNumValues().GetCount() == 0)
				{
					if(pQ)
						Label.Insert(0,pQ->GetLabel().AsJCharPtr());
					else
						Label = "Error";
				}
			}
			else
			{
				if(m_pDlgItem->m_iModality == 4 && pQ)
				{
					Label.Insert(0," - ");
					Label.Insert(0,pQ->GetLabel().AsJCharPtr());
				}
			}
			if (PB.m_pIBquestion && PB.GetNumValues().GetCount() > 0)
			{
				CIBClass*    pOldClass    = PB.m_pIBquestion->DetachClass();
				CIBNumValue* pOldNumValue = PB.m_pIBquestion->DetachNumValue();
				const JList<JFlt32> & List_NumValues = PB.GetNumValues();
				for (List_NumValues.MoveFirst(); List_NumValues.IsValid(); List_NumValues.MoveNext())
				{
					JList<JFlt32> Value; Value.AddTail() = List_NumValues.GetItem();
					CIBNumValue* pNumValue = new CIBNumValue;
					pNumValue->SetOperator(IBloc::Nop);
					pNumValue->SetValues(Value);
					PB.m_pIBquestion->SetNumValue(pNumValue);
					CIBuilder	tIBuilder;
					CIBSegment	tISegment;
					tIBuilder.SetOperator(IBloc::Or);
					tIBuilder.Add(&tISegment); 
					tISegment.SetOperator(IBloc::Or);
					if(PB.m_pIBquestion)
						tISegment.Add(PB.m_pIBquestion);
					else
						tISegment.Add(PB.m_pIBitem);
					CString NomValue = Label;
					if (List_NumValues.GetItem() >= 0.0) {
						if (NomValue.IsEmpty()) NomValue.Format("%.0f", (double)List_NumValues.GetItem());
						else                    NomValue.AppendFormat(" - %.0f", (double)List_NumValues.GetItem());
					}
					else {
						if (NomValue.IsEmpty()) NomValue.Format("%s", "#");
						else                    NomValue.AppendFormat(" - %s", "#");
					}
					// Validate the label
					this->ValidateLabel(NomValue);
					// Build the Item with all questions
					JLabel Lbl = NomValue;
					// Create the Target Here and Add it to the Palette
					CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
					pItem->SetLabel(Lbl,true);
					// Set the builder object
					if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),tIBuilder))
						// Add the Item to the Palette
						IItems.AddTail() = pItem;
					else
						delete pItem;
					tIBuilder.RemoveSegment(&tISegment);
					if(PB.m_pIBquestion)
						tISegment.RemoveQuestion(PB.m_pIBquestion);
					else
						tISegment.RemoveItem(PB.m_pIBitem);
				}
				PB.m_pIBquestion->SetNumValue(pOldNumValue);
				PB.m_pIBquestion->SetClass(pOldClass);
			}
			else
			{
				CIBuilder	tIBuilder;
				CIBSegment	tISegment;
				tIBuilder.SetOperator(IBloc::Or);
				tIBuilder.Add(&tISegment); 
				tISegment.SetOperator(IBloc::Or);
				if(PB.m_pIBquestion)
					tISegment.Add(PB.m_pIBquestion);
				else
					tISegment.Add(PB.m_pIBitem);
				// Validate the label
				this->ValidateLabel(Label);
				// Build the Item with all questions
				JLabel Lbl = Label;
				// Create the Target Here and Add it to the Palette
				CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
				pItem->SetLabel(Lbl,true);
				// Set the builder object
				if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),tIBuilder))
					// Add the Item to the Palette
					IItems.AddTail() = pItem;
				else
					delete pItem;
				tIBuilder.RemoveSegment(&tISegment);
				if(PB.m_pIBquestion)
					tISegment.RemoveQuestion(PB.m_pIBquestion);
				else
					tISegment.RemoveItem(PB.m_pIBitem);
			}
		}
		return true;
	}

	// If the current modality is valid and we have values
	if(PB.m_Map_pIBModalite.IsValid() && PB.m_Map_pIModalite.IsValid())
	{
		// Retreive data
		CIBModalite * pModalite				= PB.m_Map_pIBModalite.GetItem();
		JList<const IModalite *> & Modas	= PB.m_Map_pIModalite.GetItem();
		// For each modality build its item
		for(Modas.MoveFirst(); Modas.IsValid(); Modas.MoveNext())
		{
			CString Nom	= Label;
			// Add the modality label
			if(Nom.GetLength()>0) Nom += " - "; 
			Nom += Modas.GetItem()->GetLabel().AsJCharPtr();
			// Set the current modality ID
			JList<JInt32> Values;
			Values.AddTail() = Modas.GetItem()->GetID();
			pModalite->SetValues(Values);
			BuildPaletteL(IItems, PBuilders, Nom);
		}
		if(PB.m_List_DimSel.IsValid())
			PB.m_List_DimSel.MovePrev();
		else
			PB.m_List_DimSel.MoveLast();
	}
	else
		return false;

	return true;
}

bool CPaletteMode::BuildPaletteX(JList<IItem *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label)
{
	if(!PBuilders.IsValid())
		return false;

	bool bValidDimSel		= false;
	bool bHasDimSel			= false;
	int Sel					= 0; 
	CPaletteBuilder & PB	= PBuilders.GetItem();

	if(PB.m_List_DimSel.GetCount()>0)
	{
		bHasDimSel = true;
		// Check for Dimension selection
		if(PB.m_List_DimSel.IsValid())
		{
			// Get current selection
			Sel = PB.m_List_DimSel.GetItem();
			// Step over current selection
			PB.m_List_DimSel.MoveNext();
			// Position all dimension data
			PB.m_Map_pIBModalite	.MoveTo(Sel);
			PB.m_Map_pIModalite		.MoveTo(Sel);
			PB.m_Map_DimType		.MoveTo(Sel);
			// This Question has dimensions
			bValidDimSel = true;
		}
	}

	if(!bHasDimSel || !bValidDimSel)
	{
		if (PB.m_pIBquestion && PB.GetNumValues().GetCount() > 0)
		{
			CIBClass*    pOldClass    = PB.m_pIBquestion->DetachClass();
			CIBNumValue* pOldNumValue = PB.m_pIBquestion->DetachNumValue();
			const JList<JFlt32> & List_NumValues = PB.GetNumValues();
			for (List_NumValues.MoveFirst(); List_NumValues.IsValid(); List_NumValues.MoveNext())
			{
				JList<JFlt32> Value; Value.AddTail() = List_NumValues.GetItem();
				CIBNumValue* pNumValue = new CIBNumValue;
				pNumValue->SetOperator(IBloc::Nop);
				pNumValue->SetValues(Value);
				PB.m_pIBquestion->SetNumValue(pNumValue);
				CString NomValue = Label;
				if (List_NumValues.GetItem() >= 0.0) {
					if (NomValue.IsEmpty()) NomValue.Format("%.0f", (double)List_NumValues.GetItem());
					else                    NomValue.AppendFormat(" - %.0f", (double)List_NumValues.GetItem());
				}
				else {
					if (NomValue.IsEmpty()) NomValue.Format("%s", "#");
					else                    NomValue.AppendFormat(" - %s", "#");
				}
				// Step to the next question
				PBuilders.MoveNext();
				// Is there another question
				if(PBuilders.IsValid())
				{
					// Set the questions selection to the beginning
					PBuilders.GetItem().m_List_DimSel.MoveFirst();
					// Build the Question
					BuildPaletteX(IItems, PBuilders, NomValue);
					PBuilders.MovePrev();
				}
				else
				{
					// Validate the label
					this->ValidateLabel(NomValue);
					// Build the Item with all questions
					JLabel Lbl = NomValue;
					// Create the Target Here and Add it to the Palette
					CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
					pItem->SetLabel(Lbl,true);
					// Set the builder object
					if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),m_IBuilder))
						// Add the Item to the Palette
						IItems.AddTail() = pItem;
					else
						delete pItem;
					PBuilders.MoveLast();
				}
			}
			PB.m_pIBquestion->SetNumValue(pOldNumValue);
			PB.m_pIBquestion->SetClass(pOldClass);
		}
		else
		{
			// Step to the next question
			PBuilders.MoveNext();
			// Is there another question
			if(PBuilders.IsValid())
			{
				// Set the questions selection to the beginning
				PBuilders.GetItem().m_List_DimSel.MoveFirst();
				// Build the Question
				BuildPaletteX(IItems, PBuilders, Label);
				PBuilders.MovePrev();
			}
			else
			{
				// Validate the label
				this->ValidateLabel(Label);
				// Build the Item with all questions
				JLabel Lbl = Label;
				// Create the Target Here and Add it to the Palette
				CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
				pItem->SetLabel(Lbl,true);
				// Set the builder object
				if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),m_IBuilder))
					// Add the Item to the Palette
					IItems.AddTail() = pItem;
				else
					delete pItem;
				PBuilders.MoveLast();
			}
		}
	}
	else
	{
		// If the current modality is valid and we have values
		if(PB.m_Map_pIBModalite.IsValid() && PB.m_Map_pIModalite.IsValid())
		{
			// Retreive data
			CIBModalite * pModalite				= PB.m_Map_pIBModalite.GetItem();
			JList<const IModalite *> & Modas	= PB.m_Map_pIModalite.GetItem();
			// For each modality build its item
			for(Modas.MoveFirst(); Modas.IsValid(); Modas.MoveNext())
			{
				// Add the modality label
				CString Nom = Label;
				if(Nom.GetLength()>0) Nom += " - "; 
				// Add the modality label
				Nom += Modas.GetItem()->GetLabel().AsJCharPtr();
				// Set the current modality ID
				JList<JInt32> Values;
				Values.AddTail() = Modas.GetItem()->GetID();
				pModalite->SetValues(Values);
				BuildPaletteX(IItems, PBuilders, Nom);
			}
			if(PB.m_List_DimSel.IsValid())
				PB.m_List_DimSel.MovePrev();
			else
				PB.m_List_DimSel.MoveLast();
		}
		else
			return false;
	}
	return true;
}

// returns the current modes label
void CPaletteMode::SetModeTitle(void)
{
	CString Label;
	LOADSTRING(Label, IDS_RI_PALETTES);
	if(m_pDlgItem->m_iModality == 4)
	{
		CString Filtre;
		LOADSTRING(Filtre, IDS_RI_FILTRES);
		Label.Append(Filtre);
	}
	m_pDlgItem->m_Mode_Title.SetWindowText(Label);
	m_pDlgItem->m_Mode_Title.Invalidate();
}

/// Generate Items based on the current valid configuration
bool CPaletteMode::BuildSelectedItemsPalette()
{
	m_SelectedItems.Reset();

	if(!m_bItemValid)
		return false;

	if(!m_pIBquestion && !m_pIBitem)
		return false;

	// If the current question is not valid remove it
	if(m_pIBquestion && !m_pIBquestion->IsValid())
		return false;
	// Remove any Invalide items
	if(m_pIBitem && !m_pIBitem->IsValid())
		return false;

	// Get first palette
	m_Map_PaletteBuilder.MoveFirst();
	m_Map_PaletteBuilder.GetItem().m_List_DimSel.MoveFirst(); 

	// Move to first dimension
	CString FieldName = "";
	JList<CItemWrapper *> Items;
	
	// TODO get the mode
	bool Mode = (GetQuestionOperator()==IBloc::Or);
	// if (BuildPalette(Items, m_Map_PaletteBuilder, FieldName, Mode) && Items.GetCount())
	if (BuildPaletteModeLine(Items, m_Map_PaletteBuilder, FieldName, Mode) && Items.GetCount())
	{
		for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		{
			if(Items.GetItem()->GetHeader().IsEmpty())
			{
				Items.GetItem()->SetHeader(CString(_T("?")));
			}

			m_SelectedItems.AddTail() = Items.GetItem();
		}
	}
	return true;
}

void CPaletteMode::GetSelectedItems(JList< CItemWrapper *> &Items)
{
	Items.Reset();
	if(m_SelectedItems.GetCount())
		Items.Swap(m_SelectedItems);
}

// Positionne les items sélectionnés directement
void CPaletteMode::SetSelectedItems(JList< CItemWrapper *> &Items)
{
	m_SelectedItems.Reset();
	if(Items.GetCount())
		m_SelectedItems.Swap(Items);
}

bool CPaletteMode::BuildPaletteModeLine(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label, bool Mode)
{
	if(!PBuilders.IsValid())
		return false;

	if(Mode)
	{
		for(PBuilders.MoveFirst(); PBuilders.IsValid(); PBuilders.MoveNext())
		{
			// Set the questions selection to the beginning
			PBuilders.GetItem().m_List_DimSel.MoveFirst();

			// Ident Item
			JInt32 IdItem = PBuilders.GetKey();
			if (m_pDlgItem->QuestionSelActive(IdItem))
			{
				// Build Palette linear
				BuildPaletteLModeLine(IItems,PBuilders,Label);
			}
		}
		return true;
	}
	else
		// Build Palette compiled
		return BuildPaletteXModeLine(IItems,PBuilders,Label);
}

bool CPaletteMode::BuildPaletteLModeLine(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label)
{
	if(!PBuilders.IsValid())
		return false;

	bool bValidDimSel		= false;
	bool bHasDimSel			= false;
	int Sel					= 0; 
	CPaletteBuilder & PB	= PBuilders.GetItem();

	if(PB.m_List_DimSel.GetCount()>0)
	{
		bHasDimSel = true;
		// Check for Dimension selection
		if(PB.m_List_DimSel.IsValid())
		{
			// Get current selection
			Sel = PB.m_List_DimSel.GetItem();
			// Step over current selection
			PB.m_List_DimSel.MoveNext();
			// Position all dimension data
			PB.m_Map_pIBModalite	.MoveTo(Sel);
			PB.m_Map_pIModalite		.MoveTo(Sel);
			PB.m_Map_DimType		.MoveTo(Sel);
			// This Question has dimensions
			bValidDimSel = true;
		}
	}

	if(!bHasDimSel || !bValidDimSel)
	{
		// Mode linear add the Item
		if(PB.m_pIBitem || PB.m_pIBquestion)
		{
			const IQuestion * pQ = 0;
			if(PB.m_pIBquestion)
				pQ = (const IQuestion *)m_pDlgItem->GetTerrain()->GetQuestionByID(PB.m_pIBquestion->GetID());
			else
				pQ = (const IQuestion *)m_pDlgItem->GetItems()->GetQuestionByID(PB.m_pIBitem->GetID());
			if(Label.IsEmpty())
			{
				if (PB.GetNumValues().GetCount() == 0)
				{
					if(pQ)
						Label = pQ->GetLabel().AsJCharPtr();
					else
						Label = "Error"; 
				}
			}
			else
			{
				if(m_pDlgItem->m_iModality == 4 && pQ)
				{
					Label.Insert(0," - ");
					Label.Insert(0,pQ->GetLabel().AsJCharPtr());
				}
			}
			if (PB.m_pIBquestion && PB.GetNumValues().GetCount() > 0)
			{
				CIBClass*    pOldClass    = PB.m_pIBquestion->DetachClass();
				CIBNumValue* pOldNumValue = PB.m_pIBquestion->DetachNumValue();
				const JList<JFlt32> & List_NumValues = PB.GetNumValues();
				for (List_NumValues.MoveFirst(); List_NumValues.IsValid(); List_NumValues.MoveNext())
				{
					JList<JFlt32> Value; Value.AddTail() = List_NumValues.GetItem();
					CIBNumValue* pNumValue = new CIBNumValue;
					pNumValue->SetOperator(IBloc::Nop);
					pNumValue->SetValues(Value);
					PB.m_pIBquestion->SetNumValue(pNumValue);
					CIBuilder	tIBuilder;
					CIBSegment	tISegment;
					tIBuilder.SetOperator(IBloc::Or);
					tIBuilder.Add(&tISegment); 
					tISegment.SetOperator(IBloc::Or);
					tISegment.Add(PB.m_pIBquestion);
					CString NomValue = Label;
					if (List_NumValues.GetItem() >= 0.0) {
						if (NomValue.IsEmpty()) NomValue.Format("%.0f", (double)List_NumValues.GetItem());
						else                    NomValue.AppendFormat(" - %.0f", (double)List_NumValues.GetItem());
					}
					else {
						if (NomValue.IsEmpty()) NomValue.Format("%s", "#");
						else                    NomValue.AppendFormat(" - %s", "#");
					}
					// Validate the label
					this->ValidateLabel(NomValue);
					// Build the Item with all questions
					JLabel Lbl = NomValue;
					// Create the Target Here and Add it to the Palette
					CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
					pItem->SetLabel(Lbl,true);
					// Set the builder object
					if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),tIBuilder))
					{
						CString StrHeader = "";
						CItemWrapper *pIW = new CItemWrapper();
						IItems.AddTail() = pIW;
						if(m_pDlgItem->m_iModality == 4)
						{
							StrHeader = m_pDlgItem->m_QFilter;
						}
						else
						{
							if(pQ)
								StrHeader = pQ->GetLabel().AsJCharPtr(); 
							if(!PB.m_Label.IsEmpty())
								StrHeader = PB.m_Label;
							else if(!PB.m_DefaultLabel.IsEmpty())
								StrHeader = PB.m_DefaultLabel;
						}
						pIW->SetItemConstruit(pItem);
						pIW->SetHeader(StrHeader);
					}
					else
						delete pItem;
					tIBuilder.RemoveSegment(&tISegment);
					if(PB.m_pIBquestion)
						tISegment.RemoveQuestion(PB.m_pIBquestion);
					else
						tISegment.RemoveItem(PB.m_pIBitem);
				}
				PB.m_pIBquestion->SetNumValue(pOldNumValue);
				PB.m_pIBquestion->SetClass(pOldClass);
			}
			else
			{
				CIBuilder	tIBuilder;
				CIBSegment	tISegment;
				tIBuilder.SetOperator(IBloc::Or);
				tIBuilder.Add(&tISegment); 
				tISegment.SetOperator(IBloc::Or);
				if(PB.m_pIBquestion)
					tISegment.Add(PB.m_pIBquestion);
				else
					tISegment.Add(PB.m_pIBitem);
				// Validate the label
				this->ValidateLabel(Label);
				// Build the Item with all questions
				JLabel Lbl = Label;
				// Create the Target Here and Add it to the Palette
				CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
				pItem->SetLabel(Lbl,true);
				// Set the builder object
				if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),tIBuilder))
				{
					CString StrHeader = "";
					CItemWrapper *pIW = new CItemWrapper();
					IItems.AddTail() = pIW;
					if(m_pDlgItem->m_iModality == 4)
					{
						StrHeader = m_pDlgItem->m_QFilter;
					}
					else
					{
						if(pQ)
							StrHeader = pQ->GetLabel().AsJCharPtr(); 
						if(!PB.m_Label.IsEmpty())
							StrHeader = PB.m_Label;
						else if(!PB.m_DefaultLabel.IsEmpty())
							StrHeader = PB.m_DefaultLabel;
					}
					pIW->SetItemConstruit(pItem);
					pIW->SetHeader(StrHeader);
				}
				else
					delete pItem;
				tIBuilder.RemoveSegment(&tISegment);
				if(PB.m_pIBquestion)
					tISegment.RemoveQuestion(PB.m_pIBquestion);
				else
					tISegment.RemoveItem(PB.m_pIBitem);
			}
		}
		return true;
	}
	else
	{
		// If the current modality is valid and we have values
		if(PB.m_Map_pIBModalite.IsValid() && PB.m_Map_pIModalite.IsValid())
		{
			// Retreive data
			CIBModalite * pModalite				= PB.m_Map_pIBModalite.GetItem();
			JList<const IModalite *> & Modas	= PB.m_Map_pIModalite.GetItem();


			// For each modality build its item
			JList<JInt32> Values;
			for(Modas.MoveFirst(); Modas.IsValid(); Modas.MoveNext())
			{
				CString Nom	= Label;
				// Add the modality label
				if(Nom.GetLength()>0)	Nom += " - "; 
				Nom += Modas.GetItem()->GetLabel().AsJCharPtr();

				// Set the current modality ID
				// ?????????? JList<JInt32> Values;

				Values.AddTail() = Modas.GetItem()->GetID();
				pModalite->SetValues(Values);
				BuildPaletteLModeLine(IItems, PBuilders, Nom);
			}
			if(PB.m_List_DimSel.IsValid())
				PB.m_List_DimSel.MovePrev();
			else
				PB.m_List_DimSel.MoveLast();
		}
		else
			return false;
	}
	return true;
}

bool CPaletteMode::BuildPaletteXModeLine(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label)
{
	if(!PBuilders.IsValid())
		return false;

	bool bValidDimSel		= false;
	bool bHasDimSel			= false;
	int Sel					= 0; 
	CPaletteBuilder & PB	= PBuilders.GetItem();

	if(PB.m_List_DimSel.GetCount()>0)
	{
		bHasDimSel = true;
		// Check for Dimension selection
		if(PB.m_List_DimSel.IsValid())
		{
			// Get current selection
			Sel = PB.m_List_DimSel.GetItem();
			// Step over current selection
			PB.m_List_DimSel.MoveNext();
			// Position all dimension data
			PB.m_Map_pIBModalite	.MoveTo(Sel);
			PB.m_Map_pIModalite		.MoveTo(Sel);
			PB.m_Map_DimType		.MoveTo(Sel);
			// This Question has dimensions
			bValidDimSel = true;
		}
	}

	if(!bHasDimSel || !bValidDimSel)
	{
		if (PB.m_pIBquestion && PB.GetNumValues().GetCount() > 0)
		{
			CIBClass*    pOldClass    = PB.m_pIBquestion->DetachClass();
			CIBNumValue* pOldNumValue = PB.m_pIBquestion->DetachNumValue();
			const JList<JFlt32> & List_NumValues = PB.GetNumValues();
			for (List_NumValues.MoveFirst(); List_NumValues.IsValid(); List_NumValues.MoveNext())
			{
				JList<JFlt32> Value; Value.AddTail() = List_NumValues.GetItem();
				CIBNumValue* pNumValue = new CIBNumValue;
				pNumValue->SetOperator(IBloc::Nop);
				pNumValue->SetValues(Value);
				PB.m_pIBquestion->SetNumValue(pNumValue);
				CString NomValue = Label;
				if (List_NumValues.GetItem() >= 0.0) {
					if (NomValue.IsEmpty()) NomValue.Format("%.0f", (double)List_NumValues.GetItem());
					else                    NomValue.AppendFormat(" - %.0f", (double)List_NumValues.GetItem());
				}
				else {
					if (NomValue.IsEmpty()) NomValue.Format("%s", "#");
					else                    NomValue.AppendFormat(" - %s", "#");
				}
				// Step to the next question
				PBuilders.MoveNext();
				// Is there another question
				if(PBuilders.IsValid())
				{
					// Set the questions selection to the beginning
					PBuilders.GetItem().m_List_DimSel.MoveFirst();
					// Build the Question
					BuildPaletteXModeLine(IItems, PBuilders, NomValue);
					PBuilders.MovePrev();
				}
				else
				{
					// Geet the Question or Item
					const IQuestion * pQ = 0;
					if(PB.m_pIBquestion)
						pQ = (const IQuestion *)m_pDlgItem->GetTerrain()->GetQuestionByID(PB.m_pIBquestion->GetID());
					else
						pQ = (const IQuestion *)m_pDlgItem->GetItems()->GetQuestionByID(PB.m_pIBitem->GetID());
					// Validate the label
					this->ValidateLabel(NomValue);
					// Build the Item with all questions
					JLabel Lbl = NomValue;
					// Create the Target Here and Add it to the Palette
					CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
					pItem->SetLabel(Lbl,true);
					// Set the builder object
					if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),m_IBuilder))
					{
						CItemWrapper *pIW = new CItemWrapper();
						pIW->SetItemConstruit(pItem);
						if(!m_PaletteXLabel.IsEmpty())
							pIW->SetHeader(m_PaletteXLabel);
						else if(!m_DefaultPaletteXLabel.IsEmpty())
							pIW->SetHeader(m_DefaultPaletteXLabel);
						IItems.AddTail() = pIW;
					}
					else
						delete pItem;
					PBuilders.MoveLast();
				}
			}
			PB.m_pIBquestion->SetNumValue(pOldNumValue);
			PB.m_pIBquestion->SetClass(pOldClass);
		}
		else
		{
			// Step to the next question
			PBuilders.MoveNext();
			// Is there another question
			if(PBuilders.IsValid())
			{
				// Set the questions selection to the beginning
				PBuilders.GetItem().m_List_DimSel.MoveFirst();
				// Build the Question
				BuildPaletteXModeLine(IItems, PBuilders, Label);
				PBuilders.MovePrev();
			}
			else
			{
				// Geet the Question or Item
				const IQuestion * pQ = 0;
				if(PB.m_pIBquestion)
					pQ = (const IQuestion *)m_pDlgItem->GetTerrain()->GetQuestionByID(PB.m_pIBquestion->GetID());
				else
					pQ = (const IQuestion *)m_pDlgItem->GetItems()->GetQuestionByID(PB.m_pIBitem->GetID());
				// Validate the label
				this->ValidateLabel(Label);
				// Build the Item with all questions
				JLabel Lbl = Label;
				// Create the Target Here and Add it to the Palette
				CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
				pItem->SetLabel(Lbl,true);
				// Set the builder object
				if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),m_IBuilder))
				{
					CItemWrapper *pIW = new CItemWrapper();
					pIW->SetItemConstruit(pItem);
					if(!m_PaletteXLabel.IsEmpty())
						pIW->SetHeader(m_PaletteXLabel);
					else if(!m_DefaultPaletteXLabel.IsEmpty())
						pIW->SetHeader(m_DefaultPaletteXLabel);
					IItems.AddTail() = pIW;
				}
				else
					delete pItem;
				PBuilders.MoveLast();
			}
		}
	}
	else
	{
		// If the current modality is valid and we have values
		if(PB.m_Map_pIBModalite.IsValid() && PB.m_Map_pIModalite.IsValid())
		{
			// Retreive data
			CIBModalite * pModalite				= PB.m_Map_pIBModalite.GetItem();
			JList<const IModalite *> & Modas	= PB.m_Map_pIModalite.GetItem();

			// For each modality build its item
			JList<JInt32> Values;
			for(Modas.MoveFirst(); Modas.IsValid(); Modas.MoveNext())
			{
				// Add the modality label
				CString Nom = Label;
				if(Label.GetLength()>0) Nom += " - "; 
				// Add the modality label
				Nom += Modas.GetItem()->GetLabel().AsJCharPtr();
				// Set the current modality ID
				// JList<JInt32> Values;
				Values.AddTail() = Modas.GetItem()->GetID();
				pModalite->SetValues(Values);
				BuildPaletteXModeLine(IItems, PBuilders, Nom);
			}
			if(PB.m_List_DimSel.IsValid())
				PB.m_List_DimSel.MovePrev();
			else
				PB.m_List_DimSel.MoveLast();
		}
		else
			return false;
	}
	return true;
}

// Updates the question list
void CPaletteMode::UpdateQuestionList(long BlockId)
{
	int TopBlock = 0;

	// Get the selected question
	JInt32 IDs = m_pDlgItem->m_List_Questions.GetSelectedID();   

	JList<IQuestion const *> & Question_List = m_pDlgItem->m_Tree_Theme.GetQuestionList();

	//m_pDlgItem->m_List_Questions.ResetContent();
	// Initialise boite liste des questions
	m_pDlgItem->InitListQuestion();

	// Map des Id Questions valides
	JMap <JInt32,JInt32> MapIdQuValid;
	MapIdQuValid.Reset(); 

	// Find the questions coresponding to the current mode
    for(Question_List.MoveFirst();Question_List.IsValid(); Question_List.MoveNext())
	{
		IQuestion const * pQuestion = Question_List.GetItem();

		JInt32 Mask = m_pDlgItem->GetContingentMask();

		if(pQuestion->IsInContingent(m_pDlgItem->GetContingentMask()))
		{
			int Pos = m_pDlgItem->m_List_Questions.AddItem(pQuestion);
			if(BlockId !=0 && pQuestion->IsBlocQuestion() && pQuestion->GetBlocID() == BlockId)
				TopBlock = Pos;
		}

		// Mars 2006
		// Vérifie que le SAS est en concordance avec les questions sélectionnées
		JInt32 BlocID = pQuestion->GetBlocID();
		JInt32 ID     = pQuestion->GetID(); 
		MapIdQuValid.MoveTo(ID);
		if (!MapIdQuValid.IsValid())
			MapIdQuValid.Add(ID); 
	}

	// Mise à jour des segments avec la nouvelle sélection de questions dans TreeItem
	int SelSeg = -1;
	if (FindDelete_IBMapquestion(MapIdQuValid,SelSeg))
	{
		if (SelSeg != -1)
		{
			FindSet_IBsegment(SelSeg,false,false);
			if(m_pDlgItem->m_List_SAS.GetCount())
			{
				m_pDlgItem->m_List_SAS.SetCurSel(0);
				OnSelchange_ListSas();
			}
			else
			{
				m_pDlgItem->m_List_Questions.SetCurSel(LB_ERR); 
				OnSelchange_ListQuestions();
			}
		}

		// Mise à jour de la sélection dans liste dimension 1
		// UpdateDimensionsList();

		// Mise à jour général
		UpdateIBuilder(true, false);
	}

	if((m_pPseudoItem && m_pPseudoItem->HasItems()) || m_Selected_UserItems.GetCount())
		m_pDlgItem->m_List_Questions.AddItem(&m_pDlgItem->m_BlocQuestion); 

	// If there is a Pseudo question add it to the list
	if(m_pPseudoItem && m_pPseudoItem->HasItems())
	{
		m_pDlgItem->m_List_Questions.AddItem(m_pPseudoItem);
	}

	for(m_Selected_UserItems.MoveFirst();m_Selected_UserItems.IsValid(); m_Selected_UserItems.MoveNext())
	{
		m_pDlgItem->m_List_Questions.AddItem(m_pDlgItem->GetItems()->GetQuestionByID(m_Selected_UserItems.GetKey()));
	}

	m_pDlgItem->m_List_Questions.SetSelectedID(IDs); 
	int CurSel = m_pDlgItem->m_List_Questions.GetCurSel();
	if(BlockId==0)
		m_pDlgItem->m_List_Questions.SetTopIndex(CurSel);
	else
		m_pDlgItem->m_List_Questions.SetTopIndex(TopBlock);

	m_pSelectedQuestion = 0;

	// Bidouille Correction problème affichage (Alain 8/07/2013)
	m_pDlgItem->m_List_Questions.ResizeList();
		
	// Redraw
	m_pDlgItem->m_List_Questions.Invalidate();
	m_pDlgItem->m_List_Questions.RedrawWindow();
	

	// ???? Mise à jour des listes Dimension 1 et Dimension 2
	int NbSelQu = m_pDlgItem->m_List_Questions.GetSelCount();
	if (m_pDlgItem->m_List_Questions.GetSelCount() <= 0)
	{
		m_pDlgItem->m_List_Dimension1.ResetContent(); 
		m_pDlgItem->m_List_Dimension2.ResetContent(); 
	}
}

// Finds all current questions in all segment or delete them
bool CPaletteMode::FindDelete_IBMapquestion(JMap <JInt32,JInt32> &MapQuestion, int &SelSeg)
{
	// Flag Suppression Ok (si au moins 1 suppression)
	int NbSuppOk = 0;

	int Count = m_IBuilder.GetSegmentCount();
	SelSeg = m_pDlgItem->m_Tab_Segment.GetCurSel();

	// Balaye tous les segments
	for(int x = m_IBuilder.GetSegmentCount()-1; x >= 0; x--)
	{
		// Récupére le segment
		CIBSegment * pIBsegmentTmp		=	m_IBuilder.GetSegment(x);

		// Récup question associé à l'ident dans segment
		if (pIBsegmentTmp)
		{
			// Récup les questions du segment
			JList<JInt32> LstQuestionsID;
			pIBsegmentTmp->GetQuestionsID(LstQuestionsID); 

			// Balaye les questions, et vire ceux qui ne sont plus sélectionnées dans le TreeItem
			for(LstQuestionsID.MoveFirst(); LstQuestionsID.IsValid(); LstQuestionsID.MoveNext())
			{
				// Récup Ident Question	
				JInt32 IDQuestion = LstQuestionsID.GetItem();

				// Vérifie qu'elle est toujours sélectionnée
				MapQuestion.MoveTo(IDQuestion);
				if (!MapQuestion.IsValid() && pIBsegmentTmp->IsValid())
				{
					// Récupère l'objet question
					CIBQuestion *	pIBquestion	=	pIBsegmentTmp->FindQuestion(IDQuestion);
					if(pIBquestion)
					{
						// On vire la question
						if (pIBsegmentTmp->DeleteQuestion(pIBquestion))
						{
							NbSuppOk++;

							// Update Map_PaletteBuilder
							m_Map_PaletteBuilder.MoveTo(IDQuestion);
							if(m_Map_PaletteBuilder.IsValid())
								m_Map_PaletteBuilder.Remove();
						}

						// Vire aussi la question en cours de sélection
						if (m_pSelectedQuestion && m_pSelectedQuestion->GetID() == IDQuestion)
							m_pSelectedQuestion = 0;

						// On vérifie que le segment a toujours lieu d'être ("non vide")	
						if(!pIBsegmentTmp->IsValid())
						{
							m_IBuilder.DeleteSegment(pIBsegmentTmp);
							if (pIBsegmentTmp == m_pIBsegment)
							{
								m_pIBsegment = 0;
							}
							if (pIBquestion == m_pIBquestion)
								m_pIBquestion	= 0;

							m_pIBitem		= 0;

							Count--;
							if(Count < SelSeg)
								SelSeg--;
						}
					}
				}
			}
		}
	}

	return (NbSuppOk > 0);
}

///////////////////////////////////////////////////////////////////////
// Quel mode pour le Sas
bool CPaletteMode:: ModeCroisementPalette()
{
	// Rien ici
	return false;
}

///////////////////////////////////////////////////////////////////////
// Récupération palette
bool CPaletteMode:: SendPalette(JStream &Stream, int Mode)
{
	// rien
	return false;
}

///////////////////////////////////////////////////////////////////////
// Récupération palette
bool CPaletteMode:: RecvPalette(JStream &Stream, int Mode)
{
	// rien
	return false;
}

///////////////////////////////////////////////////////////////////////
// Recopie elmt palette
bool CPaletteMode::CopyPalette(CAbstractMode * pPaletteMode)
{
	// rien
	return false;
}

//////////////////////////////////////////////////////////////////////////////////
// Récupère les infos segments et IBuilder via les infos Questions et Items
void CPaletteMode::SetSegmentIBuilder()
{
	// Valide que pour le mode pupitre
	if (!m_pDlgItem->GetTypeRepItem())
	{
		bool NewSegment = false;

		// Récupère m_pIBquestion à partir 
		if (!m_pIBsegment)
		{
			m_pIBsegment = new CIBSegment();
			NewSegment   = true;
		}

		if (m_pIBsegment)
		{
			// Réinitialise toutes les infos segment
			m_pIBsegment->Reset();

			// Ajoute les élmt Questions et Items
			JList <JInt32> ValuesInt32;

			int Nb = m_Map_PaletteBuilder.GetCount();

			for (m_Map_PaletteBuilder.MoveFirst(); m_Map_PaletteBuilder.IsValid(); m_Map_PaletteBuilder.MoveNext())
			{	
				// Récup un élmt CPaletteBuilder
				CPaletteBuilder	&PaletteBuilder = m_Map_PaletteBuilder.GetItem();

				JBool QuestionExist = (PaletteBuilder.m_pIBquestion != 0);
				JBool ItemExist		= (PaletteBuilder.m_pIBitem != 0);

				// Ajoute IBquestion si valide
				if (PaletteBuilder.m_pIBquestion && PaletteBuilder.m_pIBquestion->IsValid()) 
					m_pIBsegment->Add(PaletteBuilder.m_pIBquestion); 

				// Ajoute IBitem si valide
				if (PaletteBuilder.m_pIBitem && PaletteBuilder.m_pIBitem->IsValid()) 
				{
					// Ajoute elmt Item au segment
					m_pIBsegment->Add(PaletteBuilder.m_pIBitem); 
				}
			}

			// Ajoute au IBuilder s'il s'agit d'un nouveau segment
			if (NewSegment)
				m_IBuilder.Add(m_pIBsegment); 

		}	
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Positionne le pseudo item
void CPaletteMode::SetPseudoItem()
{
	// Valide que pour le mode pupitre
	if (!m_pDlgItem->GetTypeRepItem())
	{
		bool MajPseudoOk = false;

		// Ajoute les élmt Questions et Items
		JList <JInt32> ValuesInt32;
		for (m_Map_PaletteBuilder.MoveFirst(); m_Map_PaletteBuilder.IsValid(); m_Map_PaletteBuilder.MoveNext())
		{	
			// Récup un élmt CPaletteBuilder
			CPaletteBuilder	&PaletteBuilder = m_Map_PaletteBuilder.GetItem();

			// Ajoute IBitem si valide
			if (PaletteBuilder.m_pIBitem && PaletteBuilder.m_pIBitem->IsValid()) 
			{
				// Ajoute elmt Item au segment
				// m_pIBsegment->Add(PaletteBuilder.m_pIBitem); 

				// Test si ce sont des pseudos items
				if (PaletteBuilder.m_pIBitem->GetID() == 0 && MajPseudoOk == false)
				{
					// Si Palette Builder Item, on en profite pour sauvegarder le Pseudo Item
					PaletteBuilder.m_pIBitem->GetValues(ValuesInt32);

					// Nouveau Pseudo Item
					// UpdatePseudoItem(ValuesInt32);
					UpdatePseudoItemLineMode(ValuesInt32);
					
					// La mise à jour est fiate
					MajPseudoOk = true;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Sauvegarde des palettes temporaires
bool CPaletteMode::SaveMemento(JMemento	* pMemento, int Mode)
{
	// rien
	return false;
}

//////////////////////////////////////////////////////////////////////
// Récupération des palettes temporaires
bool CPaletteMode::LoadMemento(JMemento	* pMemento, int Mode)
{
	// rien
	return false;
}
