#pragma once

#include "FileHeader.h"
#include "Segment.h"

/**
 * \ingroup JFCSOURCE
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
class CSegments
{
public:
	CSegments(void);
	virtual ~CSegments(void);
	/// Gets the default population segment and contingent mask
	JInt32 GetDefaultPopSegment(CBitArray & PopSegment, JUnt32 & ContingentMask, bool AllSegment);
	/// Returns the segments
	JBool GetSegments(JVector<CSegment *> & Segments);
	/// Load the Relations d'equivalence
	JBool Load(JStringEx &Name);

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
	/// Vector of segments
	SEGMENTVECTOR			m_vSegments;

private:

	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	/// Load the Segments
	JVoid LoadSegments			(JArchive *pFile, JInt32 Version);

	/// assignment operator
	CSegments & operator =(const CSegments & rhs);
	/// copy constructor
	CSegments(const CSegments & rhs);

};
