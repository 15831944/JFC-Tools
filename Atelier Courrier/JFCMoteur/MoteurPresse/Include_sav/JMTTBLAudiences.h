//
// Fichier: JMTTBLAudiences.h
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMT_TABLE_AUDIENCES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMT_TABLE_AUDIENCES_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATVariable.h"
#include "JMTFR03DATLectorat.h"

// on inclut les d�finitions n�cessaires
#include "JMTTBLVolContacts.h"

class JMTTBLAudiences  
{
public:
	// le constructeur
	JMTTBLAudiences();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour d�finir le nombre d'�l�ments
	JVoid SetCount(JInt32 NbIndividus);

	// les fonctions pour accumuler les �l�ments
	JVoid AddItems(const JMTFR03DATLectorat & DATLectorat);
	JVoid AddItems(const JMTFR03DATVariable & DATVariable, JInt32 NbInsertions);
	JVoid AddItems(const JMTFR03DATVariable & DATVariable, JInt32 PtEtalonnage, JInt32 NbJours);
	JVoid AddItems(const JMTTBLVolContacts  & TBLVolContacts);

	// la fonction pour r�cup�rer les �l�ments
	const JFlt64 & Item(JInt32 IdxIndividu) const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTTBLAudiences & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTTBLAudiences();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTTBLAudiences & operator =(const JMTTBLAudiences & Source);

	// le constructeur copie
	JMTTBLAudiences(const JMTTBLAudiences & Source);

private:
	JArray<JFlt64> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
