// FicVague.h: interface for the CFicVague class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICVAGUE_H__C0A6F744_9D4F_11D1_A42E_004005327F70__INCLUDED_)
#define AFX_FICVAGUE_H__C0A6F744_9D4F_11D1_A42E_004005327F70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define NBFIELDVAGUE			4
#define TAILLEVAGUECODE			4
#define TAILLEVAGUELIBEL		30
#define TAILLEVAGUEOCCUP		1
#define TAILLEVAGUENBINDIV		5



#include "fictbl.h"
class CFicVague 
{
private:
	struct Record
	{	
		char Code[TAILLEVAGUECODE+1];		//unique
		char Libelle[TAILLEVAGUELIBEL+1];		//unique
		char Occupe;
		long NbIndividu;
	};
	CPtrArray m_Vague;
	void FreeRecord();
	CFicTbl m_Fic;

public:
	CFicVague();
	virtual ~CFicVague();

	short LoadFile(LPCSTR aNomFic);
	short AddRecord(LPCSTR aNomFic, char * aCode, char * aLibelle, char aOccupe, long aNbIndiv, char fsave);
	short SaveFile(LPCSTR aNomFic);

	short GetListVague(CStringArray & aListVague);
	long GetNbIndiv(LPCSTR aCode);
	long GetNbIndiv(short NrIndexVague);
	short GetNrIndex(LPCSTR aCode);
	short GetNbVague();

	LPTSTR GetVagueCode(LPCSTR aNom);
	LPCSTR GetCodeVague(short NrIndexVague);
};

#endif // !defined(AFX_FICVAGUE_H__C0A6F744_9D4F_11D1_A42E_004005327F70__INCLUDED_)
