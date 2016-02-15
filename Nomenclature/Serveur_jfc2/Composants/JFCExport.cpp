// JFCExport.cpp: implementation of the CJFCExport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JFCExport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "JFCFileException.h"

CJFCExport::CJFCExport()
{
	m_File=_T("");
	m_Type=0;
	m_fFile=false;
	m_Separator=JFCEX_SEPARATOR;
}

CJFCExport::~CJFCExport()
{
	CloseJFCExport();
}

int CJFCExport::DoJFCExport(void)
{
	//Ouvrir la boite de dialogue pour le choix du fichier, et du séparateur (';' par défaut)
	char BASED_CODE szFilter[] = "Fichiers export (*.csv)|*.csv|Fichiers text (*.txt)|*.txt|All Files (*.*)|*.*||";
	CFileDialog dlg(false, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, NULL);
	if(dlg.DoModal()==IDCANCEL)	return(0);
	m_File=dlg.GetPathName();
	return(1);
}

int CJFCExport::DoJFCExport(int Type, LPCSTR File,	bool Erase)
{
	CString Str;
	switch(Type)
	{
	case JFCEX_TEXT:
		m_Type=Type;
		break;
	default:
		Str.Format("Export JFC...\nLe type %d de l'export n'est pas valide.", Type);
		AfxMessageBox(Str);
		return(0);
	}
	m_File=File;
	if(!File)
	{
		char BASED_CODE szFilter[] = "Fichier export (*.csv)|*.csv|Fichier texte (*.txt)|*.txt|Tous les fichiers (*.*)|*.*||";
		char BASED_CODE szExtension[] = "csv";
		CFileDialog dlg(false, szExtension, NULL, OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, NULL);
		if(IDCANCEL==dlg.DoModal())
			return(0);
		m_File=dlg.GetPathName();
	}
	CFileStatus rStatus;
	if(!Erase && CFile::GetStatus(m_File, rStatus)==FALSE)
	{
		Str.Format("Export JFC...\nLe fichier %s n'est pas valide.", m_File);
		AfxMessageBox(Str);
		return(0);
	}
	return(1);
}

bool CJFCExport::OpenJFCExport(void)
{
	if(m_fFile)	return false;
	CString Str;
	CJFCFileException Exception;
	//ouvrir le fichier
	if(m_pFile.Open(m_File, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone, &Exception)==0)
	{
		Str.Format("Export JFC...\nLa création du fichier %s  a provoqué l'erreur suivante:",m_File);
		Exception.AfficheErreur(Str);
		return(false);
	}
	m_fFile=true;
	return true;
}

bool CJFCExport::CloseJFCExport(void)
{
	//fermer le fichier
	if(!m_fFile)	return false;
	m_pFile.Close();
	m_fFile=false;
	return true;
}

bool CJFCExport::SetStr(LPCTSTR Str, TCHAR Sep)
{
	if(!m_fFile)	return false;
	if(!Str)	return false;
	short n=GetStrSize(Str);
	if(!n)	return false;
	char * buf=new char[n+2];
	memcpy(buf, Str, n);
	buf[n]=Sep;
	buf[n+1]=0;
	try{
		m_pFile.Write(buf, n+2);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	if(buf)	delete [] buf;
	return true;
}

bool CJFCExport::SetChar(const TCHAR Car, TCHAR Sep)
{
	if(!m_fFile)	return false;
	char buf[3];
	buf[0]=Car;	buf[1]=Sep;	buf[2]=0;
	try{
		m_pFile.Write(buf,3);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::SetInt(const int Nb, TCHAR Sep, TCHAR Sign, LPCTSTR Type)
{
	if(!m_fFile)	return false;
	char buf[20+1];
	LPCTSTR tmp;
	short n=0, ns=0;
	memset(buf, 0, 21);
	
	_itoa(Nb, buf, 10);
	n=GetStrSize(buf);
	if(Sign)
	{
		memcpy(buf, tmp=SetSign(buf, Sign), n+1);
		if(tmp) delete (LPSTR)tmp;
		n+=1;
	}
	if(Type)
	{
		ns=GetStrSize(Type);
		memcpy(buf, tmp=SetType(buf, Type), n+ns);
		if(tmp) delete (LPSTR)tmp;
		n+=ns;
	}
	buf[n]=Sep;	buf[n+1]=0;
	try{
		m_pFile.Write(buf,n+2);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::SetLong(const long Nb, TCHAR Sep, TCHAR Sign, LPCTSTR Type)
{
	if(!m_fFile)	return false;
	char buf[20+1];
	LPCTSTR tmp;
	short n=0, ns=0;
	memset(buf, 0, 21);
	
	_ltoa(Nb, buf, 10);
	n=GetStrSize(buf);
	if(Sign)
	{
		memcpy(buf, tmp=SetSign(buf, Sign), n);
		if(tmp) delete (LPSTR)tmp;
		n+=1;
	}
	if(Type)
	{
		ns=GetStrSize(Type);
		memcpy(buf, tmp=SetType(buf, Type), n+ns);
		if(tmp) delete (LPSTR)tmp;
		n+=ns;
	}
	buf[n]=Sep;	buf[n+1]=0;
	try{
		m_pFile.Write(buf,n+2);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::SetDouble(const double Nb, const short Pre, TCHAR Sep, TCHAR Sign, LPCTSTR Type)
{
	if(!m_fFile)	return false;
	char * buff;	//les 20chiffres + la virgule à rajouter + le 0 de fin
	char buf[20+1];
	LPCTSTR tmp;
	int dec, sign;
	short n=0, ns=0;
	memset(buf, 0, 21);
	
	buff=_fcvt(Nb, Pre, &dec, &sign);
	n=GetStrSize(buff);
	memcpy(buf, buff, n);
	if(!n){
		buf[0]='0';
		n++;
		dec=0;
	}
	if(Pre)
	{
		char str[32];
		memset(str,0,sizeof(str));
		short i=0, j=0;
		if(!dec)	//chiffre <1
			str[j++]='0';
		while(i<n)
		{
			if(i==dec){
				str[j++]=',';
				str[j++]=buf[i++];
			}
			else str[j++]=buf[i++];
		}
		if(!dec)	n++;
		memcpy(buf, str, ++n);
	}
	if(Sign)
	{
		memcpy(buf,tmp=SetSign(buf, Sign), n+1);
		if(tmp) delete (LPSTR)tmp;
		n+=1;
	}
	if(Type)
	{
		ns=GetStrSize(Type);
		memcpy(buf, tmp=SetType(buf, Type), n+ns);
		if(tmp) delete (LPSTR)tmp;
		n+=ns;
	}
	buf[n]=Sep;	buf[n+1]=0;
	try{
		m_pFile.Write(buf,n+2);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::SetDateTime(const struct tm * time, TCHAR sep)
{
	if(!m_fFile)	return false;
	if(!time)	return false;
	char buf[64+1];
	memset(buf, 0, 64);
	size_t nCar;
	if((nCar=strftime(buf, sizeof(buf), "%d/%m/%Y", time))==0)	return false;
	buf[nCar]=sep;
	try{
		m_pFile.Write(buf,nCar+2);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::SetDateTime(const COleDateTime * time, TCHAR sep)
{
	if(!m_fFile)	return false;
	CString ptime;
	LPCTSTR buf;
	ptime=time->Format("%d/%m/%Y");
	ptime+=sep;
	buf=(LPCTSTR)ptime;
	try{
		m_pFile.Write(buf,ptime.GetLength()+1);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::SetSeparator(const short nbSep, TCHAR Sep)
{
	if(!m_fFile)	return false;
	if(nbSep<=0)	return false;
	char sep[2];
	sep[0]=Sep;	sep[1]=0;
	try{
		for(short i=0 ; i<nbSep ; i++)	m_pFile.Write(sep,1);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::SetLineFeed(const short nbLF)
{
	if(!m_fFile)	return false;
	if(nbLF<=0)		return false;
	char lf[2];
	lf[0]=JFCEX_LF;	lf[1]=0;
	try{
		for(short i=0 ; i<nbLF ; i++)	m_pFile.Write(lf,1);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

LPCTSTR CJFCExport::SetSign(LPCTSTR Str, TCHAR Sign)
{
	if(!Str)	return NULL;
	if(!Sign)	return Str;
	char * buf;
	short n=GetStrSize(Str);
	buf=new char[n+1];
	buf[0]=Sign;
	n=0;
	while(Str[n]!=0)	buf[n+1]=Str[n++];
	return buf;
}

LPCTSTR CJFCExport::SetType(LPCTSTR Str, LPCTSTR Type)
{
	if(!Str)	return NULL;
	if(!Type)	return Str;
	char * buf;
	short n=GetStrSize(Str), m=GetStrSize(Type);
	buf=new char[n+m+1];
	memcpy(buf, Str, n);
	m=0;
	while(Type[m]!=0)	buf[n++]=Type[m++];
	buf[n]=0;
	return buf;
}

short CJFCExport::GetStrSize(LPCTSTR Str)
{
	if(!Str)	return 0;
	short n=0;
	while(Str[n]!=0)	n++;
	return n;
}
