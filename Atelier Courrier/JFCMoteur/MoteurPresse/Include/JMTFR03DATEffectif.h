//
// Fichier: JMTFR03DATEffectif.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_EFFECTIF_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_EFFECTIF_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03DATEffectif
{
public:
	// les constructeurs
	JMTFR03DATEffectif();
	JMTFR03DATEffectif(const JMTFR03DATEffectif & Source);

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03DATEffectif & operator =(const JMTFR03DATEffectif & Source);

	// le destructeur
	~JMTFR03DATEffectif();

public:
	JFlt64x m_Effectif; // l'effectif pond�r�
	JInt32x m_NbCas;    // le nombre de cas
};

// fin de l'inclusion conditionnelle
#endif
