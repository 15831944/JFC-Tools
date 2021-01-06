//
// Fichier: JMTTBLDistributions.h
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMT_TABLE_DISTRIBUTIONS_H

// on définit la macro pour éviter les inclusions multiples
#define JMT_TABLE_DISTRIBUTIONS_H

// on inclut les définitions nécessaires
#include "JMTFR03DATVariable.h"
#include "JMTFR03DATLectorat.h"

class JMTTBLDistributions  
{
public:
	// le constructeur
	JMTTBLDistributions();

	// les fonctions pour récupérer le nombre d'éléments
	JInt32 GetRowCount() const;
	JInt32 GetColCount() const;

	// la fonction pour définir le nombre d'éléments
	JVoid SetCount(JInt32 NbIndividus, JInt32 NbClasses);

	// la fonction pour accumuler les éléments
	JVoid AddItems(const JMTFR03DATVariable & DATVariable, JInt32 NbInsertions, JFlt64 CoefTrusquin);

	// la fonction pour récupérer les éléments
	const JFlt64 & Item(JInt32 IdxIndividu, JInt32 IdxClasse) const;

	// la fonction pour permuter les éléments
	JVoid Swap(JMTTBLDistributions & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTTBLDistributions();

private:
	// l'opérateur pour recopier les éléments
	JMTTBLDistributions & operator =(const JMTTBLDistributions & Source);

	// le constructeur copie
	JMTTBLDistributions(const JMTTBLDistributions & Source);

private:
	JMatrix<JFlt64> m_TBLItems; // la table des éléments

private:
	JArray<JInt32> m_TBLMax; // la table des maximums
};

// fin de l'inclusion conditionnelle
#endif
