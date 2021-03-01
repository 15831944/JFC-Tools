
#pragma once

#pragma warning( disable : 4786 )

#include "VarAgrSupport.h"
#include "FileHeader.h"

#include <memory>

/**
 * \ingroup JFCSOURCE
 * This class manages the supports of an Aggregated variable
 * \sa CVarAgrSupport CVarAgregationManager CTerrain
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
class CVarAgregation  
{

public:

	CVarAgregation();
	virtual ~CVarAgregation();

	/**
	 * Load the Supports for the given variable
	 * \param &Name is the Terrain Name
	 * \param NumVariable is the variable number
	 * \return True if loaded
	 */
	JBool Load(const JStringEx &Name, JInt32 NumVariable);

	JInt32 GetDivider() { return m_lDiviseur; } ;

	/**
	 * Gets the calculated values for a given Support Index
	 * \param Index is the Support Index
	 * \return a pointer to the calculated values vector
	 */	
	const JFLT32VECTOR *	GetNumValuesByIdx(JInt32 Index);
	const CBitArray *		GetBinValuesByIdx(JInt32 Index);
	/**
	 * Gets the calculated values for the given Dimension Ids and Support Ids
	 * \param  DimensionIds is the vector JFC Support Dimension Ids
	 * \param  SupportIds is the vector of Support Ids for each dimension
	 * \return a pointer to the calculated values vector
	 */	
	const JFLT32VECTOR *	GetNumValuesByIds(const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds);
	const CBitArray *		GetBinValuesByIds(const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds);

	JBool					HasBinValues() { return m_Nature==3; } ;

private:

	/// NAture de la support
	JInt32				m_Nature;
	/// Variable Number
	JInt32				m_lVarNum;
	/// File Name
	JStringEx			m_sName;
	/// Source ID
	JInt32				m_lSourceID;
	/// Terrain ID
	JInt32				m_lTerrainID;
	/// File Header
	CFileHeader			m_FileHeader;
	/// Nombre d'individus
	JInt32				m_lNbIndividus;
	/// Nombre de dimensions
	JInt32				m_lNbDimensions;
	/// Dimension Ids
	JMap<JInt32,JInt32>	m_mDimensionID;
	/// Divider
	JInt32				m_lDiviseur;
	/// a Map of CVarAgrSupport objects
	VARAGRSUPPORTMAP	m_mVarSupport;
	/// list of CVarAgrSupport auto pointers
	VARAGRSUPPORTLIST	m_lVarSupport;

private:
	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	/// Load the index thematique
	JVoid LoadVariable			(JArchive *pFile, JInt32 Version);
	/// assignment operator
	CVarAgregation & operator =(const CVarAgregation & rhs);
	/// copy constructor
	CVarAgregation(const CVarAgregation & rhs);
};

/// Auto Pointer to a CVarAgregation Object
typedef std::auto_ptr<CVarAgregation> PTR_VARAGREGATION;
/// Map of PTR_VARAGREGATION auto pointers
typedef JMap<JInt32, PTR_VARAGREGATION> VARAGREGATIONMAP;
