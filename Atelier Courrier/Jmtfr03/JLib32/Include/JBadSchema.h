//
// Fichier: JBadSchema.h
// Auteur:  Sylvain SAMMURI
// Date:    20/08/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_BADSCHEMA_H
#define JLIB_BADSCHEMA_H

// on inclut les définitions nécessaires
#include "..\Include\JException.h"

class JBadSchema : public JException
{
private:
	// le constructeur
	JBadSchema();

private:
	// l'unique instance de l'exception
	static JBadSchema m_Instance;

public:
	// la fonction pour récupérer l'instance de l'exception
	static JBadSchema* GetInstance();

public:
	// la fonction pour récupérer le message
	virtual const JChar* GetMessage() const;

	// le destructeur
	virtual ~JBadSchema();

private:
	// l'opérateur d'affectation
	JBadSchema & operator =(const JBadSchema & Source);

	// le constructeur copie
	JBadSchema(const JBadSchema & Source);
};

// on finit l'inclusion conditionnelle
#endif
