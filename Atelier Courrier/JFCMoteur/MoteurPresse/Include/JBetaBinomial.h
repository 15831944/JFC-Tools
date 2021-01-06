//
// Fichier: JBetaBinomial.h
// Auteur:  Sylvain SAMMURI
// Date:    09/05/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JBETABINOMIAL_H

// on définit la macro pour éviter les inclusions multiples
#define JBETABINOMIAL_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JBetaBinomial
{
public:
	// le constructeur
	JBetaBinomial();

	// les fonctions pour calculer les éléments
	JVoid SetItems(JFlt64 A, JFlt64 B, JInt32 N);
	JVoid SetItems(JFlt64 A, JFlt64 B, JFlt64 N); // avec interpolation linéraire

	// la fonction pour récupérer les éléments
	JFlt64 Item(JInt32 K) const;

	// la fonction pour permuter les éléments
	JVoid Swap(JBetaBinomial & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JBetaBinomial();

private:
	// les fonctions pour calculer les éléments
	JVoid OnMakeItems(JFlt64 A, JFlt64 B, JInt32 N, JArray<JFlt64> & Items) const;
	JVoid OnMakeItems(JFlt64 A, JFlt64 B, JInt32 N1, JInt32 N2, JFlt64 N, JArray<JFlt64> & Items) const;

private:
	// l'opérateur pour recopier les éléments
	JBetaBinomial & operator =(const JBetaBinomial & Source);

	// le constructeur copie
	JBetaBinomial(const JBetaBinomial & Source);

private:
	JArray<JFlt64> m_Items; // les éléments
};

// fin de l'inclusion conditionnelle
#endif
