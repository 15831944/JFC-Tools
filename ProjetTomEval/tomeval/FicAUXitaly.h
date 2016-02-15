// FicAUXitaly.h: interface for the CFicAUXitaly class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICAUXITALY_H__CAFBC1E0_FE72_11D3_989E_004095420987__INCLUDED_)
#define AFX_FICAUXITALY_H__CAFBC1E0_FE72_11D3_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProbaRubrica
{
public:
	DWORD m_Nr;
	float m_Proba;
};
typedef CArray<CProbaRubrica,CProbaRubrica&> CTabProbaRubrica;

class CRubrica
{
public:
	CTabProbaRubrica m_TabProba;
	CMap<WORD,WORD,int,int> m_IdxMapNbInsert;
	CPtrArray m_CtsIndiv; // [IdxMapNbInsert]
};



class CFicAUXitaly  
{
public:


	CTabProbaRubrica * m_TabProba; // [NbIndiv]
	DWORD m_NbIndiv;
	int m_NbRubrica;
	
	CMap<DWORD,DWORD,int,int>m_MapRubrica;
//	CTabProbaRubrica * m_TabProbaRubrica; // [rubrica]
	CRubrica  * m_TabProbaRubrica; // [rubrica]

	
	CFicAUXitaly();
	virtual ~CFicAUXitaly();
	char m_fVagueLoaded;
	short LoadVague(LPCSTR NomFic,HWND hWndMaitre,CDWordArray * NrRubrica);
//	short TrouveProbas(int Rubrica,short&NbCas,SCouvIndiv* &TabProba);

	void FreeVague();
};

#endif // !defined(AFX_FICAUXITALY_H__CAFBC1E0_FE72_11D3_989E_004095420987__INCLUDED_)
