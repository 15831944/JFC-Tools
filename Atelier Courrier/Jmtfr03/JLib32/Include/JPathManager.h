//
// Fichier: JPathManager.h
// Auteur:  Sylvain SAMMURI
// Date:    21/10/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_PATHMANAGER_H
#define JLIB_PATHMANAGER_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JPath.h"

class JPathManager
{
public:
	// la fonction pour créer un répertoire
	static JVoid MakeDirectory(const JPath & Path);

	// la fonction pour tester un répertoire
	static JBool IsDirectoryExists(const JPath & Path);

	// la fonction pour tester un fichier
	static JBool IsFileExists(const JPath & Path);

	// la fonction pour valider une mise à jour de fichier
	static JVoid UpdateFile(const JPath & Path);

	// la fonction pour détruire un fichier
	static JVoid RemoveFile(const JPath & Path);
};

// on finit l'inclusion conditionnelle
#endif
