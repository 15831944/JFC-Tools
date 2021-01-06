//
// Fichier: JMTTBLAudiences.h
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMT_TABLE_AUDIENCES_H

// on définit la macro pour éviter les inclusions multiples
#define JMT_TABLE_AUDIENCES_H

// on inclut les définitions nécessaires
#include "JMTFR03DATVariable.h"
#include "JMTFR03DATLectorat.h"

// on inclut les définitions nécessaires
#include "JMTTBLVolContacts.h"

class JMTTBLAudiences  
{
public:
	// le constructeur
	JMTTBLAudiences();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour définir le nombre d'éléments
	JVoid SetCount(JInt32 NbIndividus);

	// les fonctions pour accumuler les éléments
	JVoid AddItems(const JMTFR03DATLectorat & DATLectorat);
	JVoid AddItems(const JMTFR03DATVariable & DATVariable, JInt32 NbInsertions);
	JVoid AddItems(const JMTFR03DATVariable & DATVariable, JInt32 PtEtalonnage, JInt32 NbJours);
	JVoid AddItems(const JMTTBLVolContacts  & TBLVolContacts);

	// la fonction pour récupérer les éléments
	const JFlt64 & Item(JInt32 IdxIndividu) const;

	// la fonction pour permuter les éléments
	JVoid Swap(JMTTBLAudiences & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTTBLAudiences();

private:
	// l'opérateur pour recopier les éléments
	JMTTBLAudiences & operator =(const JMTTBLAudiences & Source);

	// le constructeur copie
	JMTTBLAudiences(const JMTTBLAudiences & Source);

private:
	JArray<JFlt64> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
