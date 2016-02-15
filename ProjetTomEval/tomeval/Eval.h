// Eval.h: interface for the CEval class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVAL_H__84C1AFA1_C8A5_11D1_95EA_444553540000__INCLUDED_)
#define AFX_EVAL_H__84C1AFA1_C8A5_11D1_95EA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEval  
{
	CArray<CTomjob*,CTomjob*> m_TomCDZ; 
	short m_EtatTom;
	CTomjob m_Tom;
	CWordArray m_NrLienAffected; 

	CTomjob * DeleteTomCDZ(short NrLien);
	void DeleteLien(short NrLien);
	void VerifieLeLien(short NrLien);

public:
	int GetNbLienAffecte();
	CEval();
	virtual ~CEval();
	short LanceEval(HWND hWnd,HWND hWndMaitre,LPCSTR TOMJOB,LPCSTR RESTOM);
	short LanceEvalMarginal(short NrLien,HWND hWndMaitre,LPCSTR TOMJOB,LPCSTR RESTOM,long Option);
	short LanceEvalMarginal(CTomjob * Tom,bool fCalculMonoSource=0);
	short TermineEvalMarginal(short NrLien);
	short GetRestom(short NrLien);
	short AjouteSpotMarginal(short NrLien);
	short AjouteSpotZoom(short NrLien);
	void BreakCalcul();
	short TerminerPossible();
	short GetNrLien();
	CTomjob * GetTomCDZ(short NrLien,bool flagCreate=0);
};

#endif // !defined(AFX_EVAL_H__84C1AFA1_C8A5_11D1_95EA_444553540000__INCLUDED_)
