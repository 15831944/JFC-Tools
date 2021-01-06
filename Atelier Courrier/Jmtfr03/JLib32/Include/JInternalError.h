//
// Fichier: JInternalError.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_INTERNALERROR_H
#define JLIB_INTERNALERROR_H

// on inclut les définitions nécessaires
#include "..\Include\JException.h"

class JInternalError : public JException
{
private:
	// le constructeur
	JInternalError();

private:
	// l'unique instance de l'exception
	static JInternalError m_Instance;

public:
	// la fonction pour récupérer l'instance de l'exception
	static JInternalError* GetInstance();

public:
	// la fonction pour récupérer le message
	virtual const JChar* GetMessage() const;

	// le destructeur
	virtual ~JInternalError();

private:
	// l'opérateur d'affectation
	JInternalError & operator =(const JInternalError & Source);

	// le constructeur copie
	JInternalError(const JInternalError & Source);
};

// on finit l'inclusion conditionnelle
#endif
