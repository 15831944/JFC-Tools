// FicVagueCIB.cpp: implementation of the CFicVagueCIB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "tomeval.h"
#include "Fictbl.h"
#include "FicCIB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFicVagueCIB::CFicVagueCIB()
{
	m_fVagueLoaded=0;
}

CFicVagueCIB::~CFicVagueCIB()
{
	FreeVague();
}

short CFicVagueCIB::LoadVague(LPCSTR pCheminVague,LPCSTR pCodeVague,short NbIndiv)
{
	FreeVague();
	m_NbIndiv=NbIndiv;

	CString NomFic;
	CFicTbl FicCib;
	short c;

	NomFic.Format("%s\\%s.cib",pCheminVague,pCodeVague);
	m_NomFicPDX.Format("%s\\%s.pdx",pCheminVague,pCodeVague);
	if(FicCib.OpenFile(NomFic))return(1);
	if((m_NbCible=FicCib.FindStar())==0)return(1);
	m_NrCible=new short [m_NbCible];
	m_LibelleCible.SetSize(m_NbCible);
	FicCib.SetLineFormat(3,30);
	FicCib.FindHash();
	for(c=0;c<m_NbCible;c++){
		FicCib.ReadLine();
		m_NrCible[c]=FicCib.GetShort(0);
		m_LibelleCible[c]=FicCib.GetCharPtr(1);
	}
	FicCib.CloseFile();

	m_fVagueLoaded=1;
	return(0);
}

//#ifdef OPTION_MEDIACONSULT
short CFicVagueCIB::LoadVague(LPCSTR pNomFic,short &NbIndiv)
{
	FreeVague();

	m_NomFicPDX=pNomFic;
	CFile f;
	CFileException e;
	if( !f.Open( pNomFic, CFile::modeRead, &e ) )   {
		CString err;
		err.Format("Erreur à l'ouverture du fichier %s : %s",pNomFic,e.m_cause);
		AfficheErreur(err);

	}

	m_NbIndiv=NbIndiv=(short)(f.GetLength()/4);

	f.Close();

	m_fVagueLoaded=1;
	return(0);
}
//#endif

void CFicVagueCIB::FreeVague()
{
	if(!m_fVagueLoaded)return;
	delete [] m_NrCible;
	m_fVagueLoaded=0;
}

short CFicVagueCIB::LoadPoidsCible(short NrCible,long * &CoeffEx)
{
	short i;
	for(i=0;i<m_NbCible;i++)if(m_NrCible[i]==NrCible)break;
	if(i>=m_NbCible){
		AfficheErreur("Numéro de cible inconnue");
		return(1);
	}

	CStdioFile FicPDX;
	if(!FicPDX.Open(m_NomFicPDX,CFile::modeRead  | CFile::typeBinary) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",m_NomFicPDX);
		AfficheErreur(MsgErr);
		return(1);
	}

	FicPDX.Seek(i*sizeof(long)*m_NbIndiv,CFile::begin);
	FicPDX.Read(CoeffEx,sizeof(long)*m_NbIndiv);
	FicPDX.Close();
	return(0);
}

short CFicVagueCIB::LoadPoidsCibleIdx(short NrIndex,long * &CoeffEx)
{
	CStdioFile FicPDX;
	if(!FicPDX.Open(m_NomFicPDX,CFile::modeRead  | CFile::typeBinary) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",m_NomFicPDX);
		AfficheErreur(MsgErr);
		return(1);
	}

	FicPDX.Seek(NrIndex*sizeof(long)*m_NbIndiv,CFile::begin);
	FicPDX.Read(CoeffEx,sizeof(long)*m_NbIndiv);
	FicPDX.Close();
	return(0);
}

short CFicVagueCIB::GetNbIndiv()
{
	if(!m_fVagueLoaded)AfficheErreur("Erreur: la vague n'est pas chargée");
	return(m_NbIndiv);
}
