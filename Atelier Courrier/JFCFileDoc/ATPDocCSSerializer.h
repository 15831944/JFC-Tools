//
// Fichier: ATPDocCSSerializer.h
// Auteur:  Alain CHAMBARD
// Date:    07/04/2004
// Sérialisation du document sélection Choix Support
//
#pragma once

#include "JATPDocChoixSupport.h"

class AFX_EXT_CLASS CATPDocCSSerializer
{

public:
	// Sérialisation du document
	static void Send(JStream & Stream, JATPDocChoixSupport * doc);
	static void Recv(JStream & Stream, JATPDocChoixSupport * doc);
	static void RecvNoVersion(JStream & Stream, JUnt32 & NoVersion, JATPDocChoixSupport * doc);
	
	// Récupération offre document
	static JBool RecvOffre(JStream & Stream, JATPOffre &Offre);

	// Récupération Offre + Plateau (supports + familles)
	JBool RecvOffrePlateau(JStream & Stream, JATPOffre &Offre, 
		                   JList <JUnt32> &LstSuppPlateau, 
						   JDate &DateEtude);

	// Constructeur
	CATPDocCSSerializer(void);

	// Destructeur
	~CATPDocCSSerializer(void);

private:

	// Numéro du version
	// static const JInt32 VERSION = 0x00000001;
	// static const JInt32 VERSIONCS = 0x00000002;

	// en ++ gestion des supports visibles
	// static const JInt32 VERSIONCS = 0x00000003;

	// en ++ gestion construction items (concerne l'offre uniquement)
	// static const JInt32 VERSIONCS = 0x00000004;

	// Version avec récupération offre au début
	// static const JInt32 VERSIONCS = 0x00000004;

	// en ++ NE MARCHE PAS SUITE AU PROBLEME ARCHIVE AVEC STDIOFILE POUR OFFRE
	// static const JInt32 VERSIONCS = 0x00000005;

	// en ++ archivage des rangs
	// static const JInt32 VERSIONCS = 0x00000006;

	// en ++ archivage sélection familles
	// static const JInt32 VERSIONCS = 0x00000007;

	// Résolution problème AEPM 2006-2007 du au code 282 (256 + 26) en CStdioFile >> CFile pour l'archivage
	static const JInt32 VERSIONCS = 0x00000008;

	// Sérialisation version 1
	static void WriteV1(JStream & Stream, JATPDocChoixSupport * doc);
	static void ReadV1(JStream & Stream,  JATPDocChoixSupport * doc);

	// Sérialisation version 2
	static void WriteV2(JStream & Stream, JATPDocChoixSupport * doc);
	static void ReadV2(JStream & Stream,  JATPDocChoixSupport * doc);

	// Sérialisation version 3
	static void WriteV3(JStream & Stream, JATPDocChoixSupport * doc);
	static void ReadV3(JStream & Stream,  JATPDocChoixSupport * doc);

	// Sérialisation version 4
	static void WriteV4(JStream & Stream, JATPDocChoixSupport * doc);
	static void ReadV4(JStream & Stream,  JATPDocChoixSupport * doc);

	// Sérialisation version 5
	static void WriteV5(JStream & Stream, JATPDocChoixSupport * doc);
	static void ReadV5(JStream & Stream,  JATPDocChoixSupport * doc);

	// Sérialisation version 6
	static void WriteV6(JStream & Stream, JATPDocChoixSupport * doc);
	static void ReadV6(JStream & Stream,  JATPDocChoixSupport * doc);

};
