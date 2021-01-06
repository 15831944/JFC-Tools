/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JTBLSources
//
//	Descriptif:
//	Table des sources
*/

//
// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JTABLE_SOURCES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JTABLE_SOURCES_H
#pragma once

// on inclut les d�finitions n�cessaires
#include "JSource.h"



class AFX_EXT_CLASS JTBLSources : private JList<JSource>
{
public:
	/// le constructeur
	JTBLSources();

	/// la fonction pour r�cup�rer un �l�ment
	const JSource * GetItem(JInt32 IdSource) const;

	/// la fonction pour r�cup�rer un �l�ment
	const JSource & GetItem() const;

	JVoid MoveFirst() const;
	JVoid MoveLast() const;
	JBool IsValid() const;
	JVoid MovePrev() const;
	JVoid MoveNext() const;

	/// les fonctions pour importer/exporter les �l�ments
	JVoid Import(CStdioFile& file);

	/// le destructeur
	~JTBLSources();
};

// fin de l'inclusion conditionnelle
#endif
