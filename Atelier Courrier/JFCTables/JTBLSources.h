/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTBLSources
//
//	Descriptif:
//	Table des sources
*/

//
// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JTABLE_SOURCES_H

// on définit la macro pour éviter les inclusions multiples
#define JTABLE_SOURCES_H
#pragma once

// on inclut les définitions nécessaires
#include "JSource.h"



class AFX_EXT_CLASS JTBLSources : private JList<JSource>
{
public:
	/// le constructeur
	JTBLSources();

	/// la fonction pour récupérer un élément
	const JSource * GetItem(JInt32 IdSource) const;

	/// la fonction pour récupérer un élément
	const JSource & GetItem() const;

	JVoid MoveFirst() const;
	JVoid MoveLast() const;
	JBool IsValid() const;
	JVoid MovePrev() const;
	JVoid MoveNext() const;

	/// les fonctions pour importer/exporter les éléments
	JVoid Import(CStdioFile& file);

	/// le destructeur
	~JTBLSources();
};

// fin de l'inclusion conditionnelle
#endif
