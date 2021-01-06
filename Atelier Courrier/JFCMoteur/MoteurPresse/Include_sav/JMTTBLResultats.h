//
// Fichier: JMTTBLResultats.h
// Auteur:  Sylvain SAMMURI
// Date:    30/06/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMT_TABLE_RESULTATS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMT_TABLE_RESULTATS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYPoids.h"
#include "JMTFR03DATPoids.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLEchantillons.h"
#include "JMTFR03TBLHomotheties.h"

// on inclut les d�finitions n�cessaires
#include "JMTTBLDistributions.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATCible.h"
#include "JMTFR03TBLPonderations.h"

class JMTTBLResultats
{
public:
	// le constructeur
	JMTTBLResultats();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour d�finir les �l�ments
	JVoid SetItems(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations, const JMTTBLDistributions & TBLDistributions);

	// la fonction pour r�cup�rer les �l�ments
	const JFlt64 & Item(JInt32 Index) const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTTBLResultats & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTTBLResultats();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTTBLResultats & operator =(const JMTTBLResultats & Source);

	// le constructeur copie
	JMTTBLResultats(const JMTTBLResultats & Source);

private:
	JArray<JFlt64> m_TBLValues; // la table des valeurs
};

// fin de l'inclusion conditionnelle
#endif
