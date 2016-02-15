// Cibjob.cpp: implementation of the CCibjob class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tomeval.h"
#include "Cibjob.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCibjob::CCibjob()
{
	m_FileLoaded=0;
}

CCibjob::~CCibjob()
{
	FreeFile();
}

int CreateFileCIBJOB(int NbIndiv)
{	
	CString FileName;
	FileName="CIBJOB";

	CStdioFile FicCibJob;
	CJFCFileException e;
	if(!FicCibJob.Open(FileName,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite,&e) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",FileName);
		e.AfficheErreur(MsgErr);
		return(1); // Erreur nr 1
	}


	short *coeff=new short[NbIndiv];

	FicCibJob.Read(coeff,sizeof(short)*NbIndiv);
	FicCibJob.Close();

	long *CoeffExCible=new long[NbIndiv];
	char *Modalite1=new char[NbIndiv];
	char *Modalite2=new char[NbIndiv];
	for(int i=0;i<NbIndiv;i++)
	{
		CoeffExCible[i]=(short)coeff[i];
		Modalite1[i]=i%3;
		Modalite2[i]=i%10;
	}

	FileName="CIBJOB.TEST";
	if(!FicCibJob.Open(FileName,CFile::modeWrite  |CFile::modeCreate | CFile::typeBinary,&e) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",FileName);
		e.AfficheErreur(MsgErr);
		return(1); // Erreur nr 1
	}

	FicCibJob.Write(CoeffExCible,sizeof(long)*NbIndiv);
	FicCibJob.Write(CoeffExCible,sizeof(long)*NbIndiv);
	FicCibJob.Write(Modalite1,sizeof(char)*NbIndiv);
	FicCibJob.Write(Modalite1,sizeof(char)*NbIndiv);
	FicCibJob.Write(Modalite2,sizeof(char)*NbIndiv);

	delete []coeff;
	delete []CoeffExCible;
	delete []Modalite1;
	delete []Modalite2;

	return 0;

}

int CreateFilePDS(int NbIndiv)
{	
	CString FileName;
	FileName="C:\\DataDev\\dataCIMBelgique\\VAGUE.PDS";

	CStdioFile FicPds;
	CJFCFileException e;
	if(!FicPds.Open(FileName,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite,&e) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",FileName);
		e.AfficheErreur(MsgErr);
		return(1); // Erreur nr 1
	}


	short *coeff=new short[NbIndiv];

	FicPds.Read(coeff,sizeof(short)*NbIndiv);
	FicPds.Close();

	FileName="C:\\DataDev\\dataCIMBelgique\\VAGUE.PDS2";
	if(!FicPds.Open(FileName,CFile::modeWrite  | CFile::modeCreate  | CFile::typeBinary|CFile::shareDenyWrite,&e) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",FileName);
		e.AfficheErreur(MsgErr);
		return(1); // Erreur nr 1
	}
	for(int u=0;u<7;u++)
	{
		double perte=0;
		for(int i=0;i<NbIndiv;i++)
		{
			short coefff=coeff[i];
			if(u<5)
			{
				short coeff2=coefff/5;
				double coeffdouble=(double)coefff/5.0;
				double coeff2double=(double)coeff2;
				perte+=coeffdouble-coeff2double;
				if(perte>=1)
				{
					coefff=coeff2+1;
					perte-=1;
				}
				else
				{
					coefff=coeff2;
				}
			}
			FicPds.Write(&coefff,sizeof(coefff));
		}
	}
	FicPds.Close();

	return 0;
}


int CCibjob::LoadFile(LPCSTR FileName,DWORD position,int NbIndiv, int NbVentil)
{
	int v;
//	CreateFileCIBJOB(NbIndiv);
//	CreateFilePDS(NbIndiv);

	FreeFile();

	CStdioFile FicCibJob;
	CJFCFileException e;
	if(!FicCibJob.Open(FileName,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite,&e) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",FileName);
		e.AfficheErreur(MsgErr);
		return(1); // Erreur nr 1
	}

	FicCibJob.Seek(position,CFile::begin);
	m_NbIndiv=NbIndiv;
	m_NbVentil=NbVentil;
	m_CoeffExCible=new long[m_NbIndiv];
	m_CoeffExEnsemble=new long[m_NbIndiv];
	m_NrRegion=new char[m_NbIndiv];
	m_NrModalite=new char*[m_NbVentil];
	for(v=0;v<m_NbVentil;v++)
		m_NrModalite[v]=new char[m_NbIndiv];

	FicCibJob.Read(m_CoeffExCible,sizeof(long)*m_NbIndiv);
	FicCibJob.Read(m_CoeffExEnsemble,sizeof(long)*m_NbIndiv);
	FicCibJob.Read(m_NrRegion,sizeof(char)*m_NbIndiv);
	for(v=0;v<m_NbVentil;v++)
		FicCibJob.Read(m_NrModalite[v],sizeof(char)*m_NbIndiv);
	FicCibJob.Close();

	m_FileLoaded=1;
	return (0); // OK, pas d'erreur
}

void CCibjob::FreeFile()
{
	if(!m_FileLoaded)return;

	delete [] m_CoeffExCible;
	delete [] m_CoeffExEnsemble;
	delete [] m_NrRegion;
	for(int v=0;v<m_NbVentil;v++)
		delete [] m_NrModalite[v];
	delete [] m_NrModalite;

	m_NbIndiv=0;
	m_NbVentil=0;
	m_FileLoaded=0;
}
