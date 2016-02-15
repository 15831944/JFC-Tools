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
	short m_Index; // Numéro dynamique d'index dans table des sources
	           // Ne jamais mémoriser ce numéro mais le Nom(ci-dessus)
			   // -1 si cette source n'est pas utilisée
	short m_NbModaGeofrac;
	// non alloué ou -1 si NbModaGeofrac>0
	CWordArray m_TabModaGeofrac;
	short m_NrRegionGeofrac; // Nr de la région pour la source de base
	                     // Toujours l'utiliser dans ->Noyau, jamais dans ->Univers
	// fin du Géofractionnaire
    CStringArray m_Vagues; //*NbVaguesCalU // max:CODEVAGUEMAX+1 /: alloué si NbPeriode>0
				// et pour l'instant uniquement les étalons
    CWordArray m_NrIndexVague; //*NbVaguesCalU // nr dynamique à ne PAS MEMORISER /: alloué si NbPeriode>0
				// et pour l'instant uniquement les étalons
};

typedef CArray<CSourceDEF, CSourceDEF&> CSourceDefArray;
#endif // !defined(AFX_SOURCEDEF_H__2113CC01_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
