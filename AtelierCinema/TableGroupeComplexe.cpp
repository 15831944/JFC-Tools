// TableGroupeComplexe.cpp: implementation of the CTableGroupeComplexe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableGroupeComplexe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableGroupeComplexe::CTableGroupeComplexe()
{

	m_NomFich="groupecomplexes.table";
	SetName(m_NomFich);
	IsModified=0;
}

// copy constructor
CTableGroupeComplexe::CTableGroupeComplexe(const CTableGroupeComplexe &Data)
{
	*this = Data;
};

CTableGroupeComplexe::~CTableGroupeComplexe()
{

}

// Chargement table des groupes de Complexes
bool CTableGroupeComplexe::LoadTable(CString CheminData)
{
	RemoveAll();

	CFichierCinema Fichier;

	CFileFind finder;
	if(!finder.FindFile(CheminData + "\\DATA\\" + m_NomFich))
	{
		return (0); 
	}

	
	// Il existe on essaie de l'ouvrir en lecture 
	if (!Fichier.OpenFileForRead(m_NomFich, 1))
	{
		return (0);
	}
	
	bool message=0;
	CString ChaineMessage;

	// On lit tous les groupes complexes que l'on place dans la table des groupe Complexes
	while(Fichier.ReadLine())
	{
		int PosElmt = 0;

		// enregistrement groupe complexe
		tagComplexeGroupe enr;

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


		// Stocke le nombre de complexes
		PosElmt++;
		enr.m_NbComplexes = atoi(Fichier.GetElement(PosElmt));
		if(enr.m_NbComplexes <= 0)
		{
			message=1;
			ChaineMessage="\nNombre de complexes = 0 incorrect";
			break;
		}

		// Taille le tableau des complexes
		enr.m_CompoComplexes.SetSize(enr.m_NbComplexes);

		// Stocke le tableau des composants complexes
		for (int IdxCmplx= 0; IdxCmplx < enr.m_NbComplexes; IdxCmplx++)
		{
			PosElmt++;

			// Code Complexe
			CString StrCodeComplexe = Fichier.GetElement(PosElmt);
			if(StrCodeComplexe.GetLength() > 6 || StrCodeComplexe.GetLength() < 3)
			{
				message=1;
				ChaineMessage="\nNombre de complexes = 0 incorrect";
				break;
			}

			// Vire le post-code "C"
			StrCodeComplexe = StrCodeComplexe.Mid(1);

			// Ajoute au tableau des complexes
			if (atoi(StrCodeComplexe) > 0)
			{
				enr.m_CompoComplexes[IdxCmplx] = atoi(StrCodeComplexe);
			}
			else
			{
				message=1;
				ChaineMessage="\nCode complexebac incorrect";
				break;
			}
			
		}
	
		// On ajoute l'enregistrement dans le tableau
		int index = Add(enr);

		// on ajoute l'index de l'�l�ment dans un map m�moire
		CString CodeGroupeComplexe;
		CodeGroupeComplexe = enr.GetLibelle();


		// Ajoute au map des groupes complexes
		MapCodeToIndex(CodeGroupeComplexe , index);

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
void CTableGroupeComplexe::MapCodeToIndex(CString code,int index)
{
	m_MapCodeToIndex.SetAt(code,index);
};

// Suppression des �l�ments
void CTableGroupeComplexe::RemoveMap()
{
	m_MapCodeToIndex.RemoveAll();
};

bool CTableGroupeComplexe::CodeExist(CString code)
{
	return (m_MapCodeToIndex.Lookup(code, index));
};


// Sauvegarde du ficher groupe de complexes
bool CTableGroupeComplexe::SaveTable()
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
		
		// Sauvegarde Nb Composants Complexes
		int NbComplexes = GetNbComplexes(i);

		// Converion en texte
		char tamponalloc[100];
		sprintf(tamponalloc, "%d", NbComplexes);

		// Ajout Info Nb Bacs
		txt += tamponalloc;

		// s�parateur
		txt += "|";

		// Ecriture des libell�s bacs
		CArray<int,int> CompoComplexes;
		GetCompoComplexes(i, CompoComplexes);
		
		for (int idxCmplx = 0; idxCmplx < NbComplexes; idxCmplx++)
		{
			// Code num�rique du complexe
			int CodeComplexe = CompoComplexes.GetAt(idxCmplx);

			// Reconstitue le code complexe
			sprintf(tamponalloc, "%s%05d", "C", CodeComplexe);

			// sauvegarde du code bac
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

