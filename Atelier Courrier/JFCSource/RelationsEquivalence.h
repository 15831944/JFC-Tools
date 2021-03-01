
#pragma once

#pragma warning( disable : 4786 )


#include "FileHeader.h"

typedef JMap<JInt32, JLabel> RELATIONMAP;

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
class CRelationsEquivalence  
{

public:

	CRelationsEquivalence();
	virtual ~CRelationsEquivalence();

	/// Load the Relations d'equivalence
	JBool Load(JStringEx &Name);

	/// Returns true if the given repere has a relation equivalence
	JBool					HasEquivalence(JInt32 Repere) const;
	/// Returns the relation equivalence for the given repere
	const JChar *			GetEquivalence(JInt32 Repere) const;

private:

	/// File Name
	JStringEx			m_sName;
	/// Source ID
	JInt32				m_lSourceID;
	/// Terrain ID
	JInt32				m_lTerrainID;
	/// File Header
	CFileHeader			m_FileHeader;
	/// Set of Elements
	RELATIONMAP			m_mRelations;

private:

	/// Load the file header
	JVoid LoadFileHeader(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader	(JArchive *pFile, JInt32 Version);
	/// Load the index thematique
	JVoid LoadRelations	(JArchive *pFile, JInt32 Version);

	/// assignment operator
	CRelationsEquivalence & operator =(const CRelationsEquivalence & rhs);
	/// copy constructor
	CRelationsEquivalence(const CRelationsEquivalence & rhs);

};

