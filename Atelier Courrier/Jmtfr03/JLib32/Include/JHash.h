//
// Fichier: JHash.h
// Auteur:  Sylvain SAMMURI
// Date:    17/07/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_HASH_H
#define JLIB_HASH_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JInternalError.h"
#include "..\Include\JInvalidCall.h"
#include "..\Include\JMemoryFull.h"

template <class JKey, class JItem> class JHash
{
public:
	// le constructeur
	JHash();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour initialiser la table de hashage
	JVoid SetHashSize(JInt32 Size);

	// la fonction pour ajouter des �l�ments
	JItem & Add(const JKey & Key, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// la fonction pour modifier l'�l�ment du curseur
	JItem & GetItem();

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JKey  & GetKey() const;
	const JItem & GetItem() const;

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JKey & Key) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JHash<JKey, JItem> & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JHash();

private:
	// on d�finit les noeuds de la liste
	class JNode
	{
	public:
		// le constructeur
		JNode(const JKey & Key) : m_Key(Key) { }

	public:
		JNode* m_pNext; // l'�l�ment suivant
		JNode* m_pPrev; // l'�l�ment pr�c�dent
		JInt32 m_Index; // l'indice de hachage
		JItem  m_Item;  // l'�l�ment du noeud
		JKey   m_Key;   // la cl� du noeud
	};

private:
	// on d�finit les entr�es de la table
	class JTable
	{
	public:
		JNode* m_pFNode; // le premier noeud
		JNode* m_pLNode; // le dernier noeud
	};

private:
	// la fonction pour construire la table
	JBool OnMakeTable(JInt32 Size);

private:
	// la fonction pour r�cup�rer l'indice
	JInt32 OnGetHashKey(const JKey & Key) const;

	// la fonction pour comparer les cl�s
	JInt32 OnCompareKey(const JKey & LKey, const JKey & RKey) const;

private:
	// les fonctions pour manipuler les noeuds
	JVoid OnTakeUpNode(JTable* pTable, JNode* pPNode, JNode* pNode);
	JVoid OnTakeDownNode(JNode* pNode);

private:
	// les fonctions pour rechercher les noeuds
	JBool OnSearchNode(const JKey & Key, JNode* & pNode) const;
	JBool OnSearchNode(const JKey & Key, JTable* & pTable, JNode* & pPNode, JNode* & pNode) const;

	// les fonctions pour parcourir les noeuds
	JVoid OnSearchFrstNode(JNode* & pNode) const;
	JVoid OnSearchNextNode(JNode* pRefNode, JNode* & pNode) const;
	JVoid OnSearchPrevNode(JNode* pRefNode, JNode* & pNode) const;
	JVoid OnSearchLastNode(JNode* & pNode) const;

private:
	// la fonction pour ajouter un noeud
	JBool OnAddNode(const JKey & Key, JTable* pTable, JNode* pPNode, JNode* & pNewNode);

	// la fonction pour supprimer un noeud
	JVoid OnRemNode(JNode* pNode);

	// la fonction pour lib�rer les noeuds
	JVoid OnRemNodes();

private:
	// l'op�rateur d'affectation
	JHash<JKey, JItem> & operator =(const JHash<JKey, JItem> & Source);

	// le constructeur copie
	JHash(const JHash<JKey, JItem> & Source);

private:
	mutable JNode* m_pCursor; // le curseur

private:
	JTable* m_pFTable; // la premi�re entr�e
	JTable* m_pLTable; // la derni�re entr�e
	JInt32  m_Count;   // la taille
};

////////////////////
// les constructeurs

template <class JKey, class JItem> JHash<JKey, JItem>::JHash()
{
	// on initialise la taille et les noeuds
	m_Count = 0; m_pFTable = m_pLTable = 0; m_pCursor = 0;
}

template <class JKey, class JItem> JHash<JKey, JItem>::JHash(const JHash<JKey, JItem> & Source)
{
	// on g�n�re une exception
	throw JInternalError::GetInstance();
}

////////////////////////////
// l'op�rateur d'affectation

template <class JKey, class JItem> JHash<JKey, JItem> & JHash<JKey, JItem>::operator =(const JHash<JKey, JItem> & Source)
{
	// on g�n�re une exception
	throw JInternalError::GetInstance();
}

///////////////////////////////////////
// la fonction pour construire la table

template <class JKey, class JItem> JBool JHash<JKey, JItem>::OnMakeTable(JInt32 Size)
{
	// on alloue la table
	JTable* pTable = new JTable[Size]; if (pTable == 0) return (false);

	// on initialise la table
	for (JInt32 Idx = 0; Idx < Size; Idx +=1) (pTable + Idx)->m_pFNode = (pTable + Idx)->m_pLNode = 0;

	// on m�morise la table
	m_pLTable = pTable + (Size - 1);
	m_pFTable = pTable;

	// on quitte
	return (true);
}

//////////////////////////////////////
// la fonction pour r�cup�rer l'indice

template <class JKey, class JItem> JInt32 JHash<JKey, JItem>::OnGetHashKey(const JKey & Key) const
{
	// on calcule la taille de la table
	JInt32 Size = (m_pLTable - m_pFTable) + 1;

	// on r�cup�re l'indice de la cl�
	JInt32 Index = Key.GetHashKey(Size); if (Index < 0) Index = -(Index + 1);

	// on renvoie l'indice
	return (Index % Size);
}

/////////////////////////////////////
// la fonction pour comparer les cl�s

template <class JKey, class JItem> JInt32 JHash<JKey, JItem>::OnCompareKey(const JKey & LKey, const JKey & RKey) const
{
	// on compare les cl�s
	if      (LKey < RKey) return (-1);
	else if (LKey > RKey) return (+1);

	// les cl�s sont �gales
	return (0);
}

//////////////////////////////////////////
// les fonctions pour manipuler les noeuds

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::OnTakeUpNode(JTable* pTable, JNode* pPNode, JNode* pNode)
{
	// on teste si on a un noeud pr�c�dent
	if (pPNode == 0)
	{
		// on initialise les liens du noeud
		pNode->m_pPrev = 0;
		pNode->m_pNext = pTable->m_pFNode;

		// on accorche le noeud au noeud suivant
		if (pNode->m_pNext != 0) pNode->m_pNext->m_pPrev = pNode; else pTable->m_pLNode = pNode;

		// on accroche le noeud au noeud pr�c�dent
		pTable->m_pFNode = pNode;
	}
	else
	{
		// on initialise les liens du noeud
		pNode->m_pPrev = pPNode;
		pNode->m_pNext = pPNode->m_pNext;

		// on accorche le noeud au noeud suivant
		if (pNode->m_pNext != 0) pNode->m_pNext->m_pPrev = pNode; else pTable->m_pLNode = pNode;

		// on accroche le noeud au noeud pr�c�dent
		pPNode->m_pNext = pNode;
	}

	// on initialise l'indice du noeud
	pNode->m_Index = pTable - m_pFTable;
}

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::OnTakeDownNode(JNode* pNode)
{
	// on r�cup�re les liens du noeud
	JNode* pPNode = pNode->m_pPrev;
	JNode* pNNode = pNode->m_pNext;

	// on r�cup�re l'entr�e de la table
	JTable* pTable = m_pFTable + pNode->m_Index;

	// on met � jour les liens du noeud
	if (pPNode != 0) pPNode->m_pNext = pNNode; else pTable->m_pFNode = pNNode;
	if (pNNode != 0) pNNode->m_pPrev = pPNode; else pTable->m_pLNode = pPNode;
}

///////////////////////////////////////////
// les fonctions pour rechercher les noeuds

template <class JKey, class JItem> JBool JHash<JKey, JItem>::OnSearchNode(const JKey & Key, JNode* & pNode) const
{
	// on initialise l'indicateur
	JBool Find = false;

	// on calcule l'indice de la cl�
	JInt32 Index = this->OnGetHashKey(Key);

	// on se place sur l'entr�e de la table
	JTable* pTable = m_pFTable + Index;

	// on se place sur le premier noeud
	pNode  = pTable->m_pFNode;

	// on initialise le r�sultat de la comparaison
	JInt32 Res = (Index % 2 != 0) ? +1 : -1;

	// on parcourt tous les �l�ments
	while (pNode != 0 && Find == false)
	{
		// on compare les cl�s
		JInt32 Comp = this->OnCompareKey(Key, pNode->m_Key);

		// on passe au noeud suivant
		if      (Comp == +Res) { pNode = 0; }
		else if (Comp == -Res) { pNode = pNode->m_pNext; }
		else                   { Find = true; }
	}

	// on quitte
	return (Find);
}

template <class JKey, class JItem> JBool JHash<JKey, JItem>::OnSearchNode(const JKey & Key, JTable* & pTable, JNode* & pPNode, JNode* & pNode) const
{
	// on initialise l'indicateur
	JBool Find = false;

	// on calcule l'indice de la cl�
	JInt32 Index = this->OnGetHashKey(Key);

	// on se place sur l'entr�e de la table
	pPNode = 0;
	pTable = m_pFTable + Index;
	pNode  = pTable->m_pFNode;

	// on initialise le r�sultat de la comparaison
	JInt32 Res = (Index % 2 != 0) ? +1 : -1;

	// on parcourt tous les �l�ments
	while (pNode != 0 && Find == false)
	{
		// on compare les cl�s
		JInt32 Comp = this->OnCompareKey(Key, pNode->m_Key);

		// on passe au noeud suivant
		if      (Comp == +Res) { pNode = 0; }
		else if (Comp == -Res) { pPNode = pNode; pNode = pNode->m_pNext; }
		else                   { Find = true; }
	}

	// on quitte
	return (Find);
}

//////////////////////////////////////////
// les fonctions pour parcourir les noeuds

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::OnSearchFrstNode(JNode* & pNode) const
{
	// on initialise le noeud
	pNode = 0;

	// on se place sur la premi�re entr�e
	JTable* pTable = m_pFTable;

	// on parcourt tous les �l�ments
	while (pTable != 0 && pNode == 0)
	{
		// on teste si l'entr�e contient un noeud
		if (pTable->m_pFNode == 0)
		{
			// on passe � l'entr�e suivante
			if (pTable != m_pLTable) pTable += 1; else pTable = 0;
		}
		else
		{
			// on renvoie le noeud
			pNode = pTable->m_pFNode;
		}
	}
}

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::OnSearchNextNode(JNode* pRefNode, JNode* & pNode) const
{
	// on se place sur le noeud suivant
	pNode = pRefNode->m_pNext;

	// on v�rifie la validit� du noeud
	if (pNode == 0)
	{
		// on r�cup�re l'entr�e de la table
		JTable* pTable = m_pFTable + pRefNode->m_Index;

		// on passe � l'entr�e suivante
		if (pTable != m_pLTable) pTable += 1; else pTable = 0;

		// on parcourt tous les �l�ments
		while (pTable != 0 && pNode == 0)
		{
			// on teste si l'entr�e contient un noeud
			if (pTable->m_pFNode == 0)
			{
				// on passe � l'entr�e suivante
				if (pTable != m_pLTable) pTable += 1; else pTable = 0;
			}
			else
			{
				// on renvoie le noeud
				pNode = pTable->m_pFNode;
			}
		}
	}
}

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::OnSearchPrevNode(JNode* pRefNode, JNode* & pNode) const
{
	// on se place sur le noeud pr�c�dent
	pNode = pRefNode->m_pPrev;

	// on v�rifie la validit� du noeud
	if (pNode == 0)
	{
		// on se place sur l'entr�e de la table
		JTable* pTable = m_pFTable + pRefNode->m_Index;

		// on passe � l'entr�e pr�c�dente
		if (pTable != m_pFTable) pTable -= 1; else pTable = 0;

		// on parcourt tous les �l�ments
		while (pTable != 0 && pNode == 0)
		{
			// on teste si l'entr�e contient un noeud
			if (pTable->m_pLNode == 0)
			{
				// on passe � l'entr�e pr�c�dente
				if (pTable != m_pFTable) pTable -= 1; else pTable = 0;
			}
			else
			{
				// on renvoie le noeud
				pNode = pTable->m_pLNode;
			}
		}
	}
}

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::OnSearchLastNode(JNode* & pNode) const
{
	// on initialise le noeud
	pNode = 0;

	// on se place sur la derni�re entr�e
	JTable* pTable = m_pLTable;

	// on parcourt tous les �l�ments
	while (pTable != 0 && pNode == 0)
	{
		// on teste si l'entr�e contient un noeud
		if (pTable->m_pLNode == 0)
		{
			// on passe � l'entr�e suivante
			if (pTable != m_pFTable) pTable -= 1; else pTable = 0;
		}
		else
		{
			// on renvoie le noeud
			pNode = pTable->m_pLNode;
		}
	}
}

////////////////////////////////////
// la fonction pour ajouter un noeud

template <class JKey, class JItem> JBool JHash<JKey, JItem>::OnAddNode(const JKey & Key, JTable* pTable, JNode* pPNode, JNode* & pNewNode)
{
	// on alloue un nouveau noeud
	JNode* pNode = new JNode(Key); if (pNode == 0) return (false);

	// on accroche le noeud � son pr�c�dent
	this->OnTakeUpNode(pTable, pPNode, pNode);

	// on renvoie le nouveau noeud
	pNewNode = pNode;

	// on incr�mente la taille
	m_Count += 1;

	// on quitte
	return (true);
}

//////////////////////////////////////
// la fonction pour supprimer un noeud

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::OnRemNode(JNode* pNode)
{
	// on d�croche le noeud de la table
	this->OnTakeDownNode(pNode);
	
	// on v�rifie la coh�rence du curseur
	if (m_pCursor == pNode) m_pCursor = 0;

	// on d�cr�mente la taille
	m_Count -= 1;

	// on lib�re le noeud
	delete pNode;
}

//////////////////////////////////////
// la fonction pour lib�rer les noeuds

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::OnRemNodes()
{
	// on r�cup�re la table
	JTable* pFTable = m_pFTable;
	JTable* pLTable = m_pLTable;

	// on r�initialise la taille et les noeuds
	m_Count = 0; m_pFTable = m_pLTable = 0; m_pCursor = 0;

	// on se place sur la premi�re entr�e
	JTable* pTable = pFTable;

	// on boucle sur toutes les tables
	while (pTable != 0)
	{
		// on se place sur le premier noeud
		JNode *pNode = pTable->m_pFNode;

		// on boucle sur tous les noeuds
		while (pNode != 0)
		{
			// on m�morise le noeud suivant
			JNode* pNext = pNode->m_pNext;

			// on supprime le noeud et on passe au suivant
			delete pNode; pNode = pNext;
		}

		// on passe � l'entr�e suivante
		if (pTable != pLTable) pTable += 1; else pTable = 0;
	}

	// on supprime la table
	delete [] pFTable;
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

template <class JKey, class JItem> JInt32 JHash<JKey, JItem>::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_Count);
}

///////////////////////////////////////////////////
// la fonction pour initialiser la table de hashage

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::SetHashSize(JInt32 Size)
{
	// on v�rifie la taille
	if (Size < 1) throw JInvalidCall::GetInstance();

	// on v�rifie l'existence de la table
	if (m_pFTable != 0) throw JInvalidCall::GetInstance();

	// on cr�e la table
	if (!this->OnMakeTable(Size)) throw JMemoryFull::GetInstance();
}

////////////////////////////////////////
// la fonction pour ajouter des �l�ments

template <class JKey, class JItem> JItem & JHash<JKey, JItem>::Add(const JKey & Key, JBool Move)
{
	// on initialise la recherche
	JNode*  pNode;
	JNode*  pPNode;
	JTable* pTable;

	// on v�rifie l'existence de la table
	if (m_pFTable == 0) throw JInvalidCall::GetInstance();

	// on cherche un noeud contenant la cl�
	if (this->OnSearchNode(Key, pTable, pPNode, pNode)) throw JInvalidCall::GetInstance();

	// on ajoute le noeud dans la table
	if (!this->OnAddNode(Key, pTable, pPNode, pNode)) throw JMemoryFull::GetInstance();

	// on met � jour le curseur
	if (Move) m_pCursor = pNode;

	// on renvoie l'�l�ment du noeud
	return (pNode->m_Item);
}

/////////////////////////////////////
// la fonction pour tester le curseur

template <class JKey, class JItem> JBool JHash<JKey, JItem>::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_pCursor != 0);
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

template <class JKey, class JItem> JItem & JHash<JKey, JItem>::GetItem()
{
	// on v�rifie la validit� du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on renvoie l'�l�ment du noeud
	return (m_pCursor->m_Item);
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

template <class JKey, class JItem> const JKey & JHash<JKey, JItem>::GetKey() const
{
	// on v�rifie la validit� du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on renvoie la cl� du noeud
	return (m_pCursor->m_Key);
}

template <class JKey, class JItem> const JItem & JHash<JKey, JItem>::GetItem() const
{
	// on v�rifie la validit� du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on renvoie l'�l�ment du noeud
	return (m_pCursor->m_Item);
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::MoveTo(const JKey & Key) const
{
	// on cherche le noeud contenant la cl�
	if (m_pFTable != 0) this->OnSearchNode(Key, m_pCursor);
}

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::MoveFirst() const
{
	// on cherche le premier noeud
	if (m_pFTable != 0) this->OnSearchFrstNode(m_pCursor);
}

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::MoveNext() const
{
	// on cherche le noeud suivant
	if (m_pCursor != 0) this->OnSearchNextNode(m_pCursor, m_pCursor);
}

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::MovePrev() const
{
	// on cherche le noeud pr�c�dent
	if (m_pCursor != 0) this->OnSearchPrevNode(m_pCursor, m_pCursor);
}

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::MoveLast() const
{
	// on cherche le dernier noeud
	if (m_pLTable != 0) this->OnSearchLastNode(m_pCursor);
}

//////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment du curseur

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::Remove(JInt32 Move)
{
	// on m�morise le curseur
	JNode* pNode = m_pCursor; if (pNode == 0) throw JInvalidCall::GetInstance();

	// on met � jour le curseur
	if      (Move > 0) this->OnSearchNextNode(pNode, m_pCursor);
	else if (Move < 0) this->OnSearchPrevNode(pNode, m_pCursor);

	// on supprime le noeud
	this->OnRemNode(pNode);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::Swap(JHash<JKey, JItem> & Source)
{
	// on permute le curseur
	JNode* pCursor = m_pCursor; m_pCursor = Source.m_pCursor; Source.m_pCursor = pCursor;
	
	// on permute la table
	JTable* pFTable = m_pFTable; m_pFTable = Source.m_pFTable; Source.m_pFTable = pFTable;
	JTable* pLTable = m_pLTable; m_pLTable = Source.m_pLTable; Source.m_pLTable = pLTable;

	// on permute la taille
	JInt32 Count = m_Count; m_Count = Source.m_Count; Source.m_Count = Count;
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

template <class JKey, class JItem> JVoid JHash<JKey, JItem>::Reset()
{
	// on lib�re les noeuds
	this->OnRemNodes();
}

/////////////////
// le destructeur

template <class JKey, class JItem> JHash<JKey, JItem>::~JHash()
{
	// on lib�re les noeuds
	this->OnRemNodes();
}

// on finit l'inclusion conditionnelle
#endif
