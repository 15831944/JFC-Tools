#pragma once

#include <memory>
#include "BitArray.h"

typedef JVector<JInt32> JINT32VECTOR;

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
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CFiltreAudience
{
public:
	CFiltreAudience(void);
	virtual ~CFiltreAudience(void);

	JArchive& Recv( JArchive& lhs, JInt32 Version, JInt32 NbIndividuals, JInt32 NbDimPrim);

	/// Returns the Support ID
	const JINT32VECTOR &		GetSupportID() const { return m_vIdSupport; } ;
	/// Returns the filter ID
	JInt32				GetFiltreID() const { return m_IdFilter; } ;
	/// Returns the Filter
	const CBitArray *	GetFiltre() const { return &m_Filter; } ;

private:

	/// Support ID
	JINT32VECTOR		m_vIdSupport;
	/// Filter ID
	JInt32				m_IdFilter;
	/// Audience Filter
	CBitArray			m_Filter;

private:

	/// assignment operator
	CFiltreAudience & operator =(const CFiltreAudience & rhs);
	/// copy constructor
	CFiltreAudience(const CFiltreAudience & rhs);
};

typedef std::auto_ptr<CFiltreAudience> PTR_FILTREAUDIENCE;
typedef JVector<PTR_FILTREAUDIENCE> FILTREAUDIENCEVECTOR;
typedef JMap<JInt32, CFiltreAudience *> FILTREAUDIENCEMAP;

