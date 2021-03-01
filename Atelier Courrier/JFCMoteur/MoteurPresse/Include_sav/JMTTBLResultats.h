//
// Fichier: JMTTBLResultats.h
// Auteur:  Sylvain SAMMURI
// Date:    30/06/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMT_TABLE_RESULTATS_H

// on définit la macro pour éviter les inclusions multiples
#define JMT_TABLE_RESULTATS_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYPoids.h"
#include "JMTFR03DATPoids.h"

// on inclut les définitions nécessaires
#include "JMTFR03TBLEchantillons.h"
#include "JMTFR03TBLHomotheties.h"

// on inclut les définitions nécessaires
#include "JMTTBLDistributions.h"

// on inclut les définitions nécessaires
#include "JMTFR03DATCible.h"
#include "JMTFR03TBLPonderations.h"

class JMTTBLResultats
{
public:
	// le constructeur
	JMTTBLResultats();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour définir les éléments
	JVoid SetItems(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations, const JMTTBLDistributions & TBLDistributions);

	// la fonction pour récupérer les éléments
	const JFlt64 & Item(JInt32 Index) const;

	// la fonction pour permuter les éléments
	JVoid Swap(JMTTBLResultats & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTTBLResultats();

private:
	// l'opérateur pour recopier les éléments
	JMTTBLResultats & operator =(const JMTTBLResultats & Source);

	// le constructeur copie
	JMTTBLResultats(const JMTTBLResultats & Source);

private:
	JArray<JFlt64> m_TBLValues; // la table des valeurs
};

// fin de l'inclusion conditionnelle
#endif
