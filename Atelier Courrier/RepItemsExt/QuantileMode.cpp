#include "stdafx.h"
#include "QuantileMode.h"
#include "DlgLibelle.h"
#include "IBQuant.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuantileMode::CQuantileMode(CDlgItems *pDlgItems) : CItemMode(pDlgItems)
{
	m_pCritereQuantile = 0;
}

CQuantileMode::~CQuantileMode()
{
	if(m_pCritereQuantile && !m_pCritereQuantile->IsRegistered())
		delete m_pCritereQuantile;
}

void CQuantileMode::StartMode()
{
	m_pCritereQuantile = 0;

	EnableOperators(FALSE);

	// m_pDlgItem->m_Button_Filtre		.EnableWindow(FALSE);
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

	Label.LoadString(IDS_RI_QUANTILES); 
	m_pDlgItem->m_Mode_Title.SetWindowText(Label);
	m_pDlgItem->m_Mode_Title.Invalidate(); 

	Label.LoadString(IDS_RI_QUANTILES);
	m_pDlgItem->m_Button_List1.SetWindowText(Label);
	ToolTip.LoadString(IDS_RI_TT_QUANTILES); 
	m_pDlgItem->m_Button_List1.SetTooltipText(ToolTip);
	m_pDlgItem->m_Button_List1.Invalidate();

	ToolTip.LoadString(IDS_RI_TT_VALID_QUANTILE); 
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

void CQuantileMode::StopMode()
{
	if(m_pCritereQuantile && !m_pCritereQuantile->IsRegistered())
		delete m_pCritereQuantile;
	m_pCritereQuantile = 0;
	CItemMode::StopMode();
}

void CQuantileMode::OnSelchange_ListQuestions()
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

void CQuantileMode::OnContextMenu(CWnd* pWnd, CPoint point)
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

bool CQuantileMode::OnAccept(CString &Name) 
{
	if(m_pCritereQuantile)
	{
		if(!m_pCritereQuantile->IsRegistered())
		{
			JLabel label = Name;
			if(m_pCritereQuantile->SetLabel(label))
			{
				if(m_pDlgItem->GetItems()->RegisterCritereQuantile(m_pCritereQuantile))
					delete m_pCritereQuantile;
				else
				{
					m_pDlgItem->MessageBox("Impossible d'enregistrer le quantile!",m_pDlgItem->GetTitle(),MB_OK);
					return false;
				}
			}
			else
			{
				m_pDlgItem->MessageBox("Le libellé du quantile est invalide !",m_pDlgItem->GetTitle(),MB_OK);
				return false;
			}
		}
		m_pCritereQuantile = 0;
		UpdateListItems();
		Name.Empty();
		return true;
	}
	return false;
}

void CQuantileMode::OnValuesQuantile(LONG Type)
{
	if(m_pCritereQuantile)
		return;
	if(!m_pSelectedQuestion || !m_pIBquestion)
		return;
	if(!m_bItemValid || !m_pIBquestion->IsValid())
	{
		CString Text;
		Text.LoadString(IDS_RI_QUANTILE_ERROR);
		m_pDlgItem->MessageBox(Text, m_pDlgItem->GetTitle(), MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	// Set the selected ID in the list box
	JList<JInt32> Values;
	m_pDlgItem->m_List_Dimension1.GetSelectedID(Values);
	if(Values.GetCount() > 0)
	{
		if(m_pSelectedQuestion->IsQuestion() && m_pSelectedQuestion->AsQuestion()->HasValues())
		{
			if      (Type ==  3) Tercile();
			else if (Type ==  4) Quartile();
			else if (Type ==  5) Quintile();
			else if (Type == 10) Decile();
		}
	}
}

void CQuantileMode::OnTout_Raz()
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

void CQuantileMode::OnItem_Raz()
{
	// m_pDlgItem->m_List_Questions.ResetContent();
	// Initialise boite liste des questions
	m_pDlgItem->InitListQuestion();

	OnSAS_Raz();
}

void CQuantileMode::OnSAS_Raz()
{
	m_Dim_Sel.Reset();

	if(m_pCritereQuantile)
	{
		if(!m_pCritereQuantile->IsRegistered())
			delete m_pCritereQuantile;
		m_pCritereQuantile = 0;

		// m_pDlgItem->m_List_Questions.ResetContent();
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

//////////////////////////////////////////////////////////////
// Suppression d'un segment
void CQuantileMode::OnSegment_Raz()
{
	// Idem Supprression SAS
	OnSAS_Raz();

	// ++ Mise à jour segment actif
}

void CQuantileMode::OnSelchange_ListItems() 
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
				if(NewValues.IsValid() && !m_pDlgItem->GetItems()->GetCritereQuantile(ID))
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

void CQuantileMode::UpdateListItems()
{
	// Get the selected question
	JList<JInt32> IDs;
	m_pDlgItem->m_List_Items.GetSelectedID(IDs);   
	m_pDlgItem->m_List_Items.ResetContent();

	// Get the list of items
	JList<IItem const *> ItemList;
	m_pDlgItem->GetItems()->GetCriteresQuantileByBlocID(ItemList,0,m_pDlgItem->GetTerrain());
	for (ItemList.MoveFirst(); ItemList.IsValid(); ItemList.MoveNext())
	{
		if (IsSearched(ItemList.GetItem()))
			m_pDlgItem->m_List_Items.AddItem(ItemList.GetItem());
	}
	m_pDlgItem->m_List_Items.SetSelectedID(IDs); 

	// Redraw
	m_pDlgItem->m_List_Items.Invalidate();
}

void CQuantileMode::UpdateTheme(int ID)
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
			Label.LoadString(IDS_RI_QUANTILES);
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
			m_pDlgItem->m_Tree_Theme			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilter		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilterRaz		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Edit_QFilter			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Items	.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Palettes.ShowWindow(SW_HIDE);
			m_pDlgItem->m_List_Items			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_Genese			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_RAZ_search		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Edit_Search			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Items	.Invalidate();
			m_pDlgItem->m_Tree_Rubrique_Palettes.Invalidate();
			m_pDlgItem->m_List_Items			.Invalidate();
		break;
	}
	m_pDlgItem->m_Label_Theme.SetWindowText(Label);
	m_pDlgItem->m_Label_Theme.Invalidate();
}

void CQuantileMode::UpdateQuestionList(long BlockId)
{
	int TopBlock = 0;

	// Get the selected question
	JInt32 IDs = m_pDlgItem->m_List_Questions.GetSelectedID();   

	JList<IQuestion const *> & Question_List = m_pDlgItem->m_Tree_Theme.GetQuestionList();  

	m_pDlgItem->m_List_Questions .ResetContent();
	m_pDlgItem->m_List_Dimension1.ResetContent();
	m_pDlgItem->m_List_Dimension2.ResetContent(); 

	// Initialise boite liste des questions
	m_pDlgItem->InitListQuestion();

	// m_pDlgItem->m_Button_Filtre.EnableWindow  (FALSE);
	m_pDlgItem->m_Button_Dim1_UP.EnableWindow  (FALSE);
	m_pDlgItem->m_Button_Dim1_DOWN.EnableWindow(FALSE);
	
	// Add the critere modifié
	if(m_pCritereQuantile)
	{
		m_pDlgItem->m_List_Questions.AddItem(m_pCritereQuantile);
		IDs = m_pCritereQuantile->GetID();
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
				CQuestion const *pQ = pQuestion->AsQuestion();
				if(pQ->HasValues()) 
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

void CQuantileMode::SetActiveSegmentQuestions()
{
	// empty the SAS
	m_pDlgItem->m_List_SAS.ResetContent();
	m_pDlgItem->m_List_SAS.Invalidate();
}

void CQuantileMode::UpdateFiltreDimension1List()
{
	CItemMode::UpdateFiltreDimension1List();
}

void CQuantileMode::UpdateDimensionsList()
{
	CItemMode::UpdateDimensionsList();

	for(m_DimDlg.MoveFirst(); m_DimDlg.IsValid(); m_DimDlg.MoveNext())
		m_DimDlg.GetItem().EnableOperators(FALSE);
}

bool CQuantileMode::UpdateItemValues(bool bValues)
{
	bool Ret = CItemMode::UpdateItemValues(bValues);
	if (m_pValuesDlg)
		m_pValuesDlg->SetQuantileMode();
	return (Ret);
}

void CQuantileMode::UpdateIBuilder(bool Changed, bool bValues)
{
	CItemMode::UpdateIBuilder(Changed,bValues);

	if(!m_pCritereQuantile)
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

void CQuantileMode::Tercile()
{
	m_pDlgItem->m_Edit_Name.SetWindowText(m_pSelectedQuestion->GetLabel().AsJCharPtr());

	// Create new critere modifié
	m_pCritereQuantile = m_pDlgItem->GetItems()->GetNewCritereQuantile();
	m_pCritereQuantile->SetLabel(m_pSelectedQuestion->GetLabel(),true); 
	// Set the Question Bloc
	m_pCritereQuantile->SetQuestionBlocId(m_pSelectedQuestion->GetBlocID());

	CreateItems(m_pCritereQuantile,3);

	m_IBuilder.Reset();
	m_IBuilder.SetOperator(GetSegmentOperator());
	FindSet_IBsegment(0);
	OnSelchange_ListQuestions();
}

void CQuantileMode::Quartile()
{
	// Positionne texte quartile
	JString Label = m_pSelectedQuestion->GetLabel().AsJCharPtr();

	m_pDlgItem->m_Edit_Name.SetWindowText(m_pSelectedQuestion->GetLabel().AsJCharPtr());

	// Create new critere modifié
	m_pCritereQuantile = m_pDlgItem->GetItems()->GetNewCritereQuantile();
	m_pCritereQuantile->SetLabel(m_pSelectedQuestion->GetLabel(),true); 
	// Set the Question Bloc
	m_pCritereQuantile->SetQuestionBlocId(m_pSelectedQuestion->GetBlocID());

	CreateItems(m_pCritereQuantile,4);

	m_IBuilder.Reset();
	m_IBuilder.SetOperator(GetSegmentOperator());
	FindSet_IBsegment(0);
	OnSelchange_ListQuestions();
}

void CQuantileMode::Quintile()
{
	m_pDlgItem->m_Edit_Name.SetWindowText(m_pSelectedQuestion->GetLabel().AsJCharPtr());

	// Create new critere modifié
	m_pCritereQuantile = m_pDlgItem->GetItems()->GetNewCritereQuantile();
	m_pCritereQuantile->SetLabel(m_pSelectedQuestion->GetLabel(),true); 
	// Set the Question Bloc
	m_pCritereQuantile->SetQuestionBlocId(m_pSelectedQuestion->GetBlocID());

	CreateItems(m_pCritereQuantile,5);

	m_IBuilder.Reset();
	m_IBuilder.SetOperator(GetSegmentOperator());
	FindSet_IBsegment(0);
	// Update the selection list
	OnSelchange_ListQuestions();
}

void CQuantileMode::Decile()
{
	m_pDlgItem->m_Edit_Name.SetWindowText(m_pSelectedQuestion->GetLabel().AsJCharPtr());

	// Create new critere modifié
	m_pCritereQuantile = m_pDlgItem->GetItems()->GetNewCritereQuantile();
	m_pCritereQuantile->SetLabel(m_pSelectedQuestion->GetLabel(),true); 
	// Set the Question Bloc
	m_pCritereQuantile->SetQuestionBlocId(m_pSelectedQuestion->GetBlocID());

	CreateItems(m_pCritereQuantile,10);

	m_IBuilder.Reset();
	m_IBuilder.SetOperator(GetSegmentOperator());
	FindSet_IBsegment(0);
	OnSelchange_ListQuestions();
}

void CQuantileMode::CreateItems(CCritereQuantile *pQuantile,int Quantile)
{
	if(m_IBuilder.IsValid())
	{
		CWaitCursor Wait;
//		for(int x = 0; x <= Quantile; x++) // Avec la classe "Sans Valeur"
		for(int x = 1; x <= Quantile; x++) // Sans la classe "Sans Valeur"
		{
			CIBuilder		Builder(m_IBuilder);
			CIBSegment *	pSegment	= Builder.GetSegment(0);
			if(!pSegment)
				return;
			CIBQuestion *	pQuestion = pSegment->FindQuestion(m_pIBquestion->GetID());
			if(!pQuestion)
				return;

			CIBQuant * pQuant = new CIBQuant;
			pQuant->AddValue(x);
			pQuant->AddValue(Quantile);

			pQuestion->SetQuant(pQuant);
			// Get a new item construit
			CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
			// Set the builder object to the item
			pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),Builder);
			// Test is the current target is valid
			if(pItem->IsValid(m_pDlgItem->GetTerrain()))
			{
				CString sLabel;
				if(x)
				{
					switch(Quantile)
					{
						case 3: // PMG
						{
							switch(x)
							{
								case 1: sLabel.Format(IDS_RI_FMT_PETIT); break;
								case 2: sLabel.Format(IDS_RI_FMT_MOYEN); break;
								case 3: sLabel.Format(IDS_RI_FMT_GROS); break;
							}
						}
						break;

						case 4: // Quartile
						{
							sLabel.Format(IDS_RI_FMT_QUARTILE,x);
						}
						break;

						case 5: // Quintile
						{
							sLabel.Format(IDS_RI_FMT_QUINTILE,x);
						}
						break;

						case 10: // Décile
						{
							sLabel.Format(IDS_RI_FMT_DECILE,x);
						}
						break;

						default: // Quantile
						{
							sLabel.Format(IDS_RI_FMT_QUANTILE,x);
						}
						break;
					}
				}
				else
					sLabel.LoadString(IDS_RI_FMT_SANSVALEUR);

				if(x > 0)
				{
					// calculate the target
					CCalcItemParams Params;
					Params.SetTerrain(m_pDlgItem->GetTerrain());
					Params.SetEchantillonID(1);
					Params.SetUniversID(m_pDlgItem->GetTerrain()->GetDefaultUnivers());
					Params.SetTypePoidsID(m_pDlgItem->GetTypePoids());
					Params.SetPopulationSegment(m_pDlgItem->GetPopulationSegment());
					Params.SetSegmentsIDs(m_pDlgItem->GetSegments());
					if(pItem->CalculateEffectif(&Params))
						sLabel.AppendFormat(" (%.0f..%.0f)", (double)Params.GetMinBorne(), (double)Params.GetMaxBorne());
				}
				JLabel Label = sLabel;
				pItem->SetLabel(Label,true);
				// Add the item to the Critere modifié
				pQuantile->AddItem(pItem);
			}
			else
			{
				delete pItem;
				return;
			}
		}
	}
}
