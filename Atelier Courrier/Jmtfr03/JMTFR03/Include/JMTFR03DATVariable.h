//
// Fichier: JMTFR03DATVariable.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_VARIABLE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_VARIABLE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATVariable
{
public:
	// le constructeur
	JMTFR03DATVariable();

	// le destructeur
	~JMTFR03DATVariable();

private:
	// l'opérateur pour recopier l'élément
	JMTFR03DATVariable & operator =(const JMTFR03DATVariable & Source);

	// le constructeur copie
	JMTFR03DATVariable(const JMTFR03DATVariable & Source);

public:
	JRef32          m_IdUnivers;     // l'identifiant de l'univers
	JBitArray       m_ItemIndividus; // le vecteur des individus
	JArray<JInt32x> m_ItemVariables; // le vecteur des variables
	JInt32x         m_Diviseur;      // le diviseur
};

// fin de l'inclusion conditionnelle
#endif
