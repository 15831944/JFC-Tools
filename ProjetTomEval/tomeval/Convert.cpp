// Convert.cpp: implementation of the CConvert class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tomeval.h"
#include "Convert.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConvert::CConvert()
{
	m_fLoaded=false;
}

CConvert::~CConvert()
{

}

short CConvert::Load(LPCSTR Path)
{
	if(m_fLoaded)return(0);
	CStdioFile f;
	CFileException e;
	CString txt="",txt1,txt2;
	char t1[10],t2[10];
	SConvert C;

#ifdef OPTION_CARAT
	txt.Format("%s\\carat.cib",Path);
#endif

#ifdef OPTION_MEDIATOP
	txt.Format("%s\\mediatop.cib",Path);
#endif

	// Ouverture du fichier de convertion
	if(txt.GetLength()<=0)return(1);
	if( !f.Open(txt, CFile::modeRead | CFile::typeText, &e ) ){
		return(1);
	}

	while(1){
		if(!f.ReadString(txt))break;
		txt.TrimRight();
		if(txt.GetLength()<=0)break;
		if(txt.GetAt(0)=='#')break;
		sscanf(txt,"%s %s",t1,t2);
		int nr1=atoi(t1);
		int nr2=atoi(t2);

		strcpy(C.m_Libelle,t1);
		C.m_Numero=nr1;
		C.m_Convert=nr2;
		m_Data.Add(C);
	}

	f.Close();
	m_fLoaded=true;
	return (0);
}

int CConvert::GetNrCible(LPCSTR Lib)
{
	if(!m_fLoaded)return(-1);
	for(int i=0;i<m_Data.GetSize();i++){
		if(!strcmp(m_Data[i].m_Libelle,Lib))return(m_Data[i].m_Convert);
	}
	AfxMessageBox("Le numéro de la cible n'est pas trouvé dans la table de convertion.");
	return(1);
}

int CConvert::GetNrCible(int NrCible)
{
	if(!m_fLoaded)return(-1);
	for(int i=0;i<m_Data.GetSize();i++){
		if(m_Data[i].m_Numero==NrCible)return(m_Data[i].m_Convert);
	}
	AfxMessageBox("Le numéro de la cible n'est pas trouvé dans la table de convertion.");
	return(1);
}
