//
// Fichier: JMTTBLVolContacts.h
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMT_TABLE_VOLCONTACTS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMT_TABLE_VOLCONTACTS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATReprise.h"
#include "JMTFR03DATVariable.h"
#include "JMTFR03DATLectorat.h"

class JMTTBLVolContacts  
{
public:
	// le constructeur
	JMTTBLVolContacts();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour d�finir le nombre d'�l�ments
	JVoid SetCount(JInt32 NbIndividus);

	// les fonctions pour accumuler les �l�ments
	JVoid AddItems(const JMTFR03DATReprise  & DATReprise,  JInt32 NbInsertions);
	JVoid AddItems(const JMTFR03DATVariable & DATVariable, JInt32 NbInsertions);
	JVoid AddItems(const JMTFR03DATLectorat & DATLectorat, JInt32 PtEtalonnage);

	// la fonction pour r�cup�rer les �l�ments
	const JFlt64 & Item(JInt32 IdxIndividu) const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTTBLVolContacts & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTTBLVolContacts();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTTBLVolContacts & operator =(const JMTTBLVolContacts & Source);

	// le constructeur copie
	JMTTBLVolContacts(const JMTTBLVolContacts & Source);

private:
	JArray<JFlt64> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
