// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_PONDERATIONS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_PONDERATIONS_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03TBLPonderations
{
public:
	// le constructeur
	JMTFR03TBLPonderations(void);
	
	// Fonction d'initialisation
	JVoid InitPonderation(JInt32 NbIndivs, JFlt64x ValInit = 1.0);

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour récupérer la pondération d'un indiv
	JFlt64x GetPonderations(JInt32 IdxIndiv) const;
 
 	// la fonction pour positionner la pondération d'un indiv
	JVoid SetPonderations(JInt32 IdxIndiv, JFlt64x Ponderation);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLPonderations & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLPonderations(void);

protected:
	// le vecteur pondération à appliquer des individus
	JArray <JFlt64x> m_TblPonderations; 


private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLPonderations & operator =(const JMTFR03TBLPonderations & Source);

	// le constructeur copie
	JMTFR03TBLPonderations(const JMTFR03TBLPonderations & Source);
};

// fin de l'inclusion conditionnelle
#endif