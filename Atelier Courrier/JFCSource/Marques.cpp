#include "stdafx.h"
#include ".\marques.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMarques::CMarques(void)
{
}

CMarques::~CMarques(void)
{
}

////////////////////////////////////////////////////////////////////////
// Fichier des liens parents / via un enfant on retrouve le parent .F2
JBool CMarques::Load(JStringEx &Name)
{

	m_sName = Name;
	m_sName.Append (".F2");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadMarques 		(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		pE->GetMessage();
		return false;
	}
	return true;
}

JVoid CMarques::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);

	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD359F2)
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

JVoid CMarques::LoadHeader(JArchive *pFile, JInt32 Version)
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
		TRACE("CMarques   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CMarques::LoadMarques(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		{
			// Récup le nombre de Marques Enfants
			pFile->Recv(m_lNbEnfants);

			// Reset du map Marques
			m_MapEnfantParent.Reset();

			// Boucle sur tous les Marques pour récupérer ident secteur associé 
			for (int IdxEnfant = 0; IdxEnfant < m_lNbEnfants; IdxEnfant++)
			{
				// Récup Ident Enfant
				JInt32 IdMarque = 0;
				pFile->Recv(IdMarque);

				// Voir si secteur non dejà existant
				m_MapEnfantParent.MoveTo(IdMarque);
				if (!m_MapEnfantParent.IsValid())
				{
					// Ident Parent Secteur
					JInt32 IdSecteur = 0;
					pFile->Recv(IdSecteur);

					// Ajout au map du lien secteur
					m_MapEnfantParent.Add(IdMarque) = IdSecteur;
				}
				else
					AfxMessageBox("Erreur dans fichier enfant-marque, un enfant ne peut avoir qu'un parent !!!");
			}
		}
		break;

	default:
		TRACE("CMarques   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JBool CMarques::GetParentMarques(JMap <JInt32, JInt32> & MapParentMarques)
{
	// Boucle sur tous les secteurs
	for (m_MapEnfantParent.MoveFirst(); m_MapEnfantParent.IsValid(); m_MapEnfantParent.MoveNext())
	{
		MapParentMarques.Add(m_MapEnfantParent.GetKey()) = m_MapEnfantParent.GetItem();
	}
	return true;
}
