//
// Fichier: JMTResultats.h
// Auteur:  Sylvain SAMMURI
// Date:    30/06/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMT_RESULTATS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMT_RESULTATS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYPoids.h"
#include "JMTFR03DATPoids.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLEchantillons.h"
#include "JMTFR03TBLHomotheties.h"

// on inclut les d�finitions n�cessaires
#include "JMTTBLVolContacts.h"
#include "JMTTBLAudiences.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATCible.h"
#include "JMTFR03TBLPonderations.h"

class JMTResultats
{
public:
	// les constructeurs
	JMTResultats();
	JMTResultats(const JMTResultats & Source);

	// la fonction pour r�cup�rer l'�l�ment
	JFlt64 AsJFlt64() const;

	// les fonctions pour d�finir l'�l�ment
	JVoid SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations);
	JVoid SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations, const JMTTBLAudiences & TBLAudiences);
	JVoid SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations, const JMTTBLVolContacts & TBLVolContacts);
	JVoid SetItem(const JBitArray & ItemIndividus, const JMTFR03KEYPoids & KEYPoids, const JMTFR03DATPoids & DATPoids, const JMTFR03TBLEchantillons & TBLEchantillons, const JMTFR03TBLHomotheties & TBLHomotheties, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations);

	// l'op�rateur pour recopier l'�l�ment
	JMTResultats & operator =(const JMTResultats & Source);

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTResultats();

private:
	JFlt64 m_Value; // la valeur
};

// fin de l'inclusion conditionnelle
#endif
