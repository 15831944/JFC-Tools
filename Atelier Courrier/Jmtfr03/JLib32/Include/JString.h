//
// Fichier: JString.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_STRING_H
#define JLIB_STRING_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JLimits.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

class JString
{
public:
	// les constructeurs
	JString();
	JString(const JString & Source);
	JString(const JChar* pSource);

	// la fonction pour tester la cha�ne
	JBool IsEmpty() const;

	// la fonction pour r�cup�rer la cl� de hashage
	JInt32 GetHashKey(JInt32 Size) const;

	// les fonctions pour s�rialiser la cha�ne
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour r�cup�rer la cha�ne
	const JChar* AsJCharPtr() const;

	// les op�rateurs d'affectation
	JString & operator =(const JChar* pSource);
	JString & operator =(const JString & Source);

	// les op�rateurs de comparaison
	JBool operator ==(const JString & Reference) const;
	JBool operator !=(const JString & Reference) const;
	JBool operator < (const JString & Reference) const;
	JBool operator <=(const JString & Reference) const;
	JBool operator > (const JString & Reference) const;
	JBool operator >=(const JString & Reference) const;

	// les fonctions pour mettre en forme la cha�ne
	JVoid TrimRight();
	JVoid TrimLeft();

	// la fonction pour r�initialiser la cha�ne
	JVoid Reset();

	// le destructeur
	~JString();

private:
	// la fonction pour remplacer la cha�ne
	JVoid OnReplaceString(const JChar* pSource);

private:
	// les fonctions pour formater la cha�ne
	JVoid OnShiftString(JInt32 Start, JInt32 Offset);
	JVoid OnTrimRight(const JChar* pSource, JInt32 & Start, JInt32 & Offset) const;
	JVoid OnTrimLeft(const JChar* pSource, JInt32 & Start, JInt32 & Offset) const;

private:
	// les fonctions pour manipuler la cha�ne
	JInt32 OnCompareString(const JChar* pSource, const JChar* pReference) const;
	JInt32 OnCheckString(const JChar* pSource) const;

private:
	JChar m_String[JSTRING_MAX]; // la cha�ne
};

// on finit l'inclusion conditionnelle
#endif
