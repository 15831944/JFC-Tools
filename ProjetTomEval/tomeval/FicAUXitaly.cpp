// FicAUXitaly.cpp: implementation of the CFicAUXitaly class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tomeval.h"
#include "FicAUXitaly.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFicAUXitaly::CFicAUXitaly()
{
	m_fVagueLoaded=0;
}

CFicAUXitaly::~CFicAUXitaly()
{
	FreeVague();
}

short CFicAUXitaly::LoadVague(LPCSTR NomFic ,HWND hWndMaitre,CDWordArray * NrRubrica)
{
	int r,i;
	FreeVague();

	CStdioFile f;
	CFileException e;

	if(!f.Open(NomFic,CFile::modeRead  | CFile::typeBinary, &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",NomFic);
		AfficheErreur(MsgErr);
		return(1);
	}
	
	if(hWndMaitre)::SendMessage(hWndMaitre,MSGTOM_INFO_LOADHABITUDE,0,0L);

	f.Read(&m_NbIndiv,sizeof(m_NbIndiv));
	ASSERT(m_NbIndiv>0);
	m_TabProba=new CTabProbaRubrica[m_NbIndiv];
	for(DWORD i=0;i<m_NbIndiv;i++)
	{
		int nb;
		f.Read(&nb,sizeof(nb));
		m_TabProba[i].SetSize(nb);
		for(r=0;r<nb;r++)
		{
			DWORD CodeRubrica;
			float Proba;
			f.Read(&CodeRubrica,sizeof(CodeRubrica));
			f.Read(&Proba,sizeof(Proba));
			m_TabProba[i][r].m_Nr=CodeRubrica;
			m_TabProba[i][r].m_Proba=Proba;
		}
		if(hWndMaitre)::SendMessage(hWndMaitre,MSGTOM_INFO_LOADHABITUDE,((i+1)*50)/m_NbIndiv,0L);
	}
	f.Close();


	// Transformation dans l'autre sens...
	// En attendant que Edddie fasse un autre fichier
	int NextIdx=0;
	m_NbRubrica=NrRubrica->GetSize();
	for(r=0;r<m_NbRubrica;r++)
	{
		m_MapRubrica.SetAt(NrRubrica->GetAt(r),r);
	}
	m_TabProbaRubrica=new CRubrica[m_NbRubrica];
	for(r=0;r<m_NbRubrica;r++)m_TabProbaRubrica[r].m_TabProba.SetSize(0,m_NbIndiv);

	bool pb=0;
	CProbaRubrica Rub;
	for( i=0;i<m_NbIndiv;i++)
	{
		for(r=0;r<m_TabProba[i].GetSize();r++)
		{	
			int idx;
			if(!m_MapRubrica.Lookup(m_TabProba[i][r].m_Nr,idx))
			{
				pb=1;
				continue;
			}
			Rub.m_Nr=i;
			Rub.m_Proba=m_TabProba[i][r].m_Proba;
			m_TabProbaRubrica[idx].m_TabProba.Add(Rub);
		}
		if(hWndMaitre)::SendMessage(hWndMaitre,MSGTOM_INFO_LOADHABITUDE,(50+((i+1)*50)/m_NbIndiv),0L);
	}
	for(r=0;r<NrRubrica->GetSize();r++)
	{
		m_TabProbaRubrica[r].m_TabProba.FreeExtra();
	}

	if(pb)
	{
		AfficheErreur("pb dans CFicAUXitaly::LoadVague");
		ASSERT(0);
	}

	delete [] m_TabProba;

	// La vague est chargée maintenannt
	m_fVagueLoaded=1;
	return(0);
}



void CFicAUXitaly::FreeVague()
{
	if(!m_fVagueLoaded)return;
	m_fVagueLoaded=0;
	
	for(int r=0;r<m_NbRubrica;r++)
	{
		for(int i=0;i<m_TabProbaRubrica[r].m_CtsIndiv.GetSize();i++)
		{
			delete [] m_TabProbaRubrica[r].m_CtsIndiv[i];
		}
	}

	for(DWORD i=0;i<m_NbIndiv;i++)
	{
	}
	
//	delete [] m_TabProba;
	delete [] m_TabProbaRubrica;

	m_NbIndiv=0;
}
