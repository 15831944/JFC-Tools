//
// Fichier: JFpu.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_FPU_H
#define JLIB_FPU_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

class JFpu
{
public:
	// le constructeur
	JFpu();

	// les fonctions pour modifier le mode d'arrondis
	JVoid SetRoundingModeUp();
	JVoid SetRoundingModeNear();
	JVoid SetRoundingModeShop();
	JVoid SetRoundingModeDown();

	// les fonctions pour récupérer l'état des indicateurs
	JBool GetInvalidOperationFlag(JBool Update);
	JBool GetOutOfRangeFlag(JBool Update);
	JBool GetPrecisionFlag(JBool Update);

	// les fonctions pour manipuler les indicateurs
	JVoid UpdateFlags();
	JVoid ResetFlags();

	// le destructeur
	~JFpu();

private:
	// l'opérateur d'affectation
	JFpu & operator =(const JFpu & Source);

	// le constructeur copie
	JFpu(const JFpu & Source);

private:
	JUnt16 m_Control; // le mot de contrôle
	JUnt16 m_Status;  // le mot d'état
};

// on finit l'inclusion conditionnelle
#endif
