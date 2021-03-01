#include "stdafx.h"
#include ".\secteurs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSecteurs::CSecteurs(void)
{
}

CSecteurs::~CSecteurs(void)
{
}

///////////////////////////////////////////////////////////////////////// 
// Fichier des liens enfants / secteurs avec liste des elmts enfants  F2
JBool CSecteurs::Load(JStringEx &Name)
{
	m_sName = Name;
	m_sName.Append (".F1");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadSecteurs 		(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CSecteurs::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);

	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD359F1)
		throw JInternalError::GetInstance(); //("File Header, Magic Number field error");

	// No compression
	if(m_FileHeader.GetCompression () != 0)
		throw JInternalError::GetInstance(); //("File Header, Compression field error");

	// No cryptage
	if(m_FileHeader.GetCryptage () != 0)
		throw JInternalError::GetInstance(); //("File Header, Cryptage field error");

	// Must not be greater than my current version
	if(m_FileHeader.GetVersion () > CURRENT_VERSION)
		throw JInternalError::GetInstance(); //("File Header, Version error");
}

JVoid CSecteurs::LoadHeader(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		{
			// Load Source and Terrain ID's
			pFile->Recv(m_lSourceID);
			pFile->Recv(m_lTerrainID);
		}
		break;

	default:
		TRACE("CSecteurs   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CSecteurs::LoadSecteurs(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		{
			// Récup le nombre de secteurs
			pFile->Recv(m_lNbSectors);

			// Reset du mp secteurs
			m_MapSecteurEnfants.Reset();

			// Boucle sur tous les secteurs pour récupérer ident secteur et liste des enfants associés
			for (int IdxSecteur = 0; IdxSecteur < m_lNbSectors; IdxSecteur++)
			{
				// Récup Ident Secteur
				JInt32 IdSecteur = 0;
				pFile->Recv(IdSecteur);
				
				// Voir si secteur non dejà existant
				m_MapSecteurEnfants.MoveTo(IdSecteur);
				if (!m_MapSecteurEnfants.IsValid())
				{
					// Init Liste des enfants et ajout au map
					JList<JInt32> & LstChildren = m_MapSecteurEnfants.Add(IdSecteur);

					// Recup Nb Child
					JInt32 NbChild = 0;
					pFile->Recv(NbChild);
					for (int IdxChild = 0; IdxChild < NbChild; IdxChild++)
					{
						// Récup Id Child et insertion dans liste
						JInt32 IdChild = 0;
						pFile->Recv(IdChild);	

						// On l'ajoute à liste des enfants
						LstChildren.AddTail() = IdChild;
					}
				}
			}
		}
		break;

	default:
		TRACE("CSecteurs   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JBool CSecteurs::GetEnfantsSecteur(JMap <JInt32, JList<JInt32>> & MapEnfantsSecteur)
{
	// Boucle sur tous les segteurs
	for (m_MapSecteurEnfants.MoveFirst(); m_MapSecteurEnfants.IsValid(); m_MapSecteurEnfants.MoveNext())
	{
		// Clef Secteur
		JInt32 KeySector = m_MapSecteurEnfants.GetKey();

		// Mise à jour map
		MapEnfantsSecteur.MoveTo(KeySector);
		if (!MapEnfantsSecteur.IsValid())
		{
			// Récupère la liste des 
			JList<JInt32> & LstMarques = MapEnfantsSecteur.Add(KeySector);

			for(m_MapSecteurEnfants.GetItem().MoveFirst(); m_MapSecteurEnfants.GetItem().IsValid(); m_MapSecteurEnfants.GetItem().MoveNext())
			{
				LstMarques.AddTail() = m_MapSecteurEnfants.GetItem().GetItem();
			}
		}
	}
	return true;
}