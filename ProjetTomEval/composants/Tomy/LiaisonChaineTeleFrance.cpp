// LiaisonChaineHertzienneThematique.cpp: implementation of the CLiaisonChaineTeleFrance class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LiaisonChaineTeleFrance.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3D9AB7D50388
CLiaisonChaineTeleFrance::CLiaisonChaineTeleFrance()
{
	m_fLoaded=0;


}

//##ModelId=3D9AB7D50389
CLiaisonChaineTeleFrance::~CLiaisonChaineTeleFrance()
{

}


// recherche d'une chaîne hertzienne à partir d'une chaîne cabsat.
// retroune 0 si la chaîne cabsat n'existe pas dans l'hertzien
//##ModelId=3D9AB7D50386
short CLiaisonChaineTeleFrance::GetNumeroMediadata(short NrStationMediacabsat)
{
	short NrStationMediadata;
	if(m_MapStation_MediacabsatToMediadata.Lookup(NrStationMediacabsat,NrStationMediadata))
	{
		return NrStationMediadata;
	}
	return 0;
}

short CLiaisonChaineTeleFrance::GetNumeroMediacabsat(short NrStationMediadata)
{
	short NrStationMediacabsat;
	if(m_MapStation_MediadataToMediacabsat.Lookup(NrStationMediadata,NrStationMediacabsat))
	{
		return  NrStationMediacabsat;
	}
	return(0);
}

bool CLiaisonChaineTeleFrance::IsChaineHertzienne(short NrStationMediadata)
{
	short NrStationMediacabsat;
	if(m_MapStation_MediadataToMediacabsat.Lookup(NrStationMediadata,NrStationMediacabsat))
	{
		return true;
	}
	return false;
}

bool CLiaisonChaineTeleFrance::IsChaineThematique(short NrStationMediadata)
{
	return(!IsChaineHertzienne(NrStationMediadata));
}

short CLiaisonChaineTeleFrance::LoadFile(LPCSTR NomFic)
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

	// Recherche du nombre avec *
	int NbLiaison=0;
	while(1)
	{
		if(!f.ReadString(txt))break;
		txt.TrimRight();
		if(txt.GetLength()<=0)break;
		if(txt.GetAt(0)=='*')
		{
			NbLiaison=atoi(txt.Mid(1));
			break;
		}
	}

	// Recherche du début avec le #
	while(1)
	{
		if(!f.ReadString(txt))break;
		txt.TrimRight();
		if(txt.GetLength()<=0)continue;
		if(txt.GetAt(0)=='#')break;
	}

	for(int l=0;l<NbLiaison;l++)
	{
		if(!f.ReadString(txt))break;
		txt.TrimRight();
		if(txt.GetLength()<=0)return 0; // Erreur

		short nr1,nr2;
		nr1=atoi(txt.Left(5));
		nr2=atoi(txt.Mid(5));
		if((nr1>0) && (nr2>0))
		m_MapStation_MediadataToMediacabsat.SetAt(nr1,nr2);
		m_MapStation_MediacabsatToMediadata.SetAt(nr2,nr1);
	}
	

	f.Close();
	m_fLoaded=true;
	m_Nomfich=NomFic;
	return (1); //OK
}
