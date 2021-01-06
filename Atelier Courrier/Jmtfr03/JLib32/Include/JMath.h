//
// Fichier: JMath.h
// Auteur:  Sylvain SAMMURI
// Date:    13/05/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_MATH_H
#define JLIB_MATH_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

class JMath
{
public:
	// la fonction pour tester la validité d'une valeur
	static JBool IsFinite(JFlt64 X);

	// les fonctions pour calculer les fonctions de base
	static JFlt64 Abs(JFlt64 X);
	static JFlt64 Chs(JFlt64 X);
	static JFlt64 Round(JFlt64 X);
	static JFlt64 Sqrt(JFlt64 X);

	// les fonctions pour calculer les logarithmes et puissances en base 2
	static JFlt64 Log2(JFlt64 X);
	static JFlt64 Pow2(JFlt64 X);

	// les fonctions pour calculer les logarithmes et puissances en base e
	static JFlt64 LogE(JFlt64 X);
	static JFlt64 PowE(JFlt64 X);

	// les fonctions pour calculer les logarithmes et puissances en base 10
	static JFlt64 Log10(JFlt64 X);
	static JFlt64 Pow10(JFlt64 X);

	// les fonctions pour calculer les logarithmes et puissances
	static JFlt64 Log(JFlt64 X, JFlt64 Y);
	static JFlt64 Pow(JFlt64 X, JFlt64 Y);
	static JFlt64 Pow(JFlt64 X, JInt32 N);

	// les fonctions pour calculer les fonctions trigonométriques (en radians)
	static JFlt64 Sin(JFlt64 X);
	static JFlt64 Cos(JFlt64 X);
	static JFlt64 Tan(JFlt64 X);
};

// on finit l'inclusion conditionnelle
#endif
