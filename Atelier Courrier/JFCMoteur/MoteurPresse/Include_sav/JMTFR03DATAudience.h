//
// Fichier: JMTFR03DATAudience.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_AUDIENCE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_AUDIENCE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATAudience
{
public:
	// les constructeurs
	JMTFR03DATAudience();
	JMTFR03DATAudience(const JMTFR03DATAudience & Source);

	// l'opérateur pour recopier l'élément
	JMTFR03DATAudience & operator =(const JMTFR03DATAudience & Source);

	// le destructeur
	~JMTFR03DATAudience();

public:
	JFlt64x m_AudienceRef;   // l'audience de référence (LDP ou LNM)
	JFlt64x m_AudienceProba; // l'audience probabiliste (LDP ou LNM)
	JFlt64x m_VolLecture1Jr; // le volume de lectures 1 jour moyen
	JInt32x m_PtEtalonnage;  // le point d'étalonnage de référence
};

// fin de l'inclusion conditionnelle
#endif
