//
// Fichier: JConvert.h
// Auteur:  Sylvain SAMMURI
// Date:    25/11/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_CONVERT_H
#define JLIB_CONVERT_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

class JConvert
{
public:
	// les fonctions pour convertir les chaînes
	static JBool ToBinary(const JChar* pString, JInt32 & Value);
	static JBool ToBinary(const JChar* pString, JUnt32 & Value);

public:
	// la fonction pour calculer la clé de hachage
	static JInt32 ToHashKey(const JChar* pString, JInt32 Size);

public:
	// les fonctions pour convertir les valeurs
	static JInt32 ToDecimal(JInt32 Value, JChar* pString);
	static JInt32 ToDecimal(JUnt32 Value, JChar* pString);
};

// on finit l'inclusion conditionnelle
#endif
