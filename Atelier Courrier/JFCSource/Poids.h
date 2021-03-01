
#pragma once


#pragma warning( disable : 4786 )

#include "FileHeader.h"
#include "Echantillon.h"
#include "LookupID.h"
#include "BitArray.h"

typedef JVector<JInt32> JINT32VECTOR;
typedef JVector<double> JDBLVECTOR;
typedef std::auto_ptr<JDBLVECTOR> PTR_JDBLVECTOR;
typedef std::auto_ptr<JINT32VECTOR> PTR_JINT32VECTOR;

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
class CPoids  
{
public:

	CPoids();
	virtual ~CPoids();

	/// Load the enquete
	JBool Load(JStringEx &Name);
	/// Returns true if this terrain has a given Echantillion, Univers and Type Poids
	JBool					HasStatisticValues(JInt32 EchantillonID, JInt32 UniversID, JInt32 TypePoidsID) const;
	/// Returns the Statistical unit Values for a given Echantillion, Univers and Type Poids
	const JDBLVECTOR *		GetStatisticValues(JInt32 EchantillonID, JInt32 UniversID, JInt32 TypePoidsID) ;
	/// Returns the individual filter for the given Echantillon
	const CBitArray *		GetEchantillonFilter(JInt32 EchantillonID) const;
	/// Returns the default univers
	JInt32					GetDefaultUnivers() const;
	/// Returns true if this terrain has a given Statistic Unit
	JBool					HasUnivers(JInt32 UniversID) const;
	/// Returns vector of valid univers
	const JINT32VECTOR *	GetUnivers(JInt32 EchantillonID) const;
	/// Returns a bit array of the individus belonging to the univers
	const CBitArray	*		GetUniversIndivs(JInt32 Univers) const;
	/// Returns the facteurs d'homothétie for the given echantillon
	//const JINT32VECTOR *	GetFacteurs(JInt32 EchantillonID) const;
	/// Returns the facteur for a given Echantillon and univers or 1.0 if not found
	double					GetFacteur(JInt32 EchantillonID, JInt32 UniversID) const;
	/// Returns vector of poids
	JBool					GetTypesPoids(JVector<JInt32>& vect) const;
	/// Find Divider
	JInt32					GetDivider(JInt32 TypePoidsID = 1) const;
	///	Returns true if poids ID exists
	JBool					HasTypePoids(JInt32 poidsID) const;
	/// Returns the default type Poids ID
	JInt32					GetDefaultTypePoidsID() const;
	/// Returns the default echantillon id for the given univers
	JInt32					GetDefaultEchantillonID(JInt32 UniversID) const;

private:

	/// File Name
	JStringEx			m_sName;
	/// Source ID
	JInt32				m_lSourceID;
	/// Terrain ID
	JInt32				m_lTerrainID;
	/// File Header
	CFileHeader			m_FileHeader;
	/// Nombre of individuals
	JInt32				m_lNbIndividus;
	/// Vector of univers ID's
	JINT32VECTOR		m_vUniversID;
	/// Vector of univers Individuals
	JMap<JInt32 , PTR_CBITARRAY> m_mpUniversIndiv;
	/// type Poids vector
	TYPEPOIDSVECTOR		m_vTypePoids;
	/// Map of poids by (Type ID/Univer ID) 
	JMap<LookupID , PTR_JINT32VECTOR >	m_mvlPoids;
	/// Map of poids by (Type ID/Univers ID)
	JMap<LookupID , PTR_JDBLVECTOR >	m_mvfPoids;
	/// Set of echantillons
	ECHANTILLONMAP		m_mEchantillons;

private:
	
	/// Load the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Load the enquete header
	JVoid LoadHeader			(JArchive *pFile, JInt32 Version);
	/// Load the Repères
	JVoid LoadData		(JArchive *pFile, JInt32 Version);

	/// assignment operator
	CPoids & operator =(const CPoids & rhs);

	/// copy constructor
	CPoids(const CPoids & rhs);
};

