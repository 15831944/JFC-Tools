//
// Fichier: JHost.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_HOST_H
#define JLIB_HOST_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JLimits.h"

// on inclut les définitions nécessaires
#include "..\Include\JStream.h"

class JHost
{
public:
	// les constructeurs
	JHost();
	JHost(const JHost & Source);
	JHost(const JChar* pSource);

	// les fonctions pour tester le serveur
	JBool IsLocal() const;
	JBool IsDistant() const;

	// la fonction pour récupérer la clé de hashage
	JInt32 GetHashKey(JInt32 Size) const;

	// les fonctions pour sérialiser le serveur
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour récupérer le serveur
	const JChar* AsJCharPtr() const;

	// les opérateurs d'affectation
	JHost & operator =(const JChar* pSource);
	JHost & operator =(const JHost & Source);

	// les opérateurs de comparaison
	JBool operator ==(const JHost & Reference) const;
	JBool operator !=(const JHost & Reference) const;
	JBool operator < (const JHost & Reference) const;
	JBool operator <=(const JHost & Reference) const;
	JBool operator > (const JHost & Reference) const;
	JBool operator >=(const JHost & Reference) const;

	// la fonction pour réinitialiser le serveur
	JVoid Reset();

	// le destructeur
	~JHost();

private:
	// les fonctions pour formater le serveur
	JInt32 OnTrimLeft(const JChar* pSource) const;
	JInt32 OnTrimRight(const JChar* pSource, JInt32 Start) const;

private:
	// les fonctions pour remplacer le serveur
	JVoid OnReplaceHost(const JChar* pSource, JInt32 Start, JInt32 Count);
	JVoid OnReplaceHost(const JChar* pSource);

private:
	// les fonctions pour manipuler le serveur
	JInt32 OnCompareHost(const JChar* pSource, const JChar* pReference) const;
	JInt32 OnCheckHost(const JChar* pSource, JInt32 Type) const;

private:
	JChar m_Host[JHOST_MAX]; // le serveur
};

// on finit l'inclusion conditionnelle
#endif
