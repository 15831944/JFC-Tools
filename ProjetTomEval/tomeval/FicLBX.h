// FicLBX.h: interface for the CFicLBX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICLBX_H__EDD814C2_B76B_11D1_95EA_444553540000__INCLUDED_)
#define AFX_FICLBX_H__EDD814C2_B76B_11D1_95EA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



#include "FicTbl.h"
class CFicLBX  
{
private:
	CString m_NomFicCRC;
	class Record
	{	
	public:
		short Numero;
		char Libelle[31];
		short NbModalite;
		CStringArray LibClasses;
	};
	CPtrArray m_Critere;
	CFicTbl m_Fic;
	char m_fVagueLoaded;
	CString m_CheminVague;
	CString m_CodeVague;

	
	public:
		LPCSTR GetLibModalite(short NrCritere,short idxModalite);
		LPCSTR GetLibCritere(short NrCritere);
	CFicLBX();
	virtual ~CFicLBX();

	short LoadVague(LPCSTR pCheminVague,LPCSTR pCodeVague);
	short LoadVagueCRX(LPCSTR NomFicCRC,LPCSTR NomFicCRX);
	void FreeVague();
	short GetNrIndex(short NrCritere);
	short GetNbModalite(short NrCritere);
	short GetNrModaliteIndiv(short NrCritere,char * NrClasseVent,short NbIndiv);
};

#endif // !defined(AFX_FICLBX_H__EDD814C2_B76B_11D1_95EA_444553540000__INCLUDED_)
