// QuestionManager.cpp: implementation of the CQuestionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestionManager.h"
#include "Convert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestionManager::CQuestionManager()
{
	m_Questions = 0;
}

CQuestionManager::~CQuestionManager()
{
}


JArchive&	CQuestionManager::Recv(JArchive& lhs, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of question
			lhs.Recv(m_Questions);

			m_lQuestions.Reset();

			for(JInt32 x = 0; x < m_Questions; x++)
			{
				PTR_QUESTION pItem(new CQuestion(m_pElements, m_pModalites, m_pSecurity));

				pItem->Recv(lhs, Version);

				// Stock the Question only when its valid
				if(pItem->IsValidQuestion())
				{
					JInt32 ID = pItem->GetID();
					JInt32 BlocID = pItem->GetBlocID();

					// Set the list of question by bloc id
					m_mBlocID.MoveTo(BlocID);
					if(!m_mBlocID.IsValid())
					{
						m_mBlocID.Add(BlocID);
						m_mBlocID.MoveTo(BlocID);
					}
					m_mBlocID.GetItem().AddTail() = pItem->AsIQuestion();
					m_mQuestionID.Add(ID) = pItem.get();
					m_lQuestions.AddTail() = pItem;
				} 
			}

			m_mItemNatif.Reset();

			JInt32 NbItemsNatif = 0;

			for(m_lQuestions.MoveFirst(); m_lQuestions.IsValid(); m_lQuestions.MoveNext())
			{
				JInt32 Qid = m_lQuestions.GetItem()->GetID();
				JInt32 Item = m_lQuestions.GetItem()->GetItemStart();

				JInt32 x = 0;
				for(; x < m_lQuestions.GetItem()->GetNbItems() ; x++)
				{
					if(m_lQuestions.GetItem()->IsValidItem(x))
						m_mItemNatif.Add(Item+x).Set(m_lQuestions.GetItem().get(),x);
				} 
				NbItemsNatif = Item + x;
			}
			//TRACE1("Number of Questions = %d\n",m_lQuestions.GetCount());
			//TRACE1("Number of items natif = %d\n",NbItemsNatif);
		}
		break;

		default:
			//TRACE("CQuestionManager Invalid Version");
			throw JInternalError::GetInstance();
		break;
	}
	return lhs;
}

void	CQuestionManager::GetQuestionsByBlocID(JInt32 BlocID, JList<const IQuestion *> &List, JBool OnlyQuanti) const
{
	if(!BlocID)
	{
		// BlocID is zero so get all the question
		for(m_lQuestions.MoveFirst(); m_lQuestions.IsValid(); m_lQuestions.MoveNext())
			List.AddTail() = m_lQuestions.GetItem()->AsIQuestion();
	}
	else
	{
		m_mBlocID.MoveTo(BlocID);
		if(m_mBlocID.IsValid())
		{
			const JList<const  IQuestion *> & Temp = m_mBlocID.GetItem();
			for(Temp.MoveFirst(); Temp.IsValid(); Temp.MoveNext())
			{
				// Récup la question 
				const IQuestion *pIQuestion = Temp.GetItem();
				CQuestion *pQuestion = (CQuestion *)pIQuestion;

				if (OnlyQuanti == false || (OnlyQuanti == true && pQuestion->AutoMoyennable() == true))
				{
					// On ajoute l'élmt question si (1 / toutes les questions à afficher, 2/ les quantis demandés et c'est une question quanti)   
					List.AddTail() = Temp.GetItem();
				}
			}
		}
	}
}

void	CQuestionManager::GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, const CString & sFilter, JBool OnlyQuanti)  const
{
	if(!BlocID)
	{
		// BlocID is zero so get all the question
		for(m_lQuestions.MoveFirst(); m_lQuestions.IsValid(); m_lQuestions.MoveNext())
		{
			const IQuestion * pIQuestion = m_lQuestions.GetItem()->AsIQuestion();
			if(Convert::ToSearchString(pIQuestion->GetLabel().AsJCharPtr()).Find(sFilter) != -1) 
				List.AddTail() = pIQuestion;
		}
	}
	else
	{
		m_mBlocID.MoveTo(BlocID);
		if(m_mBlocID.IsValid())
		{
			const JList<const  IQuestion *> & Temp = m_mBlocID.GetItem();
			for(Temp.MoveFirst(); Temp.IsValid(); Temp.MoveNext())
			{
				const IQuestion * pIQuestion = Temp.GetItem();
				if(Convert::ToSearchString(pIQuestion->GetLabel().AsJCharPtr()).Find(sFilter) != -1) 
					List.AddTail() = pIQuestion;
			}
		}
	}
}

void	CQuestionManager::GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap, JBool OnlyQuanti)  const
{
	if(!BlocID)
	{
		// BlocID is zero so get all the question
		for(m_lQuestions.MoveFirst(); m_lQuestions.IsValid(); m_lQuestions.MoveNext())
		{
			const CQuestion * pQuestion = m_lQuestions.GetItem()->AsQuestion();
			if(pQuestion->HasModality(ModalityMap) || pQuestion->HasElement(ElementMap))
				List.AddTail() = pQuestion;
		}
	}
	else
	{
		m_mBlocID.MoveTo(BlocID);
		if(m_mBlocID.IsValid())
		{
			const JList<const  IQuestion *> & Temp = m_mBlocID.GetItem();
			for(Temp.MoveFirst(); Temp.IsValid(); Temp.MoveNext())
			{
				const CQuestion * pQuestion = Temp.GetItem()->AsQuestion();
				if(pQuestion->HasModality(ModalityMap) || pQuestion->HasElement(ElementMap))
					List.AddTail() = pQuestion;
			}
		}
	}
}

void	CQuestionManager::GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap, JBool OnlyQuanti)  const
{
	if(!BlocID)
	{
		// BlocID is zero so get all the question
		for(m_lQuestions.MoveFirst(); m_lQuestions.IsValid(); m_lQuestions.MoveNext())
		{
			const CQuestion * pQuestion = m_lQuestions.GetItem()->AsQuestion();
			if(Convert::ToSearchString(pQuestion->GetLabel().AsJCharPtr()).Find(sFilter) != -1) 
				List.AddTail() = pQuestion;
			else 
			{
				if(pQuestion->HasModality(ModalityMap) || pQuestion->HasElement(ElementMap))
					List.AddTail() = pQuestion;
			}
		}
	}
	else
	{
		m_mBlocID.MoveTo(BlocID);
		if(m_mBlocID.IsValid())
		{
			const JList<const  IQuestion *> & Temp = m_mBlocID.GetItem();
			for(Temp.MoveFirst(); Temp.IsValid(); Temp.MoveNext())
			{
				const CQuestion * pQuestion = Temp.GetItem()->AsQuestion();
				if(Convert::ToSearchString(pQuestion->GetLabel().AsJCharPtr()).Find(sFilter) != -1) 
					List.AddTail() = pQuestion;
				else 
				{
					if(pQuestion->HasModality(ModalityMap) || pQuestion->HasElement(ElementMap))
						List.AddTail() = pQuestion;
				}
			}
		}
	}
}


// Returns the question corresponding to the questio ID
CQuestion const *	CQuestionManager::GetQuestionByID(JInt32 QuestionID) const
{
	m_mQuestionID.MoveTo(QuestionID);
	if(m_mQuestionID.IsValid())
		return m_mQuestionID.GetItem();
	else
		return 0;
}

// Gets an Item vector from a given Item Native index
// return the vector & the Question ID
JInt32	CQuestionManager::GetTermesByItemNatif(JInt32 ItemNatif, JVector<JInt32> &Vector) const
{
	m_mItemNatif.MoveTo(ItemNatif);
	if(m_mItemNatif.IsValid())
	{
		const SKeyPair & kp = m_mItemNatif.GetItem();
		// Get the terme vector
		kp.m_pQuestion->GetTermeVector(Vector,kp.m_IndexItem);
		// Returns the Question ID
		return kp.m_pQuestion->GetID();
	}
	else
		return 0;
}

// return the Item Natif by the given question id and termes
JInt32	CQuestionManager::GetItemNatifByTermes(JInt32 QuestionID, JVector<JInt32> &Termes) const
{
	CQuestion const * pQuestion = GetQuestionByID(QuestionID);
	if(pQuestion)
		return pQuestion->GetItemNatifByTerme(Termes);
	else
		return -1;
}

// Gets the Numeric variable index for the given ItemNatif
// This must only be called if the question is a variable quanti
// Return -1 if not found
JBool	CQuestionManager::GetVariableIndexByItemNatif(JInt32 ItemNatif, JInt32 &Index) const
{
	m_mItemNatif.MoveTo(ItemNatif);
	if(m_mItemNatif.IsValid())
	{
		const SKeyPair & kp = m_mItemNatif.GetItem();
		Index = m_mItemNatif.GetItem().m_pQuestion->GetVariableIndexByIndex(kp.m_IndexItem);
		if(Index>=0)
			return true;
	}
	return false;
}

JInt32	CQuestionManager::GetVarAgrIdByItemNatif(JInt32 ItemNatif) const
{
	m_mItemNatif.MoveTo(ItemNatif);
	if(m_mItemNatif.IsValid())
		return m_mItemNatif.GetItem().m_pQuestion->GetVarAgregatId();

	return 0;
}



// Gets the Question ID for the given ItemNatif
// Return 0 if not found
JInt32	CQuestionManager::GetQuestionIDByItemNatif(JInt32 ItemNatif) const
{
	m_mItemNatif.MoveTo(ItemNatif);
	if(m_mItemNatif.IsValid())
		return m_mItemNatif.GetItem().m_pQuestion->GetID() ;
	else
		return 0;
}

// Returns the question corresponding to the ItemNatif
const CQuestion  *	CQuestionManager::GetQuestionByItemNatif(JInt32 ItemNatif) const
{
	m_mItemNatif.MoveTo(ItemNatif);
	if(m_mItemNatif.IsValid())
		return m_mItemNatif.GetItem().m_pQuestion ;
	else
		return 0;
}

JBool	CQuestionManager::HasQuestionsByBlocID(JInt32 BlocID) const
{
	if(!BlocID)
		return (m_lQuestions.GetCount() >0);
	else
	{
		m_mBlocID.MoveTo(BlocID);
		return m_mBlocID.IsValid();
	}
	return true;
}

JBool	CQuestionManager::HasQuestionsByBlocID(JInt32 BlocID, const CString & sFilter)  const
{
	if(!BlocID)
	{
		// BlocID is zero so get all the question
		for(m_lQuestions.MoveFirst(); m_lQuestions.IsValid(); m_lQuestions.MoveNext())
		{
			if(Convert::ToSearchString(m_lQuestions.GetItem()->GetLabel().AsJCharPtr()).Find(sFilter) != -1) 
				return true;
		}
	}
	else
	{
		m_mBlocID.MoveTo(BlocID);
		if(m_mBlocID.IsValid())
		{
			const JList<const  IQuestion *> & Temp = m_mBlocID.GetItem();
			for(Temp.MoveFirst(); Temp.IsValid(); Temp.MoveNext())
			{
				if(Convert::ToSearchString(Temp.GetItem()->GetLabel().AsJCharPtr()).Find(sFilter) != -1) 
					return true;
			}
		}
	}
	return false;
}

JBool	CQuestionManager::HasQuestionsByBlocID(JInt32 BlocID, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap)  const
{
	if(!BlocID)
	{
		// BlocID is zero so get all the question
		for(m_lQuestions.MoveFirst(); m_lQuestions.IsValid(); m_lQuestions.MoveNext())
		{
			const CQuestion * pQuestion = m_lQuestions.GetItem()->AsQuestion();
			if(pQuestion->HasModality(ModalityMap) || pQuestion->HasElement(ElementMap))
				return true;
		}
	}
	else
	{
		m_mBlocID.MoveTo(BlocID);
		if(m_mBlocID.IsValid())
		{
			const JList<const  IQuestion *> & Temp = m_mBlocID.GetItem();
			for(Temp.MoveFirst(); Temp.IsValid(); Temp.MoveNext())
			{
				const CQuestion * pQuestion = Temp.GetItem()->AsQuestion();
				if(pQuestion->HasModality(ModalityMap) || pQuestion->HasElement(ElementMap))
					return true;
			}
		}
	}
	return false;
}

JBool	CQuestionManager::HasQuestionsByBlocID(JInt32 BlocID, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap)  const
{
	if(!BlocID)
	{
		// BlocID is zero so get all the question
		for(m_lQuestions.MoveFirst(); m_lQuestions.IsValid(); m_lQuestions.MoveNext())
		{
			const CQuestion * pQuestion = m_lQuestions.GetItem()->AsQuestion();
			if(Convert::ToSearchString(pQuestion->GetLabel().AsJCharPtr()).Find(sFilter) != -1) 
				return true;
			else 
			{
				if(pQuestion->HasModality(ModalityMap) || pQuestion->HasElement(ElementMap))
					return true;
			}
		}
	}
	else
	{
		m_mBlocID.MoveTo(BlocID);
		if(m_mBlocID.IsValid())
		{
			const JList<const  IQuestion *> & Temp = m_mBlocID.GetItem();
			for(Temp.MoveFirst(); Temp.IsValid(); Temp.MoveNext())
			{
				const CQuestion * pQuestion = Temp.GetItem()->AsQuestion();
				if(Convert::ToSearchString(pQuestion->GetLabel().AsJCharPtr()).Find(sFilter) != -1) 
					return true;
				else 
				{
					if(pQuestion->HasModality(ModalityMap) || pQuestion->HasElement(ElementMap))
						return true;
				}
			}
		}
	}
	return false;
}
