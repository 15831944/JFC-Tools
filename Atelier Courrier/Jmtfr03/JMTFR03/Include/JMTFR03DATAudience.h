//
// Fichier: JMTFR03DATAudience.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_AUDIENCE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_AUDIENCE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03DATAudience
{
public:
	// les constructeurs
	JMTFR03DATAudience();
	JMTFR03DATAudience(const JMTFR03DATAudience & Source);

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03DATAudience & operator =(const JMTFR03DATAudience & Source);

	// le destructeur
	~JMTFR03DATAudience();

public:
	JFlt64x m_AudienceRef;   // l'audience de r�f�rence (LDP ou LNM)
	JFlt64x m_AudienceProba; // l'audience probabiliste (LDP ou LNM)
	JFlt64x m_VolLecture1Jr; // le volume de lectures 1 jour moyen
	JInt32x m_PtEtalonnage;  // le point d'�talonnage de r�f�rence
};

// fin de l'inclusion conditionnelle
#endif
