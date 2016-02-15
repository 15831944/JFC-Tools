// TableConsomationTotalTV.h: interface for the CTableConsomationTotalTV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLECONSOMATIONTOTALTV_H__FBA142BE_DDB7_4348_B6E5_B71AC39710E3__INCLUDED_)
#define AFX_TABLECONSOMATIONTOTALTV_H__FBA142BE_DDB7_4348_B6E5_B71AC39710E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTableConsomationTotalTV  
{
public:
	double GetTEI(int individu);
	int GetPlusPetitConsomateur(int rang);
	int GetPlusGrosConsommateur(int rang);
	int LoadFile(LPCSTR NomFic);
	int LoadFile(LPCSTR Chemin,LPCSTR Etude,LPCSTR Panel);

	CTableConsomationTotalTV();
	virtual ~CTableConsomationTotalTV();
	void CalculFiltreStrateConsoCible(long * m_CoeffEx,double * m_FiltreIndividus,int NrStrate,int NbRatioConsomation);

private:
	void BindVecteurIndividuTrie();
	int m_NbIndiv;
	double * m_Consomation; //[individu]
	int * m_NrIndivTrie; //[individu]
	CString m_Nomfich;
	bool m_fLoaded;
};

#endif // !defined(AFX_TABLECONSOMATIONTOTALTV_H__FBA142BE_DDB7_4348_B6E5_B71AC39710E3__INCLUDED_)
