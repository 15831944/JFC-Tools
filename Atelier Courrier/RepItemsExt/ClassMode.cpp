#include "stdafx.h"
#include "ClassMode.h"
#include "DlgLibelle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClassMode::CClassMode(CDlgItems *pDlgItems) : CItemMode(pDlgItems)
{
	m_pCritereModifie = 0;
}

CClassMode::~CClassMode()
{
	if(m_pCritereModifie && !m_pCritereModifie->IsRegistered())
		delete m_pCritereModifie;
}

void CClassMode::StartMode()
{
	m_pCritereModifie = 0;
	
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

	Label.LoadString(IDS_RI_REGROUPE_CLASSES); 
	m_pDlgItem->m_Mode_Title.SetWindowText(Label);
	m_pDlgItem->m_Mode_Title.Invalidate();

	Label.LoadString(IDS_RI_GROUPER);
	m_pDlgItem->m_Button_Dim1_OK.SetWindowText(Label);
	ToolTip.LoadString(IDS_RI_TT_GROUPER);
	m_pDlgItem->m_Button_Dim1_OK.SetTooltipText(ToolTip);
	m_pDlgItem->m_Button_Dim1_OK.Invalidate();

	Label.LoadString(IDS_RI_CLASSES);
	m_pDlgItem->m_Button_List1.SetWindowText(Label);
	ToolTip.LoadString(IDS_RI_TT_CLASSES); 
	m_pDlgItem->m_Button_List1.SetTooltipText(ToolTip);
	m_pDlgItem->m_Button_List1.Invalidate();

	ToolTip.LoadString(IDS_RI_TT_VALID_CLASSE); 
	m_pDlgItem->m_Button_Accept.SetTooltipText(ToolTip);

	UpdateTheme(IDC_RI_RAD_QUESTIONS);
	UpdateListItems();

	m_IBuilder.Reset();
	m_IBuilder.SetOperator(GetSegmentOperator());
	FindSet_IBsegment(0);
	m_pDlgItem->m_List_Questions.SetCurSel(LB_ERR); 
	OnSelchange_ListQuestions();
}

void CClassMode::StopMode()
{
	if(m_pCritereModifie && !m_pCritereModifie->IsRegistered())
		delete m_pCritereModifie;
	m_pCritereModifie = 0;
	CItemMode::StopMode();
}

void CClassMode::OnSelchange_ListQuestions()
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

void CClassMode::OnContextMenu(CWnd* pWnd, CPoint point) 
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

bool CClassMode::OnAccept(CString &Name) 
{
	m_IdCritereModif = 0;
	if(m_pCritereModifie)
	{
		if(!m_pCritereModifie->IsRegistered())
		{
			JLabel label = Name;
			if(m_pCritereModifie->SetLabel(label))
			{
				const CCritereModifie *pData = m_pDlgItem->GetItems()->RegisterCritereModifie(m_pCritereModifie);
				if(pData)
				{
					m_IdCritereModif = pData->GetID();
					delete m_pCritereModifie;
				}
				else
				{
					m_pDlgItem->MessageBox("Impossible d'enregistrer le critère modifié !",m_pDlgItem->GetTitle(),MB_OK);
					return false;
				}
			}
			else
			{
				m_pDlgItem->MessageBox("Le libellé du critère modifié est invalide !",m_pDlgItem->GetTitle(),MB_OK);
				return false;
			}
		}
		m_pCritereModifie = 0;
		UpdateListItems();
		Name.Empty();
		return true;
	}
	return false;
}

void CClassMode::OnDim_1_Ok()
{
	if(!m_pSelectedQuestion || (!m_pIBquestion && !m_pCritereModifie))
		return;
	if(!m_bItemValid || (m_pIBquestion && !m_pIBquestion->IsValid()))
	{
		CString Text;
		Text.LoadString(IDS_RI_GROUPER_ERROR);
		m_pDlgItem->MessageBox(Text, m_pDlgItem->GetTitle(), MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	// Set the selected ID in the list box
	JList<const IModalite *> Values;
	m_pDlgItem->m_List_Dimension1.GetSelectedItems(Values);
	if(Values.GetCount() > 0)
	{
		if(m_pSelectedQuestion->IsQuestion() || m_pSelectedQuestion->IsCritereModifie())
		{
			if(m_pSelectedQuestion->IsCritereModifie() && m_pSelectedQuestion->AsCritereModifie()->IsRegistered())
				return;
			GroupDimension1();
		}
	}
}

void CClassMode::OnTout_Raz()
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

void CClassMode::OnItem_Raz()
{
	// m_pDlgItem->m_List_Questions.ResetContent();
	// Initialise boite liste des questions
	m_pDlgItem->InitListQuestion();
	
	OnSAS_Raz();
}

void CClassMode::OnSAS_Raz()
{
	m_Dim_Sel.Reset();

	if(m_pCritereModifie)
	{
		if(!m_pCritereModifie->IsRegistered())
			delete m_pCritereModifie;
		m_pCritereModifie = 0;

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

void CClassMode::OnSegment_Raz()
{
	// Idem Supprression SAS
	OnSAS_Raz();
}

void CClassMode::OnSelchange_ListItems() 
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
				if(NewValues.IsValid() && !m_pDlgItem->GetItems()->GetCritereModifie(ID))
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

///////////////////////////////////////////////////////////////////////////
// Modifiaction sélection dans la liste des classes
void CClassMode::SelChangeListClasse()
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
		if(NewValues.IsValid() && !m_pDlgItem->GetItems()->GetCritereModifie(ID))
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

////////////////////////////////////////////////////////////////////////////////////////////
// Ajoute nvlle classe
void CClassMode::AddNewClasse(JInt32 IdNewClasse)
{
	// Liste des items déjà existants
	JList<JInt32> Values;
	m_pDlgItem->m_List_Items.GetSelectedID(Values);

	// Reconstruction de la liste des items
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

	// Ajout nv ident classe
	Selected_UserItems.MoveTo(IdNewClasse);
	if (!Selected_UserItems.IsValid())
		Selected_UserItems.Add(IdNewClasse) = true; 

	// Permute pour sauver dans sélection courante
	m_Selected_UserItems.Swap(Selected_UserItems);

	UpdateQuestionList();
	JInt32 ID = m_pDlgItem->m_List_Questions.GetSelectedID();
	NewValues.MoveFirst();
	if(ID!=LB_ERR) {
		if(NewValues.IsValid() && !m_pDlgItem->GetItems()->GetCritereModifie(ID))
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

void CClassMode::UpdateListItems()
{
	// Get the selected question
	JList<JInt32> IDs;
	m_pDlgItem->m_List_Items.GetSelectedID(IDs);   
	m_pDlgItem->m_List_Items.ResetContent();

	// Get the list of items
	JList<IItem const *> ItemList;
	m_pDlgItem->GetItems()->GetCriteresModifieByBlocID(ItemList,0,m_pDlgItem->GetTerrain());
	for (ItemList.MoveFirst(); ItemList.IsValid(); ItemList.MoveNext())
	{
		// @@@@@@@@@@@@@@@@@ ItemList.GetItem()
		const IItem *pItem = ItemList.GetItem();

		if (IsSearched(ItemList.GetItem()))
		{
			m_pDlgItem->m_List_Items.AddItem(ItemList.GetItem());
		}
	}
	m_pDlgItem->m_List_Items.SetSelectedID(IDs); 

	// Redraw
	m_pDlgItem->m_List_Items.Invalidate();
}

void CClassMode::UpdateTheme(int ID)
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
			Label.LoadString(IDS_RI_CLASSES);
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

void CClassMode::UpdateQuestionList(long BlockId)
{
	int TopBlock = 0;

	// Get the selected question
	JInt32 IDs = m_pDlgItem->m_List_Questions.GetSelectedID();   

	JList<IQuestion const *> & Question_List = m_pDlgItem->m_Tree_Theme.GetQuestionList();  

	m_pDlgItem->m_List_Questions .ResetContent();
	m_pDlgItem->m_List_Dimension1.ResetContent();
	m_pDlgItem->m_List_Dimension2.ResetContent(); 

	// m_pDlgItem->m_Button_Filtre   .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_UP  .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_DOWN.EnableWindow(FALSE);

	// Add the critere modifié
	if(m_pCritereModifie)
	{
		m_pDlgItem->m_List_Questions.AddItem(m_pCritereModifie);
		IDs = m_pCritereModifie->GetID();
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
				if(pQuestion->HasDimensions() == 1 && pQuestion->AsQuestion()->IsQualitative())
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

void CClassMode::SetActiveSegmentQuestions()
{
	// empty the SAS
	m_pDlgItem->m_List_SAS.ResetContent();
	m_pDlgItem->m_List_SAS.Invalidate();
}

void CClassMode::UpdateFiltreDimension1List()
{
	CItemMode::UpdateFiltreDimension1List();
}

void CClassMode::UpdateDimensionsList()
{
	CItemMode::UpdateDimensionsList();

	for(m_DimDlg.MoveFirst(); m_DimDlg.IsValid(); m_DimDlg.MoveNext())
		m_DimDlg.GetItem().EnableOperators(FALSE);

	if(m_pCritereModifie)
	{
		m_pDlgItem->m_Header_Dimension1.ShowWindow(SW_HIDE);
		m_pDlgItem->m_Button_Dim1_OK   .ShowWindow(SW_SHOW);
	}
	else if(m_pSelectedQuestion && m_pSelectedQuestion->IsCritereModifie())
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

void CClassMode::UpdateIBuilder(bool Changed, bool bValues)
{
	CItemMode::UpdateIBuilder(Changed, bValues);

	if(!m_pCritereModifie)
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

void CClassMode::GroupDimension1()
{
	CDlgLibelle Dlg(m_pDlgItem);
	Dlg.m_Libelle = "";
	Dlg.m_CATPColorsApp = m_pDlgItem->m_CATPColorsApp;
	if(Dlg.DoModal() != IDOK)
		return;

	m_pDlgItem->m_Edit_Name.SetWindowText(m_pSelectedQuestion->GetLabel().AsJCharPtr());
	// Check if question is a critere modifie
	if(m_pSelectedQuestion->IsCritereModifie())
	{
		JList<JInt32> Values;
		// Set the selected ID in the list box
		m_pDlgItem->m_List_Dimension1.GetSelectedID(Values);
		if(Values.GetCount())
		{
			// Nom avec crochet
			CString NomGroupe;
			NomGroupe.Format("[%s]",Dlg.m_Libelle);
			JLabel Name = NomGroupe;
			m_pCritereModifie->CombineItems(m_pDlgItem->GetTerrain(),Name,Values);
		}
		m_IBuilder.Reset();
		m_IBuilder.SetOperator(GetSegmentOperator());
		FindSet_IBsegment(0);
		OnSelchange_ListQuestions();
	}
	else
	{
		// Get a new item construit
		CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
		// Set the builder object to the item
		pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),m_IBuilder);
		// Test is the current target is valid
		if(pItem->IsValid(m_pDlgItem->GetTerrain()))
		{
			// Set the label
			CString NomGroupe;
			NomGroupe.Format("[%s]",Dlg.m_Libelle);
			JLabel label = NomGroupe;
			pItem->SetLabel(label,true);
			// Create new critere modifié
			m_pCritereModifie = m_pDlgItem->GetItems()->GetNewCritereModifie();
			m_pCritereModifie->SetLabel(m_pSelectedQuestion->GetLabel(),true); 
			// Set the Question Bloc
			m_pCritereModifie->SetQuestionBlocId(m_pSelectedQuestion->GetBlocID());
			// Add the item to the Critere modifié
			m_pCritereModifie->AddItem(pItem);

			// Load the modalities selected
			JList< CIBModalite *> pIModalite;
			m_pIBquestion->GetModalites(pIModalite);
			pIModalite.MoveFirst();
			if(pIModalite.IsValid())
			{
				CIBModalite * pModalite = pIModalite.GetItem();
				// Get the modalities selected
				JList<JInt32> Values;
				pModalite->GetValues(Values);
				// Create a map of the modalities
				JMap<JInt32,JBool> Items;
				for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
					Items.Add(Values.GetItem()) = true;

				// Get all the modalites in the question
				JVector<const IModalite *> Dimension1;
				m_pSelectedQuestion->GetIModalites(Dimension1,0,0,m_pDlgItem->GetContingentMask());
				// Now for each modalité not in the selected group
				// we create a new item and add that item to the Critere modifie
				for(Dimension1.MoveFirst(); Dimension1.IsValid(); Dimension1.MoveNext())
				{
					// Find the modalité in the map
					Items.MoveTo(Dimension1.GetItem()->GetID());
					// If it is selected continue
					if(Items.IsValid())
						continue;
					pItem = CreateNewItem(m_pSelectedQuestion,Dimension1.GetItem());
					if(pItem)
						m_pCritereModifie->AddItem(pItem);
				}
			}
			m_IBuilder.Reset();
			m_IBuilder.SetOperator(GetSegmentOperator());
			FindSet_IBsegment(0);
			// Update the selection list
			OnSelchange_ListQuestions();
		}
		else
			delete	pItem;
	}
}

CItemConstruit * CClassMode::CreateNewItem(IQuestion const * pQuestion,IModalite const *pModalite)
{
	// Builder object
	CIBuilder Builder;
	// Get a new item construit
	CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();

	// Set segment operator
	Builder.SetOperator(IBloc::Or);

	// Create a new segment
	CIBSegment *pISegment = new CIBSegment();
	// Set the question operator
	pISegment->SetOperator(IBloc::And);

	// Create a new question
	CIBQuestion *pIQuestion = new CIBQuestion;
	// Set the question id
	pIQuestion->SetID(pQuestion->GetID());

	// Add the modality
	if(pQuestion->HasDimensions() && pModalite)
	{
		CIBModalite * pIModalite = new CIBModalite;
		pIModalite->AddValue(pModalite->GetID()); 
		pIModalite->SetOperator(IBloc::Or);
		pIQuestion->AddModalite(pIModalite);
		pItem->SetLabel(pModalite->GetLabel(),true); 
	}
	
	pISegment->Add(pIQuestion);
	Builder.Add(pISegment);

	// Set the builder object
	pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),Builder);
	// return if the current target is valid
	if(pItem->IsValid(m_pDlgItem->GetTerrain()))
		return pItem;

	delete pItem;
	return 0;
}
