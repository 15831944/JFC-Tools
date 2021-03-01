#pragma once

#include "BitArray.h"
#include "FileHeader.h"


/**
 * \ingroup JFCSOURCE
 * This class loads the security file
 * Blocking Questions, Elements, Modalites & Reperes Absolut
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
class CSecuritySupports
{
public:
	CSecuritySupports(void);
	virtual ~CSecuritySupports(void);
	/**
	 * Loads the SecuritySupports file
	 * \param &Name 
	 * Terrain Name
	 * \return 
	 * True if the file was loaded\n
	 * False if not loaded
	 */
	JBool	Load(JStringEx &Name);
	/**
	 *
	 * \param ItemNatif 
	 * Item Natif
	 * \return 
	 * True if the Item Natif is valid
	 */
	bool	IsSupportValid(JInt32 idSupport) const;
	
private:
	///  Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	///  Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	///  Loads the data
	JVoid LoadData				(JArchive *pFile, JInt32 Version);

private:
	/// File Name
	JStringEx			m_sName;
	/// File Header
	CFileHeader			m_FileHeader;
	/// Source ID
	JInt32				m_lSourceID;
	/// Terrain ID
	JInt32				m_lTerrainID;
	///  Number of supports
	JInt32				m_NbSupportForbiden;
	///  Map of forbiden supports
	JMap<JUnt32, JUnt32> m_IdSupportsForbiden;
};
