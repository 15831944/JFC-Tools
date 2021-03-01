//
// Fichier: JMTFR03RequeteE.h
// Auteur:  Sylvain SAMMURI
// Date:    21/06/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_REQUETE_EFFECTIF_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_REQUETE_EFFECTIF_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLTerrains.h"
#include "JMTFR03TBLModulations.h"

class JMTFR03RequeteE  
{
public:
	// on d�finit la requ�te
	class JRequete
	{
	public:
		JBitArray m_ItemEffectif; // l'item de l'effectif

	public:
		const JMTFR03TBLEchantillons* m_pTBLEchantillons; // la table des �chantillons
		const JMTFR03TBLHomotheties*  m_pTBLHomotheties;  // la table des homoth�ties

	public:
		const JMTFR03KEYPoids* m_pKEYPoids; // les poids des individus
		const JMTFR03DATPoids* m_pDATPoids; // les poids des individus
	};

public:
	// le constructeur
	JMTFR03RequeteE();

	// la fonction pour tester les �l�ments
	JBool IsValid() const;

	// la fonction pour d�finir les �l�ments
	JVoid SetItems(const JMTFR03TBLTerrains & TBLTerrains, const JMTFR03TBLModulations & TBLModulations, const JRef32 & IdSource, const JRef32 & IdTerrain);

	// la fonction pour r�cup�rer les �l�ments
	const JRequete & GetItems() const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03RequeteE & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03RequeteE();

private:
	// la fonction pour tester le filtre des modulations
	JBool IsFilterActive(const JMTFR03FLTSegments & FLTSegments) const;

private:
	// la fonction pour construire la requ�te
	JBool OnMakeRequete(const JMTFR03DATTerrain & DATTerrain, const JMTFR03DATModulation & DATModulation, JRequete & Requete);

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03RequeteE & operator =(const JMTFR03RequeteE & Source);

	// le constructeur copie
	JMTFR03RequeteE(const JMTFR03RequeteE & Source);

private:
	JArray<JRequete> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
