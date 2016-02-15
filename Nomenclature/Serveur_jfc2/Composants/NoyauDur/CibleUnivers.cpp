// CibleUnivers.cpp: implementation of the CCibleUnivers class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CibleUnivers.h"
#include "JFCFileException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCibleUnivers::CCibleUnivers()
{
	m_TableCibles.RemoveAll();
	m_Marche=_T("");
	m_Univers=_T("");
	m_CheminEtudIni=_T("");
	m_CheminUnivers=_T("");
	m_CheminUser=_T("");
	m_Init=false;
}

CCibleUnivers::CCibleUnivers(CString Chemin, CString Marche)
{
	CString msg;
	CFileStatus status;
	m_Init=false;
	if(!CFile::GetStatus(Chemin, status))   // static function
	{
		msg.Format("Initialisation du Noyau\nErreur ds le chemin %s.", Chemin);
		return;
	}
	else m_CheminEtudIni=Chemin;
	m_Marche=Marche;
	m_TableCibles.RemoveAll();
	m_Univers=_T("");
	m_CheminUnivers=_T("");
	m_CheminUser=_T("");
	m_Init=true;
}

CCibleUnivers::~CCibleUnivers()
{
}

bool CCibleUnivers::Initialisation(CString Chemin, CString Marche)
{
	if(m_Init)	return m_Init;
	CString msg;
	CFileStatus status;
	if(!CFile::GetStatus(Chemin, status))   // static function
    {
		msg.Format("Initialisation du Noyau\nErreur ds le chemin %s.", Chemin);
		return m_Init;
    }
	else m_CheminEtudIni=Chemin;

	m_Marche=Marche;
	m_Init=TRUE;
	return m_Init;
}

CTableCibleArray &CCibleUnivers::DonneTableCibles(int Univers)
{
	if(!m_Init)	return m_TableCibles;
	CWaitCursor wait;
	m_Univers.Format("U%s0%d", m_Marche, Univers);
	InitCibles();
	m_TableCibles.RemoveAll();
	ChargerCibles('s');	//cible standard
	ChargerCibles('c');	//cible construite
	return(m_TableCibles); // L'application doit libérer cette table, car elle est recréée à chaque appel
}

void CCibleUnivers::LibererCibles()
{
	m_TableCibles.RemoveAll();
	m_Marche=_T("");
	m_Univers=_T("");
	m_CheminEtudIni=_T("");
	m_CheminUnivers=_T("");
	m_CheminUser=_T("");
	m_Init=false;
}

short CCibleUnivers::ChargerCibles(char Type)
{
	CString str;
	CStdioFile Fic;
	CJFCFileException e;

	switch(Type)
	{
	case 's':
		str.Format("%s\\%s\\%sCB%c.CSV",m_CheminUnivers,m_Univers,m_Univers,Type);
		break;
	case 'c':
		str.Format("%s\\%s\\%sCB%c.CSV",m_CheminUser,m_Univers,m_Univers,Type);
		break;
	}
	if(!Fic.Open(str,CFile::modeRead, &e))
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s (%d)",str,e.m_cause);
		e.AfficheErreur(MsgErr);
		return(1);
	}

	CMap <short, short, bool,bool> Selection;
	Fic.SeekToBegin();
	while(Fic.ReadString(str))
	{
		bool x;
		CTableCIBLE Temp;
		Temp.m_Numero=(short)atoi(GetPtrPWOFic(str, ',', ',', 1));
		if(Selection.Lookup(Temp.m_Numero, x))	continue;
		else Selection.SetAt(Temp.m_Numero, true);
		Temp.m_NrSource=(short)atoi(GetPtrPWOFic(str, ',', ',', 1));
		Temp.m_NrVague=(short)atoi(GetPtrPWOFic(str, ',', ',', 1));
		Temp.m_Occupe=(short)atoi(GetPtrPWOFic(str, ',', ',', 1));
		Temp.m_temoin=(short)atoi(GetPtrPWOFic(str, ',', ',', 2));
		Temp.m_Nom=GetPtrPWOFic(str, '\"', ',', 2);
		Temp.m_Blob=GetPtrPWOFic(str, '\"', '\"', 0);
		if(Temp.m_Occupe) m_TableCibles.Add(Temp);
	}
	Fic.Close();
	return(0);
}

void CCibleUnivers::InitCibles(void)
{
	// Chemins Data,Univers,Zone,Region
	char * buf=new char[256];
	GetPrivateProfileString(_INI_Local,_INI_User,"user",buf,256,m_CheminEtudIni);
	m_CheminUser.Format("%s", buf);
	memset(buf, 0, sizeof(buf));

	GetPrivateProfileString(_INI_Local,_INI_Univers,"univers",buf,256,m_CheminEtudIni);
	m_CheminUnivers.Format("%s", buf);
	memset(buf, 0, sizeof(buf));
	delete buf;
}