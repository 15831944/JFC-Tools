/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JSRCPresse
//
//	Descriptif:
//	Point d'entr�e de la Dll JFCTablesPresse regroupe:
//	la table des titres
//	la table des periodicit�s
//	la table des formats
//	la table des filtres d'audiences
//	DesignPattern: Singleton
*/

//
// Fichier: JSRCPresse.h
// Date:    11/03/2003
//
// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JPRESSE_SOURCE_PRESSE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JPRESSE_SOURCE_PRESSE_H
#pragma once

// on inclut les d�finitions n�cessaires
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
	/// la fonction pour r�cup�rer l'instance des sources
	static const JSRCPresse & GetInstance();

public:
	/// le destructeur
	~JSRCPresse();

private:
	/// l'op�rateur pour recopier les �l�ments
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
