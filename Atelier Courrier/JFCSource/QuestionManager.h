
#pragma once

#include "IQuestion.h"
#include "Question.h"
#include "Security.h"


/**
 * \ingroup JFCSOURCE
 * This Class handles all Questions within a Terrain.  Providing a certain number of
 * interfaces to access the questions.
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-09-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class CQuestionManager  
{
public:
	CQuestionManager();

	virtual ~CQuestionManager();

	/**
	 * This sets up the necessary data for this object and \n
	 * thus must be called before calling Recv
	 * \param pElements
	 * Pointer to a map of Elements 
	 * \param pModalites
	 * Pointer to a map of Modalites
	 * \param pSecurity 
	 * Pointer to a Security Object
	 */
	void SetData(IMODALITEMAP * pElements, IMODALITEMAP * pModalites, CSecurity * pSecurity)
		{ m_pElements = pElements; m_pModalites = pModalites; m_pSecurity = pSecurity;};

	/**
	 *
	 * \param lhs 
	 * This is the archive object.
	 * \param Version 
	 * This is the expected version of the archive.
	 * \return 
	 * The archive object or
	 * Throws an JInternalError exception
	 */
	JArchive&			Recv(JArchive& lhs, JInt32 Version);

	/**
	 *
	 * \param BlocID 
	 * Bloc ID of requested questions\n
	 * If this is 0 then all questions will be selected.
	 * \param &List 
	 * A reference to a list object where the Questions will be stored
	 * \return 
	 * True if questions were found
	 */
	void	GetQuestionsByBlocID(JInt32 BlocID, JList<const IQuestion *> &List,JBool OnlyQuanti = false) const;
	void	GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, const CString & sFilter, JBool OnlyQuanti = false)  const;
	void	GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap, JBool OnlyQuanti = false)  const;
	void	GetQuestionsByBlocID(JInt32 BlocID, JList<IQuestion const *> &List, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap, JBool OnlyQuanti = false)  const;
	JBool	HasQuestionsByBlocID(JInt32 BlocID) const;
	JBool	HasQuestionsByBlocID(JInt32 BlocID, const CString & sFilter)  const;
	JBool	HasQuestionsByBlocID(JInt32 BlocID, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap)  const;
	JBool	HasQuestionsByBlocID(JInt32 BlocID, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap)  const;

	/**
	 * Gets a question by it's ID
	 * \param QuestionID 
	 * ID of the question to be retrieved.
	 * \return 
	 A pointer to the question, otherwise NULL
	 */
	CQuestion const *	GetQuestionByID(JInt32 QuestionID) const;

	/**
	 *
	 * \param ItemNatif 
	 * Item Natif index.
	 * \param &Vector 
	 * Reference to a vector where the termes of the given item natif are to be stored.
	 * \return 
	 * The ID of the question indexed by the item natif
	 */
	JInt32				GetTermesByItemNatif(JInt32 ItemNatif, JVector<JInt32> &Vector) const; 

	/**
	 *
	 * \param QuestionID 
	 * Input : Question ID
	 * \param &Termes 
	 * Input : Reference to a vector of termes
	 * \return 
	 * The item natif index
	 */
	JInt32				GetItemNatifByTermes(JInt32 QuestionID, JVector<JInt32> &Termes) const;

	/**
	 * Gets the Numeric variable Number for the given ItemNatif
	 * This must only be called if the question is a variable quanti
	 *
	 * \param ItemNatif 
	 * Item Natif Index
	 * \return 
	 * Variable Number if found\n
	 * 0 if not found
	 */
	JInt32				GetVarAgrIdByItemNatif(JInt32 ItemNatif) const;

	/**
	 * Gets the aggregation variable Support Index for the given ItemNatif
	 * This must only be called if the question is an aggregate
	 * \param ItemNatif 
	 * Input : Item Natif index
	 * \return
	 * Variable Support Index
	 */
	JBool				GetVariableIndexByItemNatif(JInt32 ItemNatif, JInt32 &Index) const;

	/**
	 * Gets the Question ID for the given ItemNatif
	 * \param ItemNatif 
	 * Item Natif index
	 * \return 
	 * The Question ID\n
	 * -1 if not found
	 */
	JInt32				GetQuestionIDByItemNatif(JInt32 ItemNatif) const;

	/**
	 * Gets a question corresponding to the ItemNatif
	 * \param ItemNatif 
	 * Item natif index
	 * \return 
	 * Pointer to the requested Question\n
	 * NULL if not found
	 */
	const CQuestion  *	GetQuestionByItemNatif(JInt32 ItemNatif) const;

private:

	struct SKeyPair
	{
		void Set(const CQuestion * pQuestion, JInt32 Index) { m_pQuestion = pQuestion; m_IndexItem=Index; } ;
		const CQuestion *	m_pQuestion;
		JInt32	m_IndexItem;
	};
	/// Nombre de questions
	JInt32					m_Questions;
	/// List of Questions
	QUESTIONLIST			m_lQuestions;
	/// Map of Questions ID and Vector Index
	JMap<JInt32,const CQuestion *>	m_mQuestionID;
	/// Map of Bloc ID and List of IQuestion *
	JMap< JInt32, JList<const  IQuestion *> >	m_mBlocID;
	/// Items Native Map Question pointer / Item index
	JMap<JInt32,SKeyPair>	m_mItemNatif;

	IMODALITEMAP *		m_pElements;
	IMODALITEMAP *		m_pModalites;
	CSecurity *			m_pSecurity;
};

