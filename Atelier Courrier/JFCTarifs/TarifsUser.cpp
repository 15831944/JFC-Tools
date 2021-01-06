#include "StdAfx.h"
#include ".\tarifsuser.h"

const JString CTarifsUser::FicTarifUser = "TARIFUSER.1B";

/////////////////////////////////////////////////////////////////////////
// Constructeur
CTarifsUser::CTarifsUser(void)
{
	// Init Nom base tarifaire
	m_NomBaseTarif = "";
}

/////////////////////////////////////////////////////////////////////////
// Destructeur
CTarifsUser::~CTarifsUser(void)
{
	// On se positionne sur le dernier �lmt
	MoveLast();

	// V�rifie qu'il est valide
	while( IsValid() )
	{
		// On supprime le tarif courant
		delete GetItem();

		// On se pointe sur le tarif pr�c�dent
		Remove(-1);
	}
}

// R�cup�re nom de la base tarifaire
JString CTarifsUser::GetNomBaseTarif()
{
	return m_NomBaseTarif;
}

////////////////////////////////////////////////////////////////////////////////////
// R�cup�rer un tarif � partir d'une dat
JBool CTarifsUser::GetTarifUser(JUnt32 IdSupport, JUnt32 IdFormat, JDate DateRech, JFlt32 & Tarif) const
{
	JBool bret = false;
	CTarifUser * pTarif;
	JDate DateOut;
	JDate DateDebTarif;

	// Recherche du tarif concern�
	for(MoveFirst(); IsValid(); MoveNext())
	{
		// R�cup�re �lmt tarif courant
		pTarif = GetItem();

		// V�rifie m�me support, m�me format
		if (pTarif->GetIDSupport() == IdSupport	&& pTarif->GetIDFormat() == IdFormat)
		{
			// R�cup�re date d�but
			DateDebTarif = pTarif->GetDateDeb();

			//on prend la date de d�but la plus proche
			if ( DateDebTarif <= DateRech && DateOut < DateDebTarif)
			{
				//sauf si notre tarif n'est plus valide
				if ( pTarif->GetDateDeb().IsValid() && DateRech > pTarif->GetDateFin())
					continue;

				// Recalage
				DateOut = DateDebTarif;

				// R�cup�re le tarif
				Tarif = pTarif->GetTarif();
			}
		}
	}

	// Valide le tarif
	if (DateOut.IsValid())
		bret = true;
	
	// Retour r�cup tarif
	return bret;
}

////////////////////////////////////////////////////////////////////////////////////
// Suppression un tarif
JVoid CTarifsUser::RemoveTarifUser(CTarifUser* pTarif)
{
	// Recherche du tarif
	MoveFirst();
	while (IsValid() && GetItem() != pTarif )
		MoveNext();

	// Si valide on le supprime d�finitivement
	if (IsValid())
	{
		 Remove(-1);
		 delete pTarif;
	}
}

/////////////////////////////////////////////////////////////////////////////////
// Chargement de la totalit� des tarifs constituant la base tarif n�
JBool CTarifsUser::Load(JUnt32 NoBaseTarif)
{
	// R�cup�re chemin user
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));

	// Chemin du fichier tarif
	CString PathTarif;
	PathTarif.Format("%s%0000d","/T",NoBaseTarif);
	PathTarif += "/";
	PathTarif += FicTarifUser.AsJCharPtr();

	try
	{
		// Cr�er instance fichier
		JArchive *pFile = JArchive::Create((const JChar*)PathTarif);

		// Chargement entete fichier base tarifaire
		LoadFileHeader		(pFile);

		// Lecture nom de la base tarifaire
		LoadFileName		(pFile);

		// Chargement des tarifs de la base atrifaire
		LoadTarifs			(pFile, m_FileHeader.GetVersion ());

		// D�sallocation
		delete pFile;

	}

	// Gestion erreur chargement base tarifaire
	catch(JException *pE)
	{
		pE->GetMessage();
		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
		return false;
	}

	// Remet le r�pertoire courant Sources
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	return true;
}


/////////////////////////////////////////////////////////////////////////////////
// Chargement de l'entete fichier base tarif
JVoid CTarifsUser::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD3481B)
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

/////////////////////////////////////////////////////////////////////////////////	
// Load nom fichier tarif
JVoid CTarifsUser::LoadFileName			(JArchive *pFile)
{
	// R�cup nom du fichier
	m_NomBaseTarif.Recv(*pFile);
}

/////////////////////////////////////////////////////////////////////////////////
// Chargement des tarifs utilisateur
JVoid CTarifsUser::LoadTarifs(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Nombre de tarifs dans cette base utilisateur
			JUnt32 NbTarif;
			pFile->Recv(NbTarif);

			// D�clare objet tarif pour r�cup�ration
			CTarifUser *pTarifUser;

			// Boucle sur tous les tarifs
			for(JUnt32 i = 0; i < NbTarif; i++ )
			{
				// Cr�er un nouvel �lmt tarif
				pTarifUser = new CTarifUser();

				// R�cup�re les infos tarifs
				pTarifUser->Recv(*pFile, Version);

				// Ajoute cette �lmt � la base
				AddTail() = pTarifUser;
			}
		}
		break;

		default:
			TRACE("CTarifsUser   Version non valide");
			throw JInternalError::GetInstance(); 
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde d'une base tarifaire
JBool CTarifsUser::Save(JUnt32 NoBaseTarif)
{
	// R�cup�re chemin user
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));

	// Chemin du fichier tarif
	CString PathTarif;
	PathTarif.Format("%s%0000d","/T",NoBaseTarif);
	PathTarif += "/";
	PathTarif += FicTarifUser.AsJCharPtr();
	
	try
	{
		// Cr�er objet archive avec fichier base tarifaire en cours
		JArchive *pFile = JArchive::Create((const JChar*)PathTarif, true);
		
		// Sauve entete fichier
		SaveFileHeader		(pFile);

		// Sauve nom base tarifaire
		SaveFileName		(pFile);

		// Sauve tarifs base utilisateur
		SaveTarifs 			(pFile, m_FileHeader.GetVersion ());

		// Vide le tampon
		pFile->Flush();

		// Supprime  
		delete pFile;
	
		// Mise � jour du fichier
		JPathManager::UpdateFile((const JChar*)PathTarif);
	}

	// Gestion erreur sauvegarde tarif
	catch(JException *pE)
	{
		pE->GetMessage();
		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
		return false;
	}

	// Remet le r�pertoire courant
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	return true;
}

JVoid CTarifsUser::SaveFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	// Check compagnie name
	m_FileHeader.SetCompagnie (0x0043464A);
	// Verify the magic number
	m_FileHeader.SetMagicNumber (0x6FD3481B);
	// No compression
	m_FileHeader.SetCompression (0);
	// No cryptage
	m_FileHeader.SetCryptage(0);
	// Must not be greater than my current version
	m_FileHeader.SetVersion(CURRENT_VERSION);

	m_FileHeader.Send(*pFile);

}

/////////////////////////////////////////////////////////////////////////////////
// Save nom fichier tarif
JVoid CTarifsUser::SaveFileName			(JArchive *pFile)
{
	// Sauve nom base tarifaire
	m_NomBaseTarif.Send(*pFile);
}

/////////////////////////////////////////////////////////////////////////////////
// Save tarifs base utilisateur
JVoid CTarifsUser::SaveTarifs			(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Set number of TarifsManuels
			pFile->Send( GetCount() );

			for( MoveFirst(); IsValid(); MoveNext())
				GetItem()->Send(*pFile, Version);
		}
		break;

		default:
			TRACE("CTarifsManuels   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}



