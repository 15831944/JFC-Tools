// Tomjob.cpp: implementation of the CTomjob class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tomjob.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3D9AB7D303BF
CTomjob::CTomjob()
{

}

//##ModelId=3D9AB7D303C0
CTomjob::~CTomjob()
{

}


//##ModelId=3D9AB7D303AB
bool CTomjob::LectureFichier(LPCSTR NomFic)
{
	if(!OuvertureFichier(NomFic))return 0;

#ifdef OPTION_CARAT
	return(LectureFichierCarat());
#elif OPTION_JFC
	return(LectureFichierJFC());
#else
Erreur: option non définie
#endif
}

//##ModelId=3D9AB7D40036
bool CTomjob::OuvertureFichier(LPCSTR NomFic)
{
	CJFCFileException e;
	CString txt;

	// Chargement du fichier
	if( !m_File.Open(NomFic, CFile::modeRead|CFile::typeText|CFile::shareDenyWrite, &e ) )   
	{
		txt.Format("Impossible d'ouvrir %s : ",NomFic);
		e.AfficheErreur(txt);
		return (0);
	}
	return (1);
}

//##ModelId=3D9AB7D303B6
bool CTomjob::LectureFichierMarginal(LPCSTR NomFic)
{
	if(!OuvertureFichier(NomFic))return 0;

	CString txt;

	// Le numéro de la source (inutile en télé)
	m_File.ReadString(txt);

#ifdef OPTION_CIMBELGE
	if(!LecturePlanFormatRadioCIM())return 0;
#elif OPTION_RUSSIE
	if(!LecturePlanFormatRadioCIM())return 0;
#else
	if(!LecturePlan())return 0;
#endif
	
	m_File.Close(); // fermeture du fichier
	
	return(1); //OK
}


//##ModelId=3D9AB7D303B5
bool CTomjob::LectureFichierJFC()
{
	int v;
	CString txt;

	// Nom du plan
	m_File.ReadString(txt);

	// Numéro de la cible,Nom de la cible
	ReadLine();
	m_NumeroCible=atoi(m_TabElement[0]);

	// Ligne blanche
	m_File.ReadString(txt);

	// TypeSuppU => Option 101 A VIRER
	m_File.ReadString(txt);

	// bêta
	ReadLine();
	m_Beta=atoi(m_TabElement[0]);
	m_Mini=m_Maxi=-1;
	
	// Nb de plans
	m_File.ReadString(txt);
	m_NbPlan=atoi(txt);

	// Nb de sources A VIRER
	m_File.ReadString(txt);

	// Position dans fichier CIBJOB (format Radio)
	m_File.ReadString(txt);
	m_PosFichierCIBJOB=atol(txt);

	// Nb de ventilations,numéro critère 1,...n
	ReadLine();
	m_NbVentilation=atoi(m_TabElement[0]);
	m_Ventilation.SetSize(m_NbVentilation);
	for(v=0;v<m_NbVentilation;v++)m_Ventilation[v]=atoi(m_TabElement[v+1]);

	// Ligne blanche
	m_File.ReadString(txt);


#ifdef OPTION_CIMBELGE
	// Nr des panels dans l'Atelier radio (not used)
	m_File.ReadString(txt);

	// Chemin et code vague
	m_File.ReadString(txt);
	int pos=txt.ReverseFind('\\');
	
	m_CheminData=txt.Left(pos);
	m_CodePanelMediadata=txt.Mid(pos+1);
#elif OPTION_RUSSIE
	// Nr des panels dans l'Atelier radio (not used)
	m_File.ReadString(txt);

	// Chemin et code vague
	m_File.ReadString(txt);
	int pos=txt.ReverseFind('\\');
	
	m_CheminData=txt.Left(pos);
	m_CodePanelMediadata=txt.Mid(pos+1);
#else
	// CODE PANEL MEDIADATA, CODE PANEL MEDIACABSAT
	ReadLine();
	m_CodePanelMediadata=m_TabElement[0];
	m_CodePanelMediaCabSat=m_TabElement[1];

	// Chemin des données
	m_File.ReadString(m_CheminData);
#endif

	// Population de la source de GRP
	ReadLine();
	m_Population=atoi(m_TabElement[0]);
	if(m_TabElement.GetSize()>=2)
		m_PopulationOffreElargie=atoi(m_TabElement[1]);
	else
		m_PopulationOffreElargie=0;

	// Nombre de cas UNIQUEMENT pour fichier CIBJOB
	m_File.ReadString(txt);
	m_NbIndiv=atoi(txt);

	// Nb de régions NOT USED
	m_File.ReadString(txt);
	int nbregion=atoi(txt);

	// Nr des régions NOT USED
	m_File.ReadString(txt);

	// Population de la source sur la région NOT USED
	m_File.ReadString(txt);

	// Population pour chacune des ventilations, et pour cacune des régions
	for(v=0;v<m_NbVentilation;v++)
	{
		for(int r=0;r<nbregion;r++)
		{
			m_File.ReadString(txt);
		}
	}

	// Ligne blanche
	m_File.ReadString(txt);

	// Le plan
#ifdef OPTION_CIMBELGE
	LecturePlanFormatRadioCIM();
#elif OPTION_RUSSIE
	LecturePlanFormatRadioCIM();
#else
	LecturePlan();
#endif

	m_File.Close(); // fermeture du fichier
	m_TabElement.RemoveAll(); //purge

	// Un petit test à virer
	CalculGRPDansJob(m_NbPlan-1);

	return (1);
}


	
//##ModelId=3D9AB7D4003C
bool CTomjob::LecturePlan()
{
	m_Plan.SetSize(0,100);
	do
	{
		ReadLine();
		if(m_TabElement.GetSize()<7)break;

		CSpotTomjob S;

		S.m_Station=atoi(m_TabElement[0]);
		S.m_horaire=atoi(m_TabElement[1]);
		S.m_JourN=GetIdxJourNomme(atoi(m_TabElement[2])); 
		S.m_GRP=atof(m_TabElement[5]);
		S.m_JourD=m_TabElement[6];
		for(int p=0;p<m_NbPlan;p++)
			S.m_NbInsertion.Add(atoi(m_TabElement[7+p]));

		m_Plan.Add(S);
	}while(1);
	return 1; //OK
}


/*
27,CIM
2,12-44 ans

2
-1
5
1
0
0

3,12,0
SOURCE\SBL06\JFC_S102
4666722
3222
10
1,2,10,11,12,20,21,22,23,24
2725838,1940884,480577,561603,311564,512817,619104,397529,374947,744954

1,1345,31,20021118,2,1,0.264261,2,1,0,0,0,1
1,1345,31,20021119,2,1,0.264261,2,1,0,0,0,1
2,1430,31,20021118,2,0,0.917211,2,0,1,0,0,1
2,1430,31,20021119,2,0,0.917211,2,0,1,0,0,1
4,1445,31,20021118,2,1,0.914944,2,0,0,1,0,1
4,1445,31,20021119,2,1,0.914944,2,0,0,1,0,1
5,1500,31,20021118,2,0,0.199085,2,0,0,0,1,1
5,1500,31,20021119,2,0,0.199085,2,0,0,0,1,1

257.81,257.81,257.81,257.81,1031.24
EOJ
*/

bool CTomjob::LecturePlanFormatRadioCIM()
{

	/*
	on dispose dans une ligne support de:
	1 - Numéro de station
	2 - Horaire
	3 - Jour nommé (1,2,4,8,16,32,64)
	4 - Jour (AAAAMMJJ)
	5 - Région
	6 - Ancrage
	7 - GRP
	8 - champs privé
	9+ - nombre d'instertions par plan

  exemple:

1,1345,31,20021118,2,1,0.264261,2,1,0,0,0,1
	*/
	m_Plan.SetSize(0,100);
	do
	{
		ReadLine();
		if(m_TabElement.GetSize()<7)break;

		CSpotTomjob S;

		S.m_Station=atoi(m_TabElement[0]);
		S.m_horaire=atoi(m_TabElement[1]);
		S.m_JourN=GetIdxJourNomme(atoi(m_TabElement[2])); 
		S.m_JourD=m_TabElement[3];
		S.m_OptionAncrage=(atoi(m_TabElement[5])==0)?0:1;
		S.m_GRP=atof(m_TabElement[6]);
		for(int p=0;p<m_NbPlan;p++)
			S.m_NbInsertion.Add(atoi(m_TabElement[8+p]));

		m_Plan.Add(S);
	}while(1);
	return 1; //OK
}

// Note: cette fonction utilise l'ancien format du fichier tomjob.
// Dans la meusure ou l'on dispose de temps pour refaire une implantation
// du moteur de calcul chez carat, il est alors envisageable de modifier
// le format du fichier tomjob de chez carat.
// les commentaires A VIRER et A MODIFIER signalent les choses à changer.
//##ModelId=3D9AB7D303AD
bool CTomjob::LectureFichierCarat()
{
	CString txt;

	// Nom du plan
	m_File.ReadString(txt);

	// Numéro de la cible,Nom de la cible
	ReadLine();
	m_NumeroCible=atoi(m_TabElement[0]);

	// Ligne blanche
	m_File.ReadString(txt);

	// TypeSuppU => Option 101 A VIRER
	m_File.ReadString(txt);

	// bêta,mini,maxi
	ReadLine();
	m_Beta=atoi(m_TabElement[0]);
	if(m_Beta>0)
	{
		m_Mini=atoi(m_TabElement[1]);
		m_Maxi=atoi(m_TabElement[2]);
	}
	else 
	{
		m_Mini=m_Maxi=-1;
	}
	

	// Nb de plans
	m_File.ReadString(txt);
	m_NbPlan=atoi(txt);

	// Nb de sources A VIRER
	m_File.ReadString(txt);

	// Nb de ventilations,numéro critère 1,...n
	ReadLine();
	m_NbVentilation=atoi(m_TabElement[0]);
	m_Ventilation.SetSize(m_NbVentilation);
	for(int v=0;v<m_NbVentilation;v++)m_Ventilation[v]=atoi(m_TabElement[v+1]);

	// Ligne blanche
	m_File.ReadString(txt);

	
	// Depuis version 3.1f
	// MEDIADATA:XXXX
	ReadLine();
	CString codeattendu="MEDIADATA:";
	int taille=codeattendu.GetLength();
	if(m_TabElement[0].Left(taille)!=codeattendu)
	{
		AfxMessageBox("Erreur dans le fichier de requête; Le code <MEDIADATA:> n'est pas trouvé !");
		return false;
	}
	m_CodePanelMediadata=m_TabElement[0].Mid(taille);

	// Depuis version 3.1f
	// MEDIACABSAT:XXXX
	ReadLine();
	codeattendu="MEDIACABSAT:";
	taille=codeattendu.GetLength();
	if(m_TabElement[0].Left(taille)!=codeattendu)
	{
		AfxMessageBox("Erreur dans le fichier de requête; Le code <MEDIACABSAT:> n'est pas trouvé !");
		return false;
	}
	m_CodePanelMediaCabSat=m_TabElement[0].Mid(taille);

	// Chemin des données
	m_File.ReadString(m_CheminData);

	// Population de la source de GRP
	m_File.ReadString(txt);
	m_Population=atoi(txt);

	// Nb de régions
	m_File.ReadString(txt);

	// Nr des régions
	m_File.ReadString(txt);

	// Ligne blanche
	m_File.ReadString(txt);

	// Le plan
	LecturePlan();

	m_File.Close(); // fermeture du fichier
	m_TabElement.RemoveAll(); //purge

	// Un petit test à virer
	CalculGRPDansJob(0);

	return (1);
}

// Cette fonction n'est plus utilisée car elle n'est pas très rapide
//##ModelId=3D9AB7D4006E
LPCSTR CTomjob::GetElement(LPCSTR Chaine,int position)
{
	static CString txt;
	int pos;

	txt=Chaine;
	for(int i=0;i<position;i++)
	{
		pos=txt.Find(',');
		if(pos>0)txt=txt.Mid(pos+1);
		else
		{
			ASSERT(0);
			txt="";
			break;
		}
	}
	pos=txt.Find(',');
	if(pos>0)txt=txt.Left(pos);
	return(txt);
}

// retourne 1 si ok
//##ModelId=3D9AB7D4004F
bool CTomjob::ReadLine()
{
	CString txt;
	m_TabElement.SetSize(0,10);
	
	// Lecture de la ligne dans le fichier
	if(!m_File.ReadString(txt))return(0);

	// un ligne blanche ou vide  ?
	if(txt.GetLength()==0)return(1);

	// Rangement des éléments dans le tableau des éléments
	int pos;
	do
	{
		// Recherche du séparateur
		pos=txt.Find(',');

		if(pos>0)
		{
			m_TabElement.Add(txt.Left(pos));
			txt=txt.Mid(pos+1);
		}
		else 
		{
			m_TabElement.Add(txt);
			break;
		}
	} while (1);

	return(1);
}


// Transformation des jours 1,2,4,8,16,32,64
// en indexes: 0,1,2,3,4,5,6
//##ModelId=3D9AB7D40045
int CTomjob::GetIdxJourNomme(int jour)
{
	if(jour==1)return(0);
	if(jour==2)return(1);
	if(jour==4)return(2);
	if(jour==8)return(3);
	if(jour==16)return(4);
	if(jour==32)return(5);
	if(jour==64)return(6);
	ASSERT(0);
	return(0);
}

double CTomjob::CalculGRPDansJob(int NrJob)
{
	double GRP=0;
	for(int i=0;i<m_Plan.GetSize();i++)
	{
		GRP+=m_Plan[i].m_GRP*m_Plan[i].m_NbInsertion[NrJob];
	}
	TRACE("GRP dans le job=%f\n",GRP);
	return GRP;
}
long CTomjob::CalculNbSpotDansJob(int NrJob)
{
	long NbSp=0;
	for(int i=0;i<m_Plan.GetSize();i++)
	{
		NbSp +=m_Plan[i].m_NbInsertion[NrJob];
	}
	TRACE("NbSpot dans le job=%f\n",NbSp);
	return NbSp;
}
