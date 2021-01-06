//
// Fichier: JMap.h
// Auteur:  Sylvain SAMMURI
// Date:    17/07/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_MAP_H
#define JLIB_MAP_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JInternalError.h"
#include "..\Include\JInvalidCall.h"
#include "..\Include\JMemoryFull.h"

template <class JKey, class JItem> class JMap
{
public:
	// le constructeur
	JMap();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter des éléments
	JItem & Add(const JKey & Key, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// la fonction pour modifier l'élément du curseur
	JItem & GetItem();

	// les fonctions pour récupérer l'élément du curseur
	const JKey  & GetKey() const;
	const JItem & GetItem() const;

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JKey & Key, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMap<JKey, JItem> & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMap();

private:
	// on définit les noeuds de l'arbre
	class JNode
	{
	public:
		// le constructeur
		JNode(const JKey & Key) : m_Key(Key) { }

	public:
		JNode* m_pPNode;    // le noeud parent
		JNode* m_pLNode;    // le noeud gauche
		JNode* m_pRNode;    // le noeud droite
		JInt08 m_Direction; // la direction du noeud
		JInt08 m_Balance;   // l'équilibre du noeud
		JItem  m_Item;      // l'élément du noeud
		JKey   m_Key;       // la clé du noeud
	};

private:
	// la fonction pour comparer les clés
	JInt32 OnCompareKey(const JKey & LKey, const JKey & RKey) const;

private:
	// les fonctions pour effectuer les rotations
	JVoid OnRotateNodeL(JNode* & pNode);
	JVoid OnRotateNodeR(JNode* & pNode);

private:
	// les fonctions pour réequilibrer les noeuds
	JVoid OnBalanceNodeP2(JNode* & pNode);
	JVoid OnBalanceNodeM2(JNode* & pNode);

private:
	// les fonctions pour réequilibrer les arbres
	JVoid OnBalanceTreeAdd(JNode* pNode, JInt08 Direction);
	JVoid OnBalanceTreeRem(JNode* pNode, JInt08 Direction);

private:
	// les fonctions pour manipuler les noeuds
	JVoid OnEvictNode(JNode* pRefNode, JNode* pNode);
	JVoid OnTakeDownNode(JNode* pNode, JNode* & pPNode, JInt08 & Direction);
	JVoid OnTakeUpNode(JNode* pPNode, JInt08 Direction, JNode* pNode);

private:
	// la fonction pour rechercher les noeuds
	JBool OnSearchNode(const JKey & Key, JNode* & pPNode, JInt08 & Direction, JNode* & pNode) const;

	// les fonctions pour parcourir les noeuds
	JVoid OnSearchFrstNode(JNode* & pNode) const;
	JVoid OnSearchNextNode(JNode* pRefNode, JNode* & pNode) const;
	JVoid OnSearchPrevNode(JNode* pRefNode, JNode* & pNode) const;
	JVoid OnSearchLastNode(JNode* & pNode) const;

private:
	// la fonction pour ajouter un noeud
	JBool OnAddNode(const JKey & Key, JNode* pPNode, JInt08 Direction, JNode* & pNewNode);

	// la fonction pour supprimer un noeud
	JVoid OnRemNode(JNode* pNode);

	// les fonctions pour libérer les noeuds
	JVoid OnRemNodes(JNode* pNode);
	JVoid OnRemNodes();

private:
	// l'opérateur d'affectation
	JMap<JKey, JItem> & operator =(const JMap<JKey, JItem> & Source);

	// le constructeur copie
	JMap(const JMap<JKey, JItem> & Source);

private:
	mutable JNode* m_pCursor; // le curseur

private:
	JNode* m_pFNode; // la racine
	JInt32 m_Count;  // la taille
};

////////////////////
// les constructeurs

template <class JKey, class JItem> JMap<JKey, JItem>::JMap()
{
	// on initialise la taille et les noeuds
	m_Count = 0; m_pFNode = m_pCursor = 0;
}

template <class JKey, class JItem> JMap<JKey, JItem>::JMap(const JMap<JKey, JItem> & Source)
{
	// on génère une exception
	throw JInternalError::GetInstance();
}

////////////////////////////
// l'opérateur d'affectation

template <class JKey, class JItem> JMap<JKey, JItem> & JMap<JKey, JItem>::operator =(const JMap<JKey, JItem> & Source)
{
	// on génère une exception
	throw JInternalError::GetInstance();
}

/////////////////////////////////////
// la fonction pour comparer les clés

template <class JKey, class JItem> JInt32 JMap<JKey, JItem>::OnCompareKey(const JKey & LKey, const JKey & RKey) const
{
	// on compare les clés
	if      (LKey < RKey) return (-1);
	else if (LKey > RKey) return (+1);

	// les clés sont égales
	return (0);
}

/////////////////////////////////////////////
// les fonctions pour effectuer les rotations

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnRotateNodeL(JNode* & pNode)
{
	// on récupère le père et le fils droite du noeud
	JNode* pPNode = pNode->m_pPNode;
	JNode* pRNode = pNode->m_pRNode;

	// on met à jour les liens entre les noeuds
	pNode->m_pRNode = pRNode->m_pLNode;

	// on vérifie l'existence du fils droite du noeud
	if (pNode->m_pRNode != 0)
	{
		// on met à jour les liens entre les noeuds
		pNode->m_pRNode->m_pPNode = pNode;

		// on met à jour les directions des noeuds
		pNode->m_pRNode->m_Direction = +1;
	}

	// on met à jour les liens entre les noeuds
	pRNode->m_pLNode = pNode; pNode->m_pPNode = pRNode; pRNode->m_pPNode = pPNode;

	// on met à jour les directions des noeuds
	pRNode->m_Direction = pNode->m_Direction;

	// on accroche le noeud droite à son nouveau père
	if      (pRNode->m_Direction == -1) pPNode->m_pLNode = pRNode;
	else if (pRNode->m_Direction == +1) pPNode->m_pRNode = pRNode;
	else                                        m_pFNode = pRNode;

	// on met à jour la direction du noeud
	pNode->m_Direction = -1;

	// om met à jour le noeud
	pNode = pRNode;
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnRotateNodeR(JNode* & pNode)
{
	// on récupère le père et le fils gauche du noeud
	JNode* pPNode = pNode->m_pPNode;
	JNode* pLNode = pNode->m_pLNode;

	// on met à jour les liens entre les noeuds
	pNode->m_pLNode = pLNode->m_pRNode;

	// on vérifie l'existence du fils gauche du noeud
	if (pNode->m_pLNode != 0)
	{
		// on met à jour les liens entre les noeuds
		pNode->m_pLNode->m_pPNode = pNode;

		// on met à jour les directions des noeuds
		pNode->m_pLNode->m_Direction = -1;
	}

	// on met à jour les liens entre les noeuds
	pLNode->m_pRNode = pNode; pNode->m_pPNode = pLNode; pLNode->m_pPNode = pPNode;

	// on met à jour les directions des noeuds
	pLNode->m_Direction = pNode->m_Direction;

	// on accroche le noeud gauche à son nouveau père
	if      (pLNode->m_Direction == -1) pPNode->m_pLNode = pLNode;
	else if (pLNode->m_Direction == +1) pPNode->m_pRNode = pLNode;
	else                                        m_pFNode = pLNode;

	// on met à jour la direction du noeud
	pNode->m_Direction = +1;

	// om met à jour le noeud
	pNode = pLNode;
}

/////////////////////////////////////////////
// les fonctions pour réequilibrer les noeuds

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnBalanceNodeP2(JNode* & pNode)
{
	// on teste la rotation à faire
	if (pNode->m_pRNode->m_Balance == -1)
	{
		// on effectue une rotation droite gauche sur le noeud
		this->OnRotateNodeR(pNode->m_pRNode); this->OnRotateNodeL(pNode);

		// on teste la correction à faire
		if (pNode->m_Balance >= 0)
		{
			// on corrige les déséquilibres des noeuds
			pNode->m_pRNode->m_Balance = +0;
			pNode->m_pLNode->m_Balance = -pNode->m_Balance;
		}
		else
		{
			// on corrige les déséquilibres des noeuds
			pNode->m_pRNode->m_Balance = -pNode->m_Balance;
			pNode->m_pLNode->m_Balance = +0;
		}

		// on corrige le déséquilibre du noeud
		pNode->m_Balance = 0;
	}
	else
	{
		// on effectue une rotation gauche sur le noeud
		this->OnRotateNodeL(pNode);

		// on corrige les déséquilibres des noeuds
		pNode->m_pLNode->m_Balance = 1 - pNode->m_Balance;
		pNode->m_Balance -= 1;
	}
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnBalanceNodeM2(JNode* & pNode)
{
	// on teste la rotation à faire
	if (pNode->m_pLNode->m_Balance == +1)
	{
		// on effectue une rotation gauche droite sur le noeud
		this->OnRotateNodeL(pNode->m_pLNode); this->OnRotateNodeR(pNode);

		// on teste la correction à faire
		if (pNode->m_Balance >= 0)
		{
			// on corrige les déséquilibres des noeuds
			pNode->m_pRNode->m_Balance = +0;
			pNode->m_pLNode->m_Balance = -pNode->m_Balance;
		}
		else
		{
			// on corrige les déséquilibres des noeuds
			pNode->m_pRNode->m_Balance = -pNode->m_Balance;
			pNode->m_pLNode->m_Balance = +0;
		}

		// on corrige le déséquilibre du noeud
		pNode->m_Balance = 0;
	}
	else
	{
		// on effectue une rotation droite sur le noeud
		this->OnRotateNodeR(pNode);

		// on corrige les déséquilibres des noeuds
		pNode->m_pRNode->m_Balance = -1 - pNode->m_Balance;
		pNode->m_Balance += 1;
	}
}

/////////////////////////////////////////////
// les fonctions pour réequilibrer les arbres

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnBalanceTreeAdd(JNode* pNode, JInt08 Direction)
{
	// on initialise l'indicateur
	JBool Terminate = false;

	// on remonte les noeuds de l'arbre
	while (pNode != 0 && Terminate == false)
	{
		// on corrige le déséquilibre du noeud
		pNode->m_Balance += Direction;

		// on teste le nouveau déséquilibre du noeud
		if      (pNode->m_Balance ==  0) { Terminate = true; }
		else if (pNode->m_Balance == -2) { this->OnBalanceNodeM2(pNode); Terminate = true; }
		else if (pNode->m_Balance == +2) { this->OnBalanceNodeP2(pNode); Terminate = true; }

		// on mémorise la direction du noeud
		Direction = pNode->m_Direction;

		// on se place sur le parent
		pNode = pNode->m_pPNode;
	}
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnBalanceTreeRem(JNode* pNode, JInt08 Direction)
{
	// on initialise l'indicateur
	JBool Terminate = false;

	// on remonte les noeuds de l'arbre
	while (pNode != 0 && Terminate == false)
	{
		// on corrige le déséquilibre du noeud
		pNode->m_Balance -= Direction;

		// on teste le nouveau déséquilibre du noeud
		if      (pNode->m_Balance == -1) { Terminate = true; }
		else if (pNode->m_Balance == +1) { Terminate = true; }
		else if (pNode->m_Balance == -2) { this->OnBalanceNodeM2(pNode); if (pNode->m_Balance != 0) Terminate = true; }
		else if (pNode->m_Balance == +2) { this->OnBalanceNodeP2(pNode); if (pNode->m_Balance != 0) Terminate = true; }

		// on mémorise la direction du noeud
		Direction = pNode->m_Direction;

		// on se place sur le parent
		pNode = pNode->m_pPNode;
	}
}

//////////////////////////////////////////
// les fonctions pour manipuler les noeuds

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnEvictNode(JNode* pRefNode, JNode* pNode)
{
	// on récupère les liens du noeud
	JNode* pPNode = pRefNode->m_pPNode;
	JNode* pLNode = pRefNode->m_pLNode;
	JNode* pRNode = pRefNode->m_pRNode;

	// on vérifie l'existence du noeud
	if (pNode == 0)
	{
		// on décroche le noeud de son père
		if      (pRefNode->m_Direction == -1) pPNode->m_pLNode = 0;
		else if (pRefNode->m_Direction == +1) pPNode->m_pRNode = 0;
		else                                          m_pFNode = 0;
	}
	else
	{
		// on met à jour les fils du noeud
		if (pNode != pLNode && pLNode != 0) pLNode->m_pPNode = pNode;
		if (pNode != pRNode && pRNode != 0) pRNode->m_pPNode = pNode;

		// on met à jour le parent du noeud
		if      (pRefNode->m_Direction == -1) pPNode->m_pLNode = pNode;
		else if (pRefNode->m_Direction == +1) pPNode->m_pRNode = pNode;
		else                                          m_pFNode = pNode;

		// on met à jour les liens du noeud
		if (pNode != pLNode) pNode->m_pLNode = pLNode; else pNode->m_pLNode = 0;
		if (pNode != pRNode) pNode->m_pRNode = pRNode; else pNode->m_pRNode = 0;
		
		// on recopie les liens du noeud
		pNode->m_Direction = pRefNode->m_Direction;
		pNode->m_Balance   = pRefNode->m_Balance;
		pNode->m_pPNode    = pPNode;
	}
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnTakeDownNode(JNode* pNode, JNode* & pPNode, JInt08 & Direction)
{
	// on teste si le noeud a des fils
	if (pNode->m_pLNode != 0)
	{
		// on descend à gauche
		JNode* pFNode = pNode->m_pLNode;

		// on descend à droite de l'arbre
		while (pFNode->m_pRNode != 0) pFNode = pFNode->m_pRNode;

		// on décroche le noeud que l'on a trouvé
		this->OnTakeDownNode(pFNode, pPNode, Direction);

		// on corrige le noeud de rééquilibrage
		if (pPNode == pNode) pPNode = pFNode;

		// on évince le noeud
		this->OnEvictNode(pNode, pFNode);
	}
	else if (pNode->m_pRNode != 0)
	{
		// on descend à droite
		JNode* pFNode = pNode->m_pRNode;

		// on descend à gauche de l'arbre
		while (pFNode->m_pLNode != 0) pFNode = pFNode->m_pLNode;

		// on décroche le noeud que l'on a trouvé
		this->OnTakeDownNode(pFNode, pPNode, Direction);

		// on corrige le noeud de rééquilibrage
		if (pPNode == pNode) pPNode = pFNode;

		// on évince le noeud
		this->OnEvictNode(pNode, pFNode);
	}
	else
	{
		// on renvoie le parent du noeud et la direction
		pPNode = pNode->m_pPNode; Direction = pNode->m_Direction;

		// on évince le noeud
		this->OnEvictNode(pNode, 0);
	}
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnTakeUpNode(JNode* pPNode, JInt08 Direction, JNode* pNode)
{
	// on accroche le noeud à son père
	if      (Direction == -1) pPNode->m_pLNode = pNode;
	else if (Direction == +1) pPNode->m_pRNode = pNode;
	else                              m_pFNode = pNode;

	// on initialise le noeud
	pNode->m_pPNode    = pPNode;
	pNode->m_Direction = Direction;
	pNode->m_Balance   = 0;
	pNode->m_pLNode    = 0;
	pNode->m_pRNode    = 0;
}

/////////////////////////////////////////
// la fonction pour rechercher les noeuds

template <class JKey, class JItem> JBool JMap<JKey, JItem>::OnSearchNode(const JKey & Key, JNode* & pPNode, JInt08 & Direction, JNode* & pNode) const
{
	// on initialise le père
	pPNode    = 0;
	Direction = 0;

	// on se place à la racine
	pNode = m_pFNode;

	// on initialise l'indicateur
	JBool Find = false;

	// on descend dans l'arbre
	while (pNode != 0 && Find == false)
	{
		// on compare les clés
		JInt32 Comp = this->OnCompareKey(Key, pNode->m_Key);

		// on passe au noeud suivant
		if      (Comp == -1) { pPNode = pNode; Direction = -1; pNode = pNode->m_pLNode; }
		else if (Comp == +1) { pPNode = pNode; Direction = +1; pNode = pNode->m_pRNode; }
		else                 { Find = true; }
	}

	// on quitte
	return (Find);
}

//////////////////////////////////////////
// les fonctions pour parcourir les noeuds

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnSearchFrstNode(JNode* & pNode) const
{
	// on se place sur la racine
	pNode = m_pFNode;

	// on descend à gauche de l'arbre
	while (pNode->m_pLNode != 0) pNode = pNode->m_pLNode;
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnSearchNextNode(JNode* pRefNode, JNode* & pNode) const
{
	// on se place sur le noeud
	pNode = pRefNode;

	// on teste si on peut descendre dans l'arbre
	if (pNode->m_pRNode != 0)
	{
		// on descend à droite
		pNode = pNode->m_pRNode;

		// on descend à gauche de l'arbre
		while (pNode->m_pLNode != 0) pNode = pNode->m_pLNode;
	}
	else
	{
		// on remonte les noeuds de l'arbre
		while (pNode->m_Direction == +1) pNode = pNode->m_pPNode;

		// on remonte sur le parent
		pNode = pNode->m_pPNode;
	}
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnSearchPrevNode(JNode* pRefNode, JNode* & pNode) const
{
	// on se place sur le noeud
	pNode = pRefNode;

	// on teste si on peut descendre dans l'arbre
	if (pNode->m_pLNode != 0)
	{
		// on descend à gauche
		pNode = pNode->m_pLNode;

		// on descend à droite de l'arbre
		while (pNode->m_pRNode != 0) pNode = pNode->m_pRNode;
	}
	else
	{
		// on remonte les noeuds de l'arbre
		while (pNode->m_Direction == -1) pNode = pNode->m_pPNode;

		// on remonte sur le parent
		pNode = pNode->m_pPNode;
	}
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnSearchLastNode(JNode* & pNode) const
{
	// on se place sur la racine
	pNode = m_pFNode;

	// on descend à gauche de l'arbre
	while (pNode->m_pRNode != 0) pNode = pNode->m_pRNode;
}

////////////////////////////////////
// la fonction pour ajouter un noeud

template <class JKey, class JItem> JBool JMap<JKey, JItem>::OnAddNode(const JKey & Key, JNode* pPNode, JInt08 Direction, JNode* & pNewNode)
{
	// on alloue un nouveau noeud
	JNode* pNode = new JNode(Key); if (pNode == 0) return (false);

	// on accroche le noeud à son père
	this->OnTakeUpNode(pPNode, Direction, pNode);

	// on rééquilibre l'arbre
	this->OnBalanceTreeAdd(pPNode, Direction);

	// on renvoie le nouveau noeud
	pNewNode = pNode;

	// on incrémente la taille
	m_Count += 1;

	// on quitte
	return (true);
}

//////////////////////////////////////
// la fonction pour supprimer un noeud

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnRemNode(JNode* pNode)
{
	// on initialise la recherche
	JNode* pPNode;
	JInt08 Direction;

	// on décroche le noeud de l'arbre
	this->OnTakeDownNode(pNode, pPNode, Direction);

	// on rééquilibre l'arbre
	this->OnBalanceTreeRem(pPNode, Direction);

	// on vérifie la cohérence du curseur
	if (m_pCursor == pNode) m_pCursor = 0;

	// on décrémente la taille
	m_Count -= 1;

	// on libère le noeud
	delete pNode;
}

////////////////////////////////////////
// les fonctions pour libérer les noeuds

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnRemNodes(JNode* pNode)
{
	// on supprime les noeuds gauche et droite
	if (pNode->m_pLNode != 0) this->OnRemNodes(pNode->m_pLNode);
	if (pNode->m_pRNode != 0) this->OnRemNodes(pNode->m_pRNode);

	// on supprime le noeud
	delete pNode;
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::OnRemNodes()
{
	// on mémorise le premier élément
	JNode* pNode = m_pFNode;

	// on réinitialise la taille et les noeuds
	m_Count = 0; m_pFNode = m_pCursor = 0;

	// on supprime tous les noeuds
	if (pNode != 0) this->OnRemNodes(pNode);
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

template <class JKey, class JItem> JInt32 JMap<JKey, JItem>::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_Count);
}

////////////////////////////////////////
// la fonction pour ajouter des éléments

template <class JKey, class JItem> JItem & JMap<JKey, JItem>::Add(const JKey & Key, JBool Move)
{
	// on initialise la recherche
	JNode* pNode;
	JNode* pPNode;
	JInt08 Direction;

	// on cherche un noeud contenant la clé
	if (this->OnSearchNode(Key, pPNode, Direction, pNode)) throw JInvalidCall::GetInstance();

	// on ajoute le noeud dans l'arbre
	if (!this->OnAddNode(Key, pPNode, Direction, pNode)) throw JMemoryFull::GetInstance();
	
	// on met à jour le curseur
	if (Move) m_pCursor = pNode;

	// on renvoie l'élément du noeud
	return (pNode->m_Item);
}

/////////////////////////////////////
// la fonction pour tester le curseur

template <class JKey, class JItem> JBool JMap<JKey, JItem>::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_pCursor != 0);
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

template <class JKey, class JItem> JItem & JMap<JKey, JItem>::GetItem()
{
	// on vérifie la validité du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on renvoie l'élément du noeud
	return (m_pCursor->m_Item);
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

template <class JKey, class JItem> const JKey & JMap<JKey, JItem>::GetKey() const
{
	// on vérifie la validité du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on renvoie la clé du noeud
	return (m_pCursor->m_Key);
}

template <class JKey, class JItem> const JItem & JMap<JKey, JItem>::GetItem() const
{
	// on vérifie la validité du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on renvoie l'élément du noeud
	return (m_pCursor->m_Item);
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::MoveTo(const JKey & Key, JInt32 Move) const
{
	// on initialise la recherche
	JNode* pPNode;
	JInt08 Direction;

	// on cherche le noeud contenant la clé
	if (!this->OnSearchNode(Key, pPNode, Direction, m_pCursor))
	{
		// on teste le noeud de remplacement
		if (Move < 0)
		{
			// on récupère le noeud précédent
			if      (Direction == +1) m_pCursor = pPNode;
			else if (Direction == -1) this->OnSearchPrevNode(pPNode, m_pCursor);
		}
		else if (Move > 0)
		{
			// on récupère le noeud suivant
			if      (Direction == +1) this->OnSearchNextNode(pPNode, m_pCursor);
			else if (Direction == -1) m_pCursor = pPNode;
		}
	}
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::MoveFirst() const
{
	// on cherche le premier noeud
	if (m_pFNode != 0) this->OnSearchFrstNode(m_pCursor);
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::MoveNext() const
{
	// on cherche le noeud suivant
	if (m_pCursor != 0) this->OnSearchNextNode(m_pCursor, m_pCursor);
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::MovePrev() const
{
	// on cherche le noeud précédent
	if (m_pCursor != 0) this->OnSearchPrevNode(m_pCursor, m_pCursor);
}

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::MoveLast() const
{
	// on cherche le dernier noeud
	if (m_pFNode != 0) this->OnSearchLastNode(m_pCursor);
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'élément du curseur

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::Remove(JInt32 Move)
{
	// on mémorise le curseur
	JNode* pNode = m_pCursor; if (pNode == 0) throw JInvalidCall::GetInstance();

	// on met à jour le curseur
	if      (Move > 0) this->OnSearchNextNode(pNode, m_pCursor);
	else if (Move < 0) this->OnSearchPrevNode(pNode, m_pCursor);

	// on supprime le noeud
	this->OnRemNode(pNode);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::Swap(JMap<JKey, JItem> & Source)
{
	// on permute le curseur
	JNode* pCursor = m_pCursor; m_pCursor = Source.m_pCursor; Source.m_pCursor = pCursor;
	
	// on permute les noeuds
	JNode* pFNode = m_pFNode; m_pFNode = Source.m_pFNode; Source.m_pFNode = pFNode;

	// on permute la taille
	JInt32 Count = m_Count; m_Count = Source.m_Count; Source.m_Count = Count;
}

////////////////////////////////////////
// la fonction pour libérer les éléments

template <class JKey, class JItem> JVoid JMap<JKey, JItem>::Reset()
{
	// on libère les noeuds
	this->OnRemNodes();
}

/////////////////
// le destructeur

template <class JKey, class JItem> JMap<JKey, JItem>::~JMap()
{
	// on libère les noeuds
	this->OnRemNodes();
}

// on finit l'inclusion conditionnelle
#endif
