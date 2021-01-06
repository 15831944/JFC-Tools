
#pragma once

#pragma warning( disable : 4786 )

#include <memory>

/// A Vector of JInt32
typedef JVector<JInt32> JINT32VECTOR;

/**
 * \ingroup JFCSOURCE
 * This class holds the Type poids values
 * \sa CPoids CEchantillon CTerrain 
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
class CTypePoids  
{
public:

	CTypePoids();
	virtual ~CTypePoids();

	/**
	 * Gets the Type ID
	 * \return the Type ID
	 */
	JInt32	GetID() const { return m_ID; }
	/**
	 * Get the Divider value
	 * \return the divider value
	 */
	JInt32	GetDivider() const { return m_lDiviseur; }
	
	/**
	 * Loads this object from a JArchive
	 * \param lhs reference to the JArchive object
	 * \param Version file format version
	 * \return the same JArchive reference
	 */
	JArchive& Recv(JArchive& lhs, JInt32 Version);

private:

	JInt32			m_ID;///< Type poids ID
	JInt32			m_lDiviseur;///< The divider

private:

	/// assignment operator
	CTypePoids & operator =(const CTypePoids & rhs);
	/// copy constructor
	CTypePoids(const CTypePoids & rhs);

};

/// A Vector of CTypePoids objects
typedef JVector<CTypePoids> TYPEPOIDSVECTOR;

