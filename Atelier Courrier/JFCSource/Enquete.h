
#pragma once

#pragma warning( disable : 4786 )

#include "IndexThematique.h"
#include "Element.h"
#include "QuestionManager.h"
#include "Modalite.h"
#include "FileHeader.h"
#include "Security.h"


class CTerrain;

/**
 * \ingroup JFCSOURCE
 *
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
class CEnquete  
{

public:

	CEnquete();
	virtual ~CEnquete();

	/// Load the enquete
	JBool				Load					(JStringEx &Name, CSecurity * pSecurity);

	/// Fills a CTreeCtrl with the index thematique tree structure
	JBool				FillIndexThematique		(CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM hParent, JBool AllLeaves = false, JBool OnlyQuanti = false);

	/// Gets the list of questions with BlocID
	/// If blocID = 0 then all questions are taken
	void				GetQuestionsByBlocID	(JInt32 BlocID, JList<IQuestion const *> &List, JBool OnlyQuanti = false) const;
	void				GetQuestionsByBlocID	(JInt32 BlocID, JList<IQuestion const *> &List, const CString & sFilter, JBool OnlyQuanti = false)  const;
	void				GetQuestionsByBlocID	(JInt32 BlocID, JList<IQuestion const *> &List, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap, JBool OnlyQuanti = false)  const;
	void				GetQuestionsByBlocID	(JInt32 BlocID, JList<IQuestion const *> &List, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap, JBool OnlyQuanti = false)  const;
	JBool				HasQuestionsByBlocID	(JInt32 BlocID) const;
	JBool				HasQuestionsByBlocID	(JInt32 BlocID, const CString & sFilter)  const;
	JBool				HasQuestionsByBlocID	(JInt32 BlocID, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap)  const;
	JBool				HasQuestionsByBlocID	(JInt32 BlocID, const CString & sFilter, const JMap<JInt32,bool> & ElementMap, const JMap<JInt32,bool> & ModalityMap)  const;
	bool				FindElements			(JMap<JInt32,bool> & ElementMap, const CString & sFilter);
	bool				FindModalities			(JMap<JInt32,bool> & ModalityMap, const CString & sFilter);
	/// Returns the question corresponding to the questio ID
	CQuestion const *	GetQuestionByID(JInt32 QuestionID) const;
	/// Gets the Termes of the given item native into the Vector
	/// and returns the Question ID
	JInt32				GetTermesByItemNatif(JInt32 ItemNatif, JVector<JInt32> &Vector) const
							{ return m_Questions.GetTermesByItemNatif(ItemNatif, Vector); } ;
	/// Get the Item Natif by the given question id and termes
	JInt32				GetItemNatifByTermes(JInt32 QuestionID, JVector<JInt32> &Termes) const;
	/// Gets the Numeric variable Number for the given ItemNatif
	/// This must only be called if the question is a variable quanti
	/// Return false if not found
	JInt32				GetVarAgrIdByItemNatif(JInt32 ItemNatif) const;
	/// Gets the Aggregat variable Index for the given ItemNatif
	/// This must only be called if the question is a variable quanti
	JBool				GetVarIdxByItemNatif(JInt32 ItemNatif, JInt32 & Index) const;
	/// Gets the Question ID for the given ItemNatif
	/// Return 0 if not found
	JInt32				GetQuestionIDByItemNatif(JInt32 ItemNatif) const;
	/// Returns the question corresponding to the ItemNatif
	const CQuestion  *	GetQuestionByItemNatif(JInt32 ItemNatif) const;
	///Teste si un element existe
	JBool				HasElement(JInt32 elemID) const;
	///Get a pointeer to the element
	const CElement *	GetElement(JInt32 elemID) const;

	///Teste si un modalite existe
	JBool				HasModalite(JInt32 modaID) const;
	// Get a pointer to the modalite
	const CModalite *	GetModalite(JInt32 modaID) const;
	///Teste si un support existe dans le terrain
	JBool				HasSupport(JInt32 elemID) const;
	/// Returns the source ID
	JInt32				GetSourceID() const { return m_lSourceID; } ;
	/// Returns the terrain ID
	JInt32				GetTerrainID() const { return m_lTerrainID; } ;

private:

	/// File Name
	JStringEx			m_sName;
	/// Source ID
	JInt32				m_lSourceID;
	/// Terrain ID
	JInt32				m_lTerrainID;
	/// File Header
	CFileHeader			m_FileHeader;
	/// Index thematique
	CIndexThematique *	m_pIndexThematique;
	/// Set of Elements
	IMODALITEMAP			m_mElements;
	/// Set of Modalités
	IMODALITEMAP			m_mModalites;
	/// Question manager
	CQuestionManager	m_Questions;

private:

	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	/// Load the index thematique
	JVoid LoadIndexThematique	(JArchive *pFile, JInt32 Version);
	/// Load the elements
	JVoid LoadElements			(JArchive *pFile, JInt32 Version);
	/// Load the modalités
	JVoid LoadModalites			(JArchive *pFile, JInt32 Version);
	/// Load the questions
	JVoid LoadQuestions			(JArchive *pFile, JInt32 Version, CSecurity * pSecurity);

	/// assignment operator not implemented
	CEnquete & operator =(const CEnquete & rhs);
	/// copy constructor not implemented
	CEnquete(const CEnquete & rhs);

};

