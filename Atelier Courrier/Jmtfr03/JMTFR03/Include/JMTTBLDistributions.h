//
// Fichier: JMTTBLDistributions.h
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMT_TABLE_DISTRIBUTIONS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMT_TABLE_DISTRIBUTIONS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATVariable.h"
#include "JMTFR03DATLectorat.h"

class JMTTBLDistributions  
{
public:
	// le constructeur
	JMTTBLDistributions();

	// les fonctions pour r�cup�rer le nombre d'�l�ments
	JInt32 GetRowCount() const;
	JInt32 GetColCount() const;

	// la fonction pour d�finir le nombre d'�l�ments
	JVoid SetCount(JInt32 NbIndividus, JInt32 NbClasses);

	// la fonction pour accumuler les �l�ments
	JVoid AddItems(const JMTFR03DATVariable & DATVariable, JInt32 NbInsertions, JFlt64 CoefTrusquin);

	// la fonction pour r�cup�rer les �l�ments
	const JFlt64 & Item(JInt32 IdxIndividu, JInt32 IdxClasse) const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTTBLDistributions & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTTBLDistributions();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTTBLDistributions & operator =(const JMTTBLDistributions & Source);

	// le constructeur copie
	JMTTBLDistributions(const JMTTBLDistributions & Source);

private:
	JMatrix<JFlt64> m_TBLItems; // la table des �l�ments

private:
	JArray<JInt32> m_TBLMax; // la table des maximums
};

// fin de l'inclusion conditionnelle
#endif
