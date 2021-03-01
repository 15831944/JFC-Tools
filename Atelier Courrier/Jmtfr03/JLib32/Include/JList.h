//
// Fichier: JList.h
// Auteur:  Sylvain SAMMURI
// Date:    17/07/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_LIST_H
#define JLIB_LIST_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JInternalError.h"
#include "..\Include\JInvalidCall.h"
#include "..\Include\JMemoryFull.h"

template <class JItem> class JList
{
public:
	// le constructeur
	JList();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// les fonctions pour ajouter des éléments
	JItem & AddHead(JBool Move = false);
	JItem & AddAfter(JBool Move = false);
	JItem & AddBefore(JBool Move = false);
	JItem & AddTail(JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// la fonction pour modifier l'élément du curseur
	JItem & GetItem();

	// les fonctions pour récupérer l'élément du curseur
	JInt32 GetIndex() const;
	const JItem & GetItem() const;

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(JInt32 Index) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JList<JItem> & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JList();

private:
	// on définit les noeuds de la liste
	class JNode
	{
	public:
		JNode* m_pNext; // l'élément suivant
		JNode* m_pPrev; // l'élément précédent
		JItem  m_Item;  // l'élément du noeud
	};

private:
	// les fonctions pour ajouter les noeuds
	JBool OnAddPrevNode(JNode* pRefNode, JNode* & pNewNode);
	JBool OnAddNextNode(JNode* pRefNode, JNode* & pNewNode);

	// la fonction pour supprimer un noeud
	JVoid OnRemNode(JNode* pNode);

	// la fonction pour libérer les noeuds
	JVoid OnRemNodes();

private:
	// l'opérateur d'affectation
	JList<JItem> & operator =(const JList<JItem> & Source);

	// le constructeur copie
	JList(const JList<JItem> & Source);

private:
	mutable JNode* m_pCursor; // le curseur

private:
	JNode* m_pFNode; // le premier noeud
	JNode* m_pLNode; // le dernier noeud
	JInt32 m_Count;  // la taille
};

////////////////////
// les constructeurs

template <class JItem> JList<JItem>::JList()
{
	// on initialise la taille et les noeuds
	m_Count = 0; m_pFNode = m_pLNode = m_pCursor = 0;
}

template <class JItem> JList<JItem>::JList(const JList<JItem> & Source)
{
	// on génère une exception
	throw JInternalError::GetInstance();
}

////////////////////////////
// l'opérateur d'affectation

template <class JItem> JList<JItem> & JList<JItem>::operator =(const JList<JItem> & Source)
{
	// on génère une exception
	throw JInternalError::GetInstance();
}

////////////////////////////////////////
// les fonctions pour ajouter les noeuds

template <class JItem> JBool JList<JItem>::OnAddPrevNode(JNode* pRefNode, JNode* & pNewNode)
{
	// on alloue un nouveau noeud
	JNode* pNode = new JNode; if (pNode == 0) return (false);

	// on teste le noeud de référence
	if (pRefNode != 0)
	{
		// on récupère le noeud précédent
		JNode* pPrevNode = pRefNode->m_pPrev;

		// on accroche le noeud dans la liste
		pRefNode->m_pPrev = pNode; if (pPrevNode != 0) pPrevNode->m_pNext = pNode; else m_pFNode = pNode;

		// on initialise le noeud
		pNode->m_pPrev = pPrevNode;
		pNode->m_pNext = pRefNode;
	}
	else
	{
		// on accroche le noeud dans la liste
		m_pFNode = m_pLNode = pNode;

		// on initialise le noeud
		pNode->m_pNext = 0;
		pNode->m_pPrev = 0;
	}

	// on renvoie le nouveau noeud
	pNewNode = pNode;

	// on incrémente la taille
	m_Count += 1;

	// on quitte
	return (true);
}

template <class JItem> JBool JList<JItem>::OnAddNextNode(JNode* pRefNode, JNode* & pNewNode)
{
	// on alloue un nouveau noeud
	JNode* pNode = new JNode; if (pNode == 0) return (false);

	// on teste le noeud de référence
	if (pRefNode != 0)
	{
		// on récupère le noeud suivant
		JNode* pNextNode = pRefNode->m_pNext;

		// on accroche le noeud dans la liste
		pRefNode->m_pNext = pNode; if (pNextNode != 0) pNextNode->m_pPrev = pNode; else m_pLNode = pNode;

		// on initialise le noeud
		pNode->m_pNext = pNextNode;
		pNode->m_pPrev = pRefNode;
	}
	else
	{
		// on accroche le noeud dans la liste
		m_pFNode = m_pLNode = pNode;

		// on initialise le noeud
		pNode->m_pNext = 0;
		pNode->m_pPrev = 0;
	}

	// on renvoie le nouveau noeud
	pNewNode = pNode;

	// on incrémente la taille
	m_Count += 1;

	// on quitte
	return (true);
}

//////////////////////////////////////
// la fonction pour supprimer un noeud

template <class JItem> JVoid JList<JItem>::OnRemNode(JNode* pNode)
{
	// on récupère les noeuds précédents et suivants
	JNode* pPrevNode = pNode->m_pPrev;
	JNode* pNextNode = pNode->m_pNext;

	// on décroche le noeud de la liste
	if (pPrevNode != 0) pPrevNode->m_pNext = pNextNode; else m_pFNode = pNextNode;
	if (pNextNode != 0) pNextNode->m_pPrev = pPrevNode; else m_pLNode = pPrevNode;

	// on vérifie la cohérence du curseur
	if (m_pCursor == pNode) m_pCursor = 0;

	// on décrémente la taille
	m_Count -= 1;

	// on libère le noeud
	delete pNode;
}

//////////////////////////////////////
// la fonction pour libérer les noeuds

template <class JItem> JVoid JList<JItem>::OnRemNodes()
{
	// on mémorise le premier élément
	JNode* pNode = m_pFNode;

	// on réinitialise la taille et les noeuds
	m_Count = 0; m_pFNode = m_pLNode = m_pCursor = 0;

	// on boucle sur tous les noeuds
	while (pNode != 0)
	{
		// on mémorise le noeud suivant
		JNode* pNext = pNode->m_pNext;

		// on supprime le noeud et on passe au suivant
		delete pNode; pNode = pNext;
	}
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

template <class JItem> JInt32 JList<JItem>::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_Count);
}

//////////////////////////////////////////
// les fonctions pour ajouter des éléments

template <class JItem> JItem & JList<JItem>::AddHead(JBool Move)
{
	// on initialise le noeud
	JNode* pNode = 0;

	// on ajoute le noeud en début de liste
	if (!this->OnAddPrevNode(m_pFNode, pNode)) throw JMemoryFull::GetInstance();

	// on met à jour le curseur
	if (Move) m_pCursor = pNode;

	// on renvoie l'élément du noeud
	return (pNode->m_Item);
}

template <class JItem> JItem & JList<JItem>::AddAfter(JBool Move)
{
	// on initialise le noeud
	JNode* pNode = 0;

	// on vérifie la validité du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on ajoute le noeud après le curseur
	if (!this->OnAddNextNode(m_pCursor, pNode)) throw JMemoryFull::GetInstance();

	// on met à jour le curseur
	if (Move) m_pCursor = pNode;

	// on renvoie l'élément du noeud
	return (pNode->m_Item);
}

template <class JItem> JItem & JList<JItem>::AddBefore(JBool Move)
{
	// on initialise le noeud
	JNode* pNode = 0;

	// on vérifie la validité du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on ajoute le noeud après le curseur
	if (!this->OnAddPrevNode(m_pCursor, pNode)) throw JMemoryFull::GetInstance();

	// on met à jour le curseur
	if (Move) m_pCursor = pNode;

	// on renvoie l'élément du noeud
	return (pNode->m_Item);
}

template <class JItem> JItem & JList<JItem>::AddTail(JBool Move)
{
	// on initialise le noeud
	JNode* pNode = 0;

	// on ajoute le noeud en fin de liste
	if (!this->OnAddNextNode(m_pLNode, pNode)) throw JMemoryFull::GetInstance();

	// on met à jour le curseur
	if (Move) m_pCursor = pNode;

	// on renvoie l'élément du noeud
	return (pNode->m_Item);
}

/////////////////////////////////////
// la fonction pour tester le curseur

template <class JItem> JBool JList<JItem>::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_pCursor != 0);
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

template <class JItem> JItem & JList<JItem>::GetItem()
{
	// on vérifie la validité du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on renvoie l'élément du noeud
	return (m_pCursor->m_Item);
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

template <class JItem> JInt32 JList<JItem>::GetIndex() const
{
	// on se place sur le curseur
	JNode* pNode = m_pCursor; if (pNode == 0) throw JInvalidCall::GetInstance();

	// on calcule l'indice du curseur
	JInt32 Idx = 0; while (pNode->m_pPrev != 0) { pNode = pNode->m_pPrev; Idx += 1; }

	// on renvoie l'indice du curseur
	return (Idx);
}

template <class JItem> const JItem & JList<JItem>::GetItem() const
{
	// on vérifie la validité du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on renvoie l'élément du noeud
	return (m_pCursor->m_Item);
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

template <class JItem> JVoid JList<JItem>::MoveTo(JInt32 Index) const
{
	// on vérifie la validité de l'indice
	if (Index >= 0)
	{
		// on déplace le curseur sur le premier noeud
		m_pCursor = m_pFNode;

		// on avance le curseur jusqu'au noeud désiré
		JInt32 Idx = 0; while (m_pCursor != 0 && Idx < Index) { m_pCursor = m_pCursor->m_pNext; Idx += 1; }
	}
	else
	{
		// on réinitialise le curseur
		m_pCursor = 0;
	}
}

template <class JItem> JVoid JList<JItem>::MoveFirst() const
{
	// on déplace le curseur sur le premier noeud
	m_pCursor = m_pFNode;
}

template <class JItem> JVoid JList<JItem>::MoveNext() const
{
	// on déplace le curseur sur le noeud suivant
	if (m_pCursor != 0) m_pCursor = m_pCursor->m_pNext;
}

template <class JItem> JVoid JList<JItem>::MovePrev() const
{
	// on déplace le curseur sur le noeud précédent
	if (m_pCursor != 0) m_pCursor = m_pCursor->m_pPrev;
}

template <class JItem> JVoid JList<JItem>::MoveLast() const
{
	// on déplace le curseur sur le dernier noeud
	m_pCursor = m_pLNode;
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

template <class JItem> JVoid JList<JItem>::Remove(JInt32 Move)
{
	// on mémorise le curseur
	JNode* pNode = m_pCursor; if (pNode == 0) throw JInvalidCall::GetInstance();

	// on met à jour le curseur
	if      (Move > 0) m_pCursor = m_pCursor->m_pNext;
	else if (Move < 0) m_pCursor = m_pCursor->m_pPrev;

	// on supprime le noeud
	this->OnRemNode(pNode);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

template <class JItem> JVoid JList<JItem>::Swap(JList<JItem> & Source)
{
	// on permute le curseur
	JNode* pCursor = m_pCursor; m_pCursor = Source.m_pCursor; Source.m_pCursor = pCursor;
	
	// on permute les noeuds
	JNode* pFNode = m_pFNode; m_pFNode = Source.m_pFNode; Source.m_pFNode = pFNode;
	JNode* pLNode = m_pLNode; m_pLNode = Source.m_pLNode; Source.m_pLNode = pLNode;

	// on permute la taille
	JInt32 Count = m_Count; m_Count = Source.m_Count; Source.m_Count = Count;
}

////////////////////////////////////////
// la fonction pour libérer les éléments

template <class JItem> JVoid JList<JItem>::Reset()
{
	// on libère les noeuds
	this->OnRemNodes();
}

/////////////////
// le destructeur

template <class JItem> JList<JItem>::~JList()
{
	// on libère les noeuds
	this->OnRemNodes();
}

// on finit l'inclusion conditionnelle
#endif
