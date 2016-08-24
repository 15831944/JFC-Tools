// TableGroupeBAC.cpp: implementation of the CTableGroupeBAC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableGroupeBAC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableGroupeBAC::CTableGroupeBAC()
{
	m_NomFich="groupebacs.table";
	SetName(m_NomFich);
	IsModified=0;
}

// copy constructor
CTableGroupeBAC::CTableGroupeBAC(const CTableGroupeBAC &Data)
{
	*this = Data;
};

CTableGroupeBAC::~CTableGroupeBAC()
{

}

// Chargement table des groupes de Bacs
bool CTableGroupeBAC::LoadTable(CString CheminData)
{
	RemoveAll();

	CFichierCinema Fichier;
	CStdioFile File;

	CFileFind finder;
	
	if(!finder.FindFile(CheminData + "\\DATA\\" + m_NomFich))
	{
		return (0); 
	}


	/*
	// Pour le fichier table groupe bacs, au d�part ce n'est pas une erreur si non existant
	if(!Fichier.FileExist(m_NomFich))   
	{
		AfxMessageBox("Le fichier des groupes de Bacs n'existent pas encore",MB_OK | MB_APPLMODAL);   
		return 0;
	}
	*/

	// Il existe on essaie de l'ouvrir en lecture 
	if (!Fichier.OpenFileForRead(m_NomFich, 1))
	{
		return (0);
	}
	
	bool message=0;
	CString ChaineMessage;

	// On lit tous les groupes bacs que l'on place dans la table des groupe Bacs
	while(Fichier.ReadLine())
	{
		int PosElmt = 0;

		// enregistrement groupe bac
		tagBACGroupe enr;

		// On stocke d'abord le libell�
		enr.m_LibelleGroupe = Fichier.GetElement(PosElmt);

		// le libell� code est il valide ?
		if(enr.m_LibelleGroupe.GetLength() > 30)
		{
			ChaineMessage+="\nLe libell� est sup�rieur � 30 caract�res";
			enr.m_LibelleGroupe = enr.m_LibelleGroupe.Left(30);
		}

		// On enl�ve les espaces se trouvant apr�s le libell�
		enr.m_LibelleGroupe.TrimRight();


		// Stocke le nombre de bacs
		PosElmt++;
		enr.m_NbBacs = atoi(Fichier.GetElement(PosElmt));
		if(enr.m_NbBacs <= 0)
		{
			message=1;
			ChaineMessage="\nNombre de bacs = 0 incorrect";
			break;
		}

		// Taille le tableau des bacs
		enr.m_CompoBacs.SetSize(enr.m_NbBacs);

		// Stocke le tableau des composants bacs
		for (int IdxBac = 0; IdxBac < enr.m_NbBacs; IdxBac++)
		{
			PosElmt++;

			// Code bac
			CString StrCodeBac = Fichier.GetElement(PosElmt);
			if(StrCodeBac.GetLength() > 7 || StrCodeBac.GetLength() < 3)
			{
				message=1;
				ChaineMessage="\nNombre de bacs = 0 incorrect";
				break;
			}

			// Vire le post-code "B"
			StrCodeBac = StrCodeBac.Mid(1);

			// Ajoute au tableau des bacs
			if (atoi(StrCodeBac) > 0)
			{
				enr.m_CompoBacs[IdxBac] = atoi(StrCodeBac);
			}
			else
			{
				message=1;
				ChaineMessage="\nCode bac incorrect";
				break;
			}
			
		}
	
		// On ajoute l'enregistrement dans le tableau
		int index = Add(enr);

		// on ajoute l'index de l'�l�ment dans un map m�moire
		CString CodeGroupeBac;
		CodeGroupeBac = enr.GetLibelle();


		// m_MapCodeToIndex.SetAt(CodeGroupeBac , index);
		MapCodeToIndex(CodeGroupeBac , index);

	}

	if(message)
	{
		AfxMessageBox("le format du fichier: "+m_NomFich+" n'est pas valide"+ChaineMessage,MB_OK | MB_APPLMODAL);   
	}

	// fermeture du fichier
	Fichier.Close(); 
	return (1);
}

// d�finition de m_MapCodeToIndex
void CTableGroupeBAC::MapCodeToIndex(CString code,int index)
{
	m_MapCodeToIndex.SetAt(code,index);
};

// Suppression des �l�ments
void CTableGroupeBAC::RemoveMap()
{
	m_MapCodeToIndex.RemoveAll();
};

bool CTableGroupeBAC::CodeExist(CString code)
{
	return (m_MapCodeToIndex.Lookup(code, index));
};


// Sauvegarde du ficher groupe de bacs
bool CTableGroupeBAC::SaveTable()
{
	CString txt;

	// la table a �t� modifi�
	IsModified = 0;

	// sauvegarde
	CFichierCinema Fichier;


	if(!Fichier.OpenFileForWrite(m_NomFich))
	{
		return (0);	
	}

	// Nombre de groupes d�j� existant
	int NbGroupeExistant = GetSize();


	// Ecriture autoris�e, on r��crit la totalit� du fichier
	for(int i=0;i<GetSize();i++)
	{
		// sauvegarde du libell�
		CString Libelle;
		Libelle = GetLibelle(i);

		// troncature du libell� � 30 caract�res
		txt = Libelle.Left(30) ;
		
		// s�parateur
		txt += "|";
		
		// Sauvegarde Nb Composants Bacs
		int NbBacs = GetNbBacs(i);

		// Converion en texte
		char tamponalloc[100];
		sprintf(tamponalloc, "%d", NbBacs);

		// Ajout Info Nb Bacs
		txt += tamponalloc;

		// s�parateur
		txt += "|";

		// Ecriture des libell�s bacs
		CArray<long, long> CompoBacs;
		
		GetCompoBacs(i, CompoBacs);
		
		/* AVANT en numerique
		for (int idxBac=0; idxBac < NbBacs; idxBac++)
		{
			// Code num�rique du bac
			int CodeBac = CompoBacs.GetAt(idxBac);

			// Reconstitue le code bac
			sprintf(tamponalloc, "%s%05d", "B", CodeBac);

			// sauvegarde du code bac
			txt += tamponalloc;

			// s�parateur
			txt += "|"; 
		}
		*/

		for (int idxBac=0; idxBac < NbBacs; idxBac++)
		{
			// Code num�rique du bac
			long CodeBac = CompoBacs.GetAt(idxBac);

			// Reconstitue le code bac
			sprintf(tamponalloc, "%s%06d", "B", CodeBac);

			/*
			// Valeur code
			CString CodeStr = tamponalloc;
			CodeBac = atoi(CodeStr.Mid(1));

			if (CodeStr.Left(1) == "1")
				sprintf(tamponalloc, "%s%05d", "B", CodeBac);
			else if (CodeStr.Left(1) == "2")
				sprintf(tamponalloc, "%s%05d%s", "B", CodeBac, "A");
			else if (CodeStr.Left(1) == "3")
				sprintf(tamponalloc, "%s%05d%s", "B", CodeBac, "M"); 
			else if (CodeStr.Left(1) == "4")
				sprintf(tamponalloc, "%s%05d%s", "B", CodeBac, "N"); 

			// sauvegarde du code bac
			// txt += CodeBac;
			*/

			txt += tamponalloc;

			// s�parateur
			txt += "|"; 
		}

		// Saute une ligne
		txt+="\n";
		
		// Ecriture de la ligne groupe bacs
		if(!Fichier.WriteLine(txt))
		{
			return (0);
		}
		
	}
	// caract�re de fin de fichier
	if(!Fichier.WriteLine("#"))
	{
		return (0);
	}
	// fermeture du fichier
	Fichier.Close(); 
	return (1);
}
