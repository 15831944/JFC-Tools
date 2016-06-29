// FicLBX.cpp: implementation of the CFicLBX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "tomeval.h"
#include "FicLBX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFicLBX::CFicLBX()
{
	m_fVagueLoaded=0;
	m_Fic.SetLineFormat(3,30,2);
}

CFicLBX::~CFicLBX()
{
	FreeVague();
}



short CFicLBX::LoadVague(LPCSTR pCheminVague,LPCSTR pCodeVague)
{
//	CWaitCursor Wait;
	if(m_fVagueLoaded)FreeVague();


	short i,z, NbRec;
	Record * R;
	CString NomFic;
	NomFic.Format("%s\\%s.LBX",pCheminVague,pCodeVague);
	m_CheminVague=pCheminVague;
	m_CodeVague=pCodeVague;

	m_NomFicCRC.Format("%s\\%s.CRC",m_CheminVague,m_CodeVague);

	//Ouvre le fichier
	m_Critere.SetSize(0);
	if(m_Fic.OpenFile(NomFic, 'r'))	return(1);
	
	//Cherche l'* - recupere le nb de rec
	NbRec=m_Fic.FindStar();
	if(NbRec<=0)
	{	
		m_Fic.CloseFile();	
		return(1);
	}

	//Cherche le #
	if(m_Fic.FindHash())
	{	
		m_Fic.CloseFile();
		return(1);
	}

	//Alloue de l'espace au pointeur d'array
	m_Critere.SetSize(NbRec);

	//Recupere les enregistrement
	for(i=0 ; i<NbRec ; i++)
	{	//recupere l'enregistrement
		if(m_Fic.ReadLine())
		{
			m_Critere.SetSize(i-1);
			m_Fic.CloseFile();
			return(1);
		}
		//Recupere les differents champs de l'enregistrement
		R= new Record;
		R->Numero=m_Fic.GetShort(0);
		strcpy(R->Libelle, m_Fic.GetCharPtr(1));		//lecture du libelle
		R->NbModalite=m_Fic.GetShort(2);
		
		//copie la structure dans le pointeur d'array
		m_Critere[i]=(void*)R;
	}

	// on Lit les libellés des modalités
	m_Fic.SetLineFormat(30);
	for(i=0;i<m_Critere.GetSize();i++){
		m_Fic.FindHash();
		R=(Record*)m_Critere[i];
		for(z=0;z<R->NbModalite;z++){
			m_Fic.ReadLine();
			R->LibClasses.Add(m_Fic.GetCharPtr(0));
		}
	}

	//Ferme le fichier
	m_Fic.CloseFile();
	m_fVagueLoaded=1;
	return(0);
}

short CFicLBX::LoadVagueCRX(LPCSTR NomFicCRC,LPCSTR NomFicCRX)
{
//	CWaitCursor Wait;
	if(m_fVagueLoaded)FreeVague();

	m_NomFicCRC=NomFicCRC;

	CFile f;
	CFileException e;
	if( !f.Open( NomFicCRX, CFile::modeRead, &e ) )   {
		CString err;
		err.Format("Erreur à l'ouverture du fichier %s : %s",NomFicCRX,e.m_cause);
		AfficheErreur(err);
	}
	
	int NbRec=f.GetLength();

	//Alloue de l'espace au pointeur d'array
	m_Critere.SetSize(NbRec);

	//Recupere les enregistrement
	Record * R;
	for(int i=0 ; i<NbRec ; i++)
	{	
		//Recupere les differents champs de l'enregistrement
		R= new Record;
		R->Numero=i+1;
		char octet;
		f.Read(&octet,1);
		R->NbModalite=octet;
		
		//copie la structure dans le pointeur d'array
		m_Critere[i]=(void*)R;
	}

	f.Close();
	m_fVagueLoaded=1;
	return(0);
}



void CFicLBX::FreeVague()
{
	Record * R;
	if(!m_fVagueLoaded)return;
	short i,NbRec=m_Critere.GetSize();
	for(i=0;i<NbRec;i++){
		R=(Record*)m_Critere[i];
		delete R;
	}
	m_Critere.RemoveAll();
	
	m_fVagueLoaded=0;
}

// -1 si erreur
short CFicLBX::GetNrIndex(short NrCritere)
{
	short NbRec=m_Critere.GetSize();
	for(short i=0;i<NbRec;i++){
		Record * rec=(Record *)m_Critere[i];
		if(rec->Numero==NrCritere)return(i);
	}
	return(-1);
}

short CFicLBX::GetNbModalite(short NrCritere)
{
	short NbRec=m_Critere.GetSize();
	for(short i=0;i<NbRec;i++){
		Record * rec=(Record *)m_Critere[i];
		if(rec->Numero==NrCritere)return(rec->NbModalite);
	}
	return(-1);
}

short CFicLBX::GetNrModaliteIndiv(short NrCritere,char * NrClasseVent,short NbIndiv)
{
	short i,idx;
	CStdioFile FicCRC;
	CString MsgErr;
	CFileException e;

	idx=GetNrIndex(NrCritere);
	if(idx<0){
		AfficheErreur("Erreur: Index du critère invalide");
		return(1);
	}
	
	if(!FicCRC.Open(m_NomFicCRC,CFile::modeRead  | CFile::typeBinary, &e ) )
	{
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",m_NomFicCRC);
		AfficheErreur(MsgErr);
		return(1);
	}
	
	FicCRC.Seek(idx*NbIndiv,CFile::begin);
	FicCRC.Read(NrClasseVent,NbIndiv);
	for(i=0;i<NbIndiv;i++)NrClasseVent[i]--;
	FicCRC.Close();
	return(0);
}


LPCSTR CFicLBX::GetLibCritere(short NrCritere)
{
	Record * R;
	short i=GetNrIndex(NrCritere);
	R=(Record*)m_Critere[i];
	return(R->Libelle);
}

LPCSTR CFicLBX::GetLibModalite(short NrCritere, short idxModalite)
{
	Record * R;
	short i=GetNrIndex(NrCritere);
	R=(Record*)m_Critere[i];
	return(R->LibClasses[idxModalite]);
}
