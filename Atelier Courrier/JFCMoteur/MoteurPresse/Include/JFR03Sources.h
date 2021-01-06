//
// Fichier: JFR03Sources.h
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JFR03_SOURCES_H

// on définit la macro pour éviter les inclusions multiples
#define JFR03_SOURCES_H

// on inclut les définitions nécessaires
#include "JFR03TBLSources.h"
#include "JFR03TBLTerrains.h"

class JFR03Sources
{
private:
	// le constructeur
	JFR03Sources();

private:
	// l'unique instance des éléments
	static JFR03Sources m_Instance;

public:
	// la fonction pour récupérer l'instance des éléments
	static const JFR03Sources & GetInstance(JBool Reload = false);

public:
	// le destructeur
	~JFR03Sources();

private:
	// l'opérateur pour recopier les éléments
	JFR03Sources & operator =(const JFR03Sources & Source);

	// le constructeur copie
	JFR03Sources(const JFR03Sources & Source);

public:
	JFR03TBLSources  m_TBLSources;  // les sources
	JFR03TBLTerrains m_TBLTerrains; // les terrains
};

// fin de l'inclusion conditionnelle
#endif
