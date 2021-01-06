//
// Fichier: ATPDocSerializer.h
// Date:    02/03/2004
//

#pragma once

#include "JATPOffre.h"
#include "JATPDocCreuset.h"

// on inclut les définitions nécessaires

//class AFX_EXT_CLASS CATPDocSerializer
class CATPDocSerializer
{
public:
	// Sérialisation recv & send
	static void Send(JStream & Stream, JATPDocCreuset * doc);
	static void Recv(JStream & Stream, JATPDocCreuset * doc);

	// JBool RecvOffrePlateau(JStream & Stream, JATPOffre &Offre, JList <JUnt32> &LstSuppPlateau);

public:
	CATPDocSerializer(void);
	~CATPDocSerializer(void);

	// static const JInt32 VERSION = 0x00000001;

	// Version 8 avec récupération mode cible pondérée
	static const JInt32 VERSION = 0x00000008;
	
	// Version 1 archivage
	static void WriteV1(JStream & Stream, JATPDocCreuset * doc);
	static void ReadV1(JStream & Stream, JATPDocCreuset * doc);

	// Version 4 archivage avec récupération des items
	static void WriteV4(JStream & Stream, JATPDocCreuset * doc);
	static void ReadV4(JStream & Stream, JATPDocCreuset * doc);

	// Version 5 : Version 4 + archivage quick plan
	static void WriteV5(JStream & Stream, JATPDocCreuset * doc);
	static void ReadV5(JStream & Stream, JATPDocCreuset * doc);

	// Version 7 : Idem version 5 mais changement de version à cause de l'archivage supplémentaire 
	// des familles dans l'Offre + sauvegarde des tarifs manuels
	static void WriteV7(JStream & Stream, JATPDocCreuset * doc);
	static void ReadV7(JStream & Stream, JATPDocCreuset * doc);

	// Version 8 : Idem version 7 avec en + ecriture et lecture mode cible pondérée
	static void WriteV8(JStream & Stream, JATPDocCreuset * doc);
	static void ReadV8(JStream & Stream, JATPDocCreuset * doc);
};
