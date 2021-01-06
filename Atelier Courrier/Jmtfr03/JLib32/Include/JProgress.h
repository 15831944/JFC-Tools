//
// Fichier: JProgress.h
// Auteur:  Sylvain SAMMURI
// Date:    19/02/2004
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_PROGRESS_H
#define JLIB_PROGRESS_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

class JProgress
{
protected:
	// le constructeur
	JProgress();

public:
	// la fonction pour d�finir le nombre d'incr�ments
	virtual JVoid SetCount(JInt32 Count) = 0;

	// la fonction pour passer � l'incr�ment suivant
	virtual JVoid StepIt() = 0;

	// le destructeur
	virtual ~JProgress();

private:
	// l'op�rateur d'affectation
	JProgress & operator =(const JProgress & Source);

	// le constructeur copie
	JProgress(const JProgress & Source);
};

// on finit l'inclusion conditionnelle
#endif
