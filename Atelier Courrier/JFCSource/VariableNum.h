
#pragma once

#pragma warning( disable : 4786 )

#include <memory>

#include "BitArray.h"

typedef JVector<JInt32> JINT32VECTOR;
typedef JVector<JFlt32> JFLT32VECTOR;

/**
 * \ingroup JFCSOURCE
 * This Class Handles the Numerical Variables.
 * \sa CVariablesNumeriques CTerrain
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
class CVariableNum  
{
public:

	CVariableNum();
	virtual ~CVariableNum();

	/**
	 * Loads the Numerical Variable from the given JArchive
	 * \param lhs is a JArchive Reference
	 * \param Version is the format version of the Archive
	 * \param NbIndividuals is the Number of individuals
	 * \return a Reference to the same JArchive object
	 */
	JArchive &				Recv( JArchive& lhs, JInt32 Version, JInt32 NbIndividuals);
	/**
	 * Get the calculated Numerical values 
	 * \return a pointer to the calculated values
	 */
	const JFLT32VECTOR *	GetNumValues();
	/**
	 * Get the Numerical Variables Divider
	 * \return the Divider
	 */
	JInt32					GetDivider() { return m_lDiviseur; } ;
private:

	/// Number of usefull individus
	JInt32				m_lNbIndivUtil;
	/// The divider for the values
	JInt32				m_lDiviseur;
	/// Vector if individus répondants
	CBitArray			m_Repondants;
	/// Vector if individus touché
	CBitArray			m_Individus;
	/// Vector of values
	JINT32VECTOR		m_vValues;
	/// Vector of calculated values
	JFLT32VECTOR		m_vFValues;

private:

	/// assignment operator
	CVariableNum & operator =(const CVariableNum & rhs);
	/// copy constructor
	CVariableNum(const CVariableNum & rhs);
};

/// auto pointer to a CVariableNum object
typedef std::auto_ptr<CVariableNum> PTR_VARIABLENUM;
/// Vector of  PTR_VARIABLENUM auto pointer objects
typedef JVector<PTR_VARIABLENUM> VARIABLENUMVECTOR;
