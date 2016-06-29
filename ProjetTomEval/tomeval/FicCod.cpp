// FicCod.cpp: implementation of the CFicCod class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tomeval.h"
#include "FicCod.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFicCOD::CFicCOD()
{
	m_fVagueLoaded=0;
}

CFicCOD::~CFicCOD()
{
	FreeVague();
}

short CFicCOD::LoadVague(LPCSTR NomFic)
{
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

	m_NbRubrica=(int)(f.GetLength()/12);
	f.SeekToBegin();

	m_RubricaMainFrame.SetSize(m_NbRubrica);
	m_RubricaAdBox.SetSize(m_NbRubrica);
	m_StationAdBox.SetSize(m_NbRubrica);


	for(int i=0;i<m_NbRubrica;i++)
	{
		DWORD RubricaMainFrame;
		DWORD RubricaAdBox;
		DWORD StationAdBox;
		f.Read(&RubricaMainFrame,sizeof(RubricaMainFrame));
		f.Read(&RubricaAdBox,sizeof(RubricaAdBox));
		f.Read(&StationAdBox,sizeof(StationAdBox));
		m_RubricaMainFrame[i]=RubricaMainFrame;
		m_RubricaAdBox[i]=RubricaAdBox;
		m_StationAdBox[i]=StationAdBox;
	}
	f.Close();

	// La vague est chargée maintenannt
	m_fVagueLoaded=1;
	return(0);
}

void CFicCOD::FreeVague()
{
	if(!m_fVagueLoaded)return;
	m_fVagueLoaded=0;

	m_RubricaMainFrame.RemoveAll();
	m_RubricaAdBox.RemoveAll();
	m_StationAdBox.RemoveAll();
}

DWORD CFicCOD::TrouveRubricaMainFrame(DWORD Station, DWORD RubricaAdBox)
{
	for(int n=0;n<m_NbRubrica;n++)
	{
		if(m_StationAdBox[n]==Station && m_RubricaAdBox[n]==RubricaAdBox)
		{
			return(m_RubricaMainFrame[n]);
		}
	}
	return(0);
}
