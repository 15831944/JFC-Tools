
#pragma once

#include "ItemsNatif.h"

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
class CItemsNatifManager  
{
public:
	CItemsNatifManager();
	virtual ~CItemsNatifManager();

	/// Load the enquete
	JBool Load(JStringEx &Name);

	const CBitArray * GetVectorIndividus(JInt32 IndexItem);

private:

	/// File Name
	JStringEx			m_sName;
	/// Source ID
	JInt32				m_lSourceID;
	/// Terrain ID
	JInt32				m_lTerrainID;
	/// File Header
	CFileHeader			m_FileHeader;
	/// Nombre d'individus
	JInt32				m_lNbIndividus;
	/// Nombre total des items natifs
	JInt32				m_lNbItemsTot;
	/// Nombre d'items natif par fichier
	JInt32				m_lDecoupage;
	/// Set of Elements
	ITEMSNATIFMAP		m_mItemsNatif;

private:

	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	/// Load the index thematique
	JVoid LoadDef				(JArchive *pFile, JInt32 Version);

	/// assignment operator
	CItemsNatifManager & operator =(const CItemsNatifManager & rhs);
	/// copy constructor
	CItemsNatifManager(const CItemsNatifManager & rhs);
};

