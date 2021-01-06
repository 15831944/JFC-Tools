//
// Fichier: JMTFR03DATModulation.h
// Auteur:  Sylvain SAMMURI
// Date:    18/06/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_MODULATION_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_MODULATION_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03FLTSegments.h"
#include "JMTFR03FLTFiltres.h"
#include "JMTFR03FLTPoids.h"
#include "JMTFR03FLTTitres.h"

class JMTFR03DATModulation
{
public:
	// le constructeur
	JMTFR03DATModulation();

	// le destructeur
	~JMTFR03DATModulation();

private:
	// l'op�rateur pour recopier l'�l�ment
	JMTFR03DATModulation & operator =(const JMTFR03DATModulation & Source);

	// le constructeur copie
	JMTFR03DATModulation(const JMTFR03DATModulation & Source);

public:
	JMTFR03FLTSegments m_FLTSegments; // le filtre des segments de population
	JMTFR03FLTFiltres  m_FLTFiltres;  // le filtre des filtres d'audience
	JMTFR03FLTPoids    m_FLTPoids;    // le filtre des poids
	JMTFR03FLTTitres   m_FLTTitres;   // le filtre des titres
};

// fin de l'inclusion conditionnelle
#endif
