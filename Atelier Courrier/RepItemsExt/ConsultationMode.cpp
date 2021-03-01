#include "StdAfx.h"
#include "ConsultationMode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConsultationMode::CConsultationMode(CDlgItems *pDlgItems) : CItemMode(pDlgItems)
{
}

CConsultationMode::~CConsultationMode()
{
}

void CConsultationMode::StartMode()
{
	EnableOperators(TRUE);

	m_pDlgItem->m_Button_SAS_ET    .EnableWindow(FALSE);
	m_pDlgItem->m_Button_SAS_OU    .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Segment_OU.EnableWindow(FALSE);
	m_pDlgItem->m_Button_Segment_ET.EnableWindow(FALSE);

	// m_pDlgItem->m_Button_Filtre     .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_UP    .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_DOWN  .EnableWindow(FALSE);
	m_pDlgItem->m_Tab_Segment       .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Genese     .EnableWindow(FALSE);
	m_pDlgItem->m_Button_List2      .EnableWindow(TRUE);
	m_pDlgItem->m_Button_Accept     .EnableWindow(FALSE);
	m_pDlgItem->m_Button_CalcSegment.EnableWindow(FALSE);
	
	m_pDlgItem->m_Button_Dim1_OK   .ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Dimension1.ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Dimension2.ShowWindow(SW_HIDE);
	m_pDlgItem->m_Header_Dimension1.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Header_Dimension2.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Button_Genese    .ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_List2     .ShowWindow(SW_SHOW);

	m_pDlgItem->m_List_Items.ResetContent(); 
	
	CString Label;
	CString ToolTip;

	Label.LoadString(IDS_RI_CONSULTATION); 
	m_pDlgItem->m_Mode_Title.SetWindowText(Label);
	m_pDlgItem->m_Mode_Title.Invalidate();

	Label.LoadString(IDS_RI_ITEMS); 
	m_pDlgItem->m_Button_List1.SetWindowText(Label);
	ToolTip.LoadString(IDS_RI_TT_ITEMS); 
	m_pDlgItem->m_Button_List1.SetTooltipText(ToolTip);

	Label.LoadString(IDS_RI_PALETTES); 
	m_pDlgItem->m_Button_List2.SetWindowText(Label);
	ToolTip.LoadString(IDS_RI_TT_PALETTE); 
	m_pDlgItem->m_Button_List2.SetTooltipText(ToolTip);

	m_Dim_Sel.Reset();

	// Onglet sélection mode
	m_pDlgItem->CheckRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2,IDC_RI_RAD_QUESTIONS);

	// Passe en mode questions
	UpdateTheme(IDC_RI_RAD_QUESTIONS);
	UpdateListItems();

	m_IBuilder.Reset();
	SetSegmentOperator(IBloc::Or );
	m_IBuilder.SetOperator(IBloc::Or);
	FindSet_IBsegment(0);
	m_pDlgItem->m_List_Questions.SetCurSel(LB_ERR); 
	OnSelchange_ListQuestions();
}

void CConsultationMode::StopMode()
{
	CItemMode::StopMode();
}

bool CConsultationMode::OnAccept(CString &Name) 
{
	return false;
}

// Displays the selected theme
void CConsultationMode::UpdateTheme(int ID)
{
	CString Label;

	switch(ID)
	{
		case IDC_RI_RAD_QUESTIONS :
			Label.LoadString(IDS_RI_INDEX_THEMATIQUE);
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
			Label.LoadString(IDS_RI_ITEMS_CONST);
			m_pDlgItem->m_Tree_Theme			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilter		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilterRaz		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Edit_QFilter			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Items	.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE);
			m_pDlgItem->m_List_Items			.ShowWindow(SW_SHOW);
			m_pDlgItem->ResizeListItems(true);
			m_pDlgItem->m_Button_Genese			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Button_RAZ_search		.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Edit_Search			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Tree_Rubrique_Items	.Invalidate();
			m_pDlgItem->m_Tree_Rubrique_Palettes.Invalidate();
			m_pDlgItem->m_List_Items			.Invalidate();
			UpdateListItems();
		break;

		case IDC_RI_RAD_LIST_2:
			Label.LoadString(IDS_RI_PALETTES);
			m_pDlgItem->m_Tree_Theme			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilter		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilterRaz		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Edit_QFilter			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Items	.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Palettes.ShowWindow(SW_SHOW);
			m_pDlgItem->m_List_Items			.ShowWindow(SW_SHOW);
			m_pDlgItem->ResizeListItems(false);
			m_pDlgItem->m_Button_Genese			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Button_RAZ_search		.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Edit_Search			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Tree_Rubrique_Items	.Invalidate();
			m_pDlgItem->m_Tree_Rubrique_Palettes.Invalidate();
			m_pDlgItem->m_List_Items			.Invalidate();
			UpdateListItems();
		break;
	}

	m_pDlgItem->m_Label_Theme.SetWindowText(Label);
	m_pDlgItem->m_Label_Theme.Invalidate();
}

void CConsultationMode::UpdateListItems()
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
					long Pos = m_pDlgItem->m_List_Items.AddItem(vItems.GetItem());
					m_Selected_UserItems.MoveTo(vItems.GetItem()->GetID());
					if(m_Selected_UserItems.IsValid())
						m_pDlgItem->m_List_Items.SetSel(Pos, TRUE);
				}
			}
			m_pDlgItem->m_Button_Genese.EnableWindow(FALSE);
			m_pDlgItem->m_List_Items.Invalidate();
		}
		break;
	}
}

void CConsultationMode::OnSelchange_ListItems() 
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
			JList<JInt32> Values;
			// Set the selected ID in the list box
			m_pDlgItem->m_List_Items.GetSelectedID(Values);
			m_pDlgItem->m_Button_Genese.EnableWindow(FALSE);
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
			UpdateQuestionList();
			NewValues.MoveFirst();
			if (NewValues.IsValid())
				m_pDlgItem->m_List_Questions.SetSelectedID(NewValues.GetItem());
			else
			{
				Values.MoveFirst();
				if (Values.IsValid())
					m_pDlgItem->m_List_Questions.SetSelectedID(Values.GetItem());
			}
			OnSelchange_ListQuestions();
			UpdateIBuilder(true, false);
		}
		break;
	}

	m_pDlgItem->m_Button_Genese.EnableWindow(FALSE);
}

void CConsultationMode::OnSelchange_ListQuestions()
{
	// AfxMessageBox("????", MB_ICONERROR);
	if(m_pIBsegment)
	{
		// Reset the segment to zero
		m_pIBsegment->Reset();
		m_pIBsegment->SetOperator(GetQuestionOperator());

		// Set the question pointer to zero
		m_pIBquestion = 0;
		m_pIBitem     = 0;

		m_pDlgItem->m_Static_Effectif.SetText("");
		m_pDlgItem->m_Static_NbCas   .SetText("");
		m_pDlgItem->m_Static_Percent .SetText("");
	}
	CItemMode::OnSelchange_ListQuestions(); 
}

void CConsultationMode::SetActiveSegmentQuestions()
{
	// empty the SAS
	m_pDlgItem->m_List_SAS.ResetContent();
	m_pDlgItem->m_List_SAS.Invalidate();
}

void CConsultationMode::OnContextMenu(CWnd* pWnd, CPoint point) 
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

void CConsultationMode::UpdateIBuilder(bool Changed, bool bValues)
{
	CItemMode::UpdateIBuilder(Changed, bValues);

	m_pDlgItem->m_Edit_Name    .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Accept.EnableWindow(FALSE);
}
