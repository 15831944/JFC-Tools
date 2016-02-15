// SourceDEF.h: interface for the CSourceDEF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOURCEDEF_H__2113CC01_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_SOURCEDEF_H__2113CC01_5C6A_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSourceDEF  
{
public:
	CSourceDEF();
	virtual ~CSourceDEF();
	CSourceDEF & operator=(const CSourceDEF &Data);
	CSourceDEF(const CSourceDEF &Data);

	CString m_Nom;
	short m_Index; // Num�ro dynamique d'index dans table des sources
	           // Ne jamais m�moriser ce num�ro mais le Nom(ci-dessus)
			   // -1 si cette source n'est pas utilis�e
	short m_NbModaGeofrac;
	// non allou� ou -1 si NbModaGeofrac>0
	CWordArray m_TabModaGeofrac;
	short m_NrRegionGeofrac; // Nr de la r�gion pour la source de base
	                     // Toujours l'utiliser dans ->Noyau, jamais dans ->Univers
	// fin du G�ofractionnaire
    CStringArray m_Vagues; //*NbVaguesCalU // max:CODEVAGUEMAX+1 /: allou� si NbPeriode>0
				// et pour l'instant uniquement les �talons
    CWordArray m_NrIndexVague; //*NbVaguesCalU // nr dynamique � ne PAS MEMORISER /: allou� si NbPeriode>0
				// et pour l'instant uniquement les �talons
};

typedef CArray<CSourceDEF, CSourceDEF&> CSourceDefArray;
#endif // !defined(AFX_SOURCEDEF_H__2113CC01_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
