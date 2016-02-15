// TablePopulationEtalon.cpp: implementation of the CTablePopulationEtalon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TablePopulationEtalon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTablePopulationEtalon::CTablePopulationEtalon()
{
	m_fLoaded=false;

}

CTablePopulationEtalon::~CTablePopulationEtalon()
{

}

short CTablePopulationEtalon::LoadFile(LPCSTR NomFic)
{
	if(m_fLoaded)return(0);
	CStdioFile f;
	CJFCFileException e;
	CString txt;

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
		int nr1;
		int nr2;
		int pos=txt.Find(',');
		nr1=atoi(txt.Left(pos));
		txt=txt.Mid(pos+1);
		pos=txt.Find(',');
		nr2=atoi(txt.Left(pos));
		m_Lien.SetAt(nr1,nr2);
	}

	f.Close();
	m_fLoaded=true;
	m_Nomfich=NomFic;
	return (1); //OK
}

bool CTablePopulationEtalon::GetPopulation(int NumeroRecherche, int &NumeroTrouve)
{
	
	return(m_Lien.Lookup(NumeroRecherche,NumeroTrouve)!=0);

}

LPCSTR CTablePopulationEtalon::GetNomFichLien()
{
	return(m_Nomfich);

}
