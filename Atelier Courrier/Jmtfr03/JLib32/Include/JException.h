//
// Fichier: JException.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_EXCEPTION_H
#define JLIB_EXCEPTION_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

class JException
{
protected:
	// le constructeur
	JException();

public:
	// la fonction pour récupérer le message
	virtual const JChar* GetMessage() const = 0;

	// le destructeur
	virtual ~JException();

private:
	// l'opérateur d'affectation
	JException & operator =(const JException & Source);

	// le constructeur copie
	JException(const JException & Source);
};

// on finit l'inclusion conditionnelle
#endif
