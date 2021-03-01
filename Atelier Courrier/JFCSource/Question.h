
#pragma once


#pragma warning( disable : 4786 )

#include <memory>

#include "Element.h"
#include "Modalite.h"
#include "IQuestion.h"
#include "Security.h"

typedef JMatrix<JInt32> JINT32MATRIX;
typedef JVector<JInt32> JINT32VECTOR;

// #define NO_CONTINGENTS

/**
 * \ingroup JFCSOURCE EXTERNAL
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
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CQuestion : public IQuestion 
{
public:

	// Les diverses catégories de question
	enum CATEGORIE
	{		
			Qualitative_Simple		=	  1,	///< Qualitative Simple
			Qualitative_Multiple	=	  2,	///< Qualitative Multiple
			Quantitative_Pure		=	  3,	///< Quantitative Pure
			Quantitative_Assim		=	  4,	///< Quantitative Assimile
			LDP						=	  5,	///< LDP
			LNM						=	  6,	///< LNM
			Probabilite_LDP			=	  7,	///< Probabilities LDP
			Probabilite_LNM			=	  8,	///< Probabilities LNM
			Poids_QH				=	  9,	///< Poids Quart heure radio
			Probabilite_Expo		=	 10,	///< Probabilités d'exposition à un plan témoin
			Quanta_Freq				=	 11,	///< Quanta de frequentation base
			Quanta_Freq_D			= 11000,	///< Quanta de frequentation début
			Quanta_Freq_F			= 11999,	///< Quanta de frequentation fin
			Variable_Etalonage_D	= 12000,	///< Variables d'etalonage debut
			Variable_Etalonage_F	= 12999,	///< Variables d'etalonage fin
			Univers_Appariement_D	= 13000,	///< Univers d'appariement debut
			Univers_Appariement_F	= 13999,	///< Univers d'appariement fin
			Segments_Appariement_D	= 14000,	///< Segments d'appariement debut
			Segments_Appariement_F	= 14999,	///< Segments d'appariement fin
			Variables_Transfert_D	= 15000,	///< Variables de transfert debut
			Variables_Transfert_F	= 15999,	///< Variables de transfert fin
			Quanta_Freq_Brand		=	 16,	///< Quanta de frequentation pour les brands
			Quanta_Freq_Channel		=	 17,	///< Quanta de frequentation pour les channels
			Quanta_Freq_Rollup		=	 18,	///< Quanta de frequentation pour les rollups
	};

	// Les divers type de dimension
	enum TYPEDIMENSION
	{
			TypeStandard			=     1,	///< Dimension standard
			TypePresse				=     2,	///< Dimension lié à la presse	
	};

public:
	
	CQuestion(IMODALITEMAP * pElements, IMODALITEMAP * pModalites, CSecurity *pSecurity);

	virtual ~CQuestion();

	JArchive &	Recv(JArchive& lhs, JInt32 Version);

public:
	/// IQuestion interface overrides

	///  Is this object a question
	virtual JBool				IsQuestion	() const	{ return true; } ;
	///  Return this object as a const question *
	virtual const CQuestion *	AsQuestion	() const	{ return this; } ;
	///  Return this object as a question *
	virtual CQuestion *			AsQuestion	()			{ return this; } ;
	///  Get the ID
	virtual JInt32				GetID		() const { return m_ID; } ;
	///  Gets the bloc ID
	virtual JInt32				GetBlocID	() const { return m_lBlocID; } ;
	///  Get the Label
	virtual const JLabel&		GetLabel	() const { return m_Label; } ;
	///  Returns the strate mask for this question
	virtual JInt32				GetContingentMask()	const { return m_bContingent; } ;
	///	 Returns the Number of dimensions
	virtual JInt32				HasDimensions() const 
	{ 
		return GetKeySize();
	};

	/// Returns the dimension type
	virtual JInt32				DimensionType(JInt32 Dim=0)	const;

	/// Returns si question sera automatiquement moyennable (ssi toutes les dimensions de type 0)
	virtual JBool				AutoMoyennable() const;

	///  Fills the vector with a list of modalite objects
	virtual JBool				GetIModalites(JVector<const IModalite *> &Vector, JInt32 ModaliteID=0, JInt32 Dim=0, JUnt32 ContingentMask=0)	const ;
	///  gets the IModalite object for the given dimension and ID
	virtual const IModalite *	GetIModalite(JInt32 ModaliteID, JInt32 Dim)	const;
	///  Returns true if the given Modalité and Element ID Terme is valid
	virtual JBool				IsValidTerme(const JVector<JInt32> & IDTerme)	const ;
	///  Returns an item definition string for this object
	virtual JBool				ToItemString(JStringEx &String)	const ;
	///  Returns true is the modalities of this object can be reordered
	virtual JBool				CanModify() const { return false; } ;
	///  Returns true is the modalities of this object have been reordered to reflect the given ID Order
	virtual JBool				SetModaliteOrder(const JList<JInt32> & IDOrder) { return false; } ;
	///  Rename an Item returns false if name is invalid
	virtual JBool				RenameItem(JLabel &Label, JInt32 ItemID) { return false; } ;
	///	Returns true is the questions has any of the modalities in the given map
	virtual JBool				HasModality(const JMap<JInt32,bool> & ModalityMap) const;
	///	Returns true is the questions has any of the elements in the given map
	virtual JBool				HasElement(const JMap<JInt32,bool> & ElementMap) const;
	/**
	 * Check valid contingent
	 * \param ContingentMask The Contingent mask from the selected Population Segments
	 * \return True if Question is in contingent
	 */
#ifdef NO_CONTINGENTS
	virtual JBool				IsInContingent(JUnt32 ContingentMask) const { return true; } ;
#else
	virtual JBool				IsInContingent(JUnt32 ContingentMask) const { if(ContingentMask&m_bContingent) return true; else return false; } ;
#endif

public:

	// Agregat type variable
	JBool	IsAgregatDataType	() const;
	// Numeric type variable
	JBool	IsNumericDataType	() const;
	// Has Values
	JBool	HasValues			() const;
	// Is Probability
	JBool	IsProbability		() const;
	// Quanti Question
	JBool	IsQuantitative		() const; 
	// Quanti Pure Question
	JBool	IsQuantiPure		() const { return m_lCategorie == Quantitative_Pure; };
	// Quanti Assimilée Question
	JBool	IsQuantiAssim		() const { return m_lCategorie == Quantitative_Assim; };
	// Qualitatif Question
	JBool	IsQualitative		() const;
	// Qualitatif Simple Question
	JBool	IsQualiSimple		() const { return m_lCategorie == Qualitative_Simple; };
	// Qualitatif Multiple Question
	JBool	IsQualiMultiple		() const { return m_lCategorie == Qualitative_Multiple; };

	JInt32	GetCategorie		() const { return m_lCategorie; } ;
	JInt32	GetNbItems			() const { return (m_lNbTermes/GetKeySize()); } ;
	JInt32	GetKeySize			() const { return (m_DimType.GetCount()); } ;
	/// Returns the the vector of the given terme index
	JBool	GetTermeVector		(JVector<JInt32> &Terme, JInt32 Index) const;
	/// Returns the Item Natif for a given terme
	virtual JInt32	GetItemNatifByTerme(const JVector<JInt32> & Terme)	const;
	/// Gets the Numeric/Agregat variable Index for the given terme
	JInt32	GetVariableIndexByTerme		(const JVector<JInt32> & Terme) const ;
	/// Gets the Numeric/Agregat variable Index for the given terme index
	JInt32	GetVariableIndexByIndex(JInt32 Index) const ;
	/// Gets the variable agregat Id
	JInt32	GetVarAgregatId() const ;
	/// Gets the Item Natif Start Position for this Question
	JInt32	GetItemStart() const { return m_ItemStart; } ;
	/// Returns true is this is a valid term index
	JBool	IsValidItem(JInt32 Index) const ;
	/// Returns true if there is at least one valid item
	JBool	IsValidQuestion() const;

protected:

	/// Verifies the Modalite and Element ID's
	virtual JBool	VerifyKeys();
	/// Returns the Row index for the given terme (-1) if the terme was not found
	virtual JInt32	GetTermeIndex(const JVector<JInt32> & Terme)	const;
	/// Returns a support ID for a given row
	virtual JInt32	GetSupportID(JInt32 RowIndex) const;

private:

	JInt32				m_ItemStart; ///< Item Natif Start
	JInt32				m_ID;
	JInt32				m_lBlocID;
	JInt32				m_bContingent;
	JInt32				m_bEchantillon;
	JInt32				m_lCategorie;
	JLabel				m_Label;

//	JInt32				m_lNbDimModalites;
//	JINT32VECTOR		m_DimModalitiesId;
//	JInt32				m_lNbDimElements;
//	JINT32VECTOR		m_DimElementsId;

	JList<JInt32>				m_DimIds;
	JList<IQuestion::DIMTYPE>	m_DimType;
	JList<JINT32VECTOR>			m_DimModaElemIds;

	JInt32				m_lNbTermes;

	JINT32MATRIX		m_xItemKey;
	JInt32				m_ColItemNatif;
	JInt32				m_ColVarNum;

	IMODALITEMAP	const *	m_pElements;
	IMODALITEMAP	const *	m_pModalites;
	CSecurity		const *	m_pSecurity;
//	JMap<JInt32,IQuestion::DIMTYPE> m_DimensionType;

private:

	/// assignment operator
	CQuestion & operator =(const CQuestion & rhs);
	/// copy constructor
	CQuestion(const CQuestion & rhs);

};

typedef std::auto_ptr<CQuestion> PTR_QUESTION;
typedef JVector<PTR_QUESTION> QUESTIONVECTOR;
typedef JList<PTR_QUESTION> QUESTIONLIST;

