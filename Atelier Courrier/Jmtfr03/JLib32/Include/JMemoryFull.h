//
// Fichier: JMemoryFull.h
// Auteur:  Sylvain SAMMURI
// Date:    04/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_MEMORYFULL_H
#define JLIB_MEMORYFULL_H

// on inclut les définitions nécessaires
#include "..\Include\JException.h"

class JMemoryFull : public JException
{
private:
	// le constructeur
	JMemoryFull();

private:
	// l'unique instance de l'exception
	static JMemoryFull m_Instance;

public:
	// la fonction pour récupérer l'instance de l'exception
	static JMemoryFull* GetInstance();

public:
	// la fonction pour récupérer le message
	virtual const JChar* GetMessage() const;

	// le destructeur
	virtual ~JMemoryFull();

private:
	// l'opérateur d'affectation
	JMemoryFull & operator =(const JMemoryFull & Source);

	// le constructeur copie
	JMemoryFull(const JMemoryFull & Source);
};

// on finit l'inclusion conditionnelle
#endif
