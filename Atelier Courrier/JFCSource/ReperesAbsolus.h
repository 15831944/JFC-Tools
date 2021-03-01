
#pragma once

#pragma warning( disable : 4786 )


#include "FileHeader.h"
#include "Security.h"
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
class CReperesAbsolus  
{
public:

	CReperesAbsolus();
	virtual ~CReperesAbsolus();

	/// Load the enquete
	JBool	Load(JStringEx &Name, CSecurity *pSecurity);
	/// Gets a repere absolut from a given Item Natif
	JInt32	GetRepere(JInt32 ItemNatif);
	/// Gets an Item Natif from a given Repere Absolut
	JInt32	GetItemNatif(JInt32 Repere);

private:

	/// File Name
	JStringEx				m_sName;
	/// Source ID
	JInt32					m_lSourceID;
	/// Terrain ID
	JInt32					m_lTerrainID;
	/// File Header
	CFileHeader				m_FileHeader;
	/// Map of Items & reperes
	JMap<JInt32, JInt32>	m_mItems;
	JMap<JInt32, JInt32>	m_mReperes;

	CSecurity *				m_pSecurity;

private:
	/// Load the file header
	JVoid LoadFileHeader(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader	(JArchive *pFile, JInt32 Version);
	/// Load the Repères
	JVoid LoadReperes	(JArchive *pFile, JInt32 Version);

	/// assignment operator
	CReperesAbsolus & operator =(const CReperesAbsolus & rhs);
	/// copy constructor
	CReperesAbsolus(const CReperesAbsolus & rhs);

};

