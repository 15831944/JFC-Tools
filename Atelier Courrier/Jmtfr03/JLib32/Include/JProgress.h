//
// Fichier: JProgress.h
// Auteur:  Sylvain SAMMURI
// Date:    19/02/2004
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_PROGRESS_H
#define JLIB_PROGRESS_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

class JProgress
{
protected:
	// le constructeur
	JProgress();

public:
	// la fonction pour définir le nombre d'incréments
	virtual JVoid SetCount(JInt32 Count) = 0;

	// la fonction pour passer à l'incrément suivant
	virtual JVoid StepIt() = 0;

	// le destructeur
	virtual ~JProgress();

private:
	// l'opérateur d'affectation
	JProgress & operator =(const JProgress & Source);

	// le constructeur copie
	JProgress(const JProgress & Source);
};

// on finit l'inclusion conditionnelle
#endif
