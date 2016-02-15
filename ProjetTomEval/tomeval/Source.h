// Source.h: interface for the CSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOURCE_H__788E8F21_A90E_11D1_95EA_444553540000__INCLUDED_)
#define AFX_SOURCE_H__788E8F21_A90E_11D1_95EA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "FicAux.h"
#include "FicVague.h"
#include "FicCib.h"
#include "FicLbx.h"
#include "FicIND.h"
#include "FicCOD.h"
#include "FicAUXitaly.h"


// Classe de gestion du chargement de plusieurs sources
// chaque source contiend des vagues
class CSource  
{
	CString m_NomRepSource;
	char m_fSourceLoaded;
	
	CFicVague m_FicVag;

	char *m_VagueLoaded;
	CFicAUX *m_FicAux;
	CFicLBX *m_FicLbx;
#ifdef OPTION_MEDIACONSULT
	CFicIND *m_FicInd;
	CFicCOD *m_FicCod;
	CFicAUXitaly *m_FicAuxitaly;
#endif

	CArray <CFicVagueCIB *,CFicVagueCIB *> m_FicCib;
	short m_PreviusIndex;
	short m_PreviusIndexCible;
public:
	CSource();
	virtual ~CSource();

	short LoadSource(LPCSTR NomRepSource);
	short LoadSource(int);
	short LoadVague(LPCSTR CodeVague,HWND hWndMaitre=0);
	short LoadVague(LPCSTR NomFicCouv,
				   LPCSTR NomFicPoids,
				   LPCSTR NomFicIND,
				   LPCSTR NomFicCOD,
				   HWND hWndMaitre,
				   LPCSTR NomFicVentil,
				   LPCSTR NomFicNbModal);

	void FreeSource();

	short GetNbIndiv();
	CFicVagueCIB * GetFicCib();
	CFicAUX * GetFicAux();
	CFicLBX * GetFicLbx();
	CFicAUXitaly * GetFicAuxItaly();
	CFicCOD * GetFicCod();
	CFicIND * GetFicInd();

private:
	void TrouveCible(LPCSTR NomFic);
	void TrouveCible(LPCSTR m_NomRepSource,LPCSTR CodeVague, int NbIndiv);

	CStringArray m_TabNomFic;
	short m_NbIndiv;
	int m_NbVagueUtile;
	int GetNrIndex(LPCSTR NomFic);
	short m_NbVague;
};

#endif // !defined(AFX_SOURCE_H__788E8F21_A90E_11D1_95EA_444553540000__INCLUDED_)
