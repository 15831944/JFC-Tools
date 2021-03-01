
#pragma once

#pragma warning( disable : 4786 )

#include <memory>

#include "BitArray.h"

typedef JVector<JInt32> JINT32VECTOR;
typedef JVector<JFlt32> JFLT32VECTOR;

/**
 * \ingroup JFCSOURCE
 * This Class Handles Support Aggregated Variables
 * \sa CVarAgregation CVarAgregationManager CTerrain
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
class CDimSupportKey
{
public:
	CDimSupportKey();
	virtual ~CDimSupportKey(void);
	CDimSupportKey(const CDimSupportKey &rhs);
	CDimSupportKey(const JINT32VECTOR &rhs);
	CDimSupportKey & operator=(const CDimSupportKey &rhs);
	CDimSupportKey & operator=(const JINT32VECTOR &rhs);
	bool operator < (const CDimSupportKey &rhs) const;
	bool operator > (const CDimSupportKey &rhs) const;
	JINT32VECTOR	m_Ids;
};


class CVarAgrSupport  
{
public:

	CVarAgrSupport(JInt32 Nature);
	virtual ~CVarAgrSupport();

	/**
	 * Loads the Support Aggregated variable from the JArchive
	 * \param lhs is a JArchive Reference
	 * \param Version is the format version of the Archive
	 * \param NbIndividuals is the Number of individuals
	 * \return a Reference to the same JArchive object
	 */
	JArchive&	Recv( JArchive& lhs, JInt32 Version, JInt32 NbIndividuals, JInt32 NbDimensions);
	/**
	 * Gets the Dimension Support ID
	 * \return the Dimension Support ID 
	 */
	const JVector<JInt32> & GetIDs() const { return m_Ids; }

	/**
	 * Calculates and retrieves the values
	 * \param Diviseur the divider to calculate the values
	 * \return a pointer to the values vector
	 */
	const JFLT32VECTOR *	GetNumValues(JInt32 Diviseur);
	const JINT32VECTOR *	GetSupportIds() { return & m_Ids; } ;
	JInt32					GetUniverseId() { return m_IdUniverse; } ;
	const CBitArray *		GetBinValues();

private:

	JInt32				m_IdUniverse; ///< Id of the Universe de redressement
	JInt32				m_Nature; ///< Nature of the support
	JINT32VECTOR		m_Ids; ///< Support IDs
	JINT32VECTOR		m_vIndividus; ///< Support IDs
	JInt32				m_lNbIndivUtil;///< Number of usefull individuals
	JInt32				m_NbIndividus;///< Number of individuals total
	CBitArray			m_Individus;///< The individual array
	JINT32VECTOR		m_vValues; ///< The uncalculated values vector 
	JFLT32VECTOR		m_vFValues;///< The calculated values vector

private:

	/// assignment operator
	CVarAgrSupport & operator =(const CVarAgrSupport & rhs);
	/// copy constructor
	CVarAgrSupport(const CVarAgrSupport & rhs);

};

/// auto pointer to a CVarAgrSupport object
typedef std::auto_ptr<CVarAgrSupport> PTR_VARAGRSUPPORT;
/// Map of CVarAgrSupport pointers
typedef JMap<CDimSupportKey, CVarAgrSupport *> VARAGRSUPPORTMAP;
/// List of PTR_VARAGRSUPPORT auto pointers
typedef JList<PTR_VARAGRSUPPORT> VARAGRSUPPORTLIST;