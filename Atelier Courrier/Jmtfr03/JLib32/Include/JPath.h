//
// Fichier: JPath.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_PATH_H
#define JLIB_PATH_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JLimits.h"

class JPath
{
public:
	// les constructeurs
	JPath();
	JPath(const JPath & Source);
	JPath(const JChar* pSource);

	// les fonctions pour tester le chemin
	JBool IsFilename() const;
	JBool IsDirectory() const;

	// la fonction pour r�cup�rer la cl� de hashage
	JInt32 GetHashKey(JInt32 Size) const;

	// les fonctions pour s�rialiser le chemin
	JVoid Send(JStream & Stream) const;
	JVoid Recv(JStream & Stream);

	// la fonction pour r�cup�rer le chemin
	const JChar* AsJCharPtr() const;

	// les op�rateurs d'affectation
	JPath & operator =(const JChar* pSource);
	JPath & operator =(const JPath & Source);

	// les op�rateurs de comparaison
	JBool operator ==(const JPath & Reference) const;
	JBool operator !=(const JPath & Reference) const;
	JBool operator < (const JPath & Reference) const;
	JBool operator <=(const JPath & Reference) const;
	JBool operator > (const JPath & Reference) const;
	JBool operator >=(const JPath & Reference) const;

	// la fonction pour r�initialiser le chemin
	JVoid Reset();

	// le destructeur
	~JPath();

private:
	// les fonctions pour formater le chemin
	JInt32 OnTrimLeft(const JChar* pSource) const;
	JInt32 OnTrimRight(const JChar* pSource, JInt32 Start) const;

private:
	// les fonctions pour remplacer le chemin
	JVoid OnReplacePath(const JChar* pSource, JInt32 Start, JInt32 Count);
	JVoid OnReplacePath(const JChar* pSource);

private:
	// les fonctions pour manipuler le chemin
	JInt32 OnComparePath(const JChar* pSource, const JChar* pReference) const;
	JInt32 OnCheckPath(const JChar* pSource, JInt32 Type) const;

private:
	JChar m_Path[JPATH_MAX]; // le chemin
};

// on finit l'inclusion conditionnelle
#endif
