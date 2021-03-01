//
// Fichier: JMTFR03DATAttribut.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_ATTRIBUT_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_ATTRIBUT_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03DATAttribut
{
public:
	// le constructeur
	JMTFR03DATAttribut();

	// le destructeur
	~JMTFR03DATAttribut();

private:
	// l'op�rateur pour recopier l'�l�ment
	JMTFR03DATAttribut & operator =(const JMTFR03DATAttribut & Source);

	// le constructeur copie
	JMTFR03DATAttribut(const JMTFR03DATAttribut & Source);

public:
	JRef32  m_IdEchantillon; // l'identifiant de l'�chantillon
	JRef32  m_IdType;        // l'identifiant du type de titre
	JInt32x m_PtEtalonnage;  // le point d'�talonnage de r�f�rence du titre
};

// fin de l'inclusion conditionnelle
#endif
