// BlocQuestions.cpp: implementation of the CBlocQuestions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlocQuestions.h"
#include "TerrainManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlocQuestions::CBlocQuestions()
{
	m_ID=0;
	m_Label="";
}

CBlocQuestions::~CBlocQuestions()
{
}

JArchive& CBlocQuestions::Recv(JArchive &lhs, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_ID);
			m_Label.Recv(lhs);
		}
		break;

		default:
			TRACE("Question Bloc version error");
			throw JInternalError::GetInstance(); //("Question Bloc version error");
		break;
	}
	return lhs;
}

JBool CBlocQuestions::FillTree(CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM hParent, JBool AllLeaves, JBool OnlyQuanti)
{
	if (!AllLeaves)
	{
		TVINSERTSTRUCT Ins;
		Ins.hParent				=	hParent;
		Ins.hInsertAfter		=	NULL;
		Ins.item.mask			=	TVIF_PARAM|TVIF_STATE|TVIF_TEXT;
		Ins.item.state			=	(0x0001)|INDEXTOSTATEIMAGEMASK(1);
		Ins.item.stateMask		=	TVIS_STATEIMAGEMASK|0x000F ;
		Ins.item.pszText		=	(char *)m_Label.AsJCharPtr();
		Ins.item.cchTextMax		=	255;
		Ins.item.iImage			=	0;
		Ins.item.iSelectedImage	=	0;
		Ins.item.lParam			=	(DWORD)AsITheme();	

		// Ajoute la branche bloc de questions
		HTREEITEM hPar = Tree.InsertItem(&Ins);
	}
	else
	{
		// Au moins 1 question quanti à afficher
		JBool AuMoinsAffich1Quanti = false;

		// Get bloc de question uniquement quanti
		ITheme * pTheme = (ITheme *)AsITheme();
		
		if (pTheme)
		{
			// Recup les questions associées au bloc
			JList<IQuestion const *> QuestionTheme;
			pTerrain->GetQuestionsByBlocID(pTheme->GetID(), QuestionTheme);

			// Passe en revue toutes les questions du thème
			for (QuestionTheme.MoveFirst(); QuestionTheme.IsValid(); QuestionTheme.MoveNext())
			{
				// Récup Question du thème
				const IQuestion * pIQuestion = QuestionTheme.GetItem(); 

				if (pIQuestion && pIQuestion->IsQuestion())
				{
					// Récupère la question correspondante
					CQuestion *pQuestion = (CQuestion *)pIQuestion;

					int NbDims = pQuestion->HasDimensions();

					if (pQuestion)
					{
						// AOUT 2013 ne pas prendre les questions multidimensionnelles
						if (OnlyQuanti == false || (OnlyQuanti && pQuestion->AutoMoyennable() && pQuestion->HasDimensions() == 1))
						{
							AuMoinsAffich1Quanti = true;
							break;
						}
					}
				}
			}

			if (AuMoinsAffich1Quanti)
			{
				// Des questions-enfants existent
				TVINSERTSTRUCT Ins;
				Ins.hParent				=	hParent;
				Ins.hInsertAfter		=	NULL;
				Ins.item.mask			=	TVIF_CHILDREN|TVIF_PARAM|TVIF_STATE|TVIF_TEXT;
				Ins.item.state			=	(0x0001)|INDEXTOSTATEIMAGEMASK(1);
				Ins.item.stateMask		=	TVIS_STATEIMAGEMASK|0x000F ;
				Ins.item.pszText		=	(char *)m_Label.AsJCharPtr();
				Ins.item.cchTextMax		=	255;
				Ins.item.iImage			=	0;
				Ins.item.iSelectedImage	=	0;
				Ins.item.cChildren		=	1;
				Ins.item.lParam			=	(DWORD)AsITheme();	

				// On peut ajouter la branche bloc de questions
				HTREEITEM hPar = Tree.InsertItem(&Ins);

				// Ici on continue et on affiche toutes les modalités
				// On vérifie que le thème est valide
				ITheme * pTheme = AsITheme();
				if(pTheme && pTheme->IsBlocQuestion())
				{
					// Update symbole image
					ToggleItemState(pTerrain, Tree, hPar);
					
					// Complete chacune des dernieres branches thèmes avec les questions correspondantes
					UpdateQuestionTreeQuanti(pTerrain, Tree, hPar, OnlyQuanti);
				}
			}
		}
	}

	return true;
}

void CBlocQuestions::ToggleItemState(CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM &hti)
{
	/*int iImage = Tree.GetItemState( hti, TVIS_STATEIMAGEMASK )>>12;

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
	}*/

	int iImage = 1;

	if (Tree.ItemHasChildren(hti))
	{
		ToggleChildItemState(Tree, hti,iImage);
	}
	else
	{
		Tree.SetItemState(hti, INDEXTOSTATEIMAGEMASK(iImage), TVIS_STATEIMAGEMASK);
	}
	ToggleParentItemState(Tree,hti,iImage);
}

void CBlocQuestions::ToggleChildItemState(CTreeCtrl &Tree, HTREEITEM hti,int iImage)
{
	Tree.SetItemState(hti, INDEXTOSTATEIMAGEMASK(iImage), TVIS_STATEIMAGEMASK);

	if (Tree.ItemHasChildren(hti))
	{
		HTREEITEM htiChild = Tree.GetChildItem(hti);
		if (htiChild)
			ToggleChildItemState(Tree,htiChild,iImage);
		else
			return ;
		HTREEITEM htiSibling = Tree.GetNextSiblingItem (htiChild);
		while (htiSibling )
		{
			ToggleChildItemState(Tree,htiSibling,iImage);
			htiSibling = Tree.GetNextSiblingItem(htiSibling);
		}
	}
}

void CBlocQuestions::ToggleParentItemState(CTreeCtrl &Tree, HTREEITEM hti,int iImage)
{
	HTREEITEM hParent = Tree.GetParentItem(hti);
	if(hParent)
	{
		int piImage = 1;

		/*
		if(HasActifChildren(Tree, hParent))
		{
			piImage = 2;
			if(HasInactifChildren(Tree,hParent))
				piImage = 3;
		}
		*/
		Tree.SetItemState(hParent, INDEXTOSTATEIMAGEMASK(piImage), TVIS_STATEIMAGEMASK);
		ToggleParentItemState(Tree,hParent,piImage);
	}
}

bool CBlocQuestions::HasInactifChildren		(CTreeCtrl &Tree, HTREEITEM hti)
{
	if (Tree.ItemHasChildren(hti))
	{
		HTREEITEM htiChild = Tree.GetChildItem (hti);
		if (htiChild)
		{
			if((Tree.GetItemState(htiChild, TVIS_STATEIMAGEMASK )>>12) > 1)
				return true;
			if(HasActifChildren(Tree, htiChild))
				return true;
		}
		else
			return false;
		HTREEITEM htiSibling = Tree.GetNextSiblingItem (htiChild);
		while (htiSibling )
		{
			if((Tree.GetItemState(htiSibling, TVIS_STATEIMAGEMASK )>>12) > 1)
				return true;
			if(HasActifChildren(Tree, htiSibling))
				return true;
			htiSibling = Tree.GetNextSiblingItem(htiSibling);
		}
	}
	return false;
}

bool CBlocQuestions::HasActifChildren		(CTreeCtrl &Tree, HTREEITEM hti)
{
	if (Tree.ItemHasChildren(hti))
	{
		HTREEITEM htiChild = Tree.GetChildItem (hti);
		if (htiChild)
		{
			if((Tree.GetItemState(htiChild, TVIS_STATEIMAGEMASK )>>12) == 1)
				return true;
			if(HasInactifChildren(Tree,htiChild))
				return true;
		}
		else
			return false;
		HTREEITEM htiSibling = Tree.GetNextSiblingItem (htiChild);
		while (htiSibling )
		{
			if((Tree.GetItemState(htiSibling, TVIS_STATEIMAGEMASK )>>12) == 1)
				return true;
			if(HasInactifChildren(Tree,htiSibling))
				return true;
			htiSibling = Tree.GetNextSiblingItem(htiSibling);
		}
	}
	return false;
}

// Complete chacune des dernieres branches thèmes avec les questions correspondantes
void CBlocQuestions::UpdateQuestionTreeQuanti(CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM &hti, JBool OnlyQuanti)
{
	while(hti)
	{
		
		UINT Value = Tree.GetItemState(hti, TVIS_STATEIMAGEMASK );

		//if((Tree.GetItemState(hti, TVIS_STATEIMAGEMASK )>>12) == 2)
		//{
			ITheme * pTheme = (ITheme *)Tree.GetItemData(hti);
			if(pTheme->IsBlocQuestion())
			{
				// Ajout dans la liste container questions
				IQuestion *pIQuestion = ((CBlocQuestions *)pTheme)->AsIQuestion();
				JList<IQuestion const *> QuestionTheme;

				// Get bloc de question uniquement quanti
				pTerrain->GetQuestionsByBlocID(pTheme->GetID(), QuestionTheme);

				// Vérifie si bloc doit être sélectionné
				if (pTheme->IsBlocQuestion())
				{
					// Passe en revue toutes les questions du thème
					for (QuestionTheme.MoveFirst(); QuestionTheme.IsValid(); QuestionTheme.MoveNext())
					{
						// Récup Question du thème
						const IQuestion * pIQuestion = QuestionTheme.GetItem(); 

						if (pIQuestion && pIQuestion->IsQuestion())
						{
							// Récupère la question correspondante
							CQuestion *pQuestion = (CQuestion *)pIQuestion;

							if (pQuestion)
							{
								if (OnlyQuanti == false || (OnlyQuanti && pQuestion->AutoMoyennable()))
								{
									TVINSERTSTRUCT Ins;
									Ins.hParent				=	hti;
									Ins.hInsertAfter		=	NULL;
									Ins.item.mask			=	TVIF_PARAM|TVIF_STATE|TVIF_TEXT;
									Ins.item.state			=	(0x0001)|INDEXTOSTATEIMAGEMASK(1);    // A Revoir
									Ins.item.stateMask		=	TVIS_STATEIMAGEMASK|0x000F ;
									Ins.item.pszText		=	(char *)pIQuestion->GetLabel().AsJCharPtr();
									Ins.item.cchTextMax		=	255;
									Ins.item.iImage			=	0;
									Ins.item.iSelectedImage	=	0;
									Ins.item.lParam			=	(DWORD)(pQuestion)->AsIQuestion();	

									// Ajoute la question au théme correspondant
									HTREEITEM hPar = Tree.InsertItem(&Ins);
								}
							}
						}
					}
				}
			// }
		}

		hti = Tree.GetNextItem(hti,TVGN_NEXT);
	}
}