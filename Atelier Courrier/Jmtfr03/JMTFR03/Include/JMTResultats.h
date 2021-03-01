//
// Fichier: JMTResultats.h
// Auteur:  Sylvain SAMMURI
// Date:    30/06/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMT_RESULTATS_H

// on définit la macro pour éviter les inclusions multiples
#define JMT_RESULTATS_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYPoids.h"
#include "JMTFR03DATPoids.h"

// on inclut les définitions nécessaires
#include "JMTFR03TBLEchantillons.h"
#include "JMTFR03TBLHomotheties.h"

// on inclut les définitions nécessaires
#include "JMTTBLVolContacts.h"
#include "JMTTBLAudiences.h"

// on inclut les définitions nécessaires
#include "JMTFR03DATCible.h"
#include "JMTFR03TBLPonderations.h"

class JMTResultats
{
public:
	// les constructeurs
	JMTResultats();
	JMTResultats(const JMTResultats & Source);

	// la fonction pour récupérer l'élément
	JFlt64 AsJFlt64() const;

	// les fonctions pour définir l'élément
	JVoid SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations);
	JVoid SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations, const JMTTBLAudiences & TBLAudiences);
	JVoid SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations, const JMTTBLVolContacts & TBLVolContacts);
	JVoid SetItem(const JBitArray & ItemIndividus, const JMTFR03KEYPoids & KEYPoids, const JMTFR03DATPoids & DATPoids, const JMTFR03TBLEchantillons & TBLEchantillons, const JMTFR03TBLHomotheties & TBLHomotheties, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations);

	// l'opérateur pour recopier l'élément
	JMTResultats & operator =(const JMTResultats & Source);

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTResultats();

private:
	JFlt64 m_Value; // la valeur
};

// fin de l'inclusion conditionnelle
#endif
