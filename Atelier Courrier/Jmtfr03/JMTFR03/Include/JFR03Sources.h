//
// Fichier: JFR03Sources.h
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JFR03_SOURCES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JFR03_SOURCES_H

// on inclut les d�finitions n�cessaires
#include "JFR03TBLSources.h"
#include "JFR03TBLTerrains.h"

class JFR03Sources
{
private:
	// le constructeur
	JFR03Sources();

private:
	// l'unique instance des �l�ments
	static JFR03Sources m_Instance;

public:
	// la fonction pour r�cup�rer l'instance des �l�ments
	static const JFR03Sources & GetInstance(JBool Reload = false);

public:
	// le destructeur
	~JFR03Sources();

private:
	// l'op�rateur pour recopier les �l�ments
	JFR03Sources & operator =(const JFR03Sources & Source);

	// le constructeur copie
	JFR03Sources(const JFR03Sources & Source);

public:
	JFR03TBLSources  m_TBLSources;  // les sources
	JFR03TBLTerrains m_TBLTerrains; // les terrains
};

// fin de l'inclusion conditionnelle
#endif
