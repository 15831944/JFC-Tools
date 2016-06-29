// Source.cpp: implementation of the CSource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "tomeval.h"
#include "Source.h"
#include <iostream>
#include <fstream>
 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSource::CSource()
{
	m_fSourceLoaded=0;
#ifdef OPTION_MEDIACONSULT
	m_FicInd=NULL;
	m_FicCod=NULL;
	m_FicAuxitaly=NULL;
#endif
	m_FicAux=NULL;
}

CSource::~CSource()
{
	FreeSource();
}

short CSource::LoadSource(LPCSTR NomRepSource)
{
	if(m_fSourceLoaded){
		if(m_NomRepSource==NomRepSource)return(0);
		AfficheErreur("BUG: Objet CSource MONOSOURCE pour l'instand !");
		FreeSource();
	}
	
	m_NomRepSource=NomRepSource;

	// lecture des vagues de cette source
	CString NomFic;

	NomFic=m_NomRepSource;
	NomFic+="\\vague.tbl";
	m_FicVag.LoadFile(NomFic);

	short NbVague=m_FicVag.GetNbVague();
	m_VagueLoaded=new char [NbVague];
	memset(m_VagueLoaded,0,NbVague);
	m_FicAux=(CFicAUX*)new CFicAUX[NbVague];
	m_FicLbx=(CFicLBX*)new CFicLBX[NbVague];
	m_fSourceLoaded=1;
	return(0);
}

// Mediaconsult
#ifdef OPTION_MEDIACONSULT
short CSource::LoadSource(int Nb)
{
	if(m_fSourceLoaded)return(0);
	
	m_NbVague=Nb;
	m_VagueLoaded=new char [m_NbVague];
	memset(m_VagueLoaded,0,m_NbVague);
	m_FicAuxitaly=(CFicAUXitaly*)new CFicAUXitaly[m_NbVague];
	m_FicLbx=(CFicLBX*)new CFicLBX[m_NbVague];
	m_FicInd=(CFicIND*)new CFicIND[m_NbVague];
	m_FicCod=(CFicCOD*)new CFicCOD[m_NbVague];
	m_TabNomFic.SetSize(Nb);
	m_fSourceLoaded=1;
	return(0);
}
#endif
short CSource::LoadVague(LPCSTR CodeVague,HWND hWndMaitre)
{
	short idx=m_FicVag.GetNrIndex(CodeVague);
	m_PreviusIndex=idx;
	m_NbIndiv=(short)m_FicVag.GetNbIndiv(idx);
	TrouveCible(m_NomRepSource,CodeVague,m_NbIndiv);
	if(m_VagueLoaded[idx])return(0);

	m_FicAux[idx].LoadVague(m_NomRepSource,CodeVague,hWndMaitre);
	m_FicLbx[idx].LoadVague(m_NomRepSource,CodeVague);

	m_VagueLoaded[idx]=1;
	m_NbVagueUtile=0;
	return(0);
}

// Mediaconsult
#ifdef OPTION_MEDIACONSULT
short CSource::LoadVague(LPCSTR NomFicCouv,
				   LPCSTR NomFicPoids,
				   LPCSTR NomFicIND,
				   LPCSTR NomFicCOD,
				   HWND hWndMaitre,
				   LPCSTR NomFicVentil,
				   LPCSTR NomFicNbModal)
{
	TrouveCible(NomFicPoids);
	short idx=GetNrIndex(NomFicCouv);
	m_PreviusIndex=idx;
	if(m_VagueLoaded[idx])return(0);

	m_FicCod[idx].LoadVague(NomFicCOD);
	m_FicAuxitaly[idx].LoadVague(NomFicCouv,hWndMaitre,&m_FicCod[idx].m_RubricaMainFrame);
	m_FicLbx[idx].LoadVagueCRX(NomFicVentil,NomFicNbModal);
	m_FicInd[idx].LoadVague(NomFicIND,m_NbIndiv);

	m_VagueLoaded[idx]=1;
	return(0);
}
#endif

void CSource::FreeSource()
{
	if(!m_fSourceLoaded)return;

	if(m_FicAux) delete [] m_FicAux;
#ifdef OPTION_MEDIACONSULT
	if(m_FicAuxitaly) delete [] m_FicAuxitaly;
	if(m_FicInd)delete [] m_FicInd;
	if(m_FicCod)delete [] m_FicCod;
#endif
	for(int i=0;i<m_FicCib.GetSize();i++)delete m_FicCib[i];
	delete [] m_FicLbx;
	delete [] m_VagueLoaded;
	m_fSourceLoaded=0;
}



short CSource::GetNbIndiv()
{
	return(m_NbIndiv);
}

CFicVagueCIB * CSource::GetFicCib()
{
	return(m_FicCib[m_PreviusIndexCible]);
}

CFicAUX * CSource::GetFicAux()
{
	return(&m_FicAux[m_PreviusIndex]);
}

CFicLBX * CSource::GetFicLbx()
{
	return(&m_FicLbx[m_PreviusIndex]);
}

#ifdef OPTION_MEDIACONSULT
CFicAUXitaly * CSource::GetFicAuxItaly()
{
	return(&m_FicAuxitaly[m_PreviusIndex]);
}
#endif

#ifdef OPTION_MEDIACONSULT
CFicCOD * CSource::GetFicCod()
{
	return(&m_FicCod[m_PreviusIndex]);
}
#endif

#ifdef OPTION_MEDIACONSULT
CFicIND * CSource::GetFicInd()
{
	return(&m_FicInd[m_PreviusIndex]);
}
#endif
int CSource::GetNrIndex(LPCSTR NomFic)
{
	int i;
	for(i=0;i<m_NbVagueUtile;i++){
		if(m_TabNomFic[i]==NomFic)break;
	}
	if(i>=m_NbVague){
		AfficheErreur("Erreur: Buffer des sources chargée plein");
		i=0;
	}
	if(i==m_NbVagueUtile){
		m_TabNomFic[i]=NomFic;
		m_NbVagueUtile++;
	}
	return i;
}


// Trouve la cible si elle est déjà chargée,
// sinon charge la cible.
// Met à jour m_PreviusIndexCible
void CSource::TrouveCible(LPCSTR m_NomRepSource,LPCSTR CodeVague, int NbIndiv)
{
	int i;
	CString NomFicPDX;
	NomFicPDX.Format("%s\\%s.pdx",m_NomRepSource,CodeVague);
	for(i=0;i<m_FicCib.GetSize();i++){
		if(m_FicCib[i]->m_NomFicPDX==NomFicPDX)break;
	}
	if(i<m_FicCib.GetSize()){
		m_PreviusIndexCible=i;
		return;
	}
	
	// Cette cible n'est pas encore chargée
	CFicVagueCIB * Cib=new CFicVagueCIB;
	Cib->LoadVague(m_NomRepSource,CodeVague,NbIndiv);
	m_PreviusIndexCible=m_FicCib.Add(Cib);
}


// Trouve la cible si elle est déjà chargée,
// sinon charge la cible.

// Met à jour m_PreviusIndexCible et m_NbIndiv
void CSource::TrouveCible(LPCSTR NomFic)
{
	int i;
	for(i=0;i<m_FicCib.GetSize();i++){
		if(m_FicCib[i]->m_NomFicPDX==NomFic)break;
	}
	if(i<m_FicCib.GetSize()){
		m_PreviusIndexCible=i;
		m_NbIndiv=m_FicCib[i]->GetNbIndiv();
		return;
	}
	
	// Cette cible n'est pas encore chargée
	CFicVagueCIB * Cib=new CFicVagueCIB;
	Cib->LoadVague(NomFic,m_NbIndiv);
	m_PreviusIndexCible=m_FicCib.Add(Cib);
}
