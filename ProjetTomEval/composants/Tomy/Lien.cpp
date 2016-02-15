// Lien.cpp: implementation of the CLien class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lien.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3D9AB7D50343
CLien::CLien()
{
	m_fLoaded=false;

}

//##ModelId=3D9AB7D5034A
CLien::~CLien()
{

}

//##ModelId=3D9AB7D5034C
short CLien::LoadFile(LPCSTR NomFic)
{
	if(m_fLoaded)return(0);
	CStdioFile f;
	CJFCFileException e;
	CString txt;
	char t1[100],t2[100];

	// Ouverture du fichier de convertion
	if( !f.Open(NomFic, CFile::modeRead | CFile::typeText, &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",NomFic);
		e.AfficheErreur(MsgErr);
		return(0);
	}

	while(1)
	{
		if(!f.ReadString(txt))break;
		txt.TrimRight();
		if(txt.GetLength()<=0)break;
		if(txt.GetAt(0)=='#')break;
		sscanf(txt,"%s %s",t1,t2);
		int nr1=atoi(t1);
		int nr2=atoi(t2);

		m_Lien.SetAt(nr1,nr2);
	}

	f.Close();
	m_fLoaded=true;
	m_Nomfich=NomFic;
	return (1); //OK
}


//##ModelId=3D9AB7D50340
bool CLien::GetLien(int NumeroRecherche, int &NumeroTrouve)
{
	
	return(m_Lien.Lookup(NumeroRecherche,NumeroTrouve)!=0);

}

//##ModelId=3D9AB7D50336
LPCSTR CLien::GetNomFichLien()
{
	return(m_Nomfich);

}
