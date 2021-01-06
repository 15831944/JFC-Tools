//
// Fichier: JMTFR03DATEffectif.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_EFFECTIF_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_EFFECTIF_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATEffectif
{
public:
	// les constructeurs
	JMTFR03DATEffectif();
	JMTFR03DATEffectif(const JMTFR03DATEffectif & Source);

	// l'opérateur pour recopier l'élément
	JMTFR03DATEffectif & operator =(const JMTFR03DATEffectif & Source);

	// le destructeur
	~JMTFR03DATEffectif();

public:
	JFlt64x m_Effectif; // l'effectif pondéré
	JInt32x m_NbCas;    // le nombre de cas
};

// fin de l'inclusion conditionnelle
#endif
