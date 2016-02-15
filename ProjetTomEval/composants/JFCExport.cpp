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
	m_FileName=_T("");
	m_Type=0;
	m_Separator='\t';
	m_pFile=NULL;
	m_Precision=2;
}

CJFCExport::~CJFCExport()
{
	CloseJFCExport();
}

int CJFCExport::DoJFCExport()
{
	//Ouvrir la boite de dialogue pour le choix du fichier, et du séparateur (';' par défaut)
	char BASED_CODE szFilter[] = "Fichiers export (*.csv)|*.csv|Fichiers text (*.txt)|*.txt|All Files (*.*)|*.*||";
	char BASED_CODE szExtension[] = "txt";
	CFileDialog dlg(false, szExtension, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, NULL);
	if(dlg.DoModal()==IDCANCEL)	return(0);
	
	m_FileName=dlg.GetPathName();
	if(dlg.GetFileExt()=="csv")m_Separator=';';

	return(1);
}


// Note de rémi: 
// Cette fonction est conservée pour des raisons de compatibilités
// Elle n'est a priori plus utilisée
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
	m_FileName=File;
	if(!File)
	{
		char BASED_CODE szFilter[] = "Fichier export (*.csv)|*.csv|Fichier texte (*.txt)|*.txt|Tous les fichiers (*.*)|*.*||";
		char BASED_CODE szExtension[] = "csv";
		CFileDialog dlg(false, szExtension, NULL, OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, NULL);
		if(IDCANCEL==dlg.DoModal())
			return(0);
		m_FileName=dlg.GetPathName();
	}
	CFileStatus rStatus;
	if(!Erase && CFile::GetStatus(m_FileName, rStatus)==FALSE)
	{
		Str.Format("Export JFC...\nLe fichier %s n'est pas valide.", m_FileName);
		AfxMessageBox(Str);
		return(0);
	}
	return(1);
}

bool CJFCExport::OpenJFCExport(bool flagPressePapiers)
{
	if(m_pFile)	return false;
	CString Str;
	CJFCFileException Exception;

	m_ExportPressePapiers=flagPressePapiers;
	if(flagPressePapiers)
	{
		// Créer un memfile
		m_pFile=new CSharedFile;
	}
	else
	{
		// Créer un vrai CFile
		m_pFile=new CFile;

		//ouvrir le fichier
		if(m_pFile->Open(m_FileName, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone, &Exception)==0)
		{
			Str.Format("Export JFC...\nLa création du fichier %s  a provoqué l'erreur suivante:",m_FileName);
			Exception.AfficheErreur(Str);
			return(false);
		}
	}
	return true;
}

bool CJFCExport::CloseJFCExport(void)
{
	//fermer le fichier
	if(!m_pFile)	return false;

	if(m_ExportPressePapiers)
	{
		char fin=0;
		m_pFile->Write(&fin,1);

		HGLOBAL hData = ((CSharedFile *)m_pFile)->Detach();
		if (OpenClipboard(NULL))
		{
			EmptyClipboard();
			::SetClipboardData(CF_TEXT, hData);
			CloseClipboard();
		}
		else
			AfxMessageBox("Erreur dans l'export: Impossible d'ouvrir le presse-papiers");
	}
	else
	{
		m_pFile->Close();
	}

	delete m_pFile;
	m_pFile=NULL;
	return true;
}

bool CJFCExport::Write(LPCTSTR Str)
{
	if(!m_pFile)	return false;
	if(!Str)	return false;
	short n=GetStrSize(Str);
//	if(!n)	return false; // Modif rémi pour écrire ""
	char * buf=new char[n+2];
	memcpy(buf, Str, n);
	buf[n]=m_Separator;
	buf[n+1]=0;
	try{
		m_pFile->Write(buf, n+1);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	if(buf)	delete [] buf;
	return true;
}

bool CJFCExport::Write(const TCHAR Car)
{
	if(!m_pFile)	return false;

	if(Car<32)
	{
		// Le caractère '\n' déclenche un saut de ligne
		if(Car=='\n')WriteCRLF();
		else ASSERT(0);
		return true;
	}

	char buf[3];
	buf[0]=Car;	buf[1]=m_Separator;	buf[2]=0;
	try{
		m_pFile->Write(buf,2);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::Write(const int Nb, LPCTSTR Type)
{
	if(!m_pFile)	return false;
	char buf[20+1];
	LPCTSTR tmp;
	short n=0, ns=0;
	memset(buf, 0, 21);
	
	_itoa(Nb, buf, 10);
	n=GetStrSize(buf);
	if(Type)
	{
		ns=GetStrSize(Type);
		memcpy(buf, tmp=SetType(buf, Type), n+ns);
		if(tmp) delete (LPSTR)tmp;
		n+=ns;
	}
	buf[n]=m_Separator;	buf[n+1]=0;
	try{
		m_pFile->Write(buf,n+1);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::Write(const long Nb,LPCTSTR Type)
{
	if(!m_pFile)	return false;
	char buf[20+1];
	LPCTSTR tmp;
	short n=0, ns=0;
	memset(buf, 0, 21);
	
	_ltoa(Nb, buf, 10);
	n=GetStrSize(buf);
	if(Type)
	{
		ns=GetStrSize(Type);
		memcpy(buf, tmp=SetType(buf, Type), n+ns);
		if(tmp) delete (LPSTR)tmp;
		n+=ns;
	}
	buf[n]=m_Separator;	buf[n+1]=0;
	try{
		m_pFile->Write(buf,n+1);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::Write(const double Nb, const short Precision, LPCTSTR Type)
{
	if(!m_pFile)	return false;
	char * buff;	//les 20chiffres + la virgule à rajouter + le 0 de fin
	char buf[20+1];
	LPCTSTR tmp;
	int dec, sign;
	short n=0, ns=0;
	memset(buf, 0, 21);

	int Pre=(Precision<0)?m_Precision:Precision;
	
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
	if(Type)
	{
		ns=GetStrSize(Type);
		memcpy(buf, tmp=SetType(buf, Type), n+ns);
		if(tmp) delete (LPSTR)tmp;
		n+=ns;
	}
	buf[n]=m_Separator;	buf[n+1]=0;
	try{
		m_pFile->Write(buf,n+1);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::Write(const struct tm * time)
{
	if(!m_pFile)	return false;
	if(!time)	return false;
	char buf[64+1];
	memset(buf, 0, 64);
	size_t nCar;
	if((nCar=strftime(buf, sizeof(buf), "%d/%m/%Y", time))==0)	return false;
	buf[nCar]=m_Separator;
	try{
		m_pFile->Write(buf,nCar+1);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::Write(const COleDateTime * time)
{
	if(!m_pFile)	return false;
	CString ptime;
	LPCTSTR buf;
	ptime=time->Format("%d/%m/%Y");
	ptime+=m_Separator;
	buf=(LPCTSTR)ptime;
	try{
		m_pFile->Write(buf,ptime.GetLength()+1);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::WriteSeparator(const short nbSep)
{
	if(!m_pFile)	return false;
	if(nbSep<=0)	return false;
	char sep[2];
	sep[0]=m_Separator;	sep[1]=0;
	try{
		for(short i=0 ; i<nbSep ; i++)	m_pFile->Write(sep,1);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
}

bool CJFCExport::WriteCRLF(const short nbLF)
{
	if(!m_pFile)	return false;
	if(nbLF<=0)		return false;
/*	
	// Avant modif remi 14/02/2002, c'était comme ça,
	// Mais le fichier n'était pas pas dans un format texte !!!
	char lf[2];
	lf[0]=JFCEX_CR;	lf[1]=0;
	int nbcar=1;
*/
	int nbcar=2; // \r\n
	char lf[]={"\r\n"};

	try{
		for(short i=0 ; i<nbLF ; i++)	m_pFile->Write(lf,nbcar);
	}
	catch(CJFCFileException * e){
		e->AfficheErreur();
		e->Delete();
	}
	return true;
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



bool CJFCExport::OpenJFCExport(LPCSTR NomFic, char separator)
{
	m_FileName=NomFic;
	m_Separator=separator;
	return(OpenJFCExport());
}
