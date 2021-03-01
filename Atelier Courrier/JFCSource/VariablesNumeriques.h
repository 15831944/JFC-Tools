
#pragma once

#pragma warning( disable : 4786 )

#include "FileHeader.h"
#include "VariableNum.h"

/**
 * \ingroup JFCSOURCE
 * This class manages all numerical variables
 * \sa CVariableNum CTerrain
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
class CVariablesNumeriques  
{

public:

	CVariablesNumeriques();
	virtual ~CVariablesNumeriques();

	/// Load the Numerical Variables
	JBool Load(JStringEx &Name);

	/**
	 * Gets the calculated variables values
	 * \param Index is the Variable index
	 * \return a pointer to the calculated values vector
	 */
	const JFLT32VECTOR *	GetNumValues(JInt32 Index);
	/**
	 * Gets the variables divider
	 * \param Index is the Variable index
	 * \return the variables divider
	 */
	JInt32					GetDivider(JInt32 Index);

private:

	/// File Name
	JStringEx				m_sName;
	/// Source ID
	JInt32					m_lSourceID;
	/// Terrain ID
	JInt32					m_lTerrainID;
	/// File Header
	CFileHeader				m_FileHeader;
	/// Nombre d'individus
	JInt32					m_lNbIndividus;
	/// vector of variables
	VARIABLENUMVECTOR		m_vVariables;

private:

	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	/// Load the Variables
	JVoid LoadVariables			(JArchive *pFile, JInt32 Version);

	/// assignment operator
	CVariablesNumeriques & operator =(const CVariablesNumeriques & rhs);
	/// copy constructor
	CVariablesNumeriques(const CVariablesNumeriques & rhs);
};

