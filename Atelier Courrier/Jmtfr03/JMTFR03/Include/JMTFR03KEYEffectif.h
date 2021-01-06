//
// Fichier: JMTFR03KEYEffectif.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_EFFECTIF_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_EFFECTIF_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYEffectif
{
public:
	// les constructeurs
	JMTFR03KEYEffectif();
	JMTFR03KEYEffectif(const JMTFR03KEYEffectif & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYEffectif & operator =(const JMTFR03KEYEffectif & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYEffectif & Reference) const;
	JBool operator >(const JMTFR03KEYEffectif & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYEffectif();

public:
	JRef32 m_IdSource;  // l'identifiant de la source
	JRef32 m_IdTerrain; // l'identifiant du terrain
	JRef32 m_IdCible;   // l'identifiant de la cible
};

// fin de l'inclusion conditionnelle
#endif
