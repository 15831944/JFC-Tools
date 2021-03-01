//
// Fichier: JFpu.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_FPU_H
#define JLIB_FPU_H

// on inclut les d�finitions n�cessaires
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

	// les fonctions pour r�cup�rer l'�tat des indicateurs
	JBool GetInvalidOperationFlag(JBool Update);
	JBool GetOutOfRangeFlag(JBool Update);
	JBool GetPrecisionFlag(JBool Update);

	// les fonctions pour manipuler les indicateurs
	JVoid UpdateFlags();
	JVoid ResetFlags();

	// le destructeur
	~JFpu();

private:
	// l'op�rateur d'affectation
	JFpu & operator =(const JFpu & Source);

	// le constructeur copie
	JFpu(const JFpu & Source);

private:
	JUnt16 m_Control; // le mot de contr�le
	JUnt16 m_Status;  // le mot d'�tat
};

// on finit l'inclusion conditionnelle
#endif
