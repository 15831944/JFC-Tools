/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JSRCTables
//
//	Descriptif:
//	Point d'entrée de la Dll JFCTables regroupe:
//	la table des sources
//	la table des terrains
//	la table des segments
//	la table des echantillons
//	la table des univers de redressement
//	la table des dimensions
//	DesignPattern: Singleton
*/


//
// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JSRCTABLES_H

// on définit la macro pour éviter les inclusions multiples
#define JSRCTABLES_H
#pragma once

// on inclut les définitions nécessaires
#include "JTBLSources.h"
#include "JTBLTerrains.h"
#include "JTBLSegments.h"
#include "JTBLPoids.h"
#include "JTBLEchantillons.h"
#include "JTBLUnivRedressement.h"
#include "JTBLDimensions.h"



class AFX_EXT_CLASS JSRCTables
{
private:
	/// le constructeur
	JSRCTables();

private:
	/// l'unique instance des sources
	static JSRCTables m_Instance;

public:
	/// la fonction pour récupérer l'instance des sources
	static const JSRCTables & GetInstance();

public:
	/// le destructeur
	~JSRCTables();

private:
	/// l'opérateur pour recopier les éléments
	JSRCTables & operator =(const JSRCTables & Source);

	/// le constructeur copie
	JSRCTables(const JSRCTables & Source);

public:

	JTBLSources				m_TBLSources;			///< les sources
	JTBLTerrains			m_TBLTerrains;			///< les terrains
	JTBLSegments			m_TBLSegments;			///< les segments
	JTBLPoids				m_TBLPoids;				///< les types de poids normalisés
	JTBLEchantillons		m_TBLEchantillons;		///< les échantillons
	JTBLUnivRedressement	m_TBLUnivRedressement;	///< les univers de redressement
	JTBLDimensions			m_TBLDimensions;		///< les types de dimensions

};

// fin de l'inclusion conditionnelle
#endif // !defined(JSRCTABLES_H)
