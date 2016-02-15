// FicVagueCIB.h: interface for the CFicVagueCIB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICVAGUECIB_H__CE4BA381_A779_11D1_95EA_444553540000__INCLUDED_)
#define AFX_FICVAGUECIB_H__CE4BA381_A779_11D1_95EA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CFicVagueCIB  
{
	char m_fVagueLoaded;

	short m_NbIndiv;
	short m_NbCible;
	short *m_NrCible; // [m_NbCible]
	CStringArray m_LibelleCible;  
	short **m_PoidsIndiv;  // [m_NbCible][m_NbIndiv]


public:
	CString m_NomFicPDX;
	short GetNbIndiv();
	CFicVagueCIB();
	virtual ~CFicVagueCIB();

	void FreeVague();
	short LoadVague(LPCSTR pCheminVague,LPCSTR pCodeVague,short NbIndiv);
	short LoadVague(LPCSTR ,short &NbIndiv);
	short LoadPoidsCible(short NrCible,long * &CoeffEx);
	short LoadPoidsCibleIdx(short NrIndex,long * &CoeffEx);
};

#endif // !defined(AFX_FICVAGUECIB_H__CE4BA381_A779_11D1_95EA_444553540000__INCLUDED_)
