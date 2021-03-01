
#pragma once

#pragma warning( disable : 4786 )

#include "GrandTheme.h"

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
class CIndexThematique  
{
public:

	CIndexThematique();
	virtual ~CIndexThematique();
	
	/// Loads the object
	JArchive &	Recv				(JArchive& lhs, JInt32 Version);
	/// Fills a CTreeCtrl with the index thematique
	JBool		FillTree			(CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM hParent, JBool AllLeaves = false, JBool OnlyQuanti = false);

private:
	
	GRANDTHEMEVECTOR		m_vGrandThemes;
	JMap<JInt32, JInt32>	m_mGrandThemes;

private:

	/// assignment operator
	CIndexThematique & operator =(const CIndexThematique & rhs);
	/// copy constructor
	CIndexThematique(const CIndexThematique & rhs);

};

