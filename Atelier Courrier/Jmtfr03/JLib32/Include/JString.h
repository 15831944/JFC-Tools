//
// Fichier: JString.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_STRING_H
#define JLIB_STRING_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JLimits.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JString
{
public:
	// les constructeurs
	JString();
	JString(const JString & Source);
	JString(const JChar* pSource);

	// la fonction pour tester la chaîne
	JBool IsEmpty() const;

	// la fonction pour récupérer la clé de hashage
	JInt32 GetHashKey(JInt32 Size) const;

	// les fonctions pour sérialiser la chaîne
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour récupérer la chaîne
	const JChar* AsJCharPtr() const;

	// les opérateurs d'affectation
	JString & operator =(const JChar* pSource);
	JString & operator =(const JString & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JString & Reference) const;
	JBool operator !=(const JString & Reference) const;
	JBool operator < (const JString & Reference) const;
	JBool operator <=(const JString & Reference) const;
	JBool operator > (const JString & Reference) const;
	JBool operator >=(const JString & Reference) const;

	// les fonctions pour mettre en forme la chaîne
	JVoid TrimRight();
	JVoid TrimLeft();

	// la fonction pour réinitialiser la chaîne
	JVoid Reset();

	// le destructeur
	~JString();

private:
	// la fonction pour remplacer la chaîne
	JVoid OnReplaceString(const JChar* pSource);

private:
	// les fonctions pour formater la chaîne
	JVoid OnShiftString(JInt32 Start, JInt32 Offset);
	JVoid OnTrimRight(const JChar* pSource, JInt32 & Start, JInt32 & Offset) const;
	JVoid OnTrimLeft(const JChar* pSource, JInt32 & Start, JInt32 & Offset) const;

private:
	// les fonctions pour manipuler la chaîne
	JInt32 OnCompareString(const JChar* pSource, const JChar* pReference) const;
	JInt32 OnCheckString(const JChar* pSource) const;

private:
	JChar m_String[JSTRING_MAX]; // la chaîne
};

// on finit l'inclusion conditionnelle
#endif
