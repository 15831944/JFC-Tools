#include "stdafx.h"
#include "ItemMode.h"

#include "DlgItems.h"
#include "DlgFiltre.h"

#include "DlgLibelle.h"
#include "DlgUpwards.h"
#include "SearchAlgo.h"
#include <direct.h>
#include <string>


#include "JFCConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemMode::CItemMode(CDlgItems *pDlgItems) : CAbstractMode(pDlgItems)
{
	m_pSelectedQuestion = 0;
	m_pPseudoItem       = 0;

	m_IBuilder.Reset();
	m_pIBsegment  = 0;
	m_pIBquestion = 0;
	m_pIBitem     = 0;
	m_bItemValid  = 0;

	m_pValuesDlg = 0;
	m_ValuesDlgPos.SetRectEmpty(); 

	// Init 1er élmt sélection dans dimension 1
	m_PremSelListDimension_1 = -1;

	// Par défaut moyenne non active
	m_MoyenneActive = false;
}

CItemMode::~CItemMode()
{
	delete m_pValuesDlg;
	delete m_pPseudoItem;
}

// Starts the mode
void CItemMode::StartMode()
{
	EnableOperators(TRUE);

	// Activation des composants
	m_pDlgItem->m_Button_Dim1_UP    .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Dim1_DOWN  .EnableWindow(FALSE);
	m_pDlgItem->m_Tab_Segment       .EnableWindow(TRUE);
	m_pDlgItem->m_Button_Genese     .EnableWindow(FALSE);
	m_pDlgItem->m_Button_List2      .EnableWindow(FALSE);
	m_pDlgItem->m_Button_Accept     .EnableWindow(FALSE);
	m_pDlgItem->m_Button_CalcSegment.EnableWindow(TRUE);

	// Visibilité des composants
	m_pDlgItem->m_Button_Dim1_OK   .ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Dimension1.ShowWindow(SW_HIDE);
	m_pDlgItem->m_Button_Dimension2.ShowWindow(SW_HIDE);
	m_pDlgItem->m_Header_Dimension1.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Header_Dimension2.ShowWindow(SW_SHOW);
	m_pDlgItem->m_Button_Genese    .ShowWindow(SW_SHOW);
	m_pDlgItem->m_Button_List2     .ShowWindow(SW_HIDE);

	CString Label;
	LOADSTRING(Label, IDS_RI_ITEMS_CONST); 
	m_pDlgItem->m_Mode_Title.SetWindowText(Label);
	m_pDlgItem->m_Mode_Title.Invalidate();

	CString ToolTip;

	LOADSTRING(ToolTip, IDS_RI_TT_ET_Q_ITEM); 
	m_pDlgItem->m_Button_SAS_ET.SetTooltipText(ToolTip);

	LOADSTRING(ToolTip, IDS_RI_TT_OU_Q_ITEM); 
	m_pDlgItem->m_Button_SAS_OU.SetTooltipText(ToolTip);

	LOADSTRING(ToolTip, IDS_RI_TT_VALID_ITEM); 
	m_pDlgItem->m_Button_Accept.SetTooltipText(ToolTip);

	LOADSTRING(Label, IDS_RI_ITEMS);
	LOADSTRING(ToolTip, IDS_RI_TT_ITEMS);

	m_pDlgItem->m_Button_List1.SetWindowText(Label);
	m_pDlgItem->m_Button_List1.SetTooltipText(ToolTip);

	LOADSTRING(Label, IDS_RI_PALETTES); 
	LOADSTRING(ToolTip, IDS_RI_TT_PALETTE); 

	m_pDlgItem->m_Button_List2.SetWindowText(Label);
	m_pDlgItem->m_Button_List2.SetTooltipText(ToolTip);

	m_Dim_Sel.Reset();

	m_pDlgItem->CheckRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2,IDC_RI_RAD_QUESTIONS);

	// Démarre en mode Questions via Tree Questions
	UpdateTheme(IDC_RI_RAD_QUESTIONS);
	UpdateListItems();

	m_IBuilder.Reset();

	SetSegmentOperator(IBloc::Or);
	m_IBuilder.SetOperator(IBloc::Or);

	FindSet_IBsegment(0);

	m_pDlgItem->m_List_Questions.SetCurSel(LB_ERR); 
	OnSelchange_ListQuestions();
}

///////////////////////////////////////////////////////////////////////////////////////
// Récupération d'un mode en cours
void CItemMode::RecupMode()
{
	// Rien ici
}

// Stops the mode
void CItemMode::StopMode()
{
	delete m_pValuesDlg;
	m_pValuesDlg = 0;

	m_Selected_UserItems.Reset();
	m_DimDlg.Reset();

	delete m_pPseudoItem;
	m_pPseudoItem = 0;

	m_IBuilder.Reset();
	m_Dim_Sel .Reset();

	m_pDlgItem->m_List_Questions .ResetContent();  
	m_pDlgItem->m_List_Dimension1.ResetContent();
	m_pDlgItem->m_List_Dimension2.ResetContent();
	m_pDlgItem->m_List_Items     .ResetContent();
	m_pDlgItem->m_List_SAS       .ResetContent(); 
}

void CItemMode::UpdateDisplay()
{

}

/// Returns a string to the current dimension mode
CString CItemMode::GetDimensionModeString(int Mode)
{
	CString St;
	LOADSTRING(St, IDS_RI_ITEM_DIM);
	return St;
}

/// Returns nb élmts sélectionnés
int	CItemMode::GetNbSelectUserItem()
{
	
	return m_Selected_UserItems.GetCount(); 
}

// Enables all operators
void CItemMode::EnableOperators(BOOL bEnable)
{
	m_pDlgItem->m_Button_Dim1_OU   .EnableWindow(bEnable);
	m_pDlgItem->m_Button_Dim1_ET   .EnableWindow(bEnable);
	m_pDlgItem->m_Button_Dim1_NI   .EnableWindow(bEnable);
	m_pDlgItem->m_Button_Dim1_XOR  .EnableWindow(bEnable);
	m_pDlgItem->m_Button_Dim2_OU   .EnableWindow(bEnable);
	m_pDlgItem->m_Button_Dim2_ET   .EnableWindow(bEnable);
	m_pDlgItem->m_Button_Dim2_NI   .EnableWindow(bEnable);
	m_pDlgItem->m_Button_Dim2_XOR  .EnableWindow(bEnable);
	m_pDlgItem->m_Button_SAS_OU    .EnableWindow(bEnable);
	m_pDlgItem->m_Button_SAS_ET    .EnableWindow(bEnable);
	m_pDlgItem->m_Button_Segment_OU.EnableWindow(bEnable);
	m_pDlgItem->m_Button_Segment_ET.EnableWindow(bEnable);

	for(m_DimDlg.MoveFirst(); m_DimDlg.IsValid(); m_DimDlg.MoveNext())
		m_DimDlg.GetItem().EnableOperators(bEnable);
}

// Displays the selected theme
void CItemMode::UpdateTheme(int ID)
{
	CString Label;

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
			m_pDlgItem->m_List_Items			.ShowWindow(SW_SHOW);
			m_pDlgItem->ResizeListItems(true);
			m_pDlgItem->m_Button_Genese			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Button_RAZ_search		.ShowWindow(SW_SHOW);
			m_pDlgItem->m_Edit_Search			.ShowWindow(SW_SHOW);
			m_pDlgItem->m_List_Items			.Invalidate();
			m_pDlgItem->m_Tree_Rubrique_Items	.Invalidate(); 
			m_pDlgItem->m_Tree_Rubrique_Palettes.Invalidate(); 
			UpdateListItems();
		break;

		case IDC_RI_RAD_LIST_2:
			m_pDlgItem->m_Tree_Theme			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilter		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Button_QFilterRaz		.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Edit_QFilter			.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Items	.ShowWindow(SW_HIDE);
			m_pDlgItem->m_Tree_Rubrique_Palettes.ShowWindow(SW_SHOW);	
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

// Sets the applications colors
void CItemMode::SetColors()
{
	for(m_DimDlg.MoveFirst();m_DimDlg.IsValid(); m_DimDlg.MoveNext())
		m_DimDlg.GetItem().SetColors();
	
	if(m_pValuesDlg)
		m_pValuesDlg->SetColors();
}

// Sets the icon mode
void CItemMode::SetIcones()
{
	for(m_DimDlg.MoveFirst(); m_DimDlg.IsValid(); m_DimDlg.MoveNext())
		m_DimDlg.GetItem().SetIcones(m_pDlgItem->m_FIcones);
}

// Updates the segment tabs
void CItemMode::UpdateSegmentTabs(int Sel)
{
	m_pDlgItem->m_Tab_Segment.DeleteAllItems();
	for(int x = 0; x <= m_IBuilder.GetSegmentCount(); x++)
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

// Creates an item if all is valid
bool CItemMode::OnAccept(CString &Name) 
{
	if(!m_pDlgItem->IsVisualModeActive())
	{
		CString Msg;
		LOADSTRING(Msg, IDS_RI_VERIFIER);
		m_pDlgItem->MessageBox(Msg,m_pDlgItem->GetTitle(),MB_ICONERROR|MB_OK); 
		return false;
	}

	bool ret = true;

	if(!m_bItemValid)
	{
		m_pDlgItem->MessageBox("L'item est invalide!",m_pDlgItem->GetTitle(),MB_OK);
		ret = false;
	}
	else
	{
		// If the current question is not valid remove it
		if(m_pIBquestion && !m_pIBquestion->IsValid())
			m_pIBsegment->RemoveQuestion(m_pIBquestion);

		// Get a new item construit
		CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();

		// Set the builder object
		pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),m_IBuilder);

		// Test is the current target is valid
		if(pItem->IsValid(m_pDlgItem->GetTerrain()))
		{
			JLabel label = Name;

			// A voir si on autorise ou pas la création d'un item du même nom, risqué
			if(pItem->SetLabel(label))
			{
				// @@@ Sauvegarde catégorie
				pItem->SetCategorie(9); 

				// Construction du nouvel item
				const CItemConstruit * pData = m_pDlgItem->GetItems()->RegisterItemConstruit(pItem);
			
				// Ident du nouvel item (sert à rien)
				JInt32 IdNewItem = pData->GetID(); 

				// Ajoute item dans la rubrique par défaut ou autre  du tree rubrique items
				HTREEITEM HRubriqueAjout;
				HRubriqueAjout = m_pDlgItem->GetRubriqueItemDefaut();

				if (HRubriqueAjout)
				{
					// On archive le nouvel item dans la rubrique par défaut
					m_pDlgItem->m_Tree_Rubrique_Items.AddItemRubrique(HRubriqueAjout, pData->GetID(), pData->GetLabel().AsJCharPtr());
				}
				else
				{
					// En attente @@@@ il faudra demander la rubrique où sauvegarder l'item
					m_pDlgItem->m_Tree_Rubrique_Items.AddItemRubriqueDivers(pData->GetID(), pData->GetLabel().AsJCharPtr());

					// du type 
					m_pDlgItem->AddItemRubrique(pData->GetID(), pData->GetLabel().AsJCharPtr());					
				}

				// TRAC : TICKET 35 : on sauve le fichier des rubriques
				m_pDlgItem->m_Tree_Rubrique_Items.SaveFileRubrique();

				// Mise à jour de la liste
				UpdateListItems();

				if (ItemQuantitatif())
					JBool ItemQuanti = true;
			}
			else
			{
				// Prévient que ce libellé d'item est déjà autorisé, il faut donc donner un autre libellé
				CString Str;
				LOADSTRING(Str, IDS_RI_ITEMEXISTS);
				m_pDlgItem->MessageBox(Str,m_pDlgItem->GetTitle(),MB_OK);
				ret = false;
			}
		}
		else
		{
			m_pDlgItem->MessageBox("L'item est invalide!",m_pDlgItem->GetTitle(),MB_OK);
			ret = false;
		}

		if(m_pIBquestion && !m_pIBquestion->IsValid())
			// Put back the current question
			m_pIBsegment->Add(m_pIBquestion);

		delete pItem;
		Name.Empty();
	}
	m_pDlgItem->GetItems()->Save();
	return ret;
}

// Operator buttons pressed
void CItemMode::OnDim_2_Xor() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnDim_2_Et() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnDim_2_Ni() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnDim_2_Ou() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnDim_2_Raz() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnDim_1_Ok() 
{
	// nothing to do
}

void CItemMode::OnDim_1_Xor() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnDim_1_Et() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnDim_1_Ni() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnDim_1_Ou() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnDim_1_Raz() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnSAS_Et() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnSAS_Ou() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnSegment_Et() 
{
	UpdateIBuilder(true, false);
}

void CItemMode::OnSegment_Ou() 
{
	UpdateIBuilder(true, false);
}

// Retro engineering an item
bool CItemMode::OnGenese() 
{
	// Get the selected item
	JList<JInt32> Values;

	// Set the selected ID in the list box
	m_pDlgItem->m_List_Items.GetSelectedID(Values);

	// Si 1 seul élmt de sélectionné, on le décortique pour afficher ses composants
	if (Values.GetCount() == 1)
	{
		Values.MoveFirst();
		CItemConstruit * pItem = m_pDlgItem->GetItems()->GetItemConstruit(Values.GetItem());
		// No item
		if(!pItem)
			return false;

		m_Selected_UserItems.Reset();
		m_IBuilder.Reset();

		// Get the item builder definition
		if (!pItem->GetItemBuilder (m_pDlgItem->GetTerrain(),m_IBuilder, true))
		{
			return false;
		}
		else
		{
			SetSegmentOperator(m_IBuilder.GetOperator());

			// Set the first segment
			FindSet_IBsegment(0, false, false);
			
			// A VOIR ALAIN virer JUILLET 2014
			// Mise à jour liste des questions
			UpdateQuestionListGenese();

			/*
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
			*/

			return true;
		}
	}

	return false;
}

// Clear seulement sélection item
void CItemMode::OnDeselect_Items()
{
	if(m_pDlgItem->m_List_Items.GetSelCount() > 0) 
		m_pDlgItem->m_List_Items.ToggleSelection(); 

	// Vire sélection dans item
	m_Selected_UserItems.Reset();
	if(m_pPseudoItem)
	{
		m_pDlgItem->m_List_Questions.RemoveItem(m_pPseudoItem->GetID());
		delete m_pPseudoItem;
		m_pPseudoItem = 0;
	}
}

// Clearing all the current item settings
void CItemMode::OnTout_Raz()
{
	if(m_pDlgItem->m_List_Items.GetSelCount() > 0) 
		m_pDlgItem->m_List_Items.ToggleSelection(); 

	// Vire la sélection dans l'arbre
	m_pDlgItem->m_Tree_Theme.RemoveSelections();

	// Vire sélection dans item
	m_Selected_UserItems.Reset();
	if(m_pPseudoItem)
	{
		m_pDlgItem->m_List_Questions.RemoveItem(m_pPseudoItem->GetID());
		delete m_pPseudoItem;
		m_pPseudoItem = 0;
	}

	// Désinhibe la fonction vérifier
	// m_pDlgItem->m_Button_Verifier.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_CATPColorsApp));
	OnItem_Raz();

	// Init Liste sélection lignes
	if(m_pDlgItem->m_List_Compo_Palette.GetItemCount() > 0)
		m_pDlgItem->m_List_Compo_Palette.DeleteAllItems();

	// Update de toutes les dimensions
	// ????
	UpdateDimensionsList();
}

// Clearing all the current item && palettes sauf tree questions
void CItemMode::OnTout_Raz_ItemPalette()
{
	if(m_pDlgItem->m_List_Items.GetSelCount() > 0) 
		m_pDlgItem->m_List_Items.ToggleSelection(); 

	// Vire sélection dans item
	m_Selected_UserItems.Reset();
	if(m_pPseudoItem)
	{
		m_pDlgItem->m_List_Questions.RemoveItem(m_pPseudoItem->GetID());
		delete m_pPseudoItem;
		m_pPseudoItem = 0;
	}

	// Désinhibe la fonction vérifier
	OnItem_Raz();

	// Init Liste sélection lignes
	if(m_pDlgItem->m_List_Compo_Palette.GetItemCount() > 0)
		m_pDlgItem->m_List_Compo_Palette.DeleteAllItems();
}

// Clear the current item
void CItemMode::OnItem_Raz() 
{
	m_IBuilder.Reset();
	SetSegmentOperator(IBloc::Or);
	m_IBuilder.SetOperator(IBloc::Or);
	FindSet_IBsegment(0);

	// Update the selection list
	m_pDlgItem->m_List_Questions.SetTopIndex(0);

	// Init Marquage Sélection (en liaison avec SAS)
	m_pDlgItem->m_List_Questions.ResetMapSel(); 

	// ????????????????
	OnSAS_Raz();

	// Update de toutes les dimensions
	UpdateDimensionsList();
}

// Clear the current segment
void CItemMode::OnSAS_Raz() 
{
	m_Dim_Sel.Reset();

	if(m_pIBsegment)
	{
		// Positionnement des opérateurs
		m_pIBsegment->Reset();

		// Attention au positionnement opérateur par défaut (mode segment "ET", mode pupitre "OU")
		if (m_pDlgItem->GetTypeRepItem()) 
			SetQuestionOperator(IBloc::And);  
		else
			SetQuestionOperator(IBloc::Or);  

		m_pIBsegment->SetOperator(IBloc::And);

		// No current question
		m_pIBquestion = 0;
		// No current items
		m_pIBitem = 0;

		// Update the selection list
		m_pDlgItem->m_List_Questions.SetCurSel(LB_ERR); 
		OnSelchange_ListQuestions();
		UpdateIBuilder(true, false);
	}
}

// Suppression d'un segment
void CItemMode::OnSegment_Raz()
{
	// Idem Suppression SAS
	OnSAS_Raz();

	bool bUpdate = false;
	int Count = m_IBuilder.GetSegmentCount()-1;
	int Sel = m_pDlgItem->m_Tab_Segment.GetCurSel();

	if(m_pIBquestion && !m_pIBquestion->IsValid())
	{
		m_pIBsegment->DeleteQuestion(m_pIBquestion);
		m_pIBquestion = 0;
		bUpdate = true;
	}
	if(m_pIBitem && !m_pIBitem->IsValid())
	{
		m_pIBsegment->DeleteItem(m_pIBitem);
		m_pIBitem = 0;
		bUpdate = true;
	}
	if(!m_pIBsegment->IsValid())
	{
		m_IBuilder.DeleteSegment(m_pIBsegment);
		m_pIBsegment = 0;
		m_pIBquestion = 0;
		m_pIBitem = 0;
		bUpdate = true;
		Count--;
		if(Count < Sel)
			Sel--;
	}
	
	FindSet_IBsegment(Sel,false);
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
	if(!m_pDlgItem->m_bSegment)
		bUpdate = true;

	UpdateIBuilder(bUpdate, false);

	// Attention si dernier segment, on vire tout l'item, sinon bug ???
	if (m_IBuilder.GetSegmentCount() == 0)
	{
		OnItem_Raz();
	}
}

// Audience Segment changed
void CItemMode::OnChange_PopSegment()
{
	OnTout_Raz();
}

// Audience weights changed
void CItemMode::OnChange_TypePoids()
{
	UpdateIBuilder(true, false);
}

// Item segment changed
void CItemMode::OnSelchange_TabSegment(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Dim_Sel.Reset();

	// If CTRL Key is down we are in copy segment mode
	bool bCopy = false;
	if(GetAsyncKeyState(VK_CONTROL)<0)
		bCopy = true;

	bool bUpdate = false;
	int Count = m_IBuilder.GetSegmentCount()-1;
	int Sel = m_pDlgItem->m_Tab_Segment.GetCurSel();

	if(m_pIBquestion && !m_pIBquestion->IsValid())
	{
		m_pIBsegment->DeleteQuestion(m_pIBquestion);
		m_pIBquestion = 0;
		bUpdate = true;
	}
	if(m_pIBitem && !m_pIBitem->IsValid())
	{
		m_pIBsegment->DeleteItem(m_pIBitem);
		m_pIBitem = 0;
		bUpdate = true;
	}
	if(!m_pIBsegment->IsValid())
	{
		m_IBuilder.DeleteSegment(m_pIBsegment);
		m_pIBsegment = 0;
		m_pIBquestion = 0;
		m_pIBitem = 0;
		bUpdate = true;
		Count--;
		if(Count < Sel)
			Sel--;
	}
	
	FindSet_IBsegment(Sel,bCopy);
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
	if(!m_pDlgItem->m_bSegment)
		bUpdate = true;

	UpdateIBuilder(bUpdate, false);
	
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////////
// Test si item quantitatif
JBool CItemMode::ItemQuantitatif()
{
	// Par défaut ce n'est pas un quantitatif
	JBool Quantitatif = false;

	// Balaye tous les segments di Builder constrcuteur de l'item
	for (int IdxSeg = m_IBuilder.GetSegmentCount()-1; IdxSeg >=0; IdxSeg--)
	{
		// Pour chaque segment, on vérie si il y a des questions ou des items quantitatifs
		CIBSegment * pIBSegment = m_IBuilder.GetSegment(IdxSeg);

		// Balaye les elmts du segment
		if(pIBSegment->HasItems())
		{
			// récupère liste des questions
			JList <JInt32> LstQuestions;
			pIBSegment->GetQuestionsID(LstQuestions); 

			// Balayage des questions du segment
			for (LstQuestions.MoveFirst(); LstQuestions.IsValid(); LstQuestions.MoveNext())
			{
				// Id question segment
				JInt32 IdQuestion = LstQuestions.GetItem();
			
				// Question associé
				IQuestion const * pQuestion = m_pDlgItem->GetTerrain()->GetQuestionByID(IdQuestion);

				if(pQuestion)
				{
					// Vérifie si c'est une question
					const CQuestion *pQu = pQuestion->AsQuestion();

					// C'est une question, est-elle quantitative
					if (pQu)
					{
						// C'est une question, est-elle quantitative
						if (pQuestion->AsQuestion()->IsQuantiAssim() || pQuestion->AsQuestion()->IsQuantiPure())
						{
							return true;
						}
					}
				}
			}
			
			// récupére liste des items
			JList <JInt32> Items;
			pIBSegment->GetItemsID(Items);
			
			for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
			{
				IQuestion const * pQuestion = m_pDlgItem->GetItems()->GetQuestionByID(Items.GetItem());

				if(pQuestion)
				{
					// Vérifie si c'est une question
					const CQuestion *pQu = pQuestion->AsQuestion();

					// C'est une question, est-elle quantitative
					if (pQu)
					{
						if (pQuestion->AsQuestion()->IsQuantiAssim() || pQuestion->AsQuestion()->IsQuantiPure())
						{
							return true;
						}
					}
				}
				else
				{
					if(m_pPseudoItem && Items.GetItem()==0)
					{
						JList <JInt32> ItemsP;
						m_pPseudoItem->GetItems(ItemsP); 
						for (ItemsP.MoveFirst(); ItemsP.IsValid(); ItemsP.MoveNext())
						{
							JInt32 IdItem = ItemsP.GetItem();

							IItem * pItem = m_pDlgItem->GetItems()->GetIItemByID(IdItem);
							// Est ce un autre item, ou une question
							CPseudo			*  pPseudo			= pItem->AsPseudo(); 
							if (pItem)
							{
								
								JLabel LabelItem = pItem->GetLabel(); 
								JBool Ok = true;
							}
							else if (pPseudo)
							{
								JList <JInt32> ItemsP2;
								pPseudo->GetItems(ItemsP2); 
							}

							JBool Val = true;
						}
					}
				}
			}
		}

		// Items composés de questions
		else if (pIBSegment->HasQuestions())	
		{
			// récupère liste des questions
			JList <JInt32> LstQuestions;
			pIBSegment->GetQuestionsID(LstQuestions); 

			// Balayage des questions du segment
			for (LstQuestions.MoveFirst(); LstQuestions.IsValid(); LstQuestions.MoveNext())
			{
				// Id question segment
				JInt32 IdQuestion = LstQuestions.GetItem();

				// Question associé
				IQuestion const * pQuestion = m_pDlgItem->GetTerrain()->GetQuestionByID(IdQuestion);

				if(pQuestion)
				{
					// Vérifie si c'est une question
					const CQuestion *pQu = pQuestion->AsQuestion();

					// C'est une question, est-elle quantitative
					if (pQu)
					{
						// C'est une question, est-elle quantitative
						if (pQuestion->AsQuestion()->IsQuantiAssim() || pQuestion->AsQuestion()->IsQuantiPure())
						{
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////
// Balayage des segments
void CItemMode::MajTabSegment() 
{
	return;

	// Sauve la sélection segment en cours
	int SelCur = m_pDlgItem->m_Tab_Segment.GetCurSel();
	int SelSasCur = m_pDlgItem->m_List_SAS.GetCurSel();
	int SelQuCur = m_pDlgItem->m_List_Questions.GetCurSel(); 

	// Balaye tous les segments sauf le courant
	for (int IdxSeg = m_IBuilder.GetSegmentCount()-1; IdxSeg >=0; IdxSeg--)
	{
		if (IdxSeg != SelCur)
		{
			// Passe au segment 
			FindSet_IBsegment(IdxSeg,false);

			// Stocke ident nouveau segment
			CIBSegment * pSegment = m_pIBsegment;

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
	
			// Vire le segment si plus aucun items
			if (!pSegment->IsValid())
				m_IBuilder.DeleteSegment(pSegment);
	
		}
	}

	// Puis on remet le segment courant
	FindSet_IBsegment(SelCur,false);
	if(m_pDlgItem->m_List_SAS.GetCount())
	{
		m_pDlgItem->m_List_SAS.SetCurSel(SelSasCur);
		OnSelchange_ListSas();
	}
	else
	{
		m_pDlgItem->m_List_Questions.SetCurSel(SelQuCur); 
		OnSelchange_ListQuestions();
	}
	UpdateIBuilder(true, false);
}


// Mise à jour Segments suite à suppression élmt Item
void CItemMode::MajSegments()
{
	return;

	// Liste des items sélectionnés
	JList<JInt32> LstIDItemSel;
	m_pDlgItem->m_List_Items.GetSelectedID(LstIDItemSel);
	for (int x = m_IBuilder.GetSegmentCount()-1; x >= 0; x--)
	{
		// Récupére le segment
		CIBSegment * pIBsegmentTmp		=	m_IBuilder.GetSegment(x);

		if(pIBsegmentTmp->HasItems())
		{
			// enlève l'item
			JList <JInt32> LstItems;
			pIBsegmentTmp->GetItemsID(LstItems);

			for (LstItems.MoveFirst(); LstItems.IsValid(); LstItems.MoveNext())
			{
				// Id item segment
				JInt32 IdItemSeg = LstItems.GetItem();

				// Voir si cet item sélectionné dans list items, sinon le virer du segment
				LstIDItemSel.MoveTo(IdItemSeg); 

				if (!LstIDItemSel.IsValid())
				{
					// IBItem Segment associé à virer
					m_pIBitem = m_pIBsegment->FindItem(IdItemSeg);
					pIBsegmentTmp->DeleteItem(m_pIBitem); 
				}
			}
		}
	}			
}

// Selection changed in the SAS list
void CItemMode::OnSelchange_ListSas()
{
	IQuestion * pCurQuestion = 0;
	// Update selected question
	if(m_pDlgItem->m_List_SAS.GetCurSel() != LB_ERR)
	{
		pCurQuestion = (IQuestion *)m_pDlgItem->m_List_SAS.GetItemData(
			m_pDlgItem->m_List_SAS.GetCurSel());
		if(!m_pDlgItem->m_List_Questions.SetSelectedID(m_pDlgItem->m_List_SAS.GetSelectedID()))
			m_pDlgItem->m_List_Questions.SetCurSel(LB_ERR);
		QuestionChanged(pCurQuestion);
	}
	else
	{
		if(m_pDlgItem->m_List_Questions.GetCurSel() != LB_ERR) 
			OnSelchange_ListQuestions();
		else 
			QuestionChanged(pCurQuestion);
	}

	// Cache ou décache dimension 2 selon questions
	if (pCurQuestion)
	{
		int NbDim = pCurQuestion->HasDimensions();
		if (NbDim <= 1)
			m_pDlgItem->SetDimension2Visible(false);
		else
			m_pDlgItem->SetDimension2Visible(true);
	}

}

// Selection changed in the questions list
void CItemMode::OnSelchange_ListQuestions() 
{
	// AfxMessageBox("????", MB_ICONERROR);

	IQuestion * pCurQuestion = 0;

	// Update selected question
	if(m_pDlgItem->m_List_Questions.GetCurSel() != LB_ERR)
	{
		pCurQuestion = (IQuestion *)m_pDlgItem->m_List_Questions.GetItemData(m_pDlgItem->m_List_Questions.GetCurSel());
		if(!m_pDlgItem->m_List_SAS.SetSelectedID(m_pDlgItem->m_List_Questions.GetSelectedID()))
			m_pDlgItem->m_List_SAS.SetCurSel(LB_ERR);
		QuestionChanged(pCurQuestion);

		int NbDim = pCurQuestion->HasDimensions();
		if (NbDim <= 1)
			m_pDlgItem->SetDimension2Visible(false);
		else
		{
			m_pDlgItem->SetDimension2Visible(true);
		}
	}
	else
	{
		if(m_pDlgItem->m_List_SAS.GetCurSel() != LB_ERR)
 			OnSelchange_ListSas();
		else
			QuestionChanged(pCurQuestion);
	}
}

// Current question has changed
void CItemMode::QuestionChanged(IQuestion * pCurQuestion)
{
	if (pCurQuestion)
	{
		// Check if the current question selection is valid
		if(m_pIBquestion)
		{
			if(!m_pIBquestion->IsValid())
				m_pIBsegment->DeleteQuestion(m_pIBquestion);
			else
			{
				if(m_pSelectedQuestion && m_pSelectedQuestion->IsQuestion() && m_pSelectedQuestion->AsQuestion()->HasValues())
				{
					if(!m_pIBquestion->GetClass() && !m_pIBquestion->GetNumValue())
						m_pIBsegment->DeleteQuestion(m_pIBquestion);
				}
			}
			m_pIBquestion = 0;
		}
		
		if(m_pIBitem)
		{
			if(!m_pIBitem->IsValid())
			{
				// ???????????????????
				m_pIBsegment->DeleteItem(m_pIBitem);
				m_pIBitem = 0;
			}
		}

		// Get the right question
		m_pSelectedQuestion = pCurQuestion;

		FindSet_IBcontainerByType(m_pSelectedQuestion);

		// Update all the Dimension1
		UpdateDimensionsList();

		bool ChangeBuild = SetSelectedDimensions();

		UpdateIBuilder(ChangeBuild, false);// Update the ibuilder

		bool State = FALSE;
		if(m_pDlgItem->m_List_Dimension1.GetSelCount() == 1)
		{
			// @@@ Modif Oct 2013
			bool QuestPalette = false;
			if (pCurQuestion)
			{
				CPalettes *pPalette = pCurQuestion->AsPalette();
				if (pPalette)
					QuestPalette = true;
			}

			// if(pCurQuestion && pCurQuestion != m_pPseudoItem && pCurQuestion->CanModify() && pCurQuestion->HasDimensions())
			// if (pCurQuestion && pCurQuestion != m_pPseudoItem && pCurQuestion->HasDimensions() && m_pDlgItem->ValCurMode() == Mode_Palettes && pCurQuestion->AsPalette())
			if (pCurQuestion && pCurQuestion != m_pPseudoItem && pCurQuestion->HasDimensions() && m_pDlgItem->ValCurMode() == Mode_Palettes && QuestPalette)
				State = TRUE;
		}

		// m_pDlgItem->m_Button_Filtre   .EnableWindow(State);
		m_pDlgItem->m_Button_Dim1_UP  .EnableWindow(State);
		m_pDlgItem->m_Button_Dim1_DOWN.EnableWindow(State);

		m_pDlgItem->UpdateWindow();
		m_pDlgItem->SetActiveWindow();

		// Test si uniquement Valeurs numériques
		if(m_pDlgItem->m_List_Dimension1.GetCount() == 1)
		{
			// récup le libellé de la question
			CString TxtQuestion;
			const IModalite* pModalite = (const IModalite*)m_pDlgItem->m_List_Dimension1.GetItemData(0);
			TxtQuestion = pModalite->GetLabel().AsJCharPtr(); 

			// Si 1 seul élmt et c'est une variable numérique
			if (TxtQuestion == "Valeurs numériques")
			{
				// Sélectionne directement la question et affiche les valeurs numériques
				m_pDlgItem->m_List_Dimension1.SetSel(0,true);
				OnSelchange_ListDimension_1();
			}
		}

	}
}

// Modalities dimension 1 selection changed
void CItemMode::OnSelchange_ListDimension_1() 
{
	bool State = FALSE;

	// int Nb = m_pDlgItem->m_List_Dimension1.GetSelCount();

	if(m_pDlgItem->m_List_Dimension1.GetSelCount() == 1)
	{
		IQuestion* pCurQuestion = 0;
		if(m_pDlgItem->m_List_Questions.GetCurSel() != LB_ERR)
			pCurQuestion = (IQuestion *)m_pDlgItem->m_List_Questions.GetItemData(m_pDlgItem->m_List_Questions.GetCurSel());
		else if (m_pDlgItem->m_List_SAS.GetCurSel() != LB_ERR)
			pCurQuestion = (IQuestion *)m_pDlgItem->m_List_SAS.GetItemData(m_pDlgItem->m_List_SAS.GetCurSel());

		/* A VIRER
		if (pCurQuestion && pCurQuestion->CanModify())
			bool Modify = true; 

		if (pCurQuestion && pCurQuestion->HasDimensions())
			bool HasDim = true;

		if (pCurQuestion != m_pPseudoItem)
			bool DiffPseudo = true;

		if (m_pDlgItem->ValCurMode() == Mode_Palettes)
			bool IsPalette = true;
		*/

		// A VOIR Réordonnancement des items en mode construction
		// @@@ Modif Oct 2013
		bool QuestPalette = false;
		if (pCurQuestion)
		{
			CPalettes *pPalette = pCurQuestion->AsPalette();
			if (pPalette)
				QuestPalette = true;
		}
		
		if (pCurQuestion && pCurQuestion != m_pPseudoItem && pCurQuestion->HasDimensions() && m_pDlgItem->ValCurMode() == Mode_Palettes && QuestPalette)
		// if (pCurQuestion && pCurQuestion != m_pPseudoItem && pCurQuestion->HasDimensions() && m_pDlgItem->ValCurMode() == Mode_Palettes && pCurQuestion->AsPalette())
		// if(pCurQuestion && pCurQuestion != m_pPseudoItem && pCurQuestion->CanModify() && pCurQuestion->HasDimensions())
			State = TRUE;

		// Garder le 1er sélectionné
		// m_PremSelListDimension_1 = m_pDlgItem->m_List_Dimension1.GetCurSel(); 

		

		bool OKModify = true;
		
	}
	else
	{
		// Si la fenêtre visu visible, on sélectionne que le dernier élmt
		if (m_pDlgItem->IsWindowVisuVisible())
		{
			if (m_pDlgItem->m_List_Dimension1.GetSelCount() != 2)
			{
				m_PremSelListDimension_1 = -1;
			}
			else
			{
				if (m_PremSelListDimension_1 >= 0 && m_PremSelListDimension_1 < m_pDlgItem->m_List_Dimension1.GetCount())
				{
					m_pDlgItem->m_List_Dimension1.SetSel(m_PremSelListDimension_1,false); 
					m_PremSelListDimension_1 = m_pDlgItem->m_List_Dimension1.GetCurSel(); 
					m_pDlgItem->AfficherItem();
				}
			}
		}
		else
			m_PremSelListDimension_1 = -1;		
	}

	// Update liste compo mode palette si mode palette
	// m_pDlgItem->UpdateListSelCompoPalette();

	// m_pDlgItem->m_Button_Filtre   .EnableWindow(State);
	m_pDlgItem->m_Button_Dim1_UP  .EnableWindow(State);
	m_pDlgItem->m_Button_Dim1_DOWN.EnableWindow(State);

	UpdateIBuilder(true, false);
}

// Modalities dimension 2 selection changed
void CItemMode::OnSelchange_ListDimension_2() 
{
	UpdateIBuilder(true, false);
}

// Modalities dimension x selection changed
LRESULT CItemMode::OnSelChange_ListDimension_x(WPARAM wParam, LPARAM lParam)
{
	UpdateIBuilder(true, false);
	return 0;
}

// Item list selection changed
void CItemMode::OnSelchange_ListItems() 
{
	JList <CIBModalite *> pIModalite; 
	switch(m_pDlgItem->GetCheckedRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2))
	{
		case IDC_RI_RAD_LIST_1:
		{
			// Liste des valeurs
			JList<JInt32> Values;

			// Set the selected ID in the list box
			m_pDlgItem->m_List_Items.GetSelectedID(Values);
			m_pDlgItem->m_Button_Genese.EnableWindow(FALSE);

			
				
				/*== 0 ||
				m_pIBquestion->GetQuant() == 0)
				*/
		
			if(m_pIBquestion)
			{
				// m_pIBquestion->GetModalites(pIModalite);
				if(m_pIBquestion->GetID() > 0)
				{

					if (!m_pIBquestion->IsValid())
					{
						m_pIBsegment->DeleteQuestion(m_pIBquestion);
						m_pIBquestion = 0;
					}
				}

			}
			/*
			if (m_pIBquestion->GetID() > 0)
			{
				if (!m_pIBquestion->IsValid())
				{
					m_pIBsegment->DeleteQuestion(m_pIBquestion);
					m_pIBquestion = 0;
				}
			}
			*/
		
			// Mise en place de la sélection des items
			if(Values.GetCount() > 0)
			{
				// A VOIR GENESE ??????  (uniquement en mode item)
				if(Values.GetCount() == 1 && m_pDlgItem->GetTypeRepItem())
				{
					Values.MoveFirst();
					CItemConstruit * pItem = m_pDlgItem->GetItems()->GetItemConstruit(Values.GetItem());
					CIBuilder IBuilder;

					// Build List all current questions in all segment 
					// JList <JInt32> ListQuestionSAS;
					// Find_IBListQuestionSAS(ListQuestionSAS);

					// Get the item builder definition
					if(pItem && pItem->GetItemBuilder(m_pDlgItem->GetTerrain(),IBuilder))
					{
						// A VOIR if (pItem && m_pDlgItem->OnGenesePossible()) 
						//m_pDlgItem->m_Button_Genese.EnableWindow(TRUE);
						BOOL GenesePossible = TRUE;
						////////////////////////////////////////////////////////////////////////////
						// Boucle sur tous les segments
						for (int x =0; x < IBuilder.GetSegmentCount(); x++)
						{
							// Récupére le segment
							CIBSegment * pIBSegment		=	IBuilder.GetSegment(x);

							// Y a t-il déjà un item >> dans ce cas pas de genèse pour le moment
							if (pIBSegment->HasItems())
							{
								GenesePossible = FALSE;
								break;
							}
						}
						
						// Valide ou pas le bouton genèse (valide ssi seulement des questions de base)
						if (GenesePossible && m_pDlgItem->ValCurMode() == Mode_Items)
							m_pDlgItem->m_Button_Genese.EnableWindow(TRUE);
						else
							m_pDlgItem->m_Button_Genese.EnableWindow(FALSE);
					}
					else
						m_pDlgItem->m_Button_Genese.EnableWindow(FALSE);
				}

				// Mise à jour des pseudos items
				UpdatePseudoItem(Values);
			}
			else
			// Cas où aucun élmt sélectionné
			{
				// Get the items
				FindSet_Pseudo(&Values);
				UpdateQuestionList();
				OnSelchange_ListQuestions();
				UpdateIBuilder(true, false);
				delete m_pPseudoItem;
				m_pPseudoItem = 0;

				// Raz des informations
				// m_pDlgItem->OnItem_Raz();
			}

			// Maj des segments via changement items
			// MajTabSegment();
		}
		break;

		case IDC_RI_RAD_LIST_2:
		{
		}
		break;
	}

	// Update liste compo mode palette si mode palette
	if (m_pDlgItem->GetTypeRepItem() && m_pDlgItem->ValCurMode() == Mode_Palettes) 
		m_pDlgItem->UpdateListSelCompoPalette(false);

}

///////////////////////////////////////////////////////////////////////////////////
// Mise en place des pseudos items sélectionnés
void CItemMode::UpdatePseudoItem(JList<JInt32> &Values)
{
	if(!m_pPseudoItem)
	{
		m_pPseudoItem = m_pDlgItem->GetItems()->GetNewPseudo();
		CString str;
		LOADSTRING(str, IDS_RI_PSEUDO);
		JLabel Jstr = str;
		m_pPseudoItem->SetLabel(Jstr);
		m_pSelectedQuestion = 0;
	}

	if(Values.GetCount() == 1 && m_pDlgItem->GetTypeRepItem())
	{
		Values.MoveFirst();
		CItemConstruit * pItem = m_pDlgItem->GetItems()->GetItemConstruit(Values.GetItem());
		CIBuilder IBuilder;

		// Get the item builder definition
		CTerrain *pTerrain = m_pDlgItem->GetTerrain();
		if(pItem && pItem->GetItemBuilder(m_pDlgItem->GetTerrain(),IBuilder))
		{
			if (m_pDlgItem)
			{		
				BOOL GenesePossible = TRUE;
				////////////////////////////////////////////////////////////////////////////
				// Boucle sur tous les segments
				for (int x =0; x < IBuilder.GetSegmentCount(); x++)
				{
					// Récupére le segment
					CIBSegment * pIBSegment		=	IBuilder.GetSegment(x);

					// Y a t-il déjà un item >> dans ce cas pas de genèse pour le moment
					if (pIBSegment->HasItems())
					{
						GenesePossible = FALSE;
						break;
					}
				}
				
				// Valide ou pas le bouton genèse (valide ssi seulement des questions de base)
				if (GenesePossible && m_pDlgItem->ValCurMode() == Mode_Items)
					m_pDlgItem->m_Button_Genese.EnableWindow(TRUE);
				else
					m_pDlgItem->m_Button_Genese.EnableWindow(FALSE);
			}
		}
	}

	// Get the items
	FindSet_Pseudo(&Values);
	UpdateQuestionList();
	JInt32 IdPseudo = m_pPseudoItem->GetID();

	// Sélection dans boite rep item
	if (m_pDlgItem)
		m_pDlgItem->m_List_Questions.SetSelectedID(m_pPseudoItem->GetID());

	OnSelchange_ListQuestions();

	// Mise à jour globale
	UpdateIBuilder(true, false);
}

///////////////////////////////////////////////////////////////////////////////////
// Mise en place des pseudos items sélectionnés via DlgTableau
// A METTRE DANS LineMode
void CItemMode::UpdatePseudoItemLineMode(JList<JInt32> &Values)
{
	if(!m_pPseudoItem)
	{
		m_pPseudoItem = m_pDlgItem->GetItems()->GetNewPseudo();
		CString str;
		LOADSTRING(str, IDS_RI_PSEUDO);
		JLabel Jstr = str;
		m_pPseudoItem->SetLabel(Jstr);
		m_pSelectedQuestion = 0;
	}

	if(Values.GetCount() == 1)
	{
		Values.MoveFirst();
		CItemConstruit * pItem = m_pDlgItem->GetItems()->GetItemConstruit(Values.GetItem());
		CIBuilder IBuilder;

		// Get the item builder definition
		CTerrain *pTerrain = m_pDlgItem->GetTerrain();
		if(pTerrain && pItem && pItem->GetItemBuilder(m_pDlgItem->GetTerrain(),IBuilder))
		{
		}
	}

	// Get the items
	FindSet_Pseudo(&Values);
	UpdateQuestionList();
	JInt32 IdPseudo = m_pPseudoItem->GetID();

	// Mise à jour globale
	UpdateIBuilder(true, false);
}

// Items list updated
LRESULT CItemMode::OnUpdated_ItemList(WPARAM wParam, LPARAM lParam)
{
	UpdateListItems();
	// OnSelchange_ListQuestions();
	return 0;
}


// Question list updated
LRESULT CItemMode::OnUpdated_QuestionList(WPARAM wParam, LPARAM lParam)
{
	UpdateQuestionList(wParam);
	OnSelchange_ListQuestions();
	return 0;
}

void CItemMode::OnSelchange_ListValue()
{
	if(m_pIBquestion && m_pValuesDlg)
	{
		if(m_pValuesDlg->IsClass())
		{
			// Regroupement de valeurs
			m_pIBquestion->SetNumValue(0);
			m_pIBquestion->SetClass(m_pValuesDlg->GetClass());
		}
		else if(m_pValuesDlg->IsNumValue())
		{
			// Série de valeur
			m_pIBquestion->SetNumValue(m_pValuesDlg->GetNumValues());
			m_pIBquestion->SetClass(0);
		}
		else
		{
			m_pIBquestion->SetNumValue(0);
			m_pIBquestion->SetClass(0);
		}
		UpdateIBuilder(true,true);
	}
}

void CItemMode::OnChange_Value_PaletteSel()
{
	if(m_pIBquestion && m_pValuesDlg)
	{
		if(m_pValuesDlg->IsClass())
		{
			m_pIBquestion->SetNumValue(0);
			m_pIBquestion->SetClass(m_pValuesDlg->GetClass());
		}
		else if(m_pValuesDlg->IsNumValue())
		{
			m_pIBquestion->SetNumValue(m_pValuesDlg->GetNumValues());
			m_pIBquestion->SetClass(0);
		}
		else
		{
			m_pIBquestion->SetNumValue(0);
			m_pIBquestion->SetClass(0);
		}
		UpdateIBuilder(true,true);
	}
}

// Message sent from values dialog
void CItemMode::OnValuesQuantile(LONG Type)
{
	// nothing to do
}

// Updates the question list
void CItemMode::UpdateQuestionList(long BlockId)
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

	/* A VOIR OCTOBRE 2006 >> ici on ne supprime plus les élémnts sélectionnées, on vire uniquement les questions
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
	*/

	int Nbelmt = m_pDlgItem->m_List_Questions.GetCount();

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
	if (IDs < 0)
		m_pDlgItem->m_List_Questions.SetSelectedID(IDs); 
	else
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

/// Updates the question list en mode genèse
void CItemMode::UpdateQuestionListGenese(long BlockId)
{
	int TopBlock = 0;

	// Get the selected question
	JInt32 IDs = m_pDlgItem->m_List_Questions.GetSelectedID();   

	JList<IQuestion const *> & Question_List = m_pDlgItem->m_Tree_Theme.GetQuestionList();

	m_pDlgItem->m_List_Questions.ResetContent();
	// Find the questions coresponding to the current mode
	for(Question_List.MoveFirst();Question_List.IsValid(); Question_List.MoveNext())
	{
		IQuestion const * pQuestion = Question_List.GetItem();
		if(pQuestion->IsInContingent(m_pDlgItem->GetContingentMask()))
		{
			int Pos = m_pDlgItem->m_List_Questions.AddItem(pQuestion);
			if(BlockId !=0 && pQuestion->IsBlocQuestion() && pQuestion->GetBlocID() == BlockId)
				TopBlock = Pos;
		}
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

	// Redraw
	m_pDlgItem->m_List_Questions.Invalidate();
}

// Update the Modalite list box
void CItemMode::UpdateDimensionsList()
{
	// Reset Dimension1 list box etc..
	m_Dim_Sel.Reset();
	// Save old dimension types
	m_DimType_Old.Swap(m_DimType_New);

	int Dim = 0;
	int Dlg = 0;

	for(Dim=0; Dim < m_DimType_Old.GetCount() ; ++Dim)
	{
		if(Dim==0)
		{
			// Get the first dimension
			m_pDlgItem->m_List_Dimension1.GetSelectedID(m_Dim_Sel.AddTail());
		}
		else if(Dim==1)
		{
			// Get the second dimension
			m_pDlgItem->m_List_Dimension2.GetSelectedID(m_Dim_Sel.AddTail());
			Dlg = 0;
		}
		else
		{
			m_DimDlgPos.MoveTo(Dlg);
			m_DimDlg.MoveTo(Dlg);
			if(m_DimDlgPos.IsValid() && m_DimDlg.IsValid())
			{
				CDimensionDlg & Dlg = m_DimDlg.GetItem();
				Dlg.GetSelectedID(m_Dim_Sel.AddTail());
				if(!m_DimDlgPos.IsValid())
				{
					m_DimDlgPos.AddTail();
					m_DimDlgPos.MoveLast();
				}
				Dlg.GetWindowRect(&(m_DimDlgPos.GetItem()));
			}
			++Dlg;
		}
	}

	// Remove all dialogs
	m_DimDlg.Reset();
	// Remove all dimension types
	m_DimType_New.Reset();
	// Remove all dimension modalities
	m_DimList.Reset();
	m_pDlgItem->m_List_Dimension1.ResetContent();
	m_pDlgItem->m_List_Dimension2.ResetContent();

	CString TextDim1 = "";
	CString TextDim2 = "";

	// Récup texte en cours headers dimension 1 et 2
	CString ExTextDim1;
	CString ExTextDim2;
	m_pDlgItem->m_Header_Dimension1.GetWindowText(ExTextDim1); 
	m_pDlgItem->m_Header_Dimension2.GetWindowText(ExTextDim2); 
	
	if(m_pSelectedQuestion)
	{
		// Get the Dimensions
		JInt32 Dims = m_pSelectedQuestion->HasDimensions();
		for(JInt32 x = 0; x < Dims ; x++)
		{
			JVector<const IModalite *> & vDims = m_DimList.AddTail();
			m_pSelectedQuestion->GetIModalites(vDims,0,x,m_pDlgItem->GetContingentMask());
			m_DimType_New.AddTail() = m_pSelectedQuestion->DimensionType(x);

			if(x == 0)
			{
				// Update the dimension 1 Dimension1 list box
				m_pDlgItem->m_List_Dimension1.AddItems(vDims);

				// Filtre les modalités via Map Filtre s'il existe avec au moins 1 elmt
				if (m_pDlgItem->m_MapFiltrage.GetCount() > 0)
				{
					// Il faut valider ou devalider le bouton filtrage (ssi dimension 1 de la question a des ascendances)
					FiltreModaliteListDimension1();
				}
	
				// Voir si cette question a une arborencence parent sur la 1ere dimension, auquel cas filtrage autorise
				if (x==0)
				{
					JMap <JInt32, JInt32> MapEnfantParent;
					JBool AuMoins1Parent = false;
					m_pDlgItem->GetTerrain()->GetLienMarqueSecteur(MapEnfantParent);
						
					// Boucle sur toutes les modalités en cours sur dimension 1
					for (vDims.MoveFirst(); vDims.IsValid(); vDims.MoveNext())
					{
						// Récupère la modalité correspondante
						const IModalite* pModalite = vDims.GetItem();

						// Test type modalité : il faut que ca soit un elmt de type normalisé
						if (pModalite->IsElement())
						{
							const CElement *pElmtModa = pModalite->AsElement();
							JInt32 CodeElmt = pElmtModa->GetID();

							// Le code de la modalité
							JInt32 Code = pModalite->GetID();

							// On vérifiee que la clef marque existe dans map des liens marque / secteur
							MapEnfantParent.MoveTo(Code);
							if (MapEnfantParent.IsValid())
							{
								AuMoins1Parent = true;
								break;
							}
						}
					}

					if (AuMoins1Parent > 0)
					{
						// Valide le bouton filtre
						m_pDlgItem->m_Button_Filtre		.EnableWindow(TRUE);
					}
					else
					{
						// Rend invalide le bouton filtre
						m_pDlgItem->m_Button_Filtre		.EnableWindow(FALSE);
					}
				}
				
				m_pDlgItem->m_List_Dimension1.SetTopIndex(0);
				switch(m_pSelectedQuestion->DimensionType(x))
				{
					case IQuestion::MODALITE:
						{	
							if (m_pSelectedQuestion->IsQuestion() || m_pSelectedQuestion->IsPseudo())
							{
								// Affichage titre dimension1 si question
								if (Dims > 1)
									// Affichage titre dimension1 si question (et + d'une dimension)
									TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION),x+1);
								else
								{		
									if (m_pDlgItem->ValCurMode() == 1)
									{
										// en mode item uniquement
										TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION_UNIQUE));
									}
									else
									{
										// tous les autres modes
										TextDim1.Format(GETSTRING(IDS_RI_COMPOSANTS));
									}
								}
							}
							else
								// Sinon c'est un composant
								TextDim1.Format(GETSTRING(IDS_RI_COMPOSANTS));
						}
						break;

					case IQuestion::ELEMENT:
					{
							// if (m_pSelectedQuestion->IsQuestion())
							if (m_pSelectedQuestion->IsPseudo())
							{
								if (Dims > 1)
									// Affichage titre dimension1 si question (et + d'une dimension)
									TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION),x+1);
								else
								{
									if (m_pDlgItem->ValCurMode() == 1)
									{
										// en mode item uniquement
										TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION_UNIQUE));
									}
									else
									{
										// tous les autres modes
										TextDim1.Format(GETSTRING(IDS_RI_COMPOSANTS));
									}
								}
							}
							else
							{
								// Sinon c'est un composant
								if (Dims > 1)
									TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION),x+1);
								else
								{		
									if (m_pDlgItem->ValCurMode() == 1)
									{
										// en mode item uniquement
										TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION_UNIQUE));
									}
									else
									{
										// tous les autres modes
										TextDim1.Format(GETSTRING(IDS_RI_COMPOSANTS));
									}
								}
							}
						}
						break;
					default:
						return;
				}
			}
			else if(x==1)
			{
				// Update the dimension 2 Dimension1 list box
				m_pDlgItem->m_List_Dimension2.AddItems(vDims); 
				m_pDlgItem->m_List_Dimension2.SetTopIndex(0);
				switch(m_pSelectedQuestion->DimensionType(x))
				{
					case IQuestion::MODALITE:
						TextDim2.Format(GETSTRING(IDS_RI_DIM_DIMENSION),x+1);
						break;
					case IQuestion::ELEMENT:
						TextDim2.Format(GETSTRING(IDS_RI_DIM_DIMENSION),x+1);
						break;
					default:
						return;
				}
				m_DimDlgPos.MoveFirst();
			}
			else
			{
				// Get the Dimensions Dialog
				CDimensionDlg & Dlg = m_DimDlg.AddTail();
				// Set the colours
				Dlg.m_CATPColorsApp = m_pDlgItem->m_CATPColorsApp;
				// Greate the Dialog
				Dlg.Create(CDimensionDlg::IDD, m_pDlgItem);
				// Check out the dimension type
				switch(m_pSelectedQuestion->DimensionType(x))
				{
					case IQuestion::MODALITE:
						// Dimension1
						Dlg.SetData(IQuestion::MODALITE,x,vDims);
						break;
					case IQuestion::ELEMENT:
						// Dimension2
						Dlg.SetData(IQuestion::ELEMENT,x,vDims);
						break;

					default:
						return;
				}

				if(m_DimDlgPos.IsValid())
				{
					Dlg.MoveWindow(&m_DimDlgPos.GetItem(),TRUE);
					m_DimDlgPos.MoveNext();
				}
				else
				{
					// Positionnement de la boite de dialogue dimension 3
					CRect Rect1,Rect2,RectBoxQu;
					m_pDlgItem->m_Box_Question.GetWindowRect(&RectBoxQu);
					m_pDlgItem->m_Box_Dimension1.GetWindowRect(&Rect1);
					Dlg.GetWindowRect(&Rect2);

					if (m_pDlgItem->GetTypeRepItem() == true) 
						// En mode rep item
						Rect2.MoveToXY(Rect1.left+(Rect2.Width()*(x-2)),Rect1.bottom + 5);
					else
						// En mode pupitre pour dégager les fleches sélection lig, colo ou page
						Rect2.MoveToXY(RectBoxQu.right - Rect2.Width() - 50,Rect1.bottom + 5);
					
					Dlg.MoveWindow(Rect2,TRUE);
				}
				Dlg.SetIcones(m_pDlgItem->m_FIcones); 
				Dlg.ShowWindow(SW_SHOW);
			}
		}
	}

	// Réaffiche les textes headers dimension 1 et 2
	// TextDim1 = "Toto";
	TextDim1.Format(GETSTRING(IDS_RI_DIM_SEL_NEW_INGREDIENT),"");
	if (ExTextDim1 != TextDim1)
		m_pDlgItem->m_Header_Dimension1.SetText(TextDim1); 
	if (ExTextDim2 != TextDim2)
		m_pDlgItem->m_Header_Dimension2.SetText(TextDim2); 

	m_pDlgItem->m_List_Dimension1.Invalidate();
	m_pDlgItem->m_List_Dimension2.Invalidate();
}

// Update the Modalite list box apres filtre secteur
void CItemMode::UpdateFiltreDimension1List()
{
	// Reset Dimension1 list box etc..
	m_Dim_Sel.Reset();

	// Get the first dimension
	m_pDlgItem->m_List_Dimension1.GetSelectedID(m_Dim_Sel.AddTail());

	// Remove all dialogs / ici c'était la dimension 3
	// m_DimDlg.Reset();

	// Remove all dimension types
	m_DimType_New.Reset();

	// Remove all dimension modalities
	m_DimList.Reset();
	m_pDlgItem->m_List_Dimension1.ResetContent();
	
	CString TextDim1 = "";
	
	if(m_pSelectedQuestion)
	{
		// Get the Dimensions
		JInt32 Dims = m_pSelectedQuestion->HasDimensions();
		
		JVector<const IModalite *> & vDims = m_DimList.AddTail();
		m_pSelectedQuestion->GetIModalites(vDims,0,0,m_pDlgItem->GetContingentMask());
		m_DimType_New.AddTail() = m_pSelectedQuestion->DimensionType(0);

		// Update the dimension 1 Dimension1 list box
		m_pDlgItem->m_List_Dimension1.AddItems(vDims);

		// Filtre les modalités via Map Filtre s'il existe avec au moins 1 elmt
		if (m_pDlgItem->m_MapFiltrage.GetCount() > 0)
		{
			// Il faut valider ou devalider le bouton filtrage (ssi dimension 1 de la question a des ascendances)
			FiltreModaliteListDimension1();
		}

		// Voir si cette question a une arborencence parent sur la 1ere dimension, auquel cas filtrage autorise
		JMap <JInt32, JInt32> MapEnfantParent;
		JBool AuMoins1Parent = false;
		m_pDlgItem->GetTerrain()->GetLienMarqueSecteur(MapEnfantParent);

		// Boucle sur toutes les modalités en cours sur dimension 1
		for (vDims.MoveFirst(); vDims.IsValid(); vDims.MoveNext())
		{
			// Récupère la modalité correspondante
			const IModalite* pModalite = vDims.GetItem();

			// Test type modalité : il faut que ca soit un elmt de type normalisé
			if (pModalite->IsElement())
			{
				const CElement *pElmtModa = pModalite->AsElement();
				JInt32 CodeElmt = pElmtModa->GetID();

				// Le code de la modalité
				JInt32 Code = pModalite->GetID();

				// On vérifiee que la clef marque existe dans map des liens marque / secteur
				MapEnfantParent.MoveTo(Code);
				if (MapEnfantParent.IsValid())
				{
					AuMoins1Parent = true;
					break;
				}
			}
		}

		if (AuMoins1Parent > 0)
		{
			// Valide le bouton filtre
			m_pDlgItem->m_Button_Filtre		.EnableWindow(TRUE);
		}
		else
		{
			// Rend invalide le bouton filtre
			m_pDlgItem->m_Button_Filtre		.EnableWindow(FALSE);
		}
		
		m_pDlgItem->m_List_Dimension1.SetTopIndex(0);
		switch(m_pSelectedQuestion->DimensionType(0))
		{
			case IQuestion::MODALITE:
			{	
				if (m_pSelectedQuestion->IsQuestion() || m_pSelectedQuestion->IsPseudo())
				{
					// Affichage titre dimension1 si question
					if (Dims > 1)
						// Affichage titre dimension1 si question (et + d'une dimension)
						TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION),1);
					else
					{		
						if (m_pDlgItem->ValCurMode() == 1)
						{
							// en mode item uniquement
							TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION_UNIQUE));
						}
						else
						{
							// tous les autres modes
							TextDim1.Format(GETSTRING(IDS_RI_COMPOSANTS));
						}
					}
				}
				else
					// Sinon c'est un composant
					TextDim1.Format(GETSTRING(IDS_RI_COMPOSANTS));
			}
			break;

			case IQuestion::ELEMENT:
			{
				if (m_pSelectedQuestion->IsPseudo())
				{
					if (Dims > 1)
						// Affichage titre dimension1 si question (et + d'une dimension)
						TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION),1);
					else
					{
						if (m_pDlgItem->ValCurMode() == 1)
						{
							// en mode item uniquement
							TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION_UNIQUE));
						}
						else
						{
							// tous les autres modes
							TextDim1.Format(GETSTRING(IDS_RI_COMPOSANTS));
						}
					}
				}
				else
				{
					// Sinon c'est un composant
					if (Dims > 1)
						TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION),1);
					else
					{		
						if (m_pDlgItem->ValCurMode() == 1)
						{
							// en mode item uniquement
							TextDim1.Format(GETSTRING(IDS_RI_DIM_DIMENSION_UNIQUE));
						}
						else
						{
							// tous les autres modes
							TextDim1.Format(GETSTRING(IDS_RI_COMPOSANTS));
						}
					}
				}
				break;

				default:
					return;
			}
		}
	}

	// Update infos list dimension 1
	m_pDlgItem->m_List_Dimension1.Invalidate();
}

// Filtrage modalité
void CItemMode::FiltreModaliteListDimension1()
{
	JMap <JInt32, JInt32> MapEnfantParent;
	m_pDlgItem->GetTerrain()->GetLienMarqueSecteur(MapEnfantParent);

	// Attention si aucun filtre sélectionné, c'est comme si tous sélectionné
	JBool FiltreValide = false;; 
	for (m_pDlgItem->m_MapFiltrage.MoveFirst(); m_pDlgItem->m_MapFiltrage.IsValid(); m_pDlgItem->m_MapFiltrage.MoveNext())
	{
		if (m_pDlgItem->m_MapFiltrage.GetItem())
		{
			FiltreValide = true;
			break;
		}
	}

	// Nombre de modalites
	JUnt32 NbModas = m_pDlgItem->m_List_Dimension1.GetCount();

	if (FiltreValide)
	{
		for (JInt32 Idx = NbModas-1; Idx >= 0; Idx--)
		{
			// Récupère la modalité correspondante
			const IModalite* pModalite = (const IModalite*)m_pDlgItem->m_List_Dimension1.GetItemData(Idx);

			// Le code de la modalité
			JInt32 Code = pModalite->GetID();

			// Recup Code Filtrage associé
			MapEnfantParent.MoveTo(Code);
			if (MapEnfantParent.IsValid())
			{
				JInt32 IdSecteurModa = MapEnfantParent.GetItem();

				// On vérifie que la modalité appartient bien aux elmts filtrés
				m_pDlgItem->m_MapFiltrage.MoveTo(IdSecteurModa);
				if (!m_pDlgItem->m_MapFiltrage.IsValid())
				{
					// Sinon on vire ces modalités
					m_pDlgItem->m_List_Dimension1.RemoveItem(Code);
				}
				else
				{
					// Test Flag Sélection
					if (!m_pDlgItem->m_MapFiltrage.GetItem())
					{
						// Si non sélectionné
						m_pDlgItem->m_List_Dimension1.RemoveItem(Code);
					}
				}
			}
		}

		// Boucle sur toutes les modalités dimension 1 pour mettre à jour sélection courante
		int NbCurModas = m_pDlgItem->m_List_Dimension1.GetCount();
		for (JInt32 IdxModa = 0; IdxModa < m_pDlgItem->m_List_Dimension1.GetCount(); IdxModa++)
		{
			// Récupère la modalité correspondante
			const IModalite* pModalite = (const IModalite*)m_pDlgItem->m_List_Dimension1.GetItemData(IdxModa);

			// Le code de la modalité
			JInt32 Code = pModalite->GetID();

			// Voir si cette modalité est séelctionné (via map générale)
			m_pDlgItem->m_MapSelModaDimension1.MoveTo(Code);
			if (m_pDlgItem->m_MapSelModaDimension1.IsValid())
			{
				m_pDlgItem->m_List_Dimension1.SetSel(IdxModa);
			}
		}
	}
}

void CItemMode::UpdateListItems()
{
	switch(m_pDlgItem->GetCheckedRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2))
	{
		case IDC_RI_RAD_LIST_1:
		{
			// Chargement de la liste des items
			// Ajouter chargement des entetes chapitres items  (1er chapitre = brouillon) 
			JList<IItem const *> vItems;
			m_pDlgItem->m_List_Items.ResetContent(); 
			m_pDlgItem->GetItems()->GetItemsIndependant(vItems,m_pDlgItem->GetTerrain());
			
			///////////////////////////////////////////////////////////////////////////////////
			// Get only those valid for this terrain
			for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
			{
				if (IsSearched(vItems.GetItem()))
				{
					// Ident de l'item
					JInt32 IdItem = vItems.GetItem()->GetID();
				
					// Il faut de plus que l'item soit visualisable (sélectionné dans une rubrique item)
					if (m_pDlgItem->ShowItem(IdItem))
					{
						long Pos = m_pDlgItem->m_List_Items.AddItem(vItems.GetItem());
						if(m_pPseudoItem && m_pPseudoItem->HasItem(vItems.GetItem()->GetID()))
							m_pDlgItem->m_List_Items.SetSel(Pos, TRUE);
					}
				}
			}

			// Le faire ssi mode non palette et dans rep items
			if (m_pDlgItem->ValCurMode() != 4 && m_pDlgItem->GetTypeRepItem())
			{
				// NOUVEAU --- //////////////////////////////////////////////////////////////
				// Mise à jour des segments avec la nouvelle sélection d'items dans ListItem
				int SelSeg = -1;
				if (FindDelete_IBMapItem(SelSeg))
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

					// Visu final
					DoVisualisation();
				}
			}
			/////////////////////////////////////////////////////////////////////////////

			m_pDlgItem->m_List_Items.Invalidate();
		}
		break;

		case IDC_RI_RAD_LIST_2:
		{
		}
		break;
	}
}

// Sets the current item segment questions into the SAS
void CItemMode::SetActiveSegmentQuestions()
{
	int NbQu = m_pDlgItem->m_List_SAS.GetCount();

	int CurId = m_pDlgItem->m_List_SAS.GetSelectedID();
	// Get the selected question
	m_pDlgItem->m_List_SAS.ResetContent();

	JList<JInt32> Items;
	if(m_pIBsegment)
	{
		m_pIBsegment->GetQuestionsID(Items);
		for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		{
			IQuestion const * pQuestion = m_pDlgItem->GetTerrain()->GetQuestionByID(Items.GetItem());
			if(pQuestion && pQuestion->IsInContingent(m_pDlgItem->GetContingentMask()))
				m_pDlgItem->m_List_SAS.AddItem(pQuestion); 
		}

		Items.Reset();
		m_pIBsegment->GetItemsID(Items);
		for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		{
			IQuestion const * pQuestion = m_pDlgItem->GetItems()->GetQuestionByID(Items.GetItem());

			if(pQuestion)
			{
				m_pDlgItem->m_List_SAS.AddItem(pQuestion);
			}
			else
			{
				if(m_pPseudoItem && Items.GetItem()==0)
					m_pDlgItem->m_List_SAS.AddItem(m_pPseudoItem);
			}
		}
	}

	int NbQuSas = m_pDlgItem->m_List_SAS.GetCount();
	
	if(!m_pDlgItem->m_List_SAS.SetSelectedID(CurId))
	{
		int IDQuSel = m_pDlgItem->m_List_Questions.GetSelectedID();
		m_pDlgItem->m_List_SAS.SetSelectedID(m_pDlgItem->m_List_Questions.GetSelectedID());
	}

	m_pDlgItem->m_List_SAS.Invalidate();
}

// Finds the current segment or creates a new segment
bool CItemMode::FindSet_IBsegment(int Index, bool bCopy/*= false*/,bool UpdateQuLst /*= true*/)
{
	// Flag for segment found
	bool bFound = true;
	// Previous segment
	CIBSegment * pSegment = m_pIBsegment;
	m_pIBsegment = 0;
	// No current question
	m_pIBquestion = 0;
	// No current items
	m_pIBitem = 0;


	int Count = m_IBuilder.GetSegmentCount();
	if(Count <= Index)
	{
		bFound = false;
		m_pIBsegment = new CIBSegment();

		if(Index && pSegment && bCopy)
		{
			// TODO: Copy the segment need a segment copier
			std::string Def;
			pSegment->AsString(Def);
			if(Def.length())
			{
				const char *pData = Def.c_str();
				++pData;
				m_pIBsegment->FromString(&pData);
			}
			else
				m_pIBsegment->SetOperator(IBloc::And);
		}
		else
			m_pIBsegment->SetOperator(IBloc::And);

		m_IBuilder.Add(m_pIBsegment);
	}
	else
	{
		m_pIBsegment = m_IBuilder.GetSegment(Index);
	}
	if(m_pIBsegment)
		SetQuestionOperator(m_pIBsegment->GetOperator());

	// Make sure that any questions in this segment are selected
	UpdateSegmentTabs(Index);

	// Check for Pseudo Question
	if(m_pIBsegment->HasItems() && m_pIBsegment->FindItem(0))
	{
		FindSet_Pseudo();						// ????????????????????????
	}
	m_pDlgItem->m_List_SAS.ResetContent();

	// Initialise boite liste des questions
	// m_pDlgItem->m_List_Questions.ResetContent();
	// Initialise boite liste des questions
	m_pDlgItem->InitListQuestion();

	if (UpdateQuLst)
		UpdateQuestionList();

	SetActiveSegmentQuestions(); 
	return bFound;
}

// Finds the corresponding question in the current item
// and sets the modality selections accordingly
void CItemMode::FindSet_IBcontainerByType(const IQuestion * pQuestion)
{
	if(pQuestion)
	{
		// Get the right question
		if(pQuestion->IsQuestion())
			FindSet_IBquestion(m_pSelectedQuestion->GetID());
		else if(pQuestion->IsPseudo())
			FindSet_Pseudo();
		else if(pQuestion->IsCritereModifie())
			FindSet_IBitem(m_pSelectedQuestion->GetID());
		else if(pQuestion->IsCritereQuantile())
			FindSet_IBitem(m_pSelectedQuestion->GetID());
		else if(pQuestion->IsPalette())
			FindSet_IBitem(m_pSelectedQuestion->GetID());
		else if(pQuestion->IsQuestionInduite())
			FindSet_IBitem(m_pSelectedQuestion->GetID());
	}
}

// Finds the current critere quantile in the segment or creates a new question
// returns true if found
bool CItemMode::FindSet_IBitem(int id)
{
	m_pIBitem = m_pIBsegment->FindItem(id);

	if(m_pIBitem)
		return true;

	// Set the operator
	m_pIBitem = new CIBItem;
	m_pIBitem->SetOperator(IBloc::Or);
	m_pIBitem->SetID(id);
	m_pIBsegment->Add(m_pIBitem);
	SetDimension1Operator(m_pIBitem->GetOperator());

	return false;
}

// Finds the Items in the segment
void CItemMode::FindSet_Pseudo(JList<JInt32> *pValues, bool MajOpe)
{
	if (!m_pIBsegment)
		return;

	// Item Pseudo défini par ident 0
	m_pIBitem = m_pIBsegment->FindItem(0);
	if (!m_pIBitem)
	{
		// Set the operator
		m_pIBitem = new CIBItem;
		m_pIBitem->SetOperator(IBloc::Or);
		m_pIBitem->SetID(0);
		m_pIBsegment->Add(m_pIBitem);
	}

	if(m_pIBitem)
	{
		// Set the operator 
		if (MajOpe)
			SetDimension1Operator(m_pIBitem->GetOperator());

		if(m_pPseudoItem && pValues)
		{
			JMap<JInt32,JBool> map;
			// Get current Items
			JList<JInt32> val;
			m_pPseudoItem->GetItems(val);
			// Check that the Items are present
			for(val.MoveFirst(); val.IsValid(); val.MoveNext())
			{
				map.MoveTo(val.GetItem());
				if(!map.IsValid())
					map.Add(val.GetItem()) = false;
			}

			for(pValues->MoveFirst(); pValues->IsValid(); pValues->MoveNext())
			{
				map.MoveTo(pValues->GetItem());
				if(!map.IsValid())
					map.Add(pValues->GetItem()) = true;
				else
					map.GetItem() = true;
			}

			JList<JInt32> Add;
			JList<JInt32> Rem;
			for(map.MoveFirst(); map.IsValid(); map.MoveNext())
			{
				if (map.GetItem()) 
					Add.AddTail() = map.GetKey();
				else
					Rem.AddTail() = map.GetKey();
			}

			// Big problem !!!! la mise à jour n'est faite que sur le segment en cours
			// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			if (Rem.GetCount() > 0) 
				m_pPseudoItem->RemoveItems(Rem);

			if (Add.GetCount() > 0) 
				m_pPseudoItem->AddItems(Add);
			// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		}
	}
}

// Finds the current question in the segment or creates a new question
bool CItemMode::FindSet_IBquestion(int Quid)
{
	bool bFound = false;

	// MArs 2006
	if (!m_pIBsegment)
		bFound = false;
	else
        m_pIBquestion = m_pIBsegment->FindQuestion(Quid);

	// avant pas de test m_pIBSegment
	// m_pIBquestion = m_pIBsegment->FindQuestion(Quid);

	if(m_pIBquestion)
	{
		bFound = true;
	}
	else
	{
		m_pIBquestion = new CIBQuestion;
		m_pIBquestion->SetID(Quid);
		m_pIBsegment->Add(m_pIBquestion);

		// Set up the modalities
		JList<JInt32> Values;

		for(int x = 0; x < m_pSelectedQuestion->HasDimensions(); x++)
		{
			CIBModalite * pModalite = new CIBModalite;
			m_pIBquestion->AddModalite(pModalite);
			pModalite->SetOperator(IBloc::Or);
		}
	}
	return bFound;
}

// Finds all current questions in all segment or delete them
bool CItemMode::FindDelete_IBMapquestion(JMap <JInt32,JInt32> &MapQuestion, int &SelSeg)
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
							NbSuppOk++;

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

// Build Map all current questions in all segment 
void CItemMode::Find_IBListQuestionSAS(JList <JInt32> &ListQuestionSAS)
{
	// Rest map des quetsions
	ListQuestionSAS.Reset();

	// Nombre de segments
	int Count = m_IBuilder.GetSegmentCount();
	
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

				// Ajoute à la liste globale si nouvelle question
				ListQuestionSAS.MoveTo(IDQuestion);
				if (!ListQuestionSAS.IsValid())
				{
					ListQuestionSAS.AddTail() = IDQuestion;
				}
			}
		}
	}
}


// Finds all current questions in all segment or delete them  (via m_pPseudoItem)
// %%%%%%%%%%%%%%%%%%%%%%%
bool CItemMode::FindDelete_IBMapItem(int &SelSeg)
{

	JList <JInt32> LstValidItems;

	if (m_pPseudoItem)
	{
		// Liste des items valids
		m_pPseudoItem->GetItems(LstValidItems); 
	}

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
			// Récup les items du segment
			JList<JInt32> LstItemsID;
			pIBsegmentTmp->GetItemsID(LstItemsID); 

			// Balaye les questions, et vire ceux qui ne sont plus sélectionnées dans le TreeItem
			for(LstItemsID.MoveFirst(); LstItemsID.IsValid(); LstItemsID.MoveNext())
			{
				// Récup Ident Item
				JInt32 IDItem = LstItemsID.GetItem();

				// Vérifie qu'il est toujours sélectionné dals la liste des items
				// Récupère l'objet question
				CIBItem *	pIBItem	=	pIBsegmentTmp->FindItem(IDItem);
				if(pIBItem)
				{
					// Liste des modalités items
					JList<JInt32> Moda;
					pIBItem->GetValues(Moda);

					// Get the modalities et on supprime les indésirables
					bool ModaModif = false;
					for(Moda.MoveFirst(); Moda.IsValid(); Moda.MoveNext())
					{
						JInt32 ModaItemId = Moda.GetItem();

						bool ModaTrouve = false;
						for (LstValidItems.MoveFirst(); LstValidItems.IsValid(); LstValidItems.MoveNext())
						{	
							JInt32 Id = LstValidItems.GetItem();
							if (Id == ModaItemId)
							{
								ModaTrouve = true;
								break;
							}
						}
						if (ModaTrouve == false)
						{
							Moda.Remove(); 
							ModaModif = true;
							NbSuppOk++;
						}
					}

					// Remet les valeurs
					if (ModaModif)
						pIBItem->SetValues(Moda); 

					if (pIBItem && !pIBItem->IsValid()) 
					{
						// On vire l'item du segment
						pIBsegmentTmp->DeleteItem(pIBItem);
					}								

					// On vérifie que le segment a toujours lieu d'être ("non vide")	
					if(!pIBsegmentTmp->IsValid())
					{
						m_IBuilder.DeleteSegment(pIBsegmentTmp);
						if (pIBsegmentTmp == m_pIBsegment)
						{
							m_pIBsegment = 0;
						}
						pIBsegmentTmp = 0;

						if (pIBItem == m_pIBitem)
							m_pIBitem		= 0;

						Count--;
						if(Count < SelSeg)
							SelSeg--;
					}
				}
			}
		}
	}
	return (NbSuppOk > 0);
}


// Selects the modalities in the items current question
bool CItemMode::SetSelectedDimensions()
{
	bool bUpdate = false;

	if(!m_pSelectedQuestion)
		return bUpdate;

	if(m_pSelectedQuestion->IsQuestion() && m_pIBquestion)
	{
		int x = 0;

		// Load the modalities
		JList< CIBModalite *> pIModalite;
		m_pIBquestion->GetModalites(pIModalite);

		for(x=0, m_Dim_Sel.MoveFirst(), pIModalite.MoveFirst(); pIModalite.IsValid() ; pIModalite.MoveNext(), m_Dim_Sel.MoveNext(), x++)
		{
			CIBModalite * pModalite = pIModalite.GetItem();
			// Get the modalities used
			JList<JInt32> Values;
			pModalite->GetValues(Values);
			m_DimType_Old.MoveTo(x);
			m_DimType_New.MoveTo(x);

			if(x==0)
			{
				// Set the operator 
				SetDimension1Operator(pModalite->GetOperator());
				if(Values.GetCount())
					m_pDlgItem->m_List_Dimension1.SetSelectedID(Values);
				else if (m_Dim_Sel.IsValid() && m_DimType_Old.IsValid() && m_DimType_New.IsValid())
				{
					if((m_DimType_Old.GetItem() == m_DimType_New.GetItem()) && m_pDlgItem->m_List_Dimension1.SetSelectedID(Values))
						bUpdate = true;
				}
			}
			else if(x==1)
			{
				// Set the operator 
				SetDimension2Operator(pModalite->GetOperator());
				if(Values.GetCount())
					m_pDlgItem->m_List_Dimension2.SetSelectedID(Values);
				else if (m_Dim_Sel.IsValid() && m_DimType_Old.IsValid() && m_DimType_New.IsValid())
				{
					if((m_DimType_Old.GetItem() == m_DimType_New.GetItem()) && m_pDlgItem->m_List_Dimension2.SetSelectedID(Values))
						bUpdate = true;
				}
				// Move to first modeless modalites dialog
				m_DimDlg.MoveFirst(); 
			}
			else
			{
				if(m_DimDlg.IsValid())
				{
					CDimensionDlg & Dlg = m_DimDlg.GetItem();
					Dlg.SetDimensionOperator(pModalite->GetOperator());
					if(Values.GetCount())
						Dlg.SetSelectedID(Values);
					else if (m_Dim_Sel.IsValid() && m_DimType_Old.IsValid() && m_DimType_New.IsValid())
					{
						if((m_DimType_Old.GetItem() == m_DimType_New.GetItem()) && Dlg.SetSelectedID(Values))
							bUpdate = true;
					}
					// Move to next modeless modalites dialog
					m_DimDlg.MoveNext();
				}
			}
		}
	}
	else
	{
		if(m_pSelectedQuestion->HasDimensions() && m_pIBitem)
		{
			JList<JInt32> Values;
			m_pIBitem->GetValues(Values);
			
			// Set the operator 
			SetDimension1Operator(m_pIBitem->GetOperator());
			// Set the selected ID in the list box
			m_pDlgItem->m_List_Dimension1.SetSelectedID(Values);
		}
	}
	m_Dim_Sel.Reset();
	return bUpdate;
}

// Updates the current item and calculates it
void CItemMode::UpdateIBuilder(bool Changed, bool bValues)
{
	m_bItemValid = true;

	// Get the Item details Selected Dimension1 and Dimension2 et ou etc....
	bool ItemsValid = false;
	UpdateItemDetails(ItemsValid);

	// Update Item Values Dialog for quanti vars
	m_bItemValid = UpdateItemValues(bValues);

	// If the current question is not valid remove it
	if (m_pIBquestion && m_pIBquestion->IsValid())
		int NonValid = 0;

	if(m_pIBquestion && (!m_pIBquestion->IsValid() || !m_bItemValid))
	{
		m_pIBsegment->RemoveQuestion(m_pIBquestion);
	}
	// Remove any Invalide items
	if(m_pIBitem && !m_pIBitem->IsValid())
	{
		m_pIBsegment->RemoveItem(m_pIBitem);

		// sur tous les segments
	}

	if(Changed)
	{
		// Put empty results
		CString SEffectif;
		CString SPercent;
		CString SNbCas;
		bool EffectifChange = false;

		if(m_pDlgItem->IsEffectiveVisible())
		{
			EffectifChange = CalculateEffectif(SEffectif, SPercent, SNbCas);
		}

		// Mise à jour des effectifs
		m_pDlgItem->m_Static_Effectif.SetText(SEffectif + " " );
		m_pDlgItem->m_Static_NbCas   .SetText(SNbCas +" ");
		m_pDlgItem->m_Static_Percent .SetText(SPercent + " ");
	}

	// Set active items 
	SetActiveSegmentQuestions(); 

	// Put back the current invalid question
	if(m_pIBquestion && (!m_pIBquestion->IsValid() || !m_bItemValid))
	{
		m_pIBsegment->Add(m_pIBquestion);
		m_bItemValid = false;
	}

	// Put back invalid items
	if(m_pIBitem && !m_pIBitem->IsValid())
	{
		m_pIBsegment->Add(m_pIBitem);
		m_bItemValid= false;
	}

	if(!m_IBuilder.IsValid())
		m_bItemValid = false;

	DoVisualisation();
}

//////////////////////////////////////////////////////////////////////////////////
// Récupère les infos segments et IBuilder via les infos Questions et Items
void CItemMode::SetSegmentIBuilder()
{
	// rien
}

/////////////////////////////////////////////////////////////////////////////////
// Positionne le Pseudo Item
void CItemMode::SetPseudoItem()
{
	// rien		
}

// Updates the item under construction with the selected modalitites
void CItemMode::UpdateItemDetails(bool & ItemsValid)
{
	// Flag pour savoir si au moins 1 sélectionné
	bool AuMoins1Sel = false;

	// Get and set the segment operator
	m_IBuilder.SetOperator(GetSegmentOperator());

	// Check if we have a segment 
	if(m_pIBsegment)
	{
		// Get and set the question operator
		m_pIBsegment->SetOperator(GetQuestionOperator());
		// For Questions
		if(m_pIBquestion)
		{
			int x = 0;

			// Get and set the selected modalities
			JList< CIBModalite *> pIModalite;
			m_pIBquestion->GetModalites(pIModalite);

			for(x = 0, pIModalite.MoveFirst(); pIModalite.IsValid() ; pIModalite.MoveNext(),x++)
			{
				CIBModalite * pModalite = pIModalite.GetItem();
				// Get the modalities used
				JList<JInt32> Values;
				if(x==0)
				{
					// Set the operator 
					pModalite->SetOperator(GetDimension1Operator());
					// Set the selected ID in the list box
					m_pDlgItem->m_List_Dimension1.GetSelectedID(Values);
					pModalite->SetValues(Values);
				}
				else if(x==1)
				{
					// Set the operator 
					pModalite->SetOperator(GetDimension2Operator());
					// Set the selected ID in the list box
					m_pDlgItem->m_List_Dimension2.GetSelectedID(Values);
					pModalite->SetValues(Values);
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
						// Set the selected ID in the list box
						Dlg.GetSelectedID(Values);
						pModalite->SetValues(Values);
						// Move to next modeless modalites dialog
						m_DimDlg.MoveNext();
					}
				}
				if (Values.GetCount()>0) AuMoins1Sel = true; 
			}
		}

		// For items
		if(m_pIBitem)
		{
			// Is Pseudo ou Critere modifié ou Critere Quantile
			if(m_pSelectedQuestion && !m_pSelectedQuestion->IsQuestion()&& m_pSelectedQuestion->HasDimensions())
			{
				JList<JInt32> Values;

				// Set the operator 
				m_pIBitem->SetOperator(GetDimension1Operator());
				// Set the selected ID in the list box
				m_pDlgItem->m_List_Dimension1.GetSelectedID(Values);
				m_pIBitem->SetValues(Values);
				if (Values.GetCount()>0) AuMoins1Sel = true; 
			}
		}
	}
	if (AuMoins1Sel)
		ItemsValid = true;
	else
		ItemsValid = false;
}


// Update the operator modes
void CItemMode::SetDimension1Operator(IBloc::OPERATOR Operator)
{
	switch(Operator)
	{
		case IBloc::And :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_ET);
		break;

		case IBloc::Or :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_OU);
		break;

		case IBloc::Not :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_NI);
		break;

		case IBloc::Xor :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR,IDC_RI_RAD_DIM1_XOR);
		break;
	}

	// Mise à jour du bouton opération sur dimension 1

}

IBloc::OPERATOR CItemMode::GetDimension1Operator()
{
	switch(m_pDlgItem->GetCheckedRadioButton(IDC_RI_RAD_DIM1_ET,IDC_RI_RAD_DIM1_XOR))
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

void CItemMode::SetDimension2Operator(IBloc::OPERATOR Operator)
{
	switch(Operator)
	{
		case IBloc::And :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR,IDC_RI_RAD_DIM2_ET);
		break;

		case IBloc::Or :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR,IDC_RI_RAD_DIM2_OU);
		break;

		case IBloc::Not :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR,IDC_RI_RAD_DIM2_NI);
		break;

		case IBloc::Xor :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR,IDC_RI_RAD_DIM2_XOR);
		break;
	}

	// Mise à jour du bouton opération sur dimension 2
}

IBloc::OPERATOR CItemMode::GetDimension2Operator()
{
	switch(m_pDlgItem->GetCheckedRadioButton(IDC_RI_RAD_DIM2_ET,IDC_RI_RAD_DIM2_XOR))
	{
		case IDC_RI_RAD_DIM2_ET:
			return IBloc::And;
		break;

		case IDC_RI_RAD_DIM2_OU:
			return IBloc::Or;
		break;

		case IDC_RI_RAD_DIM2_NI:
			return IBloc::Not;
		break;

		case IDC_RI_RAD_DIM2_XOR:
			return IBloc::Xor;
		break;
	}
	return IBloc::Nop;
}

void CItemMode::SetQuestionOperator(IBloc::OPERATOR Operator)
{
	switch(Operator)
	{
		case IBloc::And :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU,IDC_RI_RAD_Q_ET);
			// m_pDlgItem->ActiveOpeSAS(IBloc::And);
		break;

		case IBloc::Or :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU,IDC_RI_RAD_Q_OU);
			// m_pDlgItem->ActiveOpeSAS(IBloc::Or);
		break;
	}
}

IBloc::OPERATOR CItemMode::GetQuestionOperator()
{
	switch(m_pDlgItem->GetCheckedRadioButton(IDC_RI_RAD_Q_ET,IDC_RI_RAD_Q_OU))
	{
		case IDC_RI_RAD_Q_ET:
			return IBloc::And;
		break;

		case IDC_RI_RAD_Q_OU:
			return IBloc::Or;
		break;
	}
	return IBloc::Nop;
}

void CItemMode::SetSegmentOperator(IBloc::OPERATOR Operator)
{
	switch(Operator)
	{
		case IBloc::And :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_S_ET,IDC_RI_RAD_S_OU,IDC_RI_RAD_S_ET);
		break;

		case IBloc::Or :
			m_pDlgItem->CheckRadioButton(IDC_RI_RAD_S_ET,IDC_RI_RAD_S_OU,IDC_RI_RAD_S_OU);
		break;
	}
}

IBloc::OPERATOR CItemMode::GetSegmentOperator()
{
	switch(m_pDlgItem->GetCheckedRadioButton(IDC_RI_RAD_S_ET,IDC_RI_RAD_S_OU))
	{
		case IDC_RI_RAD_S_ET:
			return IBloc::And;
		break;

		case IDC_RI_RAD_S_OU:
			return IBloc::Or;
		break;
	}
	return IBloc::Nop;
}

// Updates the item under construction with the selected values
bool CItemMode::UpdateItemValues(bool bValues)
{
	bool bValid = true;
	// Flag to remove the values dialog
	bool bValuesDlg = false;

	// Do we have a question
	if(m_pIBquestion && m_pSelectedQuestion)
	{
		// Get the question and check that its a quanti
		// If we are on a question get the pointer to the real question
		if(m_pSelectedQuestion->IsQuestion() && m_pSelectedQuestion->AsQuestion()->HasValues())
		{
			
			// Check if the Values dialog exists
			if(!m_pValuesDlg)
			{
				// Create the values dialog
				m_pValuesDlg = new CValuesDlg;
				m_pValuesDlg->m_CATPColorsApp = m_pDlgItem->m_CATPColorsApp;
				m_pValuesDlg->Create(CValuesDlg::IDD, m_pDlgItem);

				// Attention si + de 1 item, on reste avec une fenêtre numérique "volante"
				JInt32 NbItems = m_pSelectedQuestion->AsQuestion()->GetNbItems();

				// récup le libellé de la question
				CString TxtQuestion;
				const IModalite* pModalite = (const IModalite*)m_pDlgItem->m_List_Dimension1.GetItemData(0);
				TxtQuestion = pModalite->GetLabel().AsJCharPtr(); 

				// Attention positionnement spécifique dans le cadre dimension 1 pour les questions quantitatives sans libellé à 1 seule modalité
				if (NbItems == 1 && TxtQuestion == "Valeurs numériques")
				{
					// La fenêtre valeurs numériques sera locké sur le controle dimension 1
					m_pValuesDlg->SetFlightWindow(false);

					// Positionnement de la boite sous le questionnaire
					CRect Rect1,Rect2;
					m_pDlgItem->m_Box_Dimension1.GetWindowRect(&Rect1);
					m_pValuesDlg->GetWindowRect(&Rect2);
					Rect2 = Rect1;
					m_pValuesDlg->MoveWindow(Rect2,TRUE);
				}
				else
				{
					// Alignement boite valeurs à gauche de la boite items dimension 1
					CRect Rect1,Rect2;
					m_pDlgItem->m_Box_Question.GetWindowRect(&Rect1);
					m_pValuesDlg->GetWindowRect(&Rect2);
					Rect2.left   = Rect1.left - Rect2.Width() - 5;
					Rect2.right  = Rect1.left- 5;
					Rect2.bottom = Rect1.bottom + Rect2.Height(); 
					Rect2.top    = Rect1.bottom; 
					m_pValuesDlg->MoveWindow(Rect2,TRUE);
				}
			}

			if(!bValues)
				bValuesDlg = SetValuesDlg();
			else
				bValuesDlg = true;

			if(!m_pIBquestion->GetNumValue() && !m_pIBquestion->GetClass())
				bValid = false;
		}
	}
	// Remove the values dialog if necessary
	if(!bValuesDlg)
	{ 
		if(m_pValuesDlg)
		{
			if (m_pValuesDlg->m_hWnd != 0)
			{
				m_pValuesDlg->GetWindowRect(&m_ValuesDlgPos);
				m_pValuesDlg->DestroyWindow();
			}
			delete m_pValuesDlg;
			m_pValuesDlg = 0;
		}
	}
	if(m_pValuesDlg)
	{
		m_pValuesDlg->ShowWindow(SW_SHOW);
	}	

	return bValid;
}

// Sets up the values dialog
bool CItemMode::SetValuesDlg()
{
	// Test if the current target is valid
	JVector<JFlt32> Values;
	JInt32 Divider = 0;

	// Get the quanti values
	if(!GetQuestionNumericValues(m_pIBquestion,Values,Divider))
		return false;

	// on définit le mode ITEM
	m_pValuesDlg->SetItemMode();

	// Set the quanti values
	if(m_pIBquestion->GetNumValue())
		m_pValuesDlg->SetQuantiValues(Values,m_pIBquestion->GetNumValue(),Divider);
	else if(m_pIBquestion->GetClass())
		m_pValuesDlg->SetQuantiValues(Values,m_pIBquestion->GetClass(),Divider);
	else
		m_pValuesDlg->SetQuantiValues(Values,Divider);

	return true;
}

// Gets the possible numerical values for the current question configuration
bool CItemMode::GetQuestionNumericValues(CIBQuestion * pIQuestion, JVector<JFlt32> & Values, JInt32 & Divider)
{
	CWaitCursor Wait;

	bool Ret = true;

	// Create builder object
	CIBuilder tBuilder;
	tBuilder.SetOperator(IBloc::And);

	// Create segment
	CIBSegment *pSegment = new CIBSegment;
	pSegment->SetOperator(IBloc::And);

	// Add the question
	pSegment->Add(pIQuestion);
	tBuilder.Add(pSegment);

	// Get a new item construit
	CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();

	// Set the builder object
	pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),tBuilder);

	// Test if the current target is valid
	if(pItem->IsValid(m_pDlgItem->GetTerrain()))
	{
		// CWaitCursor Wait;
		// Calculate the quanti values
		pItem->CalcQuantiValues(m_pDlgItem->GetTerrain(), Values, Divider);
	}
	else
		Ret = false;

	pSegment->RemoveQuestion(pIQuestion);

	delete pItem;

	return Ret;
}

// Calculates the current items effective
bool CItemMode::CalculateEffectif(CString & SEffectif,CString & SPercent,CString & SNbCas)
{
	// Calculate the current target configuration
	// Get a new item construit
	PTR_ITEMCONSTRUIT pItem(m_pDlgItem->GetItems()->GetNewItemConstruit());
	// Set the builder object
	pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),m_IBuilder);

	// Cas ou segment vide et mode calcul multi-segment
	if (m_pDlgItem->m_bSegment == 0 && !pItem->IsValid(m_pDlgItem->GetTerrain()))
	{		
		// On ne modifie pas les résultats, via les autres segemnts existants
		return false;
	}

	// Test is the current target is valid
	if(pItem->IsValid(m_pDlgItem->GetTerrain()))
	{
		// calculate the target
		CCalcItemParams Params;
		Params.SetTerrain(m_pDlgItem->GetTerrain());
		Params.SetEchantillonID(1);
		Params.SetUniversID(m_pDlgItem->GetTerrain()->GetDefaultUnivers());
		Params.SetTypePoidsID(m_pDlgItem->GetTypePoids());
		Params.SetPopulationSegment(m_pDlgItem->GetPopulationSegment());
		Params.SetSegmentsIDs(m_pDlgItem->GetSegments());

		// Lecture Mode Cumul Valeurs Quantitatives  (pour Balmétrie notamment)  // Par défaut on reste en mode calcul sans cumul  (modif Juillet 2013)
		// JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 0) != 0?true:false;
		JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 1) != 0?true:false;
		Params.SetCumulQuanti(CumulQuanti);


		if(!m_pDlgItem->m_bSegment == 0 && m_pIBsegment->IsValid())
		{
			// Get and set the segment operator
			CIBuilder IBuilder;
			IBuilder.SetOperator(GetSegmentOperator());
			IBuilder.Add(m_pIBsegment);
			CItemConstruit *psItem = m_pDlgItem->GetItems()->GetNewItemConstruit();

			// Set the builder object
			psItem->SetItemBuilder(m_pDlgItem->GetTerrain(),IBuilder);
			CWaitCursor Wait;
			if(psItem->IsValid(m_pDlgItem->GetTerrain()) && psItem->CalculateEffectif(&Params))
			{
				if(m_pDlgItem->GetShowResults()&SHOW_EFFECTIVE)
				SEffectif.Format("%0.0f",Params.GetEffectif());
				if(m_pDlgItem->GetShowResults()&SHOW_NBCAS)
				SNbCas.Format("%d",Params.GetNbCas());
				if(Params.GetEffectifTot() && m_pDlgItem->GetShowResults()&SHOW_PERCENT)
					SPercent.Format("%0.2f%%",(Params.GetEffectif()/Params.GetEffectifTot())*100.0);
			}
			IBuilder.RemoveSegment(m_pIBsegment);
			delete psItem;
		}
		else
		{
			if(pItem->CalculateEffectif(&Params))
			{
				if(m_pDlgItem->GetShowResults()&SHOW_EFFECTIVE)
				SEffectif.Format("%0.0f",Params.GetEffectif());
				if(m_pDlgItem->GetShowResults()&SHOW_NBCAS)
				SNbCas.Format("%d",Params.GetNbCas());
				if(Params.GetEffectifTot() && m_pDlgItem->GetShowResults()&SHOW_PERCENT)
					SPercent.Format("%0.2f%%",(Params.GetEffectif()/Params.GetEffectifTot())*100.0);
			}
		}
		CString Fmt = _T("### ### ### ### ###");
		Format(Fmt,SEffectif);
		Format(Fmt,SNbCas);
		//Pas besoin de Format, car déjà fait
		//Fmt = _T("###,##");
		//Format(Fmt,SPercent);
	}

	// Les effectifs ont changés
	return true;

//	delete pItem;
}

void CItemMode::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	switch(pWnd->GetDlgCtrlID())
	{
		case IDC_RI_LIST_IITEMS:
		{
			// Récupère l'index élmt sélectionné
			BOOL Outside = TRUE;
			m_pDlgItem->m_List_Items.ScreenToClient(&point); 
			JUnt32 IdxItem = m_pDlgItem->m_List_Items.ItemFromPoint(point,Outside); 
            
			// Limite liste item
			CRect RectItem;
			m_pDlgItem->m_List_Items.GetItemRect(IdxItem,RectItem); 
			BOOL Inside = RectItem.PtInRect(point); 

			// Retour coordonnées écran
			m_pDlgItem->m_List_Items.ClientToScreen(&point); 
			
			if (IdxItem != LB_ERR && Inside != FALSE)
			{
				const IItem* pItem = (const IItem*)m_pDlgItem->m_List_Items.GetItemData(IdxItem);

				if (pItem != 0)
				{
					if (m_pDlgItem->m_List_Items.GetSelCount() > 0)
					{
						CMenu Menu;
						CString Label;

						// Option exporter
						Menu.CreatePopupMenu();

						// Attention, pour option Rep Item Simple (Internet) pas d'export
						if (m_pDlgItem->GetShowMode() != SHOWMODE_SIMPLE)
						{
							// Option Export Item
							LOADSTRING(Label, IDS_RI_EXPORTER); 
							Menu.AppendMenu(MF_STRING,IDM_RI_EXPORT_ITEM,Label);
						}

						// Menu supprimer Ok si seulement 1 item sélectionné
						if (m_pDlgItem->m_List_Items.GetSelCount() == 1)
						{
							// Option renommer
							LOADSTRING(Label, IDS_RI_RENOMMER);
							Menu.AppendMenu(MF_STRING,IDM_RI_RENAME_ITEM,Label);

							// Option supprimer
							Menu.AppendMenu(MF_SEPARATOR);
							LOADSTRING(Label, IDS_RI_SUPPRIMER); 
							Menu.AppendMenu(MF_STRING,IDM_RI_DELETE_ITEM,Label);

							// Attention visualisation valide seulement pour items (dans partie item et pallette)
							if (m_pDlgItem->ValCurMode() == 1 || m_pDlgItem->ValCurMode() == 4) 
							{
								if (m_pDlgItem->GetCheckedRadioButton(IDC_RI_RAD_QUESTIONS,IDC_RI_RAD_LIST_2) == IDC_RI_RAD_LIST_1)
								{
									// Option Visualiser Item seulement sur liste items
									Menu.AppendMenu(MF_SEPARATOR);
									LOADSTRING(Label, IDS_RI_VISUALISER); 
									Menu.AppendMenu(MF_STRING,IDM_RI_VISU_ITEM,Label);
								}
							}
						}
						
						switch(Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY,point.x,point.y,m_pDlgItem))
						{
							// Export des items sélectionnés
							case IDM_RI_EXPORT_ITEM:
							{
								// Liste des items sélectionnés
								JList<JInt32> LstIDItemSel;
								m_pDlgItem->m_List_Items.GetSelectedID(LstIDItemSel);
								
								// Export des items sélectionnés
								ExportItems(LstIDItemSel);
							}
							break;

							// Renommer l'item
							case IDM_RI_RENAME_ITEM:
							{
								RenameItem(pItem->GetID());
							}
							break;

							// Suppression de l'item
							case IDM_RI_DELETE_ITEM:
							{
								bool DeleteForce = false;
								CItemConstruit * pItemConstruit = (CItemConstruit *)(pItem->AsItemConstruit());

								if (pItemConstruit)
								{
									if (pItemConstruit->GetIDSource() != 49)  // != Balmetrie
									{
										DeleteForce = true;
									}
								}
								
								DeleteItem(pItem->GetID(), DeleteForce);
							}
							break;

							// Visualiser l'item
							case IDM_RI_VISU_ITEM:
							{
								ViewItem(pItem->GetID());
							}
							break;
						}
					}
				}
			}

			pWnd->InvalidateRect(NULL, TRUE);
			pWnd->UpdateWindow();
		}
		break;

		case IDC_RI_LIST_DIMENSION1:
		{
			BOOL Outside = TRUE;
			m_pDlgItem->m_List_Dimension1.ScreenToClient(&point); 
			JUnt32 IdxItem = m_pDlgItem->m_List_Dimension1.ItemFromPoint(point,Outside); 

			// Limite liste item
			CRect RectItem;
			m_pDlgItem->m_List_Dimension1.GetItemRect(IdxItem,RectItem); 
			BOOL Inside = RectItem.PtInRect(point); 

			// Retour coordonnées écran
			m_pDlgItem->m_List_Dimension1.ClientToScreen(&point); 
			
			// On n'est sur une dimension et en mode répertoire items
			if (IdxItem != LB_ERR && Inside != FALSE && m_pDlgItem->GetTypeRepItem()== true)
			{
				const IModalite* pModalite = (const IModalite*)m_pDlgItem->m_List_Dimension1.GetItemData(IdxItem);
				if (pModalite != 0)
				{
					// Menu contextuel Renommer ssi au moins 1 sélection
					CMenu Menu;
					CString Label;
					Menu.CreatePopupMenu();

					// Affichage du menu selon mode, nombre de sélection et type de questions (pour boite dimension 1)
					OptionMenuContextuelDim1(&Menu);

					switch(Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY,point.x,point.y,m_pDlgItem))
					{
						// Renommer un elmt 
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
			pWnd->InvalidateRect(NULL, TRUE);
			pWnd->UpdateWindow();
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// Affichage du menu selon mode, nombre de sélection et type de questions
void CItemMode::OptionMenuContextuelDim1(CMenu *pMenu)
{
	// Libelle option menu
	CString Label;

	if (m_pSelectedQuestion)
	{
		// Mode Items
		if (m_pDlgItem->ValCurMode() == 1) 
		{
							
			if (m_pSelectedQuestion->IsQuestion())
			{
				// Ss mode Questions >> Aucun ss-menu
			}
			else if (m_pSelectedQuestion->AsPseudo())
			{
				/* ????
				// 1 seule sélection
				if (m_pDlgItem->m_List_Dimension1.GetSelCount() == 1)
				{
					// Ss mode Item >> "visualiser + renommer"
					//
					// Option Visualisation
					LOADSTRING(Label, IDS_RI_VISUALISER); 
					pMenu->AppendMenu(MF_STRING,IDM_RI_VISUALISER_DIMENSION1,Label);
					// Option Renommer
					LOADSTRING(Label, IDS_RI_RENOMMER); 
					pMenu->AppendMenu(MF_STRING,IDM_RI_RENAME_DIMENSION1,Label);
				}
				else
				{
					// Plusieurs sélection >> Aucun menu
				}
				*/
			}
		}

		// Mode Palettes
		else if (m_pDlgItem->ValCurMode() == 4)
		{
			// si plusieurs sélection >> aucun menu
			if (m_pDlgItem->m_List_Dimension1.GetSelCount() > 1)
			{
				// Aucune option
			}
			else if (m_pSelectedQuestion->AsPseudo())
			{
				/* Virer car mode résultats
				// En ss mode question ou item >> seulement visualiser
				LOADSTRING(Label, IDS_RI_VISUALISER); 
				pMenu->AppendMenu(MF_STRING,IDM_RI_VISUALISER_DIMENSION1,Label);
				*/
			}
			else
			{
				/* Virer car mode résultat
				// En ss mode question ou item >> seulement visualiser
				LOADSTRING(Label, IDS_RI_VISUALISER); 
				pMenu->AppendMenu(MF_STRING,IDM_RI_VISUALISER_DIMENSION1,Label);
				*/
			}
		}

		// Mode Classes
		else if (m_pDlgItem->ValCurMode() == 2)
		{
			// si 1 seule sélection
			if (m_pDlgItem->m_List_Dimension1.GetSelCount() == 1)
			{
				// Si ss-mode question >> visualiser + renommer
				if (m_pSelectedQuestion->IsQuestion())
				{
					// pas d'option
				}
				else if (m_pSelectedQuestion->IsCritereModifie())
				{
					// Option Renommer
					LOADSTRING(Label, IDS_RI_RENOMMER); 
					pMenu->AppendMenu(MF_STRING,IDM_RI_RENAME_DIMENSION1,Label);
				}
			}
		}

		// Mode Quantiles
		else if (m_pDlgItem->ValCurMode() == 3)
		{
			// si 1 seule sélection
			if (m_pDlgItem->m_List_Dimension1.GetSelCount() == 1)
			{
				// Si ss-mode question >> Aucune option
				if (m_pSelectedQuestion->IsQuestion())
				{
				}
				// Si Ss-mode quantile >> option Visualiser + renommer
				else if (m_pSelectedQuestion->IsCritereQuantile())
				{
					// Option Renommer
					LOADSTRING(Label, IDS_RI_RENOMMER); 
					pMenu->AppendMenu(MF_STRING,IDM_RI_RENAME_DIMENSION1,Label);
				}
			}
			else
			{
				// Aucune option si plusieurs sélection
			}
		}

		// Mode Questions Induites
		else if (m_pDlgItem->ValCurMode() == 6)
		{
			// Si option compter
			//if (OptionCompter)
			//{
				// si 1 seule sélection
				if (m_pDlgItem->m_List_Dimension1.GetSelCount() == 1)
				{
					// Option Renommer
					LOADSTRING(Label, IDS_RI_RENOMMER); 
					pMenu->AppendMenu(MF_STRING,IDM_RI_RENAME_DIMENSION1,Label);
				}
				else
				// Plusieurs sélections
				{
					// Option Remplacement
					LOADSTRING(Label, IDS_RI_REMPLACER); 
					pMenu->AppendMenu(MF_STRING,IDM_RI_REMPLACER_DIMENSION1,Label);
				}
			//}
		}
	}
}

// Rename the selected item construit
void CItemMode::RenameItem(JInt32 ID)
{
	IItem * pItem = m_pDlgItem->GetItems()->GetIItemByID(ID);
	if(pItem && pItem->CanModify())
	{
		CDlgLibelle Dlg(m_pDlgItem);
		Dlg.m_CATPColorsApp = m_pDlgItem->m_CATPColorsApp;
		Dlg.m_Libelle = pItem->GetLabel().AsJCharPtr();
		if(Dlg.DoModal() == IDOK)
		{
			JLabel Label = Dlg.m_Libelle;
			if(Label != pItem->GetLabel())
			{
				if(pItem->SetLabel(Label))
				{
					m_pDlgItem->GetItems()->Save();
					UpdateQuestionList();
					UpdateListItems();
					OnSelchange_ListQuestions(); 
				}
				else
				{
					// Nouveau nom pour item non correct, déjà existant
					CString Txt;
					LOADSTRING(Txt, IDS_RI_LABELITEM_EXIST);
					AfxMessageBox(Txt);
				}
			}
		}
	}
}

// Delete the selected item construits
void CItemMode::DeleteItem(JInt32 ID, bool ForceDelete)
{
	IItem * pItem = m_pDlgItem->GetItems()->GetIItemByID(ID);
	if (pItem != 0)
	{
		if (pItem->IsItemConstruit())
		{
			CString Message;
			JMap<JInt32, JLabel> MapUpwards;

			if (!ForceDelete)
			{
				if(m_pDlgItem->GetItems()->CanUnRegisterItemConstruit(ID, &MapUpwards))
				{
					Message.Format("Voulez-vous supprimer la cible pondérée \"%s\" ?", pItem->GetLabel().AsJCharPtr());
					if(IDYES != m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_YESNO|MB_ICONQUESTION))
						return;

					if(!m_pDlgItem->GetItems()->UnRegisterItemConstruit(ID))
					{
						Message.Format("La suppression de la cible pondérée \"%s\" a échoué.",pItem->GetLabel().AsJCharPtr());
						m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_OK|MB_ICONEXCLAMATION);
						return;
					}
				}
				else
				{
					// Affichage Boite de compoistion de l'item
					Message.Format("L'item \"%s\" ne peut pas être supprimé.",pItem->GetLabel().AsJCharPtr());
					if(MapUpwards.GetCount() > 0)
					{
						CDlgUpwards Dlg(Message, pItem->GetLabel().AsJCharPtr(), MapUpwards, m_pDlgItem);
						Dlg.DoModal();
						return;
					}
					else
					{
						// Modif 21 Aout 2014 pour version 1.3.7
						// (pour forcer les items "rebelles" (eux qui bizarrement ont encore des liaisons / on le fait seulement pour Balmétrie et terrain <= 514)... pour le moemnt
						CItemConstruit * pItemConstruit = (CItemConstruit *)(pItem->AsItemConstruit());
						if (pItemConstruit)
						{
							if (pItemConstruit->GetIDSource() == 49 && 	pItemConstruit->GetIDTerrain() <= 514)
							{
								Message.Format("Voulez-vous supprimer l'item  : %s\" ?", pItem->GetLabel().AsJCharPtr());
								if(IDYES != m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_YESNO|MB_ICONQUESTION))
									return;

								// Du coup il faut forcer le nombre de Réfrence à 1
								pItemConstruit->RefCountAUn();
								if(!m_pDlgItem->GetItems()->UnRegisterItemConstruit(ID))
								{
									Message.Format("La suppression de la cible pondérée \"%s\" a échoué.",pItem->GetLabel().AsJCharPtr());
									m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_OK|MB_ICONEXCLAMATION);
									return;
								}
							}
							else
							{
								m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_OK|MB_ICONEXCLAMATION);
								return;
							}

						}
						else
							return;
					}

					// return;
				}
			}
		}
		else if (pItem->IsPalette())
		{
			CString Message;
			JMap<JInt32, JLabel> MapUpwards;
			if(m_pDlgItem->GetItems()->CanUnRegisterPalette(ID, &MapUpwards))
			{
				Message.Format("Voulez-vous supprimer la palette \"%s\" ?", pItem->GetLabel().AsJCharPtr());
				if(IDYES != m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_YESNO|MB_ICONQUESTION))
					return;
				if(!m_pDlgItem->GetItems()->UnRegisterPalette(ID))
				{
					Message.Format("La suppression de la palette \"%s\" a échoué.",pItem->GetLabel().AsJCharPtr());
					m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_OK|MB_ICONEXCLAMATION);
					return;
				}
			}
			else
			{
				// Affichage Boite de composition de la palette
				Message.Format("La palette \"%s\" ne peut pas être supprimée.",pItem->GetLabel().AsJCharPtr());
				if(MapUpwards.GetCount() > 0)
				{
					CDlgUpwards Dlg(Message, pItem->GetLabel().AsJCharPtr(), MapUpwards, m_pDlgItem);
					Dlg.DoModal();
				}
				else
					m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_OK|MB_ICONEXCLAMATION);
				return;
			}
		}
		else if (pItem->IsCritereModifie())
		{
			CString Message;
			JMap<JInt32, JLabel> MapUpwards;
			if(m_pDlgItem->GetItems()->CanUnRegisterCritereModifie(ID, &MapUpwards))
			{
				Message.Format("Voulez-vous supprimer le critère modifié \"%s\" ?", pItem->GetLabel().AsJCharPtr());
				if(IDYES != m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_YESNO|MB_ICONQUESTION))
					return;
				if(!m_pDlgItem->GetItems()->UnRegisterCritereModifie(ID))
				{
					Message.Format("La suppression du critère modifié \"%s\" a échoué.",pItem->GetLabel().AsJCharPtr());
					m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_OK|MB_ICONEXCLAMATION);
					return;
				}
			}
			else
			{
				// Affcihage boite de composition du critère
				Message.Format("Le critère modifié \"%s\" ne peut pas être supprimé.",pItem->GetLabel().AsJCharPtr());
				if(MapUpwards.GetCount() > 0)
				{
					CDlgUpwards Dlg(Message, pItem->GetLabel().AsJCharPtr(), MapUpwards, m_pDlgItem);
					Dlg.DoModal();
				}
				else
					m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_OK|MB_ICONEXCLAMATION);
				return;
			}
		}
		else if (pItem->IsCritereQuantile())
		{
			CString Message;
			JMap<JInt32, JLabel> MapUpwards;
			if(m_pDlgItem->GetItems()->CanUnRegisterCritereQuantile(ID, &MapUpwards))
			{
				Message.Format("Voulez-vous supprimer le critère quantile \"%s\" ?", pItem->GetLabel().AsJCharPtr());
				if(IDYES != m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_YESNO|MB_ICONQUESTION))
					return;
				if(!m_pDlgItem->GetItems()->UnRegisterCritereQuantile(ID))
				{
					Message.Format("La suppression du critère quantile \"%s\" a échoué.",pItem->GetLabel().AsJCharPtr());
					m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_OK|MB_ICONEXCLAMATION);
					return;
				}
			}
			else
			{
				// Affichage composition du critère quantile
				Message.Format("Le critère quantile \"%s\" ne peut pas être supprimé.",pItem->GetLabel().AsJCharPtr());
				if(MapUpwards.GetCount() > 0)
				{
					CDlgUpwards Dlg(Message, pItem->GetLabel().AsJCharPtr(), MapUpwards, m_pDlgItem);
					Dlg.DoModal();
				}
				else
					m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_OK|MB_ICONEXCLAMATION);
				return;
			}
		}
		else if (pItem->IsQuestionInduite())
		{
			CString Message;
			JMap<JInt32, JLabel> MapUpwards;
			if(m_pDlgItem->GetItems()->CanUnRegisterQuestionInduite(ID, &MapUpwards))
			{
				Message.Format("Voulez-vous supprimer la question induite \"%s\" ?", pItem->GetLabel().AsJCharPtr());
				if(IDYES != m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_YESNO|MB_ICONQUESTION))
					return;
				if(!m_pDlgItem->GetItems()->UnRegisterQuestionInduite(ID))
				{
					Message.Format("La suppression de la question induite \"%s\" a échoué.",pItem->GetLabel().AsJCharPtr());
					m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_OK|MB_ICONEXCLAMATION);
					return;
				}
			}
			else
			{
				// Affichage compistion question induite
				Message.Format("La question induite \"%s\" ne peut pas être supprimée.",pItem->GetLabel().AsJCharPtr());
				if(MapUpwards.GetCount() > 0)
				{
					CDlgUpwards Dlg(Message, pItem->GetLabel().AsJCharPtr(), MapUpwards, m_pDlgItem);
					Dlg.DoModal();
				}
				else
					m_pDlgItem->MessageBox(Message,m_pDlgItem->GetTitle(),MB_OK|MB_ICONEXCLAMATION);
				return;
			}
		}

		// Remove the Item from the pseudo question if present
		if(m_pPseudoItem && m_pPseudoItem->HasItem(ID))
		{
			// MAI 2014
			/*
			if (pItem)
			{
				CItemConstruit *pItemConstruit =  pItem->AsItemConstruit();
				pItemConstruit->RefCountAZero();
				bool Ok = true;
			}
			*/
			m_pPseudoItem->RemoveItem(ID);
		}

		// Remove User question if present from the map 
		m_Selected_UserItems.MoveTo(ID);
		if(m_Selected_UserItems.IsValid())
			m_Selected_UserItems.Remove();

		m_pDlgItem->GetItems()->Save();
		UpdateListItems();
		OnSelchange_ListItems();
	}
}

///////////////////////////////////////////////////////////////////////////////////
// Visualiser the selected item construits
void CItemMode::ViewItem(JInt32 ID)
{
	// Récupère la composition de l'item
	CTerrain *pTerrain = m_pDlgItem->GetTerrain();
	CString CompoItem = m_pDlgItem->GetItems()->GetCompoItem(ID, pTerrain); 
	CString InfoVisu;

	// Nom de l'item affichage composition
	IItem * pItem		= m_pDlgItem->GetItems()->GetIItemByID(ID);
	CString LabelItem	= pItem->GetLabel().AsJCharPtr();  

	// Affiche message
	InfoVisu.Format("%s\n\n%s","Composition de cet item :",CompoItem);
	int Result = m_pDlgItem->MessageBox(InfoVisu, LabelItem, MB_ICONINFORMATION);
}

////////////////////////////////////////////////////////////////////////////////////
// Delete the selected item construits (seulement les items) sans afficher message
bool CItemMode::DeleteItemConstruit(JInt32 ID)
{
	IItem * pItem = m_pDlgItem->GetItems()->GetIItemByID(ID);
	if (pItem != 0)
	{
		if (pItem->IsItemConstruit())
		{
			CString Message;
			JMap<JInt32, JLabel> MapUpwards;
			if(m_pDlgItem->GetItems()->CanUnRegisterItemConstruit(ID, &MapUpwards))
			{
				if(!m_pDlgItem->GetItems()->UnRegisterItemConstruit(ID))
				{
					// échec suppression item
					return false;
				}
			}
			else
			{
				// Items composés >> impossible de le supprimer
				if(MapUpwards.GetCount() > 0)
				{
					return false;
				}
			}

			// Remove the Item from the pseudo question if present
			if(m_pPseudoItem && m_pPseudoItem->HasItem(ID))
				m_pPseudoItem->RemoveItem(ID); 

			// Remove User question if present from the map 
			m_Selected_UserItems.MoveTo(ID);
			if(m_Selected_UserItems.IsValid())
				m_Selected_UserItems.Remove();

			m_pDlgItem->GetItems()->Save();
			UpdateListItems();
			OnSelchange_ListItems();

			// Suppression Ok
			return true;

		}
		return false;
	}
	return false;

}	

// Export the selected item construits
void CItemMode::ExportItem(JInt32 ID)
{
	// Remplit liste avec 1 seul ident item
	JList<JInt32> Values;
	Values.AddTail() = ID;

	// Export cet item
	m_pDlgItem->GetItems()->Export(Values, m_pDlgItem);
}

// Export selected items construits
void CItemMode::ExportItems(JList <JInt32> & ListID)
{
	// Export cette liste d'items
	m_pDlgItem->GetItems()->Export(ListID, m_pDlgItem);
}

// Preview the selected item construits
void CItemMode::PreviewItem()
{
	JList<JInt32> Values;
	m_pDlgItem->m_List_Items.GetSelectedID(Values);
	JList<JInt32> ItemValues;
	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		IItem *pItem = m_pDlgItem->GetItems()->GetIItemByID(Values.GetItem());
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
	}
	PreviewItems(ItemValues);
}

// Preview the selected item construits
void CItemMode::PreviewDimension1()
{
	JList<JInt32> Values;
	m_pDlgItem->m_List_Dimension1.GetSelectedID(Values);
	PreviewItems(Values);
}

// Rename modalites
void CItemMode::RenameDimension1(JInt32 ID)
{
	// Check if question is modifiable
	if(m_pSelectedQuestion && m_pSelectedQuestion->CanModify())
	{
		m_DimList.MoveFirst();
		if(m_DimList.IsValid())
		{
			JLabel OldName;
			JVector<const IModalite *> &vMod = m_DimList.GetItem();
			for(vMod.MoveFirst(); vMod.IsValid(); vMod.MoveNext())
			{
				if(vMod.GetItem()->GetID() == ID)
				{
					OldName = vMod.GetItem()->GetLabel();
					break;
				}
			}
			CDlgLibelle Dlg(m_pDlgItem);
			Dlg.m_CATPColorsApp = m_pDlgItem->m_CATPColorsApp;
			Dlg.m_Libelle = OldName.AsJCharPtr();
			if(IDOK == Dlg.DoModal())
			{
				JLabel Name = Dlg.m_Libelle;
				if (Name != OldName)
				{
					if(const_cast< IQuestion * >(m_pSelectedQuestion)->RenameItem(Name,ID))
					{
						m_pDlgItem->GetItems()->Save();
						UpdateDimensionsList();
					}
					else
					{
						m_pDlgItem->MessageBox("Le libellé est invalide !",m_pDlgItem->GetTitle(),MB_OK);
					}
				}
			}
		}
	}
}

// Descriptif sous forme de texte des items / en vue d'une impression via macro
void CItemMode::PreviewItems(JList<JInt32> &Values)
{
	// Init texte 
	CString TxtMacro = "";
	CString Text;

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		// Récupère l'item
		CItemConstruit * pItem = m_pDlgItem->GetItems()->GetItemConstruit(Values.GetItem());

		// Item non valide
		if(!pItem || !pItem->IsValid(m_pDlgItem->GetTerrain()))
			continue;

		// Nom composant
		Text.Format("%s\n",pItem->GetLabel().AsJCharPtr());
		TxtMacro += Text;
		
		// Source
		CString Tmp;
		m_pDlgItem->m_Label_Source.GetWindowText(Tmp);
		Text.Format("%s%s\n","Source : ", Tmp);
		TxtMacro += Text + "\n";

		// Poids
		const JSRCTables & srcTables = JSRCTables::GetInstance(); 

		// All segments
		const JVector<JInt32> & Segments= m_pDlgItem->GetSegments();
		for(Segments.MoveFirst(); Segments.IsValid(); Segments.MoveNext())
		{
			const JSegment* Seg = srcTables.m_TBLSegments.GetItem(Segments.GetItem());
			Text.Format("%s\n",Seg->m_Libelle.AsJCharPtr()); 
			TxtMacro += Text;
		}
		
		// Type de Poids
		JChar * pLibelle = srcTables.m_TBLPoids.GetLabelByID(m_pDlgItem->GetTypePoids());
		Text.Format("%s%%s\n","Type de poids : ",pLibelle);
		TxtMacro += Text;		

		// Calcul pour la cible
		CCalcItemParams Params;
		Params.SetTerrain(m_pDlgItem->GetTerrain());
		Params.SetEchantillonID(1);
		Params.SetUniversID(m_pDlgItem->GetTerrain()->GetDefaultUnivers());
		Params.SetTypePoidsID(m_pDlgItem->GetTypePoids());
		Params.SetPopulationSegment(m_pDlgItem->GetPopulationSegment());
		Params.SetSegmentsIDs(m_pDlgItem->GetSegments());
		pItem->CalculateEffectif(&Params); 
		
		// Effectif Cible
		if(m_pDlgItem->GetShowResults()&SHOW_EFFECTIVE)
			Text.Format("%s%d\n","Effectif : ",Params.GetEffectif());
		else
			Text = "\n";
		TxtMacro += Text;
		
		// Nb Cas Cible
		if(m_pDlgItem->GetShowResults()&SHOW_NBCAS)
			Text.Format("%s%d\n","Nombre de cas : ",Params.GetNbCas());
		else
			Text = "\n";
		TxtMacro += Text;
		
		// Pourcentage cibleXML percent tag
		if(m_pDlgItem->GetShowResults()&SHOW_PERCENT)
			Text.Format("%s%0.2f\n","Pourcentage : ",(Params.GetEffectif()/Params.GetEffectifTot())*100.0);
		else
			Text = "\n";
		TxtMacro += Text;
	}

	CStdioFile ExportFile;
	CFileException Error = 0;
	CString path = GETPROFILESTRINGD(SECTION_DEFAULT, KEY_INSTALL_PATH, 0);
	path += "\\TempTarget.txt";
	if(!ExportFile.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::typeText, &Error))
		return;
	ExportFile.WriteString(TxtMacro);
	ExportFile.Close();
}

void CItemMode::OnChange_SegmentCalc()
{
	UpdateIBuilder(true, false);	
}

/// Button Filtrage Modalites
void CItemMode::OnDim_Filtre()
{
	CWnd *pWnd = CWnd::FromHandle(m_pDlgItem->m_hWnd);

	// Mise en place fenêtre dialogue filtrage modalités
	CDlgFiltre DlgFiltre(pWnd);
	// CDlgFiltre DlgFiltre = new CDlgFiltre(pWnd);

	DlgFiltre.m_CATPColorsApp = m_pDlgItem->m_CATPColorsApp;

	// Récup coordonnées positionnementt boite filtre à droite dimension 1
	m_pDlgItem->m_List_Dimension1.GetWindowRect(&DlgFiltre.m_PosDlg);

	// Récupère tous les codes pour filtrage
	DlgFiltre.m_MapCodeFiltre.Swap(m_pDlgItem->m_MapFiltrage);

	// On récupère les libellés des codes filtes
	DlgFiltre.m_MapLibFiltre.Reset();
	for (DlgFiltre.m_MapCodeFiltre.MoveFirst(); DlgFiltre.m_MapCodeFiltre.IsValid(); DlgFiltre.m_MapCodeFiltre.MoveNext())
	{
		// Code Filtre Secteur
		JUnt32 IdSecteur = DlgFiltre.m_MapCodeFiltre.GetKey();

		// Elmt Secteur
		const CElement* pSecteur = m_pDlgItem->GetTerrain()->GetElement(IdSecteur);
		
		if (pSecteur)
		{	
			DlgFiltre.m_MapLibFiltre.MoveTo(IdSecteur);
			if (!DlgFiltre.m_MapLibFiltre.IsValid())
			{
				// Ajoute le libellé
				DlgFiltre.m_MapLibFiltre.Add(IdSecteur) = pSecteur->GetLabel().AsJCharPtr();
			}
		}
	}

	// Pour test
	JMap <JInt32, JInt32> MapEnfantParent;
	m_pDlgItem->GetTerrain()->GetLienMarqueSecteur(MapEnfantParent);

	int NbModaDim1 = m_pDlgItem->m_List_Dimension1.GetCount();

	// Boucle sur toutes les modalités en cours sur dimension 1
	for (JUnt32 Idx = 0; Idx < NbModaDim1; Idx++)
	{
		// Récupère la modalité correspondante
		const IModalite* pModalite = (const IModalite*)m_pDlgItem->m_List_Dimension1.GetItemData(Idx);

		if (pModalite->IsElement())
		{
			const CElement *pElmtModa = pModalite->AsElement();
			JInt32 CodeElmt = pElmtModa->GetID();
		}

		// Le code de la modalité
		JInt32 Code = pModalite->GetID();

		// On vérifiee que la clef marque existe dans map des liens marque / secteur
		MapEnfantParent.MoveTo(Code);
		if (MapEnfantParent.IsValid())
		{
			// Il faut récupérer le code secteur et le libellé associé à la marque
			JInt32 IdSecteur = MapEnfantParent.GetItem();

			// Elmt Secteur
			const CElement* pSecteur = m_pDlgItem->GetTerrain()->GetElement(IdSecteur);

			if (pSecteur)
			{
				JInt32 CodeSecteur = pSecteur->GetID();
				DlgFiltre.m_MapCodeFiltre.MoveTo(CodeSecteur);
				if (!DlgFiltre.m_MapCodeFiltre.IsValid())
				{
					// Ajoute le nouveau secteur
					DlgFiltre.m_MapCodeFiltre.Add(CodeSecteur) = false;

				}

				// puis on met le libellé
				DlgFiltre.m_MapLibFiltre.MoveTo(CodeSecteur);
				if (!DlgFiltre.m_MapLibFiltre.IsValid())
				{
					// Ajoute le libellé
					DlgFiltre.m_MapLibFiltre.Add(CodeSecteur) = pSecteur->GetLabel().AsJCharPtr();
				}
			}
		}
	}

	// Affiche la fenetre filtrage avec les elmts à filtés en cours si il y a lieu
	if (DlgFiltre.m_MapCodeFiltre.GetCount() > 0)
	{
		// voir m_pDlgItem
		if(IDOK == DlgFiltre.DoModal())
		{
			// Boucle sur toutes les modalités en cours sur dimension 1
			int NbSel = 0;
			int NbNotSel = 0;
			for (JUnt32 Idx = 0; Idx < m_pDlgItem->m_List_Dimension1.GetCount(); Idx++)
			{
				// Récupère la modalité correspondante
				const IModalite* pModalite = (const IModalite*)m_pDlgItem->m_List_Dimension1.GetItemData(Idx);

				MapEnfantParent.MoveTo(pModalite->GetID());
				if (MapEnfantParent.IsValid())
				{
					// Il faut récupérer le code secteur et le libellé associé à la marque
					JInt32 IdSecteur = MapEnfantParent.GetItem();
					DlgFiltre.m_MapCodeFiltre.MoveTo(IdSecteur);

					if (DlgFiltre.m_MapCodeFiltre.IsValid())
					{
						if (DlgFiltre.m_MapCodeFiltre.GetItem() == true)
						{
							NbSel++;
						}
						else
						{
							NbNotSel++;
						}
					}
				}
			}

			// Update Item Values Dialog for quanti vars
			// m_pDlgItem->m_ListValues.
			// UpdateItemValues(false);
			// SetValuesDlg();
			// m_pDlgItem->Selchange_ListDimension_1();
		
			// Nombre de modalités correspondantes
			JBool Ok = true;
		}
	}
	
	// Récupère filtre courant
	m_pDlgItem->m_MapFiltrage.Swap(DlgFiltre.m_MapCodeFiltre);

	// Mise à jour des modalités
	// UpdateDimensionsList();
	UpdateFiltreDimension1List();

	// Puis on remet à jour les values quantitatifs (car sélection change)
	m_pDlgItem->Selchange_ListDimension_1();
}

/// Button Modalite Down pushed
void CItemMode::OnDim_1_Up()
{
	// A VOIR Réordonnancement en mode construction / ALAIN Point 9
	// if(m_pSelectedQuestion && m_pSelectedQuestion->HasDimensions() )
	if(m_pSelectedQuestion && m_pSelectedQuestion->CanModify()&&  m_pSelectedQuestion->HasDimensions() )
	{
		JList<JInt32> Values;
		// Get the selected ID in the list box
		m_pDlgItem->m_List_Dimension1.GetSelectedID(Values);
		if(Values.GetCount() != 1)
			return;
		Values.MoveFirst();

		// Get the Modalies to reorder
		JList<JInt32> Ids;
		JVector<const IModalite *> vMods;
		m_pSelectedQuestion->GetIModalites(vMods,0,0,m_pDlgItem->GetContingentMask());
		for(vMods.MoveFirst(); vMods.IsValid(); vMods.MoveNext())
			Ids.AddTail() = vMods.GetItem()->GetID(); 

		// Reorder the Ids
		if(OrderValues(true,Ids, Values.GetItem()))
		{
			if(const_cast< IQuestion * >( m_pSelectedQuestion )->SetModaliteOrder(Ids))
			{
				UpdateDimensionsList();
				m_pDlgItem->m_List_Dimension1.SetSelectedID(Values.GetItem());
			}
		}
	}
}

/// Button Modalite Down pushed
void CItemMode::OnDim_1_Down()
{
	if(m_pSelectedQuestion && m_pSelectedQuestion->CanModify() && m_pSelectedQuestion->HasDimensions())
	{
		JList<JInt32> Values;
		// Get the selected ID in the list box
		m_pDlgItem->m_List_Dimension1.GetSelectedID(Values);
		if(Values.GetCount() != 1)
			return;
		Values.MoveFirst();

		// Get the Modalies to reorder
		JList<JInt32> Ids;
		JVector<const IModalite *> vMods;
		m_pSelectedQuestion->GetIModalites(vMods,0,0,m_pDlgItem->GetContingentMask());
		for(vMods.MoveFirst(); vMods.IsValid(); vMods.MoveNext())
		{
			JInt32 Val = vMods.GetItem()->GetID();
			Ids.AddTail() = vMods.GetItem()->GetID(); 
		}

		// Reorder the Ids
		if(OrderValues(false,Ids, Values.GetItem()))
		{
			if(const_cast< IQuestion * >( m_pSelectedQuestion )->SetModaliteOrder(Ids))
			{
				UpdateDimensionsList();
				m_pDlgItem->m_List_Dimension1.SetSelectedID(Values.GetItem());
			}
		}
	}
}

bool CItemMode::OrderValues(bool bUp,JList<JInt32> & Values, JInt32 Move)
{
	JList<JInt32> NewValues;
	bool bFirst = true;
	bool bBefore = false;

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		if(Values.GetItem() == Move)
		{
			if(bUp)
			{
				if(!bFirst)
					NewValues.AddBefore() = Values.GetItem();
				else
					NewValues.AddTail() = Values.GetItem();
			}
			else
			{
				NewValues.AddTail() = Values.GetItem();
				bBefore = true;
			}
		}
		else
		{
			if(bBefore)
			{
				NewValues.AddBefore() = Values.GetItem();
				bBefore = false;
			}
			else
			{
				NewValues.AddTail() = Values.GetItem();
			}
		}
		NewValues.MoveLast();
		bFirst = false;
	}
	Values.Swap(NewValues);
	return true;
}

void CItemMode::Format(const CString &Fmt, CString &Num, const char Decimal)
{
	CString End			=	"";
	CString Beginning	=	"";
	int LPN				=	Num.GetLength();
	int LPF				=	Fmt.GetLength();
	int PointPN			=	Num.ReverseFind(Decimal);
	int PointPF			=	Fmt.ReverseFind(Decimal);
	int ppn				=	0;
	int ppf				=	0;

	if(LPN>0 && LPF>0)
	{
		// Check if we have a Decimal
		if(PointPN == -1 || PointPF == -1)
		{
			if(PointPN == -1)
				PointPN = Num.GetLength()-1;
			else
				PointPN--;

			if(PointPF == -1)
				PointPF = Fmt.GetLength()-1;
			else
				PointPF--;
		}
		else
		{
			End.AppendChar(Decimal);

			ppn = PointPN+1;
			ppf = PointPF+1;

			while(ppn < LPN)
			{
				if(ppf >= LPF || Fmt.GetAt(ppf) ==  '#')
					End.AppendChar(Num.GetAt(ppn++)); 
				else
					End.AppendChar(Fmt.GetAt(ppf)); 
				ppf++;
			}
			PointPN--;
			PointPF--;
		}

		ppn = PointPN;
		ppf = PointPF;
		while(ppn >= 0)
		{
			if(ppf < 0 || Fmt.GetAt(ppf) ==  '#')
				Beginning.Insert(0,Num.GetAt(ppn--)); 
			else
				Beginning.Insert(0,Fmt.GetAt(ppf)); 
			ppf--;
		}
	}

	Num = Beginning + End;
}

// Prepares and calculates all seperate target items
void CItemMode::DoVisualisation(void)
{
	JBool GeneseValide = true;

	// Pas de visualisation en mode pupitre
	if (m_pDlgItem->GetTypeRepItem() == false) 
		return;

	if(!m_pDlgItem->IsVisualModeActive())
	{
		m_pDlgItem->m_Button_Accept.EnableWindow(FALSE);
		return;
	}

	m_pDlgItem->m_Button_Accept.EnableWindow((m_bItemValid) ? TRUE : FALSE);

	JList<CVisuItems>	VItems;
	JInt32 NbItems = 0;

	CIBuilder NewBuilder(m_IBuilder);
	int NbSegments = NewBuilder.GetSegmentCount();
	IBloc::OPERATOR segOp = NewBuilder.GetOperator();
	CString segOpStr = OperatorAsString(segOp);

	CIBuilder TargetBuilder;
	TargetBuilder.SetOperator(segOp);


	// Go through all segments
	for(int x = 0 ; x < NbSegments; ++x)
	{
		JInt32 NbItemsSeg = 0;

		CIBSegment *pSegment = NewBuilder.GetSegment(x);
		IBloc::OPERATOR queOp = pSegment->GetOperator();
		CString queOpStr = OperatorAsString(queOp);

		CIBSegment TargetSegment;
		TargetSegment.SetOperator(queOp);
		TargetBuilder.Add(&TargetSegment);

		CVisuItems & strateItem = VItems.AddTail();
		if(x == 0)
			strateItem.m_Label.Format(GETSTRING(IDS_RI_LABEL_SEG),x+1);
		else
			strateItem.m_Label.Format(GETSTRING(IDS_RI_LABEL_OPSEG),segOpStr,x+1);
		strateItem.m_Level = 2;

		// Check segments for Questions
		if(pSegment->HasQuestions())
		{
			JList<JInt32> qIds;
			pSegment->GetQuestionsID(qIds);
			qIds.MoveFirst();

			// go through all questions
			while(qIds.IsValid())
			{
				CIBQuestion *pTargetQuestion = pSegment->FindQuestion(qIds.GetItem());
				TargetSegment.Add(pTargetQuestion); 

				CVisuItems & qItem = VItems.AddTail();
				const CQuestion * pQuestion = m_pDlgItem->GetTerrain()->GetQuestionByID(pTargetQuestion->GetID()); 

				CString ValuesStr = "";
				CIBClass * pClass = pTargetQuestion->GetClass();
				CIBNumValue * pNumValue = pTargetQuestion->GetNumValue();

				if(pClass != 0)
				{
					ValuesStr = "    ";
					JList<IBloc::OPERATOR> OpList;
					pClass->GetOperators(OpList);
					JList<JFlt32> ValList;
					pClass->GetValues(ValList);
					for(OpList.MoveFirst(), ValList.MoveFirst(); OpList.IsValid(); OpList.MoveNext(),ValList.MoveNext())
					{
						if (ValList.GetItem() >= 0.0)
							ValuesStr.AppendFormat("%s %0.0f    ",OperatorAsString(OpList.GetItem()), ValList.GetItem());
						else
							ValuesStr.AppendFormat("%s #    ",OperatorAsString(OpList.GetItem()));
					}
				}
				else if(pNumValue != 0)
				{
					ValuesStr.Append("    =");
					JList<JFlt32> ValList;
					pNumValue->GetValues(ValList);
					for(ValList.MoveFirst(); ValList.IsValid(); ValList.MoveNext())
					{
						if (ValList.GetItem() >= 0.0)
                            ValuesStr.AppendFormat(" %0.0f,",ValList.GetItem());
						else
							ValuesStr.AppendFormat(" #,");
					}
				}
				// Modif MARS 2014
				// else if (pQuestion->HasValues())
				else if (pQuestion != 0)
				{
					if (pQuestion->HasValues())
					{
						// go to next question
						TargetSegment.RemoveQuestion(pTargetQuestion);
						qIds.MoveNext();
						continue;
					}
				}
				else if (pQuestion == 0)
				{
					
					if (GeneseValide)
						AfxMessageBox("Genese impossible...");
					GeneseValide = false;

					qIds.MoveNext();
					continue;
				}
				
				NbItems++;
				if((NbItemsSeg++) == 0)
					qItem.m_Label.Format("    %s",pQuestion->GetLabel().AsJCharPtr());
				else
					qItem.m_Label.Format("    %s  %s",queOpStr,pQuestion->GetLabel().AsJCharPtr());
				qItem.m_Level = 3;

				// Get the Modality list
				JList<CIBModalite *> Dimensions;
				pTargetQuestion->GetModalites(Dimensions);
				// Get the number of dimensions
				int DimCount = Dimensions.GetCount();
				IBloc::OPERATOR * pDimOp	= new IBloc::OPERATOR[DimCount];
				CString * pDimOpString		= new CString[DimCount];
				JList<JInt32> * pDim		= new JList<JInt32>[DimCount];

				// Get the dimensions modalities
				int y = 0;
				for(Dimensions.MoveFirst(); Dimensions.IsValid(); Dimensions.MoveNext(), ++y)
				{
					Dimensions.GetItem()->GetValues(pDim[y]);
					pDimOp[y]		= Dimensions.GetItem()->GetOperator();
					pDimOpString[y] = OperatorAsString(pDimOp[y]);
					pDim[y].MoveFirst();
				}

				y = 0;
				while(pDim[0].IsValid())
				{
					if(pDim[y].IsValid())
					{
						CVisuItems & modaItem = VItems.AddTail();
						const IModalite * pModa = pQuestion->GetIModalite(pDim[y].GetItem(),y);

						modaItem.m_Label.Append(_T("        "));
						for(int z=0; z < y; ++z)
							modaItem.m_Label.Append(_T("    "));

						CString TxtModa = pModa->GetLabel().AsJCharPtr(); 

						// Si 1 seul élmt et c'est une variable numérique
						if (TxtModa == "Valeurs numériques")
						{
							LOADSTRING(TxtModa, IDS_RI_VALEURS_NUM);
						}

						if(pDim[y].GetIndex() == 0 && pDimOp[y] != IBloc::Not)
							modaItem.m_Label.AppendFormat("%s", TxtModa);
						else
							modaItem.m_Label.AppendFormat("%s  %s",pDimOpString[y], TxtModa);
						modaItem.m_Level = qItem.m_Level+1+y;

						if(y == DimCount-1)
						{
							pTargetQuestion->ResetModalites();
							for(int z=0; z < DimCount; ++z)
							{
								CIBModalite * pModa = new CIBModalite();
								pModa->SetOperator(pDimOp[z]);
								pModa->AddValue(pDim[z].GetItem());
								pTargetQuestion->AddModalite(pModa);
							}

							// Calculate the current target configuration
							// Get a new item construit
							PTR_ITEMCONSTRUIT pItem(m_pDlgItem->GetItems()->GetNewItemConstruit());
							// Set the builder object
							pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),TargetBuilder);
							// Test is the current target is valid
							if(pItem->IsValid(m_pDlgItem->GetTerrain()))
							{
								// calculate the target
								CCalcItemParams Params;
								Params.SetTerrain(m_pDlgItem->GetTerrain());
								Params.SetEchantillonID(1);
								Params.SetUniversID(m_pDlgItem->GetTerrain()->GetDefaultUnivers());
								Params.SetTypePoidsID(m_pDlgItem->GetTypePoids());
								Params.SetPopulationSegment(m_pDlgItem->GetPopulationSegment());
								Params.SetSegmentsIDs(m_pDlgItem->GetSegments());

								// Lecture Mode Cumul Valeurs Quantitatives  (pour Balmétrie notamment)  // Par défaut on reste en mode calcul sans cumul  (modif Juillet 2013)
								// JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 0) != 0?true:false;
								JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 1) != 0?true:false;
								Params.SetCumulQuanti(CumulQuanti);



								// Calcul effectif en mode item (ssi mode rep items)
								if (m_pDlgItem->GetTypeRepItem())
								{
									if(pItem->CalculateEffectif(&Params))
									{
										modaItem.m_Effective = Params.GetEffectif();
										modaItem.m_EffectiveTotal = Params.GetEffectifTot();
										modaItem.m_NbCas = Params.GetNbCas();
									}
								}
								modaItem.m_Level = 1;
							}
							if(!ValuesStr.IsEmpty())
							{
								CVisuItems & valItem = VItems.AddTail();
								valItem.m_Label = _T("        ");
								for(int z=0; z < y; ++z)
									valItem.m_Label.Append(_T("    "));

								valItem.m_Label.AppendFormat(ValuesStr);
								valItem.m_Level = 0;
							}
						}
						else
						{
							++y;
							pDim[y].MoveFirst();
							continue;
						}
					}
					else
					{
						if(y)
							pDim[y--].MoveFirst();
					}
					pDim[y].MoveNext();
				}

				TargetSegment.RemoveQuestion(pTargetQuestion);

				// go to next question
				qIds.MoveNext();
				delete [] pDimOp;
				delete [] pDimOpString;
				delete [] pDim;
			}
		}

		if (GeneseValide == true)
		{
			// Check segments for items
			if(pSegment->HasItems())
			{
				JList<JInt32> Items;
				pSegment->GetItemsID(Items);
				// Segment->Remove
				for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
				{
					JInt32 ItemId = Items.GetItem();
					CIBItem * pTargetItem = pSegment->FindItem(ItemId);
					TargetSegment.Add(pTargetItem); 
					CVisuItems & qItem = VItems.AddTail();
					qItem.m_Level = 3;

					if(ItemId == 0)
					{
						CString Pseudo; LOADSTRING(Pseudo, IDS_RI_PSEUDO);
						NbItems++;
						if((NbItemsSeg++) == 0)
							qItem.m_Label.Format("    %s",Pseudo);
						else
							qItem.m_Label.Format("    %s  %s",queOpStr,Pseudo);
					}
					else
					{
						const IItem * pItem = m_pDlgItem->GetItems()->GetIItemByID(ItemId);
						if(pItem)
						{
							if((NbItems++) == 0)
								qItem.m_Label.Format("        %s",pItem->GetLabel().AsJCharPtr());
							else
								qItem.m_Label.Format("    %s  %s",queOpStr,pItem->GetLabel().AsJCharPtr());
						}
					}

					if(pTargetItem)
					{
						// Get the number of dimensions
						IBloc::OPERATOR ModaOp = pTargetItem->GetOperator();
						CString ModaOpString = OperatorAsString(ModaOp);
						JList<JInt32> Moda;
						pTargetItem->GetValues(Moda);

						// Get the modalities
						for(Moda.MoveFirst(); Moda.IsValid(); Moda.MoveNext())
						{
							CVisuItems & modaItem = VItems.AddTail();
							modaItem.m_Level  = 1;
							JInt32 ModaItemId = Moda.GetItem();
							const IItem * pModaItem = m_pDlgItem->GetItems()->GetIItemByID(ModaItemId);
							if(pModaItem)
							{
								JList<JInt32> ModaValues;
								ModaValues.AddTail() = ModaItemId;
								pTargetItem->SetValues(ModaValues);

								// Calculate the current target configuration
								// Get a new item construit
								PTR_ITEMCONSTRUIT pItem(m_pDlgItem->GetItems()->GetNewItemConstruit());
								// Set the builder object
								pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),TargetBuilder);
								// Test is the current target is valid
								if(pItem->IsValid(m_pDlgItem->GetTerrain()))
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
									{
										modaItem.m_Effective = Params.GetEffectif();
										modaItem.m_EffectiveTotal = Params.GetEffectifTot();
										modaItem.m_NbCas = Params.GetNbCas();
									}
								}
								modaItem.m_Label = _T("        ");
								if(Moda.GetIndex() == 0 && ModaOp != IBloc::Not)
									modaItem.m_Label.AppendFormat("%s",pModaItem->GetLabel().AsJCharPtr());
								else
									modaItem.m_Label.AppendFormat("%s  %s",ModaOpString,pModaItem->GetLabel().AsJCharPtr());
							}
						}
					}
					TargetSegment.RemoveItem(pTargetItem);
				}
			}

			TargetBuilder.RemoveSegment(&TargetSegment);
		}

		if (!GeneseValide) break;
	}


	// Visu items mais pas en mode palette
	if (GeneseValide)
		m_pDlgItem->SetData(VItems);
	
}


// Récupération des élmts constituant l'item sous forme de texte
CString CItemMode::GetVisualisation()
{
	// Initialisation texte infos Item
	CString TxtInfoItem = "";

	JList<CVisuItems>	VItems;
	JInt32 NbItems = 0;

	CIBuilder NewBuilder(m_IBuilder);
	int NbSegments = NewBuilder.GetSegmentCount();
	IBloc::OPERATOR segOp = NewBuilder.GetOperator();
	CString segOpStr = OperatorAsString(segOp);

	CIBuilder TargetBuilder;
	TargetBuilder.SetOperator(segOp);

	// Go through all segments
	for(int x = 0 ; x < NbSegments; ++x)
	{
		JInt32 NbItemsSeg = 0;

		CIBSegment *pSegment = NewBuilder.GetSegment(x);
		IBloc::OPERATOR queOp = pSegment->GetOperator();
		CString queOpStr = OperatorAsString(queOp);

		CIBSegment TargetSegment;
		TargetSegment.SetOperator(queOp);
		TargetBuilder.Add(&TargetSegment);

		CVisuItems & strateItem = VItems.AddTail();
		if(x == 0)
			strateItem.m_Label.Format(GETSTRING(IDS_RI_LABEL_SEG),x+1);
		else
			strateItem.m_Label.Format(GETSTRING(IDS_RI_LABEL_OPSEG),segOpStr,x+1);
		strateItem.m_Level = 2;

		// Check segments for Questions
		if(pSegment->HasQuestions())
		{
			JList<JInt32> qIds;
			pSegment->GetQuestionsID(qIds);
			qIds.MoveFirst();

			// go through all questions
			while(qIds.IsValid())
			{
				CIBQuestion *pTargetQuestion = pSegment->FindQuestion(qIds.GetItem());
				TargetSegment.Add(pTargetQuestion); 
				CVisuItems & qItem = VItems.AddTail();
				const CQuestion * pQuestion = m_pDlgItem->GetTerrain()->GetQuestionByID(pTargetQuestion->GetID()); 

				CString ValuesStr = "";
				CIBClass * pClass = pTargetQuestion->GetClass();
				CIBNumValue * pNumValue = pTargetQuestion->GetNumValue();
				if(pClass != 0)
				{
					ValuesStr = "    ";
					JList<IBloc::OPERATOR> OpList;
					pClass->GetOperators(OpList);
					JList<JFlt32> ValList;
					pClass->GetValues(ValList);
					for(OpList.MoveFirst(), ValList.MoveFirst(); OpList.IsValid(); OpList.MoveNext(),ValList.MoveNext())
					{
						if (ValList.GetItem() >= 0.0)
							ValuesStr.AppendFormat("%s %0.0f    ",OperatorAsString(OpList.GetItem()), ValList.GetItem());
						else
							ValuesStr.AppendFormat("%s #    ",OperatorAsString(OpList.GetItem()));
					}
				}
				else if(pNumValue != 0)
				{
					ValuesStr.Append("    =");
					JList<JFlt32> ValList;
					pNumValue->GetValues(ValList);
					for(ValList.MoveFirst(); ValList.IsValid(); ValList.MoveNext())
					{
						if (ValList.GetItem() >= 0.0)
                            ValuesStr.AppendFormat(" %0.0f,",ValList.GetItem());
						else
							ValuesStr.AppendFormat(" #,");
					}
				}

				// Attention aux questions quantitatives sans sélection, pas d'ajout visualisation
				// (modifié Septembre 2006)
				else if (pQuestion->HasValues())
				{
					// go to next question
					TargetSegment.RemoveQuestion(pTargetQuestion);
					qIds.MoveNext();
					continue;
				}

				NbItems++;
				if((NbItemsSeg++) == 0)
					qItem.m_Label.Format("    %s",pQuestion->GetLabel().AsJCharPtr());
				else
					qItem.m_Label.Format("    %s  %s",queOpStr,pQuestion->GetLabel().AsJCharPtr());
				qItem.m_Level = 3;

				// Get the Modality list
				JList<CIBModalite *> Dimensions;
				pTargetQuestion->GetModalites(Dimensions);
				// Get the number of dimensions
				int DimCount = Dimensions.GetCount();
				IBloc::OPERATOR * pDimOp	= new IBloc::OPERATOR[DimCount];
				CString * pDimOpString		= new CString[DimCount];
				JList<JInt32> * pDim		= new JList<JInt32>[DimCount];

				// Get the dimensions modalities
				int y = 0;
				for(Dimensions.MoveFirst(); Dimensions.IsValid(); Dimensions.MoveNext(), ++y)
				{
					Dimensions.GetItem()->GetValues(pDim[y]);
					pDimOp[y]		= Dimensions.GetItem()->GetOperator();
					pDimOpString[y] = OperatorAsString(pDimOp[y]);
					pDim[y].MoveFirst();
				}

				y = 0;
				while(pDim[0].IsValid())
				{
					if(pDim[y].IsValid())
					{
						CVisuItems & modaItem = VItems.AddTail();
						const IModalite * pModa = pQuestion->GetIModalite(pDim[y].GetItem(),y);

						modaItem.m_Label.Append(_T("        "));
						for(int z=0; z < y; ++z)
							modaItem.m_Label.Append(_T("    "));

						CString TxtModa = pModa->GetLabel().AsJCharPtr(); 

						// Si 1 seul élmt et c'est une variable numérique
						if (TxtModa == "Valeurs numériques")
						{
							LOADSTRING(TxtModa, IDS_RI_VALEURS_NUM);
						}

						if(pDim[y].GetIndex() == 0 && pDimOp[y] != IBloc::Not)
							modaItem.m_Label.AppendFormat("%s",TxtModa);
						else
							modaItem.m_Label.AppendFormat("%s  %s",pDimOpString[y],TxtModa);
						modaItem.m_Level = qItem.m_Level+1+y;

						if(y == DimCount-1)
						{
							pTargetQuestion->ResetModalites();
							for(int z=0; z < DimCount; ++z)
							{
								CIBModalite * pModa = new CIBModalite();
								pModa->SetOperator(pDimOp[z]);
								pModa->AddValue(pDim[z].GetItem());
								pTargetQuestion->AddModalite(pModa);
							}

							// Calculate the current target configuration
							// Get a new item construit
							PTR_ITEMCONSTRUIT pItem(m_pDlgItem->GetItems()->GetNewItemConstruit());
							// Set the builder object
							pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),TargetBuilder);
							// Test is the current target is valid
							if(pItem->IsValid(m_pDlgItem->GetTerrain()))
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
								{
									modaItem.m_Effective = Params.GetEffectif();
									modaItem.m_EffectiveTotal = Params.GetEffectifTot();
									modaItem.m_NbCas = Params.GetNbCas();
								}
								modaItem.m_Level = 1;
							}
							if(!ValuesStr.IsEmpty())
							{
								CVisuItems & valItem = VItems.AddTail();
								valItem.m_Label = _T("        ");
								for(int z=0; z < y; ++z)
									valItem.m_Label.Append(_T("    "));

								valItem.m_Label.AppendFormat(ValuesStr);
								valItem.m_Level = 0;
							}
						}
						else
						{
							++y;
							pDim[y].MoveFirst();
							continue;
						}
					}
					else
					{
						if(y)
							pDim[y--].MoveFirst();
					}
					pDim[y].MoveNext();
				}
				TargetSegment.RemoveQuestion(pTargetQuestion);
				// go to next question
				qIds.MoveNext();
				delete [] pDimOp;
				delete [] pDimOpString;
				delete [] pDim;
			}
		}

		// Check segments for items
		if(pSegment->HasItems())
		{
			JList<JInt32> Items;
			pSegment->GetItemsID(Items);
			for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
			{
				JInt32 ItemId = Items.GetItem();
				CIBItem * pTargetItem = pSegment->FindItem(ItemId);
				TargetSegment.Add(pTargetItem); 
				CVisuItems & qItem = VItems.AddTail();
				qItem.m_Level = 3;

				if(ItemId == 0)
				{
					CString Pseudo; LOADSTRING(Pseudo, IDS_RI_PSEUDO);
					if((NbItems++) == 0)
						qItem.m_Label.Format("    %s",Pseudo);
					else
						qItem.m_Label.Format("    %s  %s",queOpStr,Pseudo);
				}
				else
				{
					const IItem * pItem = m_pDlgItem->GetItems()->GetIItemByID(ItemId);
					if(pItem)
					{
						if((NbItems++) == 0)
							qItem.m_Label.Format("        %s",pItem->GetLabel().AsJCharPtr());
						else
							qItem.m_Label.Format("    %s  %s",queOpStr,pItem->GetLabel().AsJCharPtr());
					}
				}

				if(pTargetItem)
				{
					// Get the number of dimensions
					IBloc::OPERATOR ModaOp = pTargetItem->GetOperator();
					CString ModaOpString = OperatorAsString(ModaOp);
					JList<JInt32> Moda;
					pTargetItem->GetValues(Moda);

					// Get the modalities
					for(Moda.MoveFirst(); Moda.IsValid(); Moda.MoveNext())
					{
						CVisuItems & modaItem = VItems.AddTail();
						modaItem.m_Level  = 1;
						JInt32 ModaItemId = Moda.GetItem();
						const IItem * pModaItem = m_pDlgItem->GetItems()->GetIItemByID(ModaItemId);
						if(pModaItem)
						{
							JList<JInt32> ModaValues;
							ModaValues.AddTail() = ModaItemId;
							pTargetItem->SetValues(ModaValues);

							// Calculate the current target configuration
							// Get a new item construit
							PTR_ITEMCONSTRUIT pItem(m_pDlgItem->GetItems()->GetNewItemConstruit());
							// Set the builder object
							pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),TargetBuilder);
							// Test is the current target is valid
							if(pItem->IsValid(m_pDlgItem->GetTerrain()))
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
								{
									modaItem.m_Effective = Params.GetEffectif();
									modaItem.m_EffectiveTotal = Params.GetEffectifTot();
									modaItem.m_NbCas = Params.GetNbCas();
								}
							}
							modaItem.m_Label = _T("        ");
							if(Moda.GetIndex() == 0 && ModaOp != IBloc::Not)
								modaItem.m_Label.AppendFormat("%s",pModaItem->GetLabel().AsJCharPtr());
							else
								modaItem.m_Label.AppendFormat("%s  %s",ModaOpString,pModaItem->GetLabel().AsJCharPtr());
						}
					}
				}
				TargetSegment.RemoveItem(pTargetItem);
			}
		}

		TargetBuilder.RemoveSegment(&TargetSegment);
	}

	// Nombre de lignes
	CString NbLigne;
	NbLigne.Format("%s \t%d\n", "Nb Lignes :", VItems.GetCount());
	TxtInfoItem += NbLigne;

	// Boucle sur tous les items à éditer
	for (VItems.MoveFirst(); VItems.IsValid(); VItems.MoveNext())
	{
		// Attention au décalage (on ne tient pas compte des levels car à partir d'un certain niveau indication fausse !!!!)
		// on prend donc l'indentation comme sur l'écran
		CVisuItems strateItem = VItems.GetItem();
		CString TxtTot = strateItem.m_Label; 
		TxtTot = TxtTot.TrimLeft();
		int TailleEspace = strateItem.m_Label.GetLength() - TxtTot.GetLength();   
		int Niveau = TailleEspace / 4;

		for (int i=0; i<Niveau;i++)
			TxtInfoItem += "\t";

		TxtInfoItem += strateItem.m_Label.TrimLeft();

		if (strateItem.m_NbCas >= 0)
		{
			CString Txt = "";

			Txt.Format("%d\t%0.f",strateItem.m_NbCas,strateItem.m_Effective/1000);

			// Ajout Info effectif et nb cas
			for (int j=0; j<6-Niveau;j++)
				Txt = "\t" + Txt;

			// Txt.Format("\t\t%d\t%0.f",strateItem.m_NbCas,strateItem.m_Effective/1000);
			TxtInfoItem += Txt;
			TxtInfoItem += "\n";
		}
		else
			TxtInfoItem += "\n";
		
	}

	// Retour des infios des composants items
	return TxtInfoItem;
}

//Appends the operator to the string
CString	CItemMode::OperatorAsString(IBloc::OPERATOR Op)
{
	CString str;
	switch(Op)
	{
		case IBloc::Nop :
		break;

		case IBloc::And :
			LOADSTRING(str, IDS_RI_ET);
		break;

		case IBloc::Or :
			LOADSTRING(str, IDS_RI_OU);
		break;

		case IBloc::Not :
			LOADSTRING(str, IDS_RI_NI);
		break;

		case IBloc::Xor :
			LOADSTRING(str, IDS_RI_XOR);
		break;

		case IBloc::LT :
			str = _T("<");
		break;

		case IBloc::GT :
			str = _T(">");
		break;

		case IBloc::LTE :
			str = _T("<=");
		break;

		case IBloc::GTE :
			str = _T(">=");
		break;

		case IBloc::EQ :
			str = _T("=");
		break;
	}
	return str;
}

bool CItemMode::IsSearched(const IItem * item) const
{
	CString searchStr;
	m_pDlgItem->m_Edit_Search.GetWindowText(searchStr);
	return CSearchAlgo::Contains(item->GetLabel().AsJCharPtr(), searchStr);
}

///////////////////////////////////////////////////////////////////////
// Quel mode pour le Sas
bool CItemMode:: ModeCroisementPalette()
{
	// Rien ici
	return false;
}

// Suppression des fenêtres mobiles
void CItemMode::EffaceFenDimDlg(bool MajSelectedUserItem)
{
	// Vire la fenêtre modalité valeur si elle existe
	/*
	if (m_pValuesDlg)
	{
		m_pValuesDlg->RemoveValues();
		delete m_pValuesDlg;
		m_pValuesDlg = 0;
	}
	*/

	// Cache la fenêtre modalité valeur si elle existe
	if (m_pDlgItem->IsWindowVisible())
	{
		if (m_pValuesDlg)
		{
			m_pValuesDlg->ShowWindow(FALSE); 
		}
	}

	// Vire les autres fenêtres mobiles
	int ValMode = m_pDlgItem->ValCurMode();
	if (MajSelectedUserItem && m_pDlgItem->ValCurMode() != Mode_Palettes && m_pDlgItem->GetTypeRepItem())
		m_Selected_UserItems.Reset();
	
	// Les autres fenêtres
	m_DimDlg.Reset();
}

///////////////////////////////////////////////////////////////////////
// Récupération palette
bool CItemMode::SendPalette(JStream &Stream, int Mode)
{
	// rien
	return false;
}

///////////////////////////////////////////////////////////////////////
// Récupération palette
bool CItemMode::RecvPalette(JStream &Stream, int Mode)
{
	// rien
	return false;
}

///////////////////////////////////////////////////////////////////////
// Recopie elmt palette
bool CItemMode::CopyPalette(CAbstractMode * pPaletteMode)
{
	// rien
	return false;
}

//////////////////////////////////////////////////////////////////////
// Sauvegarde des palettes temporaires
bool CItemMode::SaveMemento(JMemento	* pMemento, int Mode)
{
	// rien
	return false;
}

//////////////////////////////////////////////////////////////////////
// Récupération des palettes temporaires
bool CItemMode::LoadMemento(JMemento	* pMemento, int Mode)
{
	// rien
	return false;
}
