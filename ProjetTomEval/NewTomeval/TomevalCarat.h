// TomevalCarat.h: interface for the CTomevalCarat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMEVALCARAT_H__2F6D27CE_A1F7_4741_9CDD_E63D29C30F65__INCLUDED_)
#define AFX_TOMEVALCARAT_H__2F6D27CE_A1F7_4741_9CDD_E63D29C30F65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TomEval.h"
#include "ficlbx.h"

class CTomevalCarat : public CTomEval  
{
public:
	CTomevalCarat();
	virtual ~CTomevalCarat();
	bool LanceCalculClassique(LPCSTR NomFicTomjob,LPCSTR NomFicResTom,bool flagMarginal=false);


protected:
	void TrouveJourActif();
	void EvaluerApportSemaine();
	void EvaluerApportSemaineGlissante();
private:

	// Calcul d'un résultat par jour, par semaine et par semaine glissante
	// Apport par jour
	CTabResultatJour m_TabJour;
	CTabResultatJour m_TabSemaine;
	CTabResultatJour m_TabSemaineGlissante;


};

#endif // !defined(AFX_TOMEVALCARAT_H__2F6D27CE_A1F7_4741_9CDD_E63D29C30F65__INCLUDED_)
