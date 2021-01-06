// ThemeTree.cpp : implementation file
//
#include "stdafx.h"
#include "ThemeTree.h"
#include "DlgItems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemeTree

// Constructeur
CThemeTree::CThemeTree()
{
	m_CATPColorsApp = CATPColors::COLORREPITEMS;
	m_BlockId_Sel = 0;
	m_iModality = 0;
	m_sFilter = "";

	// Par défaut mode item (=1)
	m_IdModeVisu = 1;

	// Init item par défaut
	m_hItemByDefault = 0;

}

CThemeTree::~CThemeTree()
{
}

// Positionne le mode visu en passsant aussi le mode visu (Mode items, palettes, classes,quantiles,questions induites)
// Par défaut mode items
void CThemeTree::SetModeVisu(JUnt32 IdModeVisu)
{
	m_IdModeVisu	= IdModeVisu;
}

BEGIN_MESSAGE_MAP(CThemeTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CThemeTree)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThemeTree message handlers

void CThemeTree::SetData(CTerrain *pTerrain, CItemsConstruitsBase * pIConst, CString & sFilter, int iModality)
{
	// Remove all items
	SetColors();
	DeleteAllItems();
	m_Question_List.Reset();
	m_pTerrain = pTerrain;
	m_pIConst = pIConst;
	m_sFilter = sFilter;
	m_iModality = iModality;
	m_mElements.Reset();
	m_mModalities.Reset();

	if(pTerrain && pTerrain->IsLoaded())
	{
		CString str;
		//str.LoadString(IDS_RI_GRANDTHEME);
		LOADSTRING(str, IDS_RI_GRANDTHEME);
		HTREEITEM hPar = InsertItem(TVIF_CHILDREN|TVIF_PARAM|TVIF_STATE|TVIF_TEXT,str,0,0,(0x0001)|INDEXTOSTATEIMAGEMASK(1),TVIS_STATEIMAGEMASK|0x000F,0,0,0);
		pTerrain->FillIndexThematique(*((CTreeCtrl *)this),hPar,false,false);
		Expand(hPar,TVE_EXPAND);

		// Nb Branches principales Theme Tree
		if (NbConnect() == 1) m_iModality = 1;

		switch(m_iModality)
		{
			case 1:
			case 3:
			case 4:
				m_pTerrain->FindElements(m_mElements,m_sFilter);
				m_pTerrain->FindModalities(m_mModalities,m_sFilter);
			break;
		}
		FilterTree();
		if(m_iModality != 0)
			ExpandAll();
		UpdateQuestionList();
	}
}

// Récupère état index thématique
void CThemeTree::SetEtatData(JMap<DWORD,JUnt32> *pMapSelTree)
{
	// Récupère Map Courant positionnement tree 
	m_pMapSelTree = pMapSelTree;
}


void	CThemeTree::SetColors()
{
	m_imageState.DeleteImageList ();

	switch(CATPColors::GetAppColor(m_CATPColorsApp))
	{
		// Nouvelle color tree control
		case CATPColors::BOTTLE_GREEN : 
			m_imageState.Create( IDB_RI_BITMAP1, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::EMERAUDE :
			m_imageState.Create( IDB_RI_BITMAP2, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::BLEU_PASTEL :

			m_imageState.Create( IDB_RI_BITMAP3, 15, 1, RGB(255,255,255) );
			// m_imageState.Create( IDB_RI_BITMAP4, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::OCEAN : 
			m_imageState.Create( IDB_RI_BITMAP4, 15, 1, RGB(255,255,255) );
			break;
			
		case CATPColors::LILAS :
			m_imageState.Create( IDB_RI_BITMAP5, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::SAHARA :
			m_imageState.Create( IDB_RI_BITMAP6, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::TERRE_SIENNE :
			m_imageState.Create( IDB_RI_BITMAP7, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::RUBIS :
			m_imageState.Create( IDB_RI_BITMAP8, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::GREY_MOUSE :
			m_imageState.Create( IDB_RI_BITMAP9, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::ROSE :
			m_imageState.Create( IDB_RI_BITMAP10, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::ROSE_KAKI :
			m_imageState.Create( IDB_RI_BITMAP11, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::ROSE_GRIS :
			m_imageState.Create( IDB_RI_BITMAP12, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::SABLE :
			m_imageState.Create( IDB_RI_BITMAP13, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::CAFE :
			m_imageState.Create( IDB_RI_BITMAP14, 15, 1, RGB(255,255,255) );
			break;

		case CATPColors::NEWBLUE :
			m_imageState.Create( IDB_RI_BITMAP15, 15, 1, RGB(255,255,255) );
			break;
	}
	SetImageList( &m_imageState, TVSIL_STATE );
	UpdateWindow(); 
}

void CThemeTree::RemoveSelections()
{
	HTREEITEM hti = GetRootItem();
	m_Question_List.Reset();
	RemoveSelections(hti);
	GetParent()->PostMessage(WM_USER_QUESTIONLISTUPDATED,0,0);
}

void CThemeTree::RemoveSelections(HTREEITEM hti)
{
	while(hti)
	{
		SetItemState(hti, INDEXTOSTATEIMAGEMASK(1), TVIS_STATEIMAGEMASK);
		if (ItemHasChildren(hti))
		{
			RemoveSelections(GetChildItem(hti));
		}
		hti = GetNextItem(hti,TVGN_NEXT);
	}
}

void CThemeTree::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags=0;
	HTREEITEM hti = HitTest(point,&uFlags);

	if( uFlags & (TVHT_ONITEMSTATEICON)) //|TVHT_ONITEMLABEL))
	{
		ITheme * pTheme = (ITheme *)GetItemData(hti);
		if(pTheme && pTheme->IsBlocQuestion())
			m_BlockId_Sel = pTheme->GetID();

		ToggleItemState(hti);
		UpdateQuestionList();

		// Modifie sélection du noeud courant
		SetSelectTree(hti);
	}
	else
	{
		CTreeCtrl::OnLButtonDown(nFlags, point);

		// Modifie ouverture du noeud courant
		SetOpenTree(hti);
	}
}

void CThemeTree::OnRButtonDown(UINT nFlags, CPoint point) 
{
//	CTreeCtrl::OnRButtonDown(nFlags, point);
}

void CThemeTree::ToggleItemState(HTREEITEM hti, bool WithSelection)
{
	int iImage = GetItemState( hti, TVIS_STATEIMAGEMASK )>>12;

	switch(iImage)
	{
		case 1:
			iImage = 2;
		break;

		case 2:
			iImage = 1;
		break;

		case 3:
			iImage = 2;
		break;
	}

	if (ItemHasChildren(hti))
	{
		ToggleChildItemState(hti,iImage);
	}
	else
	{
		SetItemState(hti, INDEXTOSTATEIMAGEMASK(iImage), TVIS_STATEIMAGEMASK);
	}
	ToggleParentItemState(hti,iImage);
}

void CThemeTree::ToggleChildItemState(HTREEITEM hti,int iImage)
{
	SetItemState(hti, INDEXTOSTATEIMAGEMASK(iImage), TVIS_STATEIMAGEMASK);

	if (ItemHasChildren(hti))
	{
		HTREEITEM htiChild = this->GetChildItem (hti);
		if (htiChild)
			ToggleChildItemState(htiChild,iImage);
		else
			return ;
		HTREEITEM htiSibling = GetNextSiblingItem (htiChild);
		while (htiSibling )
		{
			ToggleChildItemState(htiSibling,iImage);
			htiSibling = GetNextSiblingItem(htiSibling);
		}
	}
}

void CThemeTree::ToggleParentItemState(HTREEITEM hti,int iImage)
{
	HTREEITEM hParent = GetParentItem(hti);
	if(hParent)
	{
		int piImage = 1;

		if(HasActifChildren(hParent))
		{
			piImage = 2;
			if(HasInactifChildren(hParent))
				piImage = 3;
		}
		SetItemState(hParent, INDEXTOSTATEIMAGEMASK(piImage), TVIS_STATEIMAGEMASK);
		ToggleParentItemState(hParent,piImage);
	}
}

bool CThemeTree::HasInactifChildren(HTREEITEM hti)
{
	if (ItemHasChildren(hti))
	{
		HTREEITEM htiChild = this->GetChildItem (hti);
		if (htiChild)
		{
			if((GetItemState(htiChild, TVIS_STATEIMAGEMASK )>>12) == 1)
				return true;
			if(HasInactifChildren(htiChild))
				return true;
		}
		else
			return false;
		HTREEITEM htiSibling = GetNextSiblingItem (htiChild);
		while (htiSibling )
		{
			if((GetItemState(htiSibling, TVIS_STATEIMAGEMASK )>>12) == 1)
				return true;
			if(HasInactifChildren(htiSibling))
				return true;
			htiSibling = GetNextSiblingItem(htiSibling);
		}
	}
	return false;
}

bool CThemeTree::HasActifChildren(HTREEITEM hti)
{
	if (ItemHasChildren(hti))
	{
		HTREEITEM htiChild = this->GetChildItem (hti);
		if (htiChild)
		{
			if((GetItemState(htiChild, TVIS_STATEIMAGEMASK )>>12) > 1)
				return true;
			if(HasActifChildren(htiChild))
				return true;
		}
		else
			return false;
		HTREEITEM htiSibling = GetNextSiblingItem (htiChild);
		while (htiSibling )
		{
			if((GetItemState(htiSibling, TVIS_STATEIMAGEMASK )>>12) > 1)
				return true;
			if(HasActifChildren(htiSibling))
				return true;
			htiSibling = GetNextSiblingItem(htiSibling);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////
// Mise à jour de la liste des questions sélectionnées via le tree item des questions
void CThemeTree::UpdateQuestionList()
{
	HTREEITEM hti = GetRootItem();
	m_Question_List.Reset();
	UpdateQuestionList(hti);
	GetParent()->PostMessage(WM_USER_QUESTIONLISTUPDATED,m_BlockId_Sel,0);
	m_BlockId_Sel = 0;

	// Filtre les questions quantitatives si on est en mode questions induites
	if (m_IdModeVisu == 6)
	{
		m_Question_List.MoveFirst(); 

		// Index de la rubrique courante et nb questions dedans
		JInt32 InxRubriqueCur = -1;
		JUnt32 NbQuestionRubrique = 0;

		while (m_Question_List.IsValid())
		{
			// Récupère l'item question
			IQuestion *lpItem = (IQuestion *)m_Question_List.GetItem(); 
			CQuestion *pQuestion = lpItem->AsQuestion();

			if (pQuestion == NULL || !pQuestion->IsValidQuestion())
			{
				// Voir paragraphe précédent s'il est à virer
				if (InxRubriqueCur >= 0 && NbQuestionRubrique == 0)
				{
					// On vire le paragraphe
					m_Question_List.MoveTo(InxRubriqueCur);
					if (m_Question_List.IsValid())
						m_Question_List.Remove(1);
				}

				// Début paragraphe question
				InxRubriqueCur		= m_Question_List.GetIndex(); 
				NbQuestionRubrique	= 0;
			}	

			// Si c'est une question quantitative ou qualitative simple (avec pas plus de 1 dimension)
			if (pQuestion && pQuestion->HasDimensions() <= 1 && (pQuestion->IsQuantitative() || pQuestion->IsQualiSimple()))
			{
				// On la vire de la liste
				m_Question_List.Remove(1); 
			}
			else
			{
				// Passe à l'item question suivante
				m_Question_List.MoveNext();

				// Comptabilise 1 question en + dans la rubrique
				if (pQuestion && pQuestion->IsValidQuestion())
					NbQuestionRubrique++; 
			}
		}

		// Si dernier élmt est une rubrique sans question, on l'élimine
		if (InxRubriqueCur >= 0 && NbQuestionRubrique == 0)
		{
			// On vire le paragraphe  / A VOIR !!!!
			//m_Question_List.MoveTo(InxRubriqueCur);
			//if (m_Question_List.IsValid()) 
			//	m_Question_List.Remove(1);
		}
	}
}

void CThemeTree::UpdateQuestionList(HTREEITEM hti)
{
	while(hti)
	{
		if (ItemHasChildren(hti))
		{
			UpdateQuestionList(GetChildItem(hti));
		}
		else
		{
			UINT Value = GetItemState(hti, TVIS_STATEIMAGEMASK );

			if((GetItemState(hti, TVIS_STATEIMAGEMASK )>>12) == 2)
			{
				ITheme * pTheme = (ITheme *)GetItemData(hti);
				if(pTheme->IsBlocQuestion())
				{

					// Ajout dans la liste container questions
					// CQuestion *pQuestion = (CQuestion *)((CBlocQuestions *)pTheme)->AsIQuestion();

					IQuestion *pIQuestion = ((CBlocQuestions *)pTheme)->AsIQuestion();

					m_Question_List.AddTail() = ((CBlocQuestions *)pTheme)->AsIQuestion(); 

					switch(m_iModality)
					{
						case 1:
							// Filter Question, Elements and Modalities
							m_pTerrain->GetQuestionsByBlocID(pTheme->GetID(), m_Question_List, m_sFilter, m_mElements, m_mModalities);
							m_pIConst->GetCriteresModifieByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
							m_pIConst->GetCriteresQuantileByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
							m_pIConst->GetQuestionsInduiteByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
						break;

						case 2:
							// Filter Questions only
							m_pTerrain->GetQuestionsByBlocID(pTheme->GetID(), m_Question_List, m_sFilter);
							m_pIConst->GetCriteresModifieByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
							m_pIConst->GetCriteresQuantileByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
							m_pIConst->GetQuestionsInduiteByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
						break;

						case 3:
						case 4:
						{
							// Filter Elements and Modalities only
							CString xFilter = "";
							m_pTerrain->GetQuestionsByBlocID(pTheme->GetID(), m_Question_List, xFilter, m_mElements, m_mModalities);
							m_pIConst->GetCriteresModifieByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, 3);
							m_pIConst->GetCriteresQuantileByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, 3);
							m_pIConst->GetQuestionsInduiteByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, 3);
						}
						break;

						default:
							// Dont Filter anything
							m_pTerrain->GetQuestionsByBlocID(pTheme->GetID(), m_Question_List);
							m_pIConst->GetCriteresModifieByBlocID(m_Question_List, pTheme->GetID(), m_pTerrain);
							m_pIConst->GetCriteresQuantileByBlocID(m_Question_List, pTheme->GetID(), m_pTerrain);
							m_pIConst->GetQuestionsInduiteByBlocID(m_Question_List, pTheme->GetID(), m_pTerrain);
						break;
					}
				}
			}
		}
		hti = GetNextItem(hti,TVGN_NEXT);
	}
}


///////////////////////////////////////////////////////////////////////////////////////
// Mise à jour de la liste des questions sélectionnées via le tree item des questions
void CThemeTree::UpdateQuestionTree(JBool ModeAjout, bool bSendMessageNow)
{
	if (bSendMessageNow)
		GetParent()->SendMessage(WM_USER_ITEMQUANTILISTUPDATED,m_BlockId_Sel, ModeAjout);
	else
		GetParent()->PostMessage(WM_USER_ITEMQUANTILISTUPDATED,m_BlockId_Sel, ModeAjout);

	m_BlockId_Sel = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Au lieu d'ajouter les questions dans une liste intermédiaire, on complète les branches enfants du treecontol
void CThemeTree::UpdateQuestionTree(HTREEITEM hti)
{
	while(hti)
	{
		if (ItemHasChildren(hti))
		{
			UpdateQuestionTree(GetChildItem(hti));
		}
		else
		{
			UINT Value = GetItemState(hti, TVIS_STATEIMAGEMASK );

			if((GetItemState(hti, TVIS_STATEIMAGEMASK )>>12) == 2)
			{
				ITheme * pTheme = (ITheme *)GetItemData(hti);
				if(pTheme->IsBlocQuestion())
				{

					IQuestion *pIQuestion = ((CBlocQuestions *)pTheme)->AsIQuestion();
					switch(m_iModality)
					{
						case 1:
							// Filter Question, Elements and Modalities
							m_pTerrain->GetQuestionsByBlocID(pTheme->GetID(), m_Question_List, m_sFilter, m_mElements, m_mModalities, true);
							m_pIConst->GetCriteresModifieByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
							m_pIConst->GetCriteresQuantileByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
							m_pIConst->GetQuestionsInduiteByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
							break;

						case 2:
							// Filter Questions only
							m_pTerrain->GetQuestionsByBlocID(pTheme->GetID(), m_Question_List, m_sFilter, true);
							m_pIConst->GetCriteresModifieByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
							m_pIConst->GetCriteresQuantileByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
							m_pIConst->GetQuestionsInduiteByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, m_iModality);
							break;

						case 3:
						case 4:
							{
								// Filter Elements and Modalities only
								CString xFilter = "";
								m_pTerrain->GetQuestionsByBlocID(pTheme->GetID(), m_Question_List, xFilter, m_mElements, m_mModalities, true);
								m_pIConst->GetCriteresModifieByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, 3);
								m_pIConst->GetCriteresQuantileByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, 3);
								m_pIConst->GetQuestionsInduiteByBlocID(m_Question_List, m_sFilter, pTheme->GetID(), m_pTerrain, 3);
							}
							break;

						default:
							// Dont Filter anything
							m_pTerrain->GetQuestionsByBlocID(pTheme->GetID(), m_Question_List, true);
							m_pIConst->GetCriteresModifieByBlocID(m_Question_List, pTheme->GetID(), m_pTerrain);
							m_pIConst->GetCriteresQuantileByBlocID(m_Question_List, pTheme->GetID(), m_pTerrain);
							m_pIConst->GetQuestionsInduiteByBlocID(m_Question_List, pTheme->GetID(), m_pTerrain);
							break;
					}
				}
			}
		}
		hti = GetNextItem(hti,TVGN_NEXT);
	}
}

void CThemeTree::FilterTree()
{
	HTREEITEM hti = GetRootItem();
	m_Question_List.Reset();
	FilterTree(hti);
	GetParent()->PostMessage(WM_USER_QUESTIONLISTUPDATED,m_BlockId_Sel,0);
	m_BlockId_Sel = 0;
}

void CThemeTree::FilterTree(HTREEITEM hti)
{
	while(hti)
	{
		HTREEITEM DelItem = 0;

		if (ItemHasChildren(hti))
		{
			FilterTree(GetChildItem(hti));
			if(GetChildItem(hti) == 0)
				DelItem = hti;
		}
		else
		{
			ITheme * pTheme = (ITheme *)GetItemData(hti);
			if(pTheme->IsBlocQuestion())
			{
				bool bFound = false;
				switch(m_iModality)
				{
					case 1:
						// Filter Question, Elements and Modalities
						bFound = m_pTerrain->HasQuestionsByBlocID(pTheme->GetID(), m_sFilter, m_mElements, m_mModalities);
					break;

					case 2:
						// Filter Questions only
						bFound = m_pTerrain->HasQuestionsByBlocID(pTheme->GetID(), m_sFilter);
					break;

					case 3:
					case 4:
					{
						// Filter Elements and Modalities only
						CString xFilter = "";
						bFound = m_pTerrain->HasQuestionsByBlocID(pTheme->GetID(), xFilter, m_mElements, m_mModalities);
					}
					break;

					default:
						// Dont Filter anything
						bFound = m_pTerrain->HasQuestionsByBlocID(pTheme->GetID());
					break;
				}
				if(!bFound)
					DelItem = hti;
			}
		}
		hti = GetNextItem(hti,TVGN_NEXT);
		if(DelItem != 0)
			DeleteItem(DelItem);
	}
}


void CThemeTree::SetBlock(JInt32 BlockID)
{
	HTREEITEM hti = FindBlockQuestion(BlockID);
	if(hti && (GetItemState(hti, TVIS_STATEIMAGEMASK )>>12) == 1)
	{
		m_BlockId_Sel = BlockID;
		SetItemState(hti, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK);
		ToggleParentItemState(hti,2);
		UpdateQuestionList();
	}
}

void CThemeTree::SetBlock(JList<JInt32> & BlockID)
{
	bool bUpdate = false;

	for(BlockID.MoveFirst(); BlockID.IsValid(); BlockID.MoveNext())
	{
		HTREEITEM hti = FindBlockQuestion(BlockID.GetItem());
		if(hti && (GetItemState(hti, TVIS_STATEIMAGEMASK )>>12) == 1)
		{
			m_BlockId_Sel = BlockID.GetItem();
			SetItemState(hti, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK);
			ToggleParentItemState(hti,2);
			bUpdate = true;
		}
	}
	if(bUpdate)
		UpdateQuestionList();
}

HTREEITEM CThemeTree::FindBlockQuestion(JInt32 BlockID)
{
	HTREEITEM hti = GetRootItem();
	return FindBlockQuestion(hti,BlockID);
}

HTREEITEM CThemeTree::FindBlockQuestion(HTREEITEM hti, JInt32 BlockID)
{
	while(hti)
	{

		if (ItemHasChildren(hti))
		{
			HTREEITEM hfound = FindBlockQuestion(GetChildItem(hti),BlockID);

			if(hfound)
				return hfound;
		}
		else
		{
			ITheme * pTheme = (ITheme *)GetItemData(hti);
			if(pTheme->IsBlocQuestion() && pTheme->GetID() == BlockID)
				return hti;
		}
		hti = GetNextItem(hti,TVGN_NEXT);
	}
	return 0;
}

// Sélection des questions
void CThemeTree::SetQuestionList(JList <JInt32> & ListQuestion)
{
	if (ListQuestion.GetCount() > 0)
	{
		// Pointe sur début tree
		HTREEITEM hti = GetRootItem();

		// Sélection via les branches
		SetQuestionList(hti, ListQuestion);
	}
}

// Sélection des questions
void CThemeTree::SetQuestionList(HTREEITEM hti, JList <JInt32> &ListQuestion)
{
	while(hti)
	{
		// On ne continue que s'il reste des questions à marquer
		if (ListQuestion.GetCount() > 0)
		{
			// Analyse des branches enfants
			if (ItemHasChildren(hti))
			{
				// On positionne les questions à marquer sur les branches enfants
				SetQuestionList(GetChildItem(hti),ListQuestion);
			}
			else
			{
				// Voir dans questions du thème
				ITheme * pTheme = (ITheme *)GetItemData(hti);
				JList<IQuestion const *> QuestionTheme;
				m_pTerrain->GetQuestionsByBlocID(pTheme->GetID(), QuestionTheme);

				// Vérifie si bloc doit être sélectionné
				if(pTheme->IsBlocQuestion())
				{
					// Passe en revue toutes les questions du thème
					for (QuestionTheme.MoveFirst(); QuestionTheme.IsValid(); QuestionTheme.MoveNext())
					{
						// Récup Question du thème
						const IQuestion * pIQuestion = QuestionTheme.GetItem(); 

						// Ident Question
						JInt32 IdQuBloc = pIQuestion->GetID(); 

						// Vérifie qu'elle a été sélectionné
						for (ListQuestion.MoveLast(); ListQuestion.IsValid(); ListQuestion.MovePrev())
						{
							// Id de la question à vérifier
							JInt32 IdQuestion = ListQuestion.GetItem();
						
							// Si sélectionné, on valide le bloc, et on enlève la question des questions à séklectionner
							if (IdQuestion == IdQuBloc)
							{
								// Ident bloc de question à sélectionner
								JInt32 IdBlocQuestion = pIQuestion->GetBlocID(); 

								// On le sélectionne
								// Sélectionne la question dans le tree-items
								SetBlock(IdBlocQuestion);

								// On vire la question
								ListQuestion.Remove(); 
							}

							// Plus de question à marquer dans la liste des questions thème
							if (ListQuestion.GetCount() <= 0)
								break;

						}

						// Plus de question à sélectionner, donc plus de bloc thème à sélectonner
						if (ListQuestion.GetCount() <= 0)
							break;
					}
				}
			}

			// Plus la peine de continuer, toutes les sélections ont été marquées dans le tree control thème
			if (ListQuestion.GetCount() <= 0)
				break;

			// Passe à l'item suivant
			hti = GetNextItem(hti,TVGN_NEXT);
		}
		else
			// Liste question vide
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Nb branches 1er niveau Tree Item
JUnt32 CThemeTree::NbConnect()
{
	// Pointe sur début tree
	HTREEITEM hti = GetRootItem();

	// Init Nb Branches principales 
	JUnt32 NbC = 0;

	// Balaye les branches
	while(hti)
	{
		// Y a t-il une branche enfant
		if (ItemHasChildren(hti))
		{
			NbC++;	
			hti = GetChildItem(hti);
		}

		// On passe à la branche suivante
		hti = GetNextItem(hti,TVGN_NEXT);
	}

	return NbC;
}

////////////////////////////////////////////////////////////////////////////////////////
// Déploiement tree control
void CThemeTree::ExpandAll()
{
	HTREEITEM hti = GetRootItem();
	ExpandAll(hti);
}

////////////////////////////////////////////////////////////////////////////////////////
// Déploiement branche tree control
void CThemeTree::ExpandAll(HTREEITEM hti)
{
	while(hti)
	{
		// Y a t-il une branche enfant
		if (ItemHasChildren(hti))
		{
			Expand(hti,TVE_EXPAND);
			ExpandAll(GetChildItem(hti));
		}
		hti = GetNextItem(hti,TVGN_NEXT);
	}
}

void CThemeTree::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;
	switch (pLVCD->nmcd.dwDrawStage)
	{
		case CDDS_ITEM:
		case CDDS_POSTPAINT:
		case CDDS_PREERASE:
		case CDDS_POSTERASE:
		case CDDS_ITEMPOSTPAINT :
		case CDDS_ITEMPREERASE :
		case CDDS_ITEMPOSTERASE :
			break ;

		case CDDS_PREPAINT :
			*pResult = CDRF_NOTIFYITEMDRAW  ;
			break ;

		case CDDS_ITEMPREPAINT :
			if ( pLVCD->nmcd.uItemState&CDIS_SELECTED )
			{
				pLVCD->clrTextBk = ::GetSysColor(COLOR_WINDOW);     //CATPColors::GetColorSelect(m_CATPColorsApp);
				pLVCD->clrText   = ::GetSysColor(COLOR_WINDOWTEXT);
			}

			// on recupère l'élément par défaut
			HTREEITEM hItem = (HTREEITEM)pLVCD->nmcd.dwItemSpec;
			if (hItem == m_hItemByDefault)
			{
				// Dessine élmt par défaut avec une autre couleur
				pLVCD->clrText   = CATPColors::GetColorDark(m_CATPColorsApp);   
				pLVCD->clrTextBk = CATPColors::GetColorLight(m_CATPColorsApp);
			}

			*pResult |= CDRF_NEWFONT;
			break ;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Positionne élmt tree (noeud ouvert ou fermé)
void CThemeTree::SetOpenTree(HTREEITEM hti)
{
	if (m_pMapSelTree && hti)
	{
		// Récup ident data lié au noeud
		DWORD Param = static_cast<DWORD>(GetItemData(hti));

		//Vérifie si non déjà existant
		m_pMapSelTree->MoveTo(Param);
		if (m_pMapSelTree->IsValid())
		{
			// Mise à jour
			JUnt32 &EtatCur = m_pMapSelTree->GetItem();
			if (GetItemState(hti,TVIS_EXPANDED) & TVIS_EXPANDED)
			{
				// Elmt signalétique à déployer
				if ((EtatCur & HTI_Open) == 0)
					EtatCur += HTI_Open;
			}
			else
			{
				// Elmt signalétique déjà déployé, on supprime le déploiement
				if (EtatCur == HTI_Open || EtatCur == HTI_OpenSelect)
					EtatCur -= HTI_Open;
			}
		}
		else
		{
			// Ajout nouvelle info noeud en mode ouvert
			if (GetItemState(hti,TVIS_EXPANDED) & TVIS_EXPANDED)
				m_pMapSelTree->Add(Param) = HTI_Open; 
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Positionne élmt tree theme (sélectionné ou pas)
void CThemeTree::SetSelectTree(HTREEITEM hti)
{
	if (m_pMapSelTree && hti)
	{
		// Récup ident data lié au noeud
		DWORD Param = static_cast<DWORD>(GetItemData(hti));

		// Etat courant du noeud
		int iImage = GetItemState( hti, TVIS_STATEIMAGEMASK )>>12;

		//Vérifie si non déjà existant
		m_pMapSelTree->MoveTo(Param);
		if (m_pMapSelTree->IsValid())
		{
			// Mise à jour
			JUnt32 &EtatCur = m_pMapSelTree->GetItem();

			if (iImage == 2)
			{
				// Elmt à sélectionner
				if ((EtatCur & HTI_Select) == 0)
					EtatCur += HTI_Select;
			}
			else
			{
				// Déjà sélectionné on supprime sa sélection
				if (EtatCur == HTI_Select || EtatCur == HTI_OpenSelect)
					EtatCur -= HTI_Select;
			}
		}
		else
		{
			// Ajout nouvelle info noeud en mode ouvert
			if (iImage == 2)
				m_pMapSelTree->Add(Param) = HTI_Select; 
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Noeud tree item est-il ouvert ????
bool CThemeTree::IsTreeOuvert(HTREEITEM hti)
{
	if (m_pMapSelTree)
	{
		// Récup ident data lié au noeud
		DWORD Param = static_cast<DWORD>(GetItemData(hti));

		// Vérifie que noeud thème actif
		m_pMapSelTree->MoveTo(Param);
		if (m_pMapSelTree->IsValid())
		{
			// Récupère état du noeud
			JUnt32 EtatNoeud = m_pMapSelTree->GetItem(); 

			// Est-il déployer
			if (EtatNoeud & HTI_Open)
				return true;
			else
				return false;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Noeud tree item est-il sélectionné
bool CThemeTree::IsTreeSel(HTREEITEM hti)
{
	if (m_pMapSelTree)
	{
		// Récup ident data lié au noeud
		DWORD Param = static_cast<DWORD>(GetItemData(hti));

		// Vérifie que noeud thème actif
		m_pMapSelTree->MoveTo(Param);
		if (m_pMapSelTree->IsValid())
		{
			// Récupère état du noeud
			JUnt32 EtatNoeud = m_pMapSelTree->GetItem(); 

			// Est-il sélectionné
			if (EtatNoeud & HTI_Select)
				return true;
			else
				return false;
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Réinitialise les sélections tree
void CThemeTree::ResetMapTree()
{
	if (m_pMapSelTree)
	{
		// Map existant, on l'initialise
		m_pMapSelTree->Reset(); 
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Actualisation tree item via la sélection sauvegardée
void CThemeTree::TreeViaSauvegarde(bool WithSelection)
{
	if (m_pMapSelTree && m_pMapSelTree->GetCount() > 0)
	{
		// Pointe sur début tree
		HTREEITEM hti = GetRootItem();

		// D'office la racine déployée
		Expand(hti,TVE_EXPAND);

		// Récup ident data lié au noeud
		DWORD Param = static_cast<DWORD>(GetItemData(hti));

		m_pMapSelTree->MoveTo(Param);
		if (m_pMapSelTree->IsValid())
		{
			// Récupère état du noeud
			JUnt32 EtatNoeud = m_pMapSelTree->GetItem(); 

			// Sélectionne le noeud si précédemment sélectionné
			if ((EtatNoeud & HTI_Select) && WithSelection)
			{
				SetItemState(hti, TVIS_SELECTED,  TVIS_SELECTED);
			}
		}


		// Puis on positionne les branches déployées précédemment
		TreeViaSauvegarde(hti, WithSelection);
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Déploiement branche tree control
void CThemeTree::TreeViaSauvegarde(HTREEITEM hti, bool WithSelection)
{
	// Balaye les branches
	while(hti)
	{
		// Récup ident data lié au noeud
		DWORD Param = static_cast<DWORD>(GetItemData(hti));

		// Test si cette branche était déployé
		m_pMapSelTree->MoveTo(Param);
		if (m_pMapSelTree->IsValid())
		{
			// Récupère état du noeud
			JUnt32 EtatNoeud = m_pMapSelTree->GetItem(); 
			
			// Test s'il était ouvert
			if (EtatNoeud & HTI_Open)
			{
				Expand(hti,TVE_EXPAND);
			}

			// Sélectionne le noeud si précédemment sélectionné
			if (EtatNoeud & HTI_Select)
			{
				// Positionne la sélection
				ToggleItemState(hti, WithSelection);
			}
		}

		// Y a t-il une branche enfant
		if (ItemHasChildren(hti))
		{
			// Récupére sauvegarde déploiement et sélection sur la branche enfant
			TreeViaSauvegarde(GetChildItem(hti), WithSelection);
		}
		
		// On passe à la branche suivante
		hti = GetNextItem(hti,TVGN_NEXT);
	}
}

//////////////////////////////////////////////////////////////
//permet de connaitre la profondeur d'un item dans le tree
int CThemeTree::GetProfondeur(const HTREEITEM hitem) const
{
	int prof = -1;
	HTREEITEM hparent = hitem;
	do
	{
		hparent = GetParentItem(hparent);
		prof++;
	}while (hparent != NULL);

	return prof;
}

///////////////////////////////////////////////////////////////
// Indique l'item par défaut
void CThemeTree::SetItemByDefault(HTREEITEM hti)
{
	m_hItemByDefault = hti;

}

///////////////////////////////////////////////////////////////
// Récupère l'item par défaut
HTREEITEM CThemeTree::GetItemByDefault()
{
	return m_hItemByDefault;
}