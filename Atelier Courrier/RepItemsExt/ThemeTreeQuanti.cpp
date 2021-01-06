#include "StdAfx.h"
#include "ThemeTreeQuanti.h"
#include "DlgItemsMoyenne.h"
#include "DlgItems.h"

////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CThemeTreeQuanti::CThemeTreeQuanti()
{
	// Liste des questions quantitatives sélectionnés
	m_MapSelQuestQuanti.Reset();

	// Init sélection liste id question par défaut
	m_LstBlockId_Sel.Reset();

}

////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CThemeTreeQuanti::~CThemeTreeQuanti(void)
{
}

BEGIN_MESSAGE_MAP(CThemeTreeQuanti, CTreeCtrl)
	//{{AFX_MSG_MAP(CThemeTreeQuanti)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////////
// Récupération des datas pour l'arborescence
void CThemeTreeQuanti::SetData(CTerrain *pTerrain, CItemsConstruitsBase * pIConst,CString & sFilter, int iModality, bool UpdateNow)
{
	// Remove all items
	SetColors();
	DeleteAllItems();
	m_Question_List.Reset();
	m_pTerrain	= pTerrain;
	m_pIConst	= pIConst;
	m_sFilter	= sFilter;
	m_iModality = iModality;
	m_mElements.Reset();
	m_mModalities.Reset();

	// Récup terrain
	m_pTerrain = pTerrain;

	if(pTerrain && pTerrain->IsLoaded())
	{
		CString str;
		str.LoadString(IDS_RI_GRANDTHEME);
		HTREEITEM hPar = InsertItem(TVIF_CHILDREN|TVIF_PARAM|TVIF_STATE|TVIF_TEXT,str,0,0,(0x0001)|INDEXTOSTATEIMAGEMASK(1),TVIS_STATEIMAGEMASK|0x000F,0,0,0);

		// Remplissage de l'arbre avec toutes ses feuilles
		pTerrain->FillIndexThematique(*((CTreeCtrl *)this),hPar, true, true);

		// Expand all tree quanti
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

		// Mise à jour de l'arbre des élmts questions quantitatives  
		// (ne fait apparaitre que les branches avec au moins 1 question quanti existante)
		UpdateQuestionTree(false, UpdateNow);
	}
}


// Complete les dernières termes avec les questions associées
void CThemeTreeQuanti::FillWithAllQuestions(HTREEITEM hPar)
{
	// rien	
}

// Sélection d'un élmt de l'arbre
void CThemeTreeQuanti::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Init liste des questions
	m_LstBlockId_Sel.Reset();

	UINT uFlags=0;
	HTREEITEM hti = HitTest(point,&uFlags);

	if( uFlags & (TVHT_ONITEMSTATEICON)) //|TVHT_ONITEMLABEL))
	{
		// Active ou désactive le noeud
		ToggleItemState(hti);

		// Récup état sélection de la question
		JBool ItemSelect = (GetItemState( hti, TVIS_STATEIMAGEMASK )>>12 != 1);

		// Ajoute ou supprime les questions de cette branche
		ActiveQuestions(hti, ItemSelect);	

		// Mise à jour de la liste des items moyennables avec ajout d'une question
		// UpdateQuestionTree(true);

		// Modifie sélection du noeud courant
		SetSelectTree(hti);

		// Mise à jour de la liste
		// GetParent()->PostMessage(WM_USER_ITEMQUANTILISTUPDATED,0, ItemSelect);
		GetParent()->SendMessage(WM_USER_ITEMQUANTILISTUPDATED,0, ItemSelect);
	}
	else
	{
		CTreeCtrl::OnLButtonDown(nFlags, point);

		// Modifie ouverture du noeud courant
		SetOpenTree(hti);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Ajoute ou supprime les questions de cette branche
void CThemeTreeQuanti::ActiveQuestions(HTREEITEM hti, JBool ItemSelect)
{
	while (hti)
	{
		if (ItemHasChildren(hti))
		{
			hti = GetChildItem(hti);
			while (hti)
			{
				ActiveQuestions(hti, ItemSelect);
				hti = GetNextSiblingItem(hti);
			}
		}
		else
		{
			// Est-ce une question en fin de branche
			CQuestion *pQuestion = (CQuestion *)GetItemData(hti);
			HTREEITEM htiChild = this->GetChildItem(hti);

			if (pQuestion && pQuestion->IsQuestion() && htiChild == NULL)
			{
				// N° de la question
				m_BlockId_Sel = pQuestion->GetID();

				// Ident de la question
				JInt32 IdQuestion = pQuestion->GetID();

				// Ajoute la question au map des questions quantis
				if (ItemSelect)
				{
					m_MapSelQuestQuanti.MoveTo(pQuestion->GetID());
					if (!m_MapSelQuestQuanti.IsValid())
					{
						// Ajoute ident question à la liste
						m_MapSelQuestQuanti.Add(pQuestion->GetID());

						// Mise à jour de la liste des items moyennables avec ajout d'une question
						// UpdateQuestionTree(true);
						m_LstBlockId_Sel.MoveTo(m_BlockId_Sel);
						if (!m_LstBlockId_Sel.IsValid())
							m_LstBlockId_Sel.AddTail() = m_BlockId_Sel; 
					}
				}
				else
				{
					m_MapSelQuestQuanti.MoveTo(pQuestion->GetID());
					if (m_MapSelQuestQuanti.IsValid())
					{
						// Remove de la liste des questions sélectionnés
						m_MapSelQuestQuanti.Remove();

						// Mise à jour de la liste des items moyennables avec suppression d'une question
						// UpdateQuestionTree(false);
						m_LstBlockId_Sel.MoveTo(m_BlockId_Sel);
						if (!m_LstBlockId_Sel.IsValid())
							m_LstBlockId_Sel.AddTail() = m_BlockId_Sel; 
					}
				}
				hti = htiChild;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des sélections en cours
void CThemeTreeQuanti::ToggleItemState(HTREEITEM hti, bool WithSelection)
{
	int iImage = GetItemState( hti, TVIS_STATEIMAGEMASK )>>12;

	if (WithSelection)
	{
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
	}

	if (ItemHasChildren(hti))
	{
		ToggleChildItemState(hti,iImage);
	}
	else
	{
		if (WithSelection)
			SetItemState(hti, INDEXTOSTATEIMAGEMASK(iImage), TVIS_STATEIMAGEMASK);

	}
	ToggleParentItemState(hti,iImage);
}
// Affiche les questions pré-sélectionnées
void CThemeTreeQuanti::AffichPreselectQu()
{
	HTREEITEM hti = GetRootItem();
	AffichPreselectQu(hti);
}	

// Affiche les questions pré-sélectionnées
void CThemeTreeQuanti::AffichPreselectQu(HTREEITEM hti)
{
	while(hti)
	{
		if (ItemHasChildren(hti))
		{
			hti = GetChildItem(hti);
			AffichPreselectQu(hti);
		}
		else
		{
			int iImage = GetItemState( hti, TVIS_STATEIMAGEMASK )>>12;
			if (iImage == 2)
			{
				ActiveQuestions(hti, true);
			}
			/*
			else
			{
				// Récup ident data lié au noeud
				DWORD Param = static_cast<DWORD>(GetItemData(hti));

				//Vérifie si non déjà existant
				m_pMapSelTree->MoveTo(Param);
				if (m_pMapSelTree->IsValid())
				{
					m_pMapSelTree->Remove();
				}
			}
			*/
		}
		hti = GetNextItem(hti,TVGN_NEXT);
	}
}
