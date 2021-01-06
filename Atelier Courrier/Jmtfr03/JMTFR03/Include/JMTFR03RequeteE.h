//
// Fichier: JMTFR03RequeteE.h
// Auteur:  Sylvain SAMMURI
// Date:    21/06/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_REQUETE_EFFECTIF_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_REQUETE_EFFECTIF_H

// on inclut les définitions nécessaires
#include "JMTFR03TBLTerrains.h"
#include "JMTFR03TBLModulations.h"

class JMTFR03RequeteE  
{
public:
	// on définit la requête
	class JRequete
	{
	public:
		JBitArray m_ItemEffectif; // l'item de l'effectif

	public:
		const JMTFR03TBLEchantillons* m_pTBLEchantillons; // la table des échantillons
		const JMTFR03TBLHomotheties*  m_pTBLHomotheties;  // la table des homothéties

	public:
		const JMTFR03KEYPoids* m_pKEYPoids; // les poids des individus
		const JMTFR03DATPoids* m_pDATPoids; // les poids des individus
	};

public:
	// le constructeur
	JMTFR03RequeteE();

	// la fonction pour tester les éléments
	JBool IsValid() const;

	// la fonction pour définir les éléments
	JVoid SetItems(const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JRef32 & IdSource, const JRef32 & IdTerrain);

	// la fonction pour récupérer les éléments
	const JRequete & GetItems() const;

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03RequeteE & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03RequeteE();

private:
	// la fonction pour tester le filtre des modulations
	JBool IsFilterActive(const JMTFR03FLTSegments & FLTSegments) const;

private:
	// la fonction pour construire la requête
	JBool OnMakeRequete(const JMTFR03DATTerrain & DATTerrain, const JMTFR03DATModulation & DATModulation, JRequete & Requete);

private:
	// l'opérateur pour recopier les éléments
	JMTFR03RequeteE & operator =(const JMTFR03RequeteE & Source);

	// le constructeur copie
	JMTFR03RequeteE(const JMTFR03RequeteE & Source);

private:
	JArray<JRequete> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
