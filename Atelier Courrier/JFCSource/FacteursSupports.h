
#pragma once

#pragma warning( disable : 4786 )

#include "FileHeader.h"
#include "ValSupport.h"

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
class CFacteursSupports  
{
public:

	CFacteursSupports();
	virtual ~CFacteursSupports();

	/// Load the facteurs de valorisation fixes des supports
	JBool Load(JStringEx &Name);

	JBool GetRepartionStruct(JInt32 SupportId, JInt32 & Duree, JVector<JInt32>& vect, JInt32 & diviseur);


private:

	/// File Name
	JStringEx			m_sName;
	/// Source ID
	JInt32				m_lSourceID;
	/// Terrain ID
	JInt32				m_lTerrainID;
	/// File Header
	CFileHeader			m_FileHeader;
	/// set of support values
	VALSUPPORTMAP		m_mValSupports;

private:

	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	/// Load the index thematique
	JVoid LoadValSupports	(JArchive *pFile, JInt32 Version);

	/// assignment operator
	CFacteursSupports & operator =(const CFacteursSupports & rhs);
	/// copy constructor
	CFacteursSupports(const CFacteursSupports & rhs);

};

