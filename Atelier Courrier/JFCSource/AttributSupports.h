#pragma once

#include "FileHeader.h"
#include "AttributSupport.h"

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
class CAttributSupports
{
public:
	CAttributSupports(void);
	virtual ~CAttributSupports(void);
	/// Returns the AttributSupport
	const CAttributSupport * GetAttributSupportByIndex(JInt32 IIndex) const;
	/// Returns the AttributSupport
	const CAttributSupport * GetAttributSupportByIds(const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds) const;
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
	/// Nombre de dimensions
	JInt32					m_lNbDimensions;
	/// Dimension Ids
	JMap<JInt32,JInt32>		m_mDimensionID;
	/// a Map of CAttributSupport objects
	ATTRIBUTSUPPORTMAP		m_mAttribSupport;
	/// list of CAttributSupport auto pointers
	ATTRIBUTSUPPORTLIST		m_lAttribSupport;

private:

	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	/// Load the AttributSupports
	JVoid LoadAttributSupports	(JArchive *pFile, JInt32 Version);

	/// assignment operator
	CAttributSupports & operator =(const CAttributSupports & rhs);
	/// copy constructor
	CAttributSupports(const CAttributSupports & rhs);

};
