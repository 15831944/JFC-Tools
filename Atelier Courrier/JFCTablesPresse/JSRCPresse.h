/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JSRCPresse
//
//	Descriptif:
//	Point d'entrée de la Dll JFCTablesPresse regroupe:
//	la table des titres
//	la table des periodicités
//	la table des formats
//	la table des filtres d'audiences
//	DesignPattern: Singleton
*/

//
// Fichier: JSRCPresse.h
// Date:    11/03/2003
//
// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JPRESSE_SOURCE_PRESSE_H

// on définit la macro pour éviter les inclusions multiples
#define JPRESSE_SOURCE_PRESSE_H
#pragma once

// on inclut les définitions nécessaires
#include "JTBLFormats.h"
#include "JTBLPeriodicites.h"
#include "JTBLTitres.h"
#include "JTBLCouplagesFixes.h"
#include "JTBLFiltresAudience.h"



class AFX_EXT_CLASS JSRCPresse
{
private:
	/// le constructeur
	JSRCPresse();

private:
	/// l'unique instance des sources
	static JSRCPresse m_Instance;

public:
	/// la fonction pour récupérer l'instance des sources
	static const JSRCPresse & GetInstance();

public:
	/// le destructeur
	~JSRCPresse();

private:
	/// l'opérateur pour recopier les éléments
	JSRCPresse & operator =(const JSRCPresse & Source);

	/// le constructeur copie
	JSRCPresse(const JSRCPresse & Source);

public:

	JTBLFiltresAudience		m_TBLFiltresAudience;	///< les filtres d'audience


	JTBLPeriodicites		m_TBLPeriodicites;		///< les periodicites
	JTBLFormats				m_TBLFormats;			///< les formats
	JTBLTitres				m_TBLTitres;			///< les titres
//	JTBLCouplagesFixes		m_TBLCouplagesFixes;	///< les couplages fixes

};

// fin de l'inclusion conditionnelle
#endif // !defined(JPRESSE_SOURCE_PRESSE_H)
