//
// Fichier: JMTFR03DATAttribut.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_ATTRIBUT_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_ATTRIBUT_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATAttribut
{
public:
	// le constructeur
	JMTFR03DATAttribut();

	// le destructeur
	~JMTFR03DATAttribut();

private:
	// l'opérateur pour recopier l'élément
	JMTFR03DATAttribut & operator =(const JMTFR03DATAttribut & Source);

	// le constructeur copie
	JMTFR03DATAttribut(const JMTFR03DATAttribut & Source);

public:
	JRef32  m_IdEchantillon; // l'identifiant de l'échantillon
	JRef32  m_IdType;        // l'identifiant du type de titre
	JInt32x m_PtEtalonnage;  // le point d'étalonnage de référence du titre
};

// fin de l'inclusion conditionnelle
#endif
