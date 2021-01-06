// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_PONDERATIONS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_PONDERATIONS_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03TBLPonderations
{
public:
	// le constructeur
	JMTFR03TBLPonderations(void);
	
	// Fonction d'initialisation
	JVoid InitPonderation(JInt32 NbIndivs, JFlt64x ValInit = 1.0);

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour r�cup�rer la pond�ration d'un indiv
	JFlt64x GetPonderations(JInt32 IdxIndiv) const;
 
 	// la fonction pour positionner la pond�ration d'un indiv
	JVoid SetPonderations(JInt32 IdxIndiv, JFlt64x Ponderation);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLPonderations & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLPonderations(void);

protected:
	// le vecteur pond�ration � appliquer des individus
	JArray <JFlt64x> m_TblPonderations; 


private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLPonderations & operator =(const JMTFR03TBLPonderations & Source);

	// le constructeur copie
	JMTFR03TBLPonderations(const JMTFR03TBLPonderations & Source);
};

// fin de l'inclusion conditionnelle
#endif