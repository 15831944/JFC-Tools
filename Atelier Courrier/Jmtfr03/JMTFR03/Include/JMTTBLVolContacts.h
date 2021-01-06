//
// Fichier: JMTTBLVolContacts.h
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMT_TABLE_VOLCONTACTS_H

// on définit la macro pour éviter les inclusions multiples
#define JMT_TABLE_VOLCONTACTS_H

// on inclut les définitions nécessaires
#include "JMTFR03DATReprise.h"
#include "JMTFR03DATVariable.h"
#include "JMTFR03DATLectorat.h"

class JMTTBLVolContacts  
{
public:
	// le constructeur
	JMTTBLVolContacts();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour définir le nombre d'éléments
	JVoid SetCount(JInt32 NbIndividus);

	// les fonctions pour accumuler les éléments
	JVoid AddItems(const JMTFR03DATReprise  & DATReprise,  JInt32 NbInsertions);
	JVoid AddItems(const JMTFR03DATVariable & DATVariable, JInt32 NbInsertions);
	JVoid AddItems(const JMTFR03DATLectorat & DATLectorat, JInt32 PtEtalonnage);

	// la fonction pour récupérer les éléments
	const JFlt64 & Item(JInt32 IdxIndividu) const;

	// la fonction pour permuter les éléments
	JVoid Swap(JMTTBLVolContacts & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTTBLVolContacts();

private:
	// l'opérateur pour recopier les éléments
	JMTTBLVolContacts & operator =(const JMTTBLVolContacts & Source);

	// le constructeur copie
	JMTTBLVolContacts(const JMTTBLVolContacts & Source);

private:
	JArray<JFlt64> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
