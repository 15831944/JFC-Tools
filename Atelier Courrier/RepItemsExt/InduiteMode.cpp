#include "StdAfx.h"
#include "InduiteMode.h"
#include "IBPoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInduiteMode::CInduiteMode(CDlgItems *pDlgItems) : CItemMode(pDlgItems)
{
	m_pQuestionInduite = 0;
}

CInduiteMode::~CInduiteMode()
{
	if(m_pQuestionInduite && !m_pQuestionInduite->IsRegistered())
		delete m_pQuestionInduite;
}

void CInduiteMode::StartMode()
{
	m_pQuestionInduite = 0;

	EnableOperators(FALSE);

	// m_pDlgItem->m_Button_Filtre     .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_UP    .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_DOWN  .EnableWindow(FALSE);
	m_pDlgItem->m_Tab_Segment       .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Genese     .EnableWindow(FALSE);
	m_pDlgItem->m_Button_List2      .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Accept     .EnableWindow(FALSE);
	m_pDlgItem->m_Button_CalcSegment.EnableWindow(FALSE);
	
	m_pDlgItem->m_Button_Dim1_OK   .ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Dimension1.ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Dimension2.ShowWindow(SW_HIDE);
	m_pDlgItem->m_Header_Dimension1.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Header_Dimension2.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Button_Genese    .ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_List2     .ShowWindow(SW_HIDE);

	m_pDlgItem->m_List_Items.ResetContent(); 
	
	CString Label;
	CString ToolTip;

	Label.LoadString(IDS_RI_QUE_INDUITES); 
	m_pDlgItem->m_Mode_Title.SetWindowText(Label);
	m_pDlgItem->m_Mode_Title.Invalidate();

	Label.LoadString(IDS_RI_COMPTER);
	m_pDlgItem->m_Button_Dim1_OK.SetWindowText(Label);
	ToolTip.LoadString(IDS_RI_TT_COMPTER);
	m_pDlgItem->m_Button_Dim1_OK.SetTooltipText(ToolTip);
	m_pDlgItem->m_Button_Dim1_OK.Invalidate();

	Label.LoadString(IDS_RI_INDUITES);
	m_pDlgItem->m_Button_List1.SetWindowText(Label);
	ToolTip.LoadString(IDS_RI_TT_INDUITES); 
	m_pDlgItem->m_Button_List1.SetTooltipText(ToolTip);

	ToolTip.LoadString(IDS_RI_TT_VALID_INDUITE); 
	m_pDlgItem->m_Button_Accept.SetTooltipText(ToolTip);

	m_pDlgItem->CheckRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2,IDC_RI_RAD_QUESTIONS);
	UpdateTheme(IDC_RI_RAD_QUESTIONS);
	UpdateListItems();

	m_IBuilder.Reset();
	m_IBuilder.SetOperator(GetSegmentOperator());
	FindSet_IBsegment(0);
	m_pDlgItem->m_List_Questions.SetCurSel(LB_ERR); 
	OnSelchange_ListQuestions();
}

void CInduiteMode::StopMode()
{
	if(m_pQuestionInduite && !m_pQuestionInduite->IsRegistered())
		delete m_pQuestionInduite;
	m_pQuestionInduite = 0;
	CItemMode::StopMode();
}

void CInduiteMode::OnSelchange_ListQuestions()
{
	// AfxMessageBox("????", MB_ICONERROR);

	if(m_pIBsegment)
	{
		// Reset the segment to zero
		m_pIBsegment->Reset();
		m_pIBsegment->SetOperator(GetQuestionOperator());
		// Set the question pointer to zero
		m_pIBquestion = 0;
		m_pIBitem = 0;
	}
	CItemMode::OnSelchange_ListQuestions(); 
}

void CInduiteMode::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	switch(pWnd->GetDlgCtrlID())
	{
		case IDC_RI_LIST_IITEMS:
		{
			// Affichage menu contextuel pour liste des items questions induites
			CItemMode::OnContextMenu(pWnd, point);
		}
		break;

		case IDC_RI_LIST_DIMENSION1:
		{
			// Affichage menu contextuel pour dimensions questions induites
			OnContextMenuDim1(pWnd, point);
		}
		break;
	}
}


// Gestion menu contextuel si on est dans la fenêtre dimension 1
void CInduiteMode::OnContextMenuDim1(CWnd* pWnd, CPoint point) 
{
	INT IdxItem = m_pDlgItem->m_List_Dimension1.GetCaretIndex();
	if (IdxItem != LB_ERR)
	{
		const IModalite* pModalite = (const IModalite*)m_pDlgItem->m_List_Dimension1.GetItemData(IdxItem);
		if (pModalite != 0)
		{
			// Menu contextuel Renommer ssi 1 sélection ou au moins 1 selection en mode question induite
			if (m_pDlgItem->m_List_Dimension1.GetSelCount() == 1 || (m_pDlgItem->ValCurMode() == 6 && m_pDlgItem->m_List_Dimension1.GetSelCount() >= 1) )
			{
				CMenu Menu;
				CString Label;
				Menu.CreatePopupMenu();

				if (m_pSelectedQuestion)
				{
					// Si on n'est dans le ss-mode Quest Induite
					if (m_pSelectedQuestion->IsQuestionInduite())
					{
						// si 1 seule sélection
						if (m_pDlgItem->m_List_Dimension1.GetSelCount() == 1)
						{
							// Option Renommer
							Label.LoadString(IDS_RI_RENOMMER); 
							Menu.AppendMenu(MF_STRING,IDM_RI_RENAME_DIMENSION1,Label);
						}
						else
						// Plusieurs sélections >> uniquement menu remplacer
						{
							// Option Remplacement
							Label.LoadString(IDS_RI_REMPLACER); 
							Menu.AppendMenu(MF_STRING,IDM_RI_REMPLACER_DIMENSION1,Label);
						}
					}
					// Si on n'est dans le ss-mode Questions et comptage fait
					else if (m_pSelectedQuestion->IsQuestion())
					{
						
					}
				}

				switch(Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY,point.x,point.y,m_pDlgItem))
				{
					case IDM_RI_RENAME_DIMENSION1:
						RenameDimension1(pModalite->GetID());
					break;

					// Remplacer une chaine de carctère sur un ou plusieurs élmts
					case IDM_RI_REMPLACER_DIMENSION1:
					{	
						// Récupère la liste des élmts sélectionnés
						JList <JInt32> ListIDs;
						m_pDlgItem->m_List_Dimension1.GetSelectedID(ListIDs);

						// Lance la boite de dialogue remplacement texte, sur tout
						m_pDlgItem->RemplacerStrItem(ListIDs); 
					}
					break;
				}
			}
		}
	}
	pWnd->InvalidateRect(NULL, TRUE);
	pWnd->UpdateWindow();
}

bool CInduiteMode::OnAccept(CString &Name) 
{
	if(m_pQuestionInduite)
	{
		if(!m_pQuestionInduite->IsRegistered())
		{
			JLabel label = Name;
			if(m_pQuestionInduite->SetLabel(label))
			{
				if(m_pDlgItem->GetItems()->RegisterQuestionInduite(m_pQuestionInduite))
					delete m_pQuestionInduite;
				else
				{
					m_pDlgItem->MessageBox("Impossible d'enregistrer la question induite!",m_pDlgItem->GetTitle(),MB_OK);
					return false;
				}
			}
			else
			{
				m_pDlgItem->MessageBox("Le libellé de la question induite est invalide !",m_pDlgItem->GetTitle(),MB_OK);
				return false;
			}
		}
		m_pQuestionInduite = 0;
		UpdateListItems();
		Name.Empty();
		return true;
	}
	return false;
}

void CInduiteMode::OnDim_1_Ok()
{
	if(m_pQuestionInduite)
		return;
	if(!m_pSelectedQuestion || !m_pIBquestion)
		return;
	if(!m_bItemValid || !m_pIBquestion->IsValid())
	{
		CString Text;
		Text.LoadString(IDS_RI_INDUITE_ERROR);
		m_pDlgItem->MessageBox(Text, m_pDlgItem->GetTitle(), MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	// Get and set the selected modalities
	int x             = 0;
	int PossibleCount = 1;
	JList< CIBModalite *> pIModalite;
	m_pIBquestion->GetModalites(pIModalite);
	for(x = 0, pIModalite.MoveFirst(); pIModalite.IsValid() ; pIModalite.MoveNext(), x++)
	{
		// Get the modalities used
		CIBModalite * pModalite = pIModalite.GetItem();
		JList<JInt32> Values;
		pModalite->GetValues(Values);
		PossibleCount*=Values.GetCount();
	}

	if (PossibleCount > 0)
	{
		int Mini = PossibleCount; if (Mini > 10) Mini = 10;

		m_pDlgItem->m_Edit_Name.SetWindowText(m_pSelectedQuestion->GetLabel().AsJCharPtr());

		// Create new question induite
		m_pQuestionInduite = m_pDlgItem->GetItems()->GetNewQuestionInduite();
		m_pQuestionInduite->SetLabel(m_pSelectedQuestion->GetLabel(),true);
		// Set the Question Bloc
		m_pQuestionInduite->SetQuestionBlocId(m_pSelectedQuestion->GetBlocID());
		for(x=0; x<=Mini; x++)
		{
			// Build the Item with all questions
			IBloc::OPERATOR Op = IBloc::EQ; 
			CString ItemName;
			if (x == Mini)
			{
				if (x > 1) ItemName.Format("%d réponses et +", x);
				else       ItemName.Format("%d réponse et +",  x);
				Op = IBloc::GTE;
			}
			else if (x > 0)
			{
				if (x > 1) ItemName.Format("%d réponses", x);
				else       ItemName.Format("%d réponse",  x);
			}
			else
				ItemName.Format("Aucune réponse");

			CIBPoint * pPoint = new CIBPoint();
			pPoint->SetOperator(Op);
			pPoint->SetValue(x);

			if(m_pIBquestion)
				m_pIBquestion->SetPoint(pPoint);
			else if(m_pIBitem)
				m_pIBitem->SetPoint(pPoint);

			JLabel Lbl = ItemName;
			// Create the Target Here and Add it to the critere induite
			CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
			pItem->SetLabel(Lbl,true);
			// Set the builder object
			if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),m_IBuilder))
				m_pQuestionInduite->AddItem(pItem); 
			else
				delete pItem;
		}

		m_IBuilder.Reset();
		m_IBuilder.SetOperator(GetSegmentOperator());
		FindSet_IBsegment(0);
		// Update the selection list
		OnSelchange_ListQuestions();
	}
}

void CInduiteMode::OnTout_Raz()
{
	m_pDlgItem->m_Tree_Theme.RemoveSelections();
	m_Selected_UserItems.Reset();
	if(m_pPseudoItem)
	{
		m_pDlgItem->m_List_Questions.RemoveItem(m_pPseudoItem->GetID());
		delete m_pPseudoItem;
		m_pPseudoItem = 0;
	}
	OnItem_Raz();
}

void CInduiteMode::OnItem_Raz()
{
	// m_pDlgItem->m_List_Questions.ResetContent();
	// Initialise boite liste des questions
	m_pDlgItem->InitListQuestion();

	OnSAS_Raz();
}

void CInduiteMode::OnSAS_Raz()
{
	//m_Elem_Sel.Reset();
	m_Dim_Sel.Reset();

	if(m_pQuestionInduite)
	{
		if(!m_pQuestionInduite->IsRegistered())
			delete m_pQuestionInduite;
		m_pQuestionInduite = 0;

		//m_pDlgItem->m_List_Questions.ResetContent();
		// Initialise boite liste des questions
		m_pDlgItem->InitListQuestion();
	}

	m_IBuilder.Reset();
	m_IBuilder.SetOperator(GetSegmentOperator());
	FindSet_IBsegment(0);
	// No current question
	m_pIBquestion = 0;
	// No current items
	m_pIBitem = 0;

	m_pDlgItem->m_List_Questions.SetCurSel(LB_ERR); 
	OnSelchange_ListQuestions();
	UpdateIBuilder(true, false);
}

// Suppression d'un segment
void CInduiteMode::OnSegment_Raz()
{
	// Idem Supprression SAS
	OnSAS_Raz();
}

void CInduiteMode::OnSelchange_ListItems() 
{
	switch(m_pDlgItem->GetCheckedRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2))
	{
		case IDC_RI_RAD_LIST_1:
		{
			JList<JInt32> Values;
			// Set the selected ID in the list box
			m_pDlgItem->m_List_Items.GetSelectedID(Values);

			if(m_pIBquestion && !m_pIBquestion->IsValid())
			{
				m_pIBsegment->DeleteQuestion(m_pIBquestion);
				m_pIBquestion = 0;
			}

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
			JInt32 ID = m_pDlgItem->m_List_Questions.GetSelectedID();
			NewValues.MoveFirst();
			if(ID!=LB_ERR) {
				if(NewValues.IsValid() && !m_pDlgItem->GetItems()->GetQuestionInduite(ID))
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

		case IDC_RI_RAD_LIST_2:
		{
		}
		break;
	}
}

void CInduiteMode::UpdateListItems()
{
	// Get the selected question
	JList<JInt32> IDs;
	m_pDlgItem->m_List_Items.GetSelectedID(IDs);   
	m_pDlgItem->m_List_Items.ResetContent();

	// Get the list of items
	JList<IItem const *> ItemList;
	m_pDlgItem->GetItems()->GetQuestionsInduiteByBlocID(ItemList,0,m_pDlgItem->GetTerrain());
	for (ItemList.MoveFirst(); ItemList.IsValid(); ItemList.MoveNext())
	{
		if (IsSearched(ItemList.GetItem()))
			m_pDlgItem->m_List_Items.AddItem(ItemList.GetItem());
	}
	m_pDlgItem->m_List_Items.SetSelectedID(IDs); 

	// Redraw
	m_pDlgItem->m_List_Items.Invalidate();
}

void CInduiteMode::UpdateTheme(int ID)
{
	CString Label = "";

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
			m_pDlgItem->m_Tree_Rubrique_Items	.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE);
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

		case IDC_RI_RAD_LIST_2:
			Label.LoadString(IDS_RI_INDUITES);
			m_pDlgItem->m_Tree_Theme			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilter		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilterRaz		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Edit_QFilter			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Items	.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE);
			m_pDlgItem->m_List_Items			.ShowWindow(SW_SHOW);
			m_pDlgItem->ResizeListItems(false);
			m_pDlgItem->m_Button_Genese			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Button_RAZ_search		.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Edit_Search			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Tree_Rubrique_Items	.Invalidate();
			m_pDlgItem->m_Tree_Rubrique_Palettes.Invalidate();
			m_pDlgItem->m_List_Items			.Invalidate();
		break;
	}
	m_pDlgItem->m_Label_Theme.SetWindowText(Label);
	m_pDlgItem->m_Label_Theme.Invalidate();
}

void CInduiteMode::UpdateQuestionList(long BlockId)
{
	int TopBlock = 0;

	// Get the selected question
	JInt32 IDs = m_pDlgItem->m_List_Questions.GetSelectedID();   

	JList<IQuestion const *> & Question_List = m_pDlgItem->m_Tree_Theme.GetQuestionList();  

	m_pDlgItem->m_List_Questions .ResetContent();
	m_pDlgItem->m_List_Dimension1.ResetContent();
	m_pDlgItem->m_List_Dimension2.ResetContent(); 

	// m_pDlgItem->m_Button_Filtre.EnableWindow  (FALSE);
	m_pDlgItem->m_Button_Dim1_UP.EnableWindow  (FALSE);
	m_pDlgItem->m_Button_Dim1_DOWN.EnableWindow(FALSE);

	// Add the critere modifié
	if(m_pQuestionInduite)
	{
		m_pDlgItem->m_List_Questions.AddItem(m_pQuestionInduite);
		IDs = m_pQuestionInduite->GetID();
	}
	else
	{
		// Find the questions coresponding to the current mode
		for(Question_List.MoveFirst();Question_List.IsValid(); Question_List.MoveNext())
		{
			IQuestion const * pQuestion = Question_List.GetItem();
			if(pQuestion->IsBlocQuestion())
			{
				int Pos = m_pDlgItem->m_List_Questions.AddItem(pQuestion);
				if(BlockId != 0 && BlockId == pQuestion->GetBlocID())
					TopBlock = Pos;
			}
			else if(pQuestion->IsQuestion() && pQuestion->IsInContingent(m_pDlgItem->GetContingentMask()))
			{
				m_pDlgItem->m_List_Questions.AddItem(pQuestion);
			}
		}

		if(m_Selected_UserItems.GetCount())
			m_pDlgItem->m_List_Questions.AddItem(&m_pDlgItem->m_BlocQuestion); 

		for(m_Selected_UserItems.MoveFirst();m_Selected_UserItems.IsValid(); m_Selected_UserItems.MoveNext())
		{
			m_pDlgItem->m_List_Questions.AddItem(m_pDlgItem->GetItems()->GetQuestionByID(m_Selected_UserItems.GetKey()));
		}
	}

	m_pDlgItem->m_List_Questions.SetSelectedID(IDs); 
	int CurSel = m_pDlgItem->m_List_Questions.GetCurSel();
	if(BlockId == 0)
		m_pDlgItem->m_List_Questions.SetTopIndex(CurSel);
	else
		m_pDlgItem->m_List_Questions.SetTopIndex(TopBlock);

	m_pSelectedQuestion = 0;

	// Bidouille Correction problème affichage (Alain 8/07/2013)
	m_pDlgItem->m_List_Questions.ResizeList();

	SetActiveSegmentQuestions(); 

	// Redraw
	m_pDlgItem->m_List_Questions.Invalidate();
}

void CInduiteMode::SetActiveSegmentQuestions()
{
	// empty the SAS
	m_pDlgItem->m_List_SAS.ResetContent();
	m_pDlgItem->m_List_SAS.Invalidate();
}

void CInduiteMode::UpdateFiltreDimension1List()
{
	CItemMode::UpdateFiltreDimension1List();
}

void CInduiteMode::UpdateDimensionsList()
{
	CItemMode::UpdateDimensionsList();

	for(m_DimDlg.MoveFirst(); m_DimDlg.IsValid(); m_DimDlg.MoveNext())
		m_DimDlg.GetItem().EnableOperators(FALSE);

	if(m_pSelectedQuestion && m_pSelectedQuestion->IsQuestionInduite())
	{
		m_pDlgItem->m_Button_Dim1_OK   .ShowWindow(SW_HIDE);
		m_pDlgItem->m_Header_Dimension1.ShowWindow(SW_SHOW);
	}
	else if(m_DimType_New.GetCount() > 0)
	{
		m_pDlgItem->m_Header_Dimension1.ShowWindow(SW_HIDE);
		m_pDlgItem->m_Button_Dim1_OK   .ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlgItem->m_Button_Dim1_OK   .ShowWindow(SW_HIDE);
		m_pDlgItem->m_Header_Dimension1.ShowWindow(SW_SHOW);
	}
}

void CInduiteMode::UpdateIBuilder(bool Changed, bool bValues)
{
	CItemMode::UpdateIBuilder(Changed, bValues);

	if(!m_pQuestionInduite)
	{
		m_pDlgItem->m_Edit_Name    .EnableWindow(FALSE);
		m_pDlgItem->m_Button_Accept.EnableWindow(FALSE);
	}
	else
	{
		m_pDlgItem->m_Edit_Name    .EnableWindow(TRUE);
		m_pDlgItem->m_Button_Accept.EnableWindow(TRUE);
	}
}
