// FicAUX.h: interface for the CFicAUX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICAUX_H__91734DE1_A720_11D1_95EA_444553540000__INCLUDED_)
#define AFX_FICAUX_H__91734DE1_A720_11D1_95EA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CFicAUX  
{
// les structures utiles:

//struct CouvIndiv
//{
//	unsigned short	NrIndiv; // Numero de l'individu
//	unsigned short	Proba;	// Probabilité en millième
//};
	HWND m_hWndMaitre;
public:
	void LoadRatio(long *VCible,short NbIndiv,short * VRatio,short NbRatio);
	CFicAUX();
	virtual ~CFicAUX();

	short m_NbStation;
	short *m_NrStation; //[m_NbStation]
	short *m_NbJour; //[m_NbStation]
	short **m_CodeJour; // [m_NbStation][m_NbJour]
	short **m_NbPlage; // [m_NbStation][m_NbJour]
	short ***m_HPlage; // [m_NbStation][m_NbJour][m_NbPlage]
	short ***m_NbCode; // [m_NbStation][m_NbJour][m_NbPlage]
	short ****m_CodeNum; // [m_NbStation][m_NbJour][m_NbPlage][m_NbCode]

	short ***m_NbCas; // [m_NbStation][m_NbJour][m_NbPlage]
	SCouvIndiv **** m_TabProba; // [m_NbStation][m_NbJour][m_NbPlage][m_NbCas]

	char m_fVagueLoaded;
	short LoadVague(LPCSTR pCheminVague,LPCSTR pCodeVague,HWND hWndMaitre=0);
	short LoadVague(LPCSTR ,HWND hWndMaitre=0);
	void FreeVague();
	short TrouveProbas(short NrStation,short CodeHoraire,short HorairePassage,short Jour,short&NbCas,SCouvIndiv* &TabProba);

};

#endif // !defined(AFX_FICAUX_H__91734DE1_A720_11D1_95EA_444553540000__INCLUDED_)
