//
// Fichier: JPathManager.h
// Auteur:  Sylvain SAMMURI
// Date:    21/10/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_PATHMANAGER_H
#define JLIB_PATHMANAGER_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JPath.h"

class JPathManager
{
public:
	// la fonction pour cr�er un r�pertoire
	static JVoid MakeDirectory(const JPath & Path);

	// la fonction pour tester un r�pertoire
	static JBool IsDirectoryExists(const JPath & Path);

	// la fonction pour tester un fichier
	static JBool IsFileExists(const JPath & Path);

	// la fonction pour valider une mise � jour de fichier
	static JVoid UpdateFile(const JPath & Path);

	// la fonction pour d�truire un fichier
	static JVoid RemoveFile(const JPath & Path);
};

// on finit l'inclusion conditionnelle
#endif
