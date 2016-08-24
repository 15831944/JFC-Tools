//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CFichierCinema.h"
#include "JFCFileException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//IMPLEMENT_DYNAMIC(CFichierCinema,CStdioFile)

extern CString GCheminAccesDonnee;

CFichierCinema::CFichierCinema()
{
	m_Chemin=GCheminAccesDonnee+"\\DATA";
	m_separator = "|";
	m_delimitor="#";

	/*pp * app = AfxGetApp();

	m_Chemin=app->GetProfileString("Configuration", "CheminDonnees", m_Chemin);
	WriteProfileString("Configuration", "CheminDonnees", m_Chemin);
	*/

}

CFichierCinema::~CFichierCinema()
{
}

bool CFichierCinema::FileExist(LPCSTR NomFic)
{
	CJFCFileException e;
	CString nom;

	nom.Format("%s\\%s",m_Chemin,NomFic);

	if(!m_File.Open(nom, CFile::modeRead|CFile::typeText, &e ))   
	{
		return (0);
	}
	return (1);
}


bool CFichierCinema::OpenFileForRead(LPCSTR NomFic,int messageErreur)
{
	CJFCFileException e;
	CString nom;

	nom.Format("%s\\%s",m_Chemin,NomFic);
	// Chargement du fichier
	if(!m_File.Open(nom, CFile::modeRead|CFile::typeText|CFile::shareDenyWrite, &e ))   
	{
		if(messageErreur)
		{
			CString msg;
			msg.Format("Impossible d'ouvrir %s : ",nom);
			e.AfficheErreur(msg);
		}
		return (0);
	}
	return (1);
}
void CFichierCinema::SetSeparator(CString separateur)
{
	m_separator = separateur;
}
LPCSTR CFichierCinema::GetElement(int position)
{
	return(m_TabElement[position]);
}

// retourne 1 si ok
int CFichierCinema::ReadLine()
{
	int retour=1;
	CString txt;
	m_TabElement.SetSize(0,100);


	// Lecture de la ligne dans le fichier
	m_File.ReadString(txt);


	// Attention si ligne nulle
	if (txt == "") return 0;

	// une ligne blanche  ou vide  ?
	if(m_delimitor[0]==' ')
	{
		if((txt.GetLength()==0) ||(txt[0]==' '))
		{
			return(0);
		}
	}
	else
	{
		if(txt[0]==m_delimitor[0])return(0);
	}

	// Rangement des éléments dans le tableau des éléments
	int pos;
	do
	{
		// Recherche du séparateur
		pos=txt.Find(m_separator);

		if(pos>0)
		{
			m_TabElement.Add(txt.Left(pos));
			txt=txt.Mid(pos+1);
			int size = m_TabElement.GetSize(); 
		}
		else
		{
			m_TabElement.Add(txt);
			break;
		}
	} while (1);

	return(retour);
}







bool CFichierCinema::OpenFileForWrite(LPCSTR NomFic)
{
	CJFCFileException e;
	CString txt;
	CString nom;
	nom.Format("%s\\%s",m_Chemin,NomFic);
	if(!m_File.Open(nom,CFile::modeCreate | CFile::modeWrite | CFile::typeText|CFile::shareExclusive,&e))
	{
		txt.Format("Impossible de créer le fichier: %s : ",nom);
		e.AfficheErreur(txt);
		return (0);	
	}
	return(1); 
}

bool CFichierCinema::WriteLine(LPCSTR str)
{
	CString txt;
	
	try{
		m_File.WriteString(str);
	}
	catch(CJFCFileException e)
	{
		txt.Format("Impossible d'écrire une ligne dans le fichier");
		e.AfficheErreur(txt);
		return(0);
	}
	return(1);
}

void CFichierCinema::Close()
{
	m_File.Close();
}
// définit le répertoire dans lesquel se trouve le fichier
void CFichierCinema::SetChemin(LPCSTR chemin)
{
	m_Chemin = chemin;
}

// délimiteur de fin de fichier
void CFichierCinema::SetDelimitor(CString delimiteur)
{
	m_delimitor = delimiteur;
}
