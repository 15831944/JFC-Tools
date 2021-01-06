//
// Fichier: JMemento.h
// Auteur:  Sylvain SAMMURI
// Date:    26/06/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_MEMENTO_H
#define JLIB_MEMENTO_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JStream.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JSerialize.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JMemory.h"

class JMemento : public JStream
{
private:
	// le constructeur
	JMemento(JSerialize* pPrev, JSerialize* pNext);

public:
	// la fonction pour cr�er une instance
	static JMemento* Create();

	// le destructeur
	virtual ~JMemento();

private:
	// l'op�rateur d'affectation
	JMemento & operator =(const JMemento & Source);

	// le constructeur copie
	JMemento(const JMemento & Source);

private:
	JMemory* m_pMemory; // la m�moire
};

// on finit l'inclusion conditionnelle
#endif
