
#pragma once

#pragma warning( disable : 4786 )

#include <memory>
typedef JVector<JInt32> JINT32VECTOR;
typedef JVector<JFlt32> JFLT32VECTOR;
#include "BitArray.h"
#include "TypePoids.h"

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
class CEchantillon  
{

public:

	CEchantillon();
	virtual ~CEchantillon();

	JInt32 GetID() const { return m_ID; }

	JArchive& Recv(JArchive& lhs, JInt32 Version, JInt32 NbIndividuals);
	/// Returns true if this terrain has a given Statistic Unit
	JBool	HasUnivers(JInt32 UniversID) const;
	/// Returns vector of valid univers
	const JINT32VECTOR *	GetUnivers() const { return &m_vUniversID; };
	/// Returns the valid individual vector for this echantillon
	const CBitArray *		GetIndividus() const { return &m_Individus; };
	/// Returns the facteurs d'homothétie
	const JINT32VECTOR *	GetFacteurs() const { return &m_vFacteur; };
	/// Returns the facteur for a given univers or 1 if not found
	double					GetFacteur(JInt32 UniversID) const;


private:

	JInt32				m_ID;
	JInt32				m_bEchantillon;
	CBitArray			m_Individus;
	JInt32				m_DiviseurH;
	JINT32VECTOR		m_vUniversID;
	JINT32VECTOR		m_vFacteur;

private:
	/// assignment operator
	CEchantillon & operator =(const CEchantillon & rhs);
	/// copy constructor
	CEchantillon(const CEchantillon & rhs);
};

typedef std::auto_ptr<CEchantillon> PTR_ECHANTILLON;
typedef JMap<JInt32, PTR_ECHANTILLON> ECHANTILLONMAP;

