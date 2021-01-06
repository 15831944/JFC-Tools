//
// Fichier: JMTFR03DATVariable.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_VARIABLE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_VARIABLE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03DATVariable
{
public:
	// le constructeur
	JMTFR03DATVariable();

	// le destructeur
	~JMTFR03DATVariable();

private:
	// l'op�rateur pour recopier l'�l�ment
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
