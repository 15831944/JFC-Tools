#pragma once

#include <memory>
#include "BitArray.h"

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
class AFX_EXT_CLASS CSegment
{
public:
	CSegment(void);
	virtual ~CSegment(void);
	JArchive &				Recv( JArchive& lhs, JInt32 Version, JInt32 NbIndividuals);
	const CBitArray &		GetPopSegment() { return m_Individus; } ;
	JInt32					GetID() const { return m_ID; } ;
	JUnt32					GetContingentMask() const  { return m_xMask; } ;

private:

	/// Segment ID
	JInt32				m_ID;
	/// Contingent Mask
	JUnt32				m_xMask;
	/// Vector of individuals for this segment
	CBitArray			m_Individus;

private:

	/// assignment operator
	CSegment & operator =(const CSegment & rhs);
	/// copy constructor
	CSegment(const CSegment & rhs);
};

typedef std::auto_ptr<CSegment> PTR_SEGMENT;
typedef JVector<PTR_SEGMENT> SEGMENTVECTOR;
