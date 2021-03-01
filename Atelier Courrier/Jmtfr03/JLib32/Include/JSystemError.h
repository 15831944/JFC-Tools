//
// Fichier: JSystemError.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_SYSTEMERROR_H
#define JLIB_SYSTEMERROR_H

// on inclut les définitions nécessaires
#include "..\Include\JException.h"

class JSystemError : public JException
{
private:
	// le constructeur
	JSystemError();

private:
	// l'unique instance de l'exception
	static JSystemError m_Instance;

public:
	// la fonction pour récupérer l'instance de l'exception
	static JSystemError* GetInstance();

public:
	// la fonction pour récupérer le message
	virtual const JChar* GetMessage() const;

	// le destructeur
	virtual ~JSystemError();

private:
	// l'opérateur d'affectation
	JSystemError & operator =(const JSystemError & Source);

	// le constructeur copie
	JSystemError(const JSystemError & Source);
};

// on finit l'inclusion conditionnelle
#endif
