
#pragma once


#pragma warning( disable : 4786 )

#include <memory>

#include "FileHeader.h"
#include "BitArray.h"


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
class CItemsNatif  
{
public:

	CItemsNatif();
	virtual ~CItemsNatif();

	/// Load the enquete
	JBool Load(JStringEx &Name, JInt32 NbIndividus, JInt32 Bloc);
	const CBitArray * GetVectorIndividus(JInt32 Index) const;

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
	BITARRAYVECTOR		m_vItems;

private:

	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	/// Load the index thematique
	JVoid LoadItems				(JArchive *pFile, JInt32 Version, JInt32 NbIndividus);

	/// assignment operator
	CItemsNatif & operator =(const CItemsNatif & rhs);
	/// copy constructor
	CItemsNatif(const CItemsNatif & rhs);

};

typedef std::auto_ptr<CItemsNatif> PTR_ITEMSNATIF;
typedef JMap<JInt32, PTR_ITEMSNATIF> ITEMSNATIFMAP;

