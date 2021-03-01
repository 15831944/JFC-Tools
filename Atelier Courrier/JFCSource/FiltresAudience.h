#pragma once

#include "FileHeader.h"
#include "FiltreAudience.h"

class vLookupID
{
public:
	vLookupID(void);
	vLookupID(const vLookupID &rhs);
	virtual ~vLookupID(void);
	vLookupID & operator = (const vLookupID &rhs);
	vLookupID & Set(const JINT32VECTOR & vID1, JInt32 ID2);

	bool operator < (const vLookupID &rhs) const;
	bool operator > (const vLookupID &rhs) const;

	JINT32VECTOR	m_vID1;
	JInt32			m_ID2;
};

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
class CFiltresAudience
{
public:
	CFiltresAudience(void);
	virtual ~CFiltresAudience(void);
	/// Load the Relations d'equivalence
	JBool Load(JStringEx &Name);
	/// Returns the Audience filters
	JBool GetFiltres(JVector<CFiltreAudience  *> & Filtres);
	/// Returns the Audience filter of a given Support
	const CFiltreAudience * GetFilter(JINT32VECTOR & ReperePrim, JINT32VECTOR & SupportID, JInt32 FiltreID); 


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
	/// Nombre de primordial dimensions
	JInt32					m_NbDimPrim;
	/// Repere primordial for each dimension
	JINT32VECTOR			m_vRepereDimPrim;
	/// Vector of Audience filters
	FILTREAUDIENCEVECTOR	m_vFiltres;
	/// Map of audience filtre by Support ID
	JMap<vLookupID, CFiltreAudience *> m_mFiltres;
private:

	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	/// Load the Filtre
	JVoid LoadFiltres			(JArchive *pFile, JInt32 Version);

	/// assignment operator
	CFiltresAudience & operator =(const CFiltresAudience & rhs);
	/// copy constructor
	CFiltresAudience(const CFiltresAudience & rhs);
};
