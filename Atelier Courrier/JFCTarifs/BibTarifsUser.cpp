#include "StdAfx.h"
#include "BibTarifsUser.h"
#include "TarifsUser.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Bibliothèque des bases tarifs utilisateur
//
// A FAIRE -- Suppression d'une base tarifaire
//			  *** ou Cacher/Decacher  une base tarifaire (dans ce cas il faut un flag dans la base)	

const JUnt32 CBibTarifsUser::MaxRepTarUser = 9999;

// Constructeur
CBibTarifsUser::CBibTarifsUser(void)
{
	// Init Map des tarifs utilisateurs
	m_MapTarifUser.Reset(); 

	// Init 1er n° libre pour répertoire tarif
	m_NoFreeBase = 0;
}

// Destructeur
CBibTarifsUser::~CBibTarifsUser(void)
{
}

// Chargement bibliothèque tarifs utilisateur
JBool CBibTarifsUser::LoadBibTarif()
{
	// Init Map Rep Tarif et 1er n° libre rep avant de les récupérer
	m_MapTarifUser.Reset(); 
	m_NoFreeBase = 0;

	// Récupère path bases tarifs utilisateurs
	CString UserTarifPath;
	UserTarifPath = AfxGetApp()->GetProfileString("Data", "UserPath", 0);
	UserTarifPath += "Tarif\\";

	//si répertoire \\Tarif n'existe pas on le crée
	if (!JPathManager::IsDirectoryExists((const char*)UserTarifPath))
		JPathManager::MakeDirectory((const char*)UserTarifPath);

	// Init Nb base tarifs user utilisés
	JUnt32 NbBaseTarif = 0;

	try
	{
		// Balayage d'au maximum 9999 rep tarifs utilsateurs
		for (JUnt32 i = 1; i <= MaxRepTarUser; i++)
		{
			// BaseRep tarif
			CString TxtNo;
			TxtNo.Format("%s%00000d%s","T",i,".1B");

			// Reconstitue nouveau rep tarif 
			CString RepTarif = UserTarifPath + TxtNo;
			DWORD RetValue = GetFileAttributes(RepTarif);

			// Test si on est sur un tarif user
			if (RetValue != INVALID_FILE_ATTRIBUTES)
			{
				// Crée objet base tarifaire
				CTarifsUser TarifsUser;

				// Charge la base tarifaire
				if (TarifsUser.Load(i)) 
				{
					// récupère le nom de la base tarifaire
					JString LibBaseUser = TarifsUser.GetNomBaseTarif();

					// Si trouvé, ce répertoire constitue bien un rep tarif utilisateur
					if (LibBaseUser != "")
					{
						// Ajoute les infos au map biblio tarifs
						m_MapTarifUser.Add(i) = LibBaseUser; 
					}
				}
				else
				{
					// Ce fichier base est vide, il s'agit du fichier vierge en attente de création futur base
					m_NoFreeBase = i;

					// Pas la peine d'aller plus loin
					break;
				}
			}
		}
	}
	
	// Gestion erreur
	catch (JException* pException)
	{
		// on affiche le message
		::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		return false;
	}

	if (m_NoFreeBase == 0)
	{
		// Cas où aucun fichier tarif user trouvé >> créer 1er fichier vide
		// Ce fichier sera utilisé pour la prochaine création d'une base tarif user
		CString FicBaseTarif = UserTarifPath + "\\T0001.1B";
		JArchive *pFile = JArchive::Create((const char*)FicBaseTarif, true);
		delete pFile;
		JPathManager::UpdateFile((const char*)FicBaseTarif);
	}
	
	// No problem chargement liste répertoire tarif user Ok
	return true;
}

// Sauvegarde bibliothèque tarifs utilisateur
JBool CBibTarifsUser::SaveBibTarif()
{
	return true;
}

// Pour supprimer une base tarif
// Attention s'assurer que l'application n'utilise pas ce tarif
// *** voir si on tient compte des études stockés avec cette base tarif !!!!!
JBool CBibTarifsUser::RemoveTarif(JUnt32 NoTarif)
{
	// Créer le path directory à supprimer
	CString UserTarifPath;
	UserTarifPath = AfxGetApp()->GetProfileString("Data", "UserPath", 0);
	UserTarifPath += "Tarif\\";

	// Path Base Fichier tarif
	CString FicBaseTarif;
	FicBaseTarif.Format("%s%00000d%s","T",NoTarif,".1B");

	// Vérifie que le fichier existe
	if (JPathManager::IsFileExists((const char*)(UserTarifPath + FicBaseTarif)))
	{
		// Supprime le fichie base
		JPathManager::RemoveFile((const char*)(UserTarifPath + FicBaseTarif));

		// Se pointe sur elmt dans le map base tarif
		m_MapTarifUser.MoveTo(NoTarif);
		if (m_MapTarifUser.IsValid())
		{
			// Enlève du map biblio base tarifs
			m_MapTarifUser.Remove(); 
		}
		return true;
	}		
	else
	{
		CString TxtMess;
		TxtMess.LoadString(IDS_TAR_NOSUPP_BASE);
		AfxMessageBox(TxtMess);
		// AfxMessageBox("Suppression base tarifaire impossible", MB_ICONERROR | MB_OK);
		return false;
	}

	return true;

}	


// Pour ajouter un tarif
JBool CBibTarifsUser::AddTarif(JString NomTarif)
{
	return true;
}	

// Pour renommer un tarif
JBool CBibTarifsUser::RenameTarif(JString OldNomTarif, JString NewNomTarif) 
{
	return true;
}	

// Pour copier un tarif
JBool CBibTarifsUser::CopyTarif(JString OrgNomTarif, JString DstNomTarif)
{
	return true;
}	


// Récupération dernier no repertoire tarif dispo
JUnt32 CBibTarifsUser::GetFreeNoRepTarif()
{
	// Charge map répertoires tarifs
	LoadBibTarif();

	// Renvoie le n° libre rep dispo
	return m_NoFreeBase;
}