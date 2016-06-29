// FicIND.cpp: implementation of the CFicIND class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tomeval.h"
#include "FicIND.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFicIND::CFicIND()
{
	m_fVagueLoaded=0;
}

CFicIND::~CFicIND()
{
	FreeVague();
}

short CFicIND::LoadVague(LPCSTR NomFic,int NbIndiv)
{
	FreeVague();
	
	m_NrIndiv.SetSize(NbIndiv);

	CFile f;
	CFileException e;

	if(!f.Open(NomFic,CFile::modeRead  | CFile::typeBinary, &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",NomFic);
		AfficheErreur(MsgErr);
		return(1);
	}
	
	for(int i=0;i<NbIndiv;i++)
	{
		DWORD nr;
		f.Read(&nr,sizeof(nr));
		m_NrIndiv[i]=nr;
	}
	f.Close();

	// La vague est chargée maintenannt
	m_fVagueLoaded=1;
	return(0);
}

void CFicIND::FreeVague()
{
	if(!m_fVagueLoaded)return;
	m_fVagueLoaded=0;
	m_NrIndiv.RemoveAll();
}
