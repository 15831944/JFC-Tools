
#pragma once

#pragma warning( disable : 4786 )

#include "VarAgregation.h"

/**
 * \ingroup JFCSOURCE
 * This Class manages all Agregated Variables
 * \sa CVarAgregation CVarAgrSupport CTerrain
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
class CVarAgregationManager  
{
public:

	CVarAgregationManager(JStringEx &Name);
	virtual ~CVarAgregationManager();

	/**
	 * Gets the calculated values for a given variable and Support
	 * \param VarId is the Variable index
	 * \param SupportID is the Support ID
	 * \return a pointer to the calculated values vector
	 */
	JBool					HasVarBinValues(JInt32 VarId);	
	JInt32					GetVarAgregatDivider(JInt32 VarId);
	const JFLT32VECTOR *	GetVarNumValuesByIdx(JInt32 VarId, JInt32 Index);
	const CBitArray *		GetVarBinValuesByIdx(JInt32 VarId, JInt32 Index);
	const JFLT32VECTOR *	GetVarNumValuesByIds(JInt32 VarId, const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds);
	const CBitArray *		GetVarBinValuesByIds(JInt32 VarId, const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds);

private:

	JBool				GetVariable(JInt32 VarId);
	const JStringEx		m_sName; ///< File Name
	VARAGREGATIONMAP	m_mVariables;///< Map of CVarAgregation objects

private:
	/// assignment operator
	CVarAgregationManager & operator =(const CVarAgregationManager & rhs);
	/// copy constructor
	CVarAgregationManager(const CVarAgregationManager & rhs);
};
