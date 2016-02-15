// LockAccess.cpp: implementation of the CLockAccess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Nomenclature.h"
#include "LockAccess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLockAccess::CLockAccess()
{
	m_fFileOpen=0;
}

CLockAccess::~CLockAccess()
{
	UnlockFile();
}

bool CLockAccess::LockFile(LPCSTR nomfile)
{
	HCURSOR oldcur,cur=LoadCursor(NULL,IDC_WAIT);
	oldcur=SetCursor(cur);

	UnlockFile();
	bool No=0;

	// si niveau accés = 2 , même bloqué on peut être en mode ajout
	//theApp.m_ModeAjoutSeul = false
		
	for(int i=0;i<=5;i++)
	{
		if (i) Sleep(6000);
		if(!m_LckFile.Open(nomfile,CFile::modeWrite|CFile::shareExclusive| CFile::typeBinary| CFile::modeNoTruncate))
		{
			AfxMessageBox("Le serveur est indisponible pour le moment.");
			return(0);
		}
		if(m_LckFile.GetLength()>0)
		{
			m_LckFile.Close();
			if(i==0)
			{
				if(AfxMessageBox("Le serveur semble utilisé.\nVoulez-vous attendre 30 secondes que le serveur se libère ?",MB_YESNO|MB_ICONQUESTION)!=IDYES)
				{
					No=1;
					break;
				}
			}
		}
		else 
		{
			m_fFileOpen=1;
			break;
		}

	}

	if((m_fFileOpen==0) && (No==0))
	{
		if(AfxMessageBox("Le serveur est toujours utilisé...\nIl est possible de prendre la main de façon forcé.\nCette action risque d'endommager la nomenclature et ses fichiers si des utilisateurs sont toujours connectés.\nEtes-vous sûr de vouloir prendre la main ?",MB_YESNO|MB_ICONSTOP)==IDYES)
		{
			if(!m_LckFile.Open(nomfile,CFile::modeWrite|CFile::shareExclusive| CFile::typeBinary| CFile::modeNoTruncate))
			{
				AfxMessageBox("Le serveur est indisponible pour le moment.");
				return(0);
			}
			m_LckFile.SetLength(0);
			m_fFileOpen=1;
		}
	}

	SetCursor(oldcur);
	return(m_fFileOpen);
}

void CLockAccess::UnlockFile()
{
	if(!m_fFileOpen)return;
	m_LckFile.Close();
	m_fFileOpen=0;
}
