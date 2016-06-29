// FicVague.cpp: implementation of the CFicVague class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ficvague.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFicVague::CFicVague()
{	
	m_Fic.SetLineFormat(TAILLEVAGUECODE, TAILLEVAGUELIBEL, TAILLEVAGUEOCCUP, TAILLEVAGUENBINDIV);
}

CFicVague::~CFicVague()
{	
	FreeRecord();
}

//////////////////////////////////////////////////////////////////////
// Membres
//////////////////////////////////////////////////////////////////////

//Lecture d'un fichier et chargement des enregistrements
//retourne une erreur ou 0
short CFicVague::LoadFile(LPCSTR aNomFic)
{	
	short i, NbRec;
	struct Record * R;

	//Ouvre le fichier
	m_Vague.SetSize(0);
	if(m_Fic.OpenFile(aNomFic, 'r'))	return(1);
	
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
	m_Vague.SetSize(NbRec);

	//Recupere les enregistrement
	for(i=0 ; i<NbRec ; i++)
	{	//recupere l'enregistrement
		if(m_Fic.ReadLine())
		{
			m_Vague.SetSize(i-1);
			m_Fic.CloseFile();
			return(1);
		}
		//Recupere les differents champs de l'enregistrement
		R=new Record;
		strcpy(R->Code, m_Fic.GetCharPtr(0));			//lecture du code
		strcpy(R->Libelle, m_Fic.GetCharPtr(1));		//lecture du libelle
		R->Occupe=m_Fic.GetChar(2);			//lecture du champ occupe
		R->NbIndividu=m_Fic.GetShort(3);		//lecture du nb d'individu
		
		//copie la structure dans le pointeur d'array
		m_Vague[i]=(void *)R;
	}

	//Ferme le fichier
	m_Fic.CloseFile();
	return(0);
}

//Ajout d'un enregistrement a l'emplacement souhaite par aPosRec
//retourne une erreur ou 0
short CFicVague::AddRecord(LPCSTR aNomFic, char * aCode, char * aLibelle, char aOccupe, long aNbIndiv, char fsave)
{	
	struct Record * R;

	R=new Record;
	strcpy(R->Code, aCode);
	strcpy(R->Libelle, aLibelle);
	R->Occupe=aOccupe;
	R->NbIndividu=aNbIndiv;
	
	//copie la structure dans le pointeur d'array
	m_Vague.Add((void *)R);
	if(fsave)	SaveFile(aNomFic);
	return(0);
}

//Recupere la liste des vague
//retourne une erreur ou 0
short CFicVague::GetListVague(CStringArray & aListVague)
{	short i, NbRec;
	struct Record * R;

	//Alloue de l'espace au pointeur d'array
	NbRec=m_Vague.GetSize();
	aListVague.SetSize(NbRec);

	//copie la liste des vague dans l'argument
	for(i=0 ; i<NbRec ; i++)
	{	R=(Record *)m_Vague[i];
		aListVague[i]=R->Libelle;
	}

	return(NbRec);
}

//Recupere le nombre d'undividu pour une vague donnee
//retourne le nb d'individu trouve ou 0
long CFicVague::GetNbIndiv(LPCSTR aCode)
{	
	int i, NbRec;
	struct Record * R;

	//Alloue de l'espace au pointeur d'array
	NbRec=m_Vague.GetSize();

	//retourne le nb d'individu pour la vague specifie par aCode
	for(i=0 ; i<NbRec ; i++)
	{	
		R=(Record *)m_Vague[i];
		if(!strcmp(R->Code, aCode))
			return(R->NbIndividu);
	}
	
	return(0);
}


short CFicVague::GetNrIndex(LPCSTR aCode){
	short i, NbRec;
	struct Record * R;

	//Alloue de l'espace au pointeur d'array
	NbRec=m_Vague.GetSize();

	//retourne le nb d'individu pour la vague specifie par aCode
	for(i=0 ; i<NbRec ; i++)
	{	
		R=(Record *)m_Vague[i];
		if(!strcmp(R->Code, aCode))
			return(i);
	}
	
	return(0);


}



//Recupere le nombre d'undividu pour une vague donnee
//retourne le nb d'individu trouve ou 0
long CFicVague::GetNbIndiv(short NrIndexVague)
{	int NbRec;
	struct Record * R;

	NbRec=m_Vague.GetSize();
	if(NrIndexVague>=NbRec){
		AfficheErreur("Erreur dans le numéro de la vague");
		return(0);
	}
	R=(Record *)m_Vague[NrIndexVague];
	if(!R->Occupe){
		AfficheErreur("Cette vague n'est pas autorisée");
		return(0);
	}
	return(R->NbIndividu);
}

//Recupere le code d'une vague
LPCSTR CFicVague::GetCodeVague(short NrIndexVague)
{	
	int NbRec;
	struct Record * R;

	NbRec=m_Vague.GetSize();
	if(NrIndexVague>=NbRec){
		AfficheErreur("Erreur dans le numéro de la vague");
		return(0);
	}
	R=(Record *)m_Vague[NrIndexVague];
	if(!R->Occupe){
		AfficheErreur("Cette vague n'est pas autorisée");
		return(0);
	}
	return(R->Code);
}

//Recupere le code pour une source donnee
//retourne le code ou 0 si pas trouve
LPTSTR CFicVague::GetVagueCode(LPCSTR aNom)
{	
	short i, NbRec;
	struct Record * R;

	NbRec=m_Vague.GetSize();

	//retourne le nb d'individu pour la vague specifie par aCode
	for(i=0 ; i<NbRec ; i++)
	{	R=(Record *)m_Vague[i];
		if(!strcmp(R->Libelle, aNom))
			return(R->Code);
	}
	
	return(0);
}

short CFicVague::SaveFile(LPCSTR aNomFic)
{	
	int i, NbRec;
	struct Record * R;
	LPTSTR ligne;
	
	ligne=new char[TAILLEBUFFLINE];
	//Ouvre le fichier en ecriture
	if(m_Fic.OpenFile(aNomFic, 'w'))	return(1);

	//Ecrit l'* avec le nb de rec
	NbRec=m_Vague.GetSize();
	m_Fic.WriteStar(NbRec);

	//Ecrit le #
	m_Fic.WriteHash();
	
	//Ecrit les enregistrements precedent si existent
	for(i=0 ; i<NbRec ; i++)
	{	
		R=(Record *)m_Vague[i];
		//Ecrit les champs dans le buffer ligne
		sprintf(ligne, "%-4s %-30s %c %-5d", R->Code, R->Libelle, R->Occupe, R->NbIndividu);
		m_Fic.WriteLine(ligne);
	}
	delete [] ligne;

	//Ferme le fichier
	m_Fic.CloseFile();

	return(0);
}

void CFicVague::FreeRecord()
{	
	short NbRec=m_Vague.GetSize();
	for(short i=0;i<NbRec;i++)delete [] m_Vague[i];

	m_Vague.RemoveAll();
}

short CFicVague::GetNbVague()
{
	return(m_Vague.GetSize());
}
