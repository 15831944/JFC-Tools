//
// Fichier: JMemory.h
// Auteur:  Sylvain SAMMURI
// Date:    26/06/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_MEMORY_H
#define JLIB_MEMORY_H

// on inclut les définitions nécessaires
#include "..\Include\JSerialize.h"

// on inclut les définitions nécessaires
#include "..\Include\JLimits.h"

class JMemory : public JSerialize
{
private:
	// le constructeur
	JMemory();

public:
	// la fonction pour créer une instance
	static JMemory* Create();

public:
	// la fonction pour tester le tampon
	virtual JBool IsEmpty();

	// les fonctions pour transférer les données
	virtual JVoid  Send(JUnt08 Data);
	virtual JUnt08 Recv();

	// la fonction pour vider le tampon
	virtual JVoid Flush();

	// le destructeur
	virtual ~JMemory();

private:
	// on définit les noeuds de la liste
	class JNode
	{
	public:
		JNode* m_pNext;               // le noeud suivant
		JInt32 m_RPointer;            // le pointeur de lecture
		JInt32 m_WPointer;            // le pointeur d'écriture
		JUnt08 m_Buffer[JMEMORY_MAX]; // le tampon du noeud
	};

private:
	// les fonctions pour manipuler les noeuds
	JBool OnAddTailNode();
	JVoid OnRemHeadNode();

private:
	// la fonction pour supprimer les noeuds
	JVoid OnRemNodes();

private:
	// l'opérateur d'affectation
	JMemory & operator =(const JMemory & Source);

	// le constructeur copie
	JMemory(const JMemory & Source);

private:
	JNode* m_pRNode; // le noeud de lecture
	JNode* m_pWNode; // le noeud d'écriture
};

// on finit l'inclusion conditionnelle
#endif
