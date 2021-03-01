
#pragma once

#pragma warning( disable : 4786 )

#include <memory>

typedef JVector<JInt32> JINT32VECTOR;

/**
 * \ingroup JFCSOURCE
 * This Class is the Facteur de valorisation of a Support.
 * \sa CFacteursSupports CTerrain
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
class CValSupport  
{

public:

	CValSupport();
	virtual ~CValSupport();

	/**
	 * Gets the Support ID of this object
	 * \return The Support ID
	 */
	JInt32	GetID() const { return m_lSupportID; }
	
	/**
	 * Loads the Facteur de valorisation of a Support
	 * \param lhs is the Archive reference
	 * \param Version is the file format version
	 * \return the same JArchive object
	 */
	JArchive& Recv( JArchive& lhs, JInt32 Version);

	JVoid GetRepartionStruct(JInt32 & Duree, JVector<JInt32>& vect, JInt32 & diviseur);

private:
	/// Support ID
	JInt32				m_lSupportID;
	/// Diviseur du taux Moyen
	JInt32				m_lDiviseurTM;
	/// Taux Moyen de reprise en main
	JInt32				m_lRepriseMainTM;
	/// Diviseur de la structure de répartition
	JInt32				m_lDiviseurRepart;
	/// Durée de vie
	JInt32				m_lDureeVie;
	/// Vector of the structure de repartition
	JINT32VECTOR		m_vValues;

private:

	/// assignment operator
	CValSupport & operator =(const CValSupport & rhs);
	/// copy constructor
	CValSupport(const CValSupport & rhs);
};

/// Auto pointer for CValSupport objects
typedef std::auto_ptr<CValSupport> PTR_VALSUPPORT;
/// Map of PTR_VALSUPPORT auto pointer objects
typedef JMap<JInt32, PTR_VALSUPPORT> VALSUPPORTMAP;
