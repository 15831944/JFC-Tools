//	Classe:
//	CTarifUser
//
//	Gestion des tarifs utilisateur
#pragma once

#include "TarifUser.h"
#include "FileHeader.h"

class AFX_EXT_CLASS CTarifsUser : public JList<CTarifUser*>
{
public:
	static  const JString FicTarifUser;

public:
	// Constructeur
	CTarifsUser(void);

	// Destructeur
	~CTarifsUser(void);

	//Pour supprimer un tarif
	JVoid RemoveTarifUser(CTarifUser* tarif);

	// Récup Tarifs User
	JBool Load(JUnt32 NoBaseTarif);

	// Sauve Tarifs User
	JBool Save(JUnt32 NoBaseTarif);

	// Récupère un tarif via IdSupport/IdFormat/Date  
	JBool GetTarifUser(JUnt32 IdSupport, JUnt32 IdFormat, JDate DateRech, JFlt32 & Tarif) const;
	
	// Récupère nom base tarifaire
	JString GetNomBaseTarif();

private:
	// Entete fichier tarif
	CFileHeader					m_FileHeader;

	// Nom fichier base tarifaire
	JString						m_NomBaseTarif;

	// Load entete fichier
	JVoid LoadFileHeader		(JArchive *pFile);

	// Save entete fichier tarif
	JVoid SaveFileHeader		(JArchive *pFile);

	// Load nom fichier tarif
	JVoid LoadFileName			(JArchive *pFile);

	// Save nom fichier tarif
	JVoid SaveFileName			(JArchive *pFile);

	// Load des tarifs base utilisateur
	JVoid LoadTarifs			(JArchive *pFile, JInt32 Version);

	// Save tarifs base utilisateur
	JVoid SaveTarifs			(JArchive *pFile, JInt32 Version);

};
