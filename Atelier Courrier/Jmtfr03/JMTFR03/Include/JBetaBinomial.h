//
// Fichier: JBetaBinomial.h
// Auteur:  Sylvain SAMMURI
// Date:    09/05/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JBETABINOMIAL_H

// on d�finit la macro pour �viter les inclusions multiples
#define JBETABINOMIAL_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JBetaBinomial
{
public:
	// le constructeur
	JBetaBinomial();

	// les fonctions pour calculer les �l�ments
	JVoid SetItems(JFlt64 A, JFlt64 B, JInt32 N);
	JVoid SetItems(JFlt64 A, JFlt64 B, JFlt64 N); // avec interpolation lin�raire

	// la fonction pour r�cup�rer les �l�ments
	JFlt64 Item(JInt32 K) const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JBetaBinomial & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JBetaBinomial();

private:
	// les fonctions pour calculer les �l�ments
	JVoid OnMakeItems(JFlt64 A, JFlt64 B, JInt32 N, JArray<JFlt64> & Items) const;
	JVoid OnMakeItems(JFlt64 A, JFlt64 B, JInt32 N1, JInt32 N2, JFlt64 N, JArray<JFlt64> & Items) const;

private:
	// l'op�rateur pour recopier les �l�ments
	JBetaBinomial & operator =(const JBetaBinomial & Source);

	// le constructeur copie
	JBetaBinomial(const JBetaBinomial & Source);

private:
	JArray<JFlt64> m_Items; // les �l�ments
};

// fin de l'inclusion conditionnelle
#endif
