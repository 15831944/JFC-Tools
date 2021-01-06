//
// Fichier: JVector.h
// Auteur:  Sylvain SAMMURI
// Date:    17/07/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_VECTOR_H
#define JLIB_VECTOR_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JInternalError.h"
#include "..\Include\JInvalidCall.h"
#include "..\Include\JMemoryFull.h"

template <class JItem> class JVector
{
public:
	// le constructeur
	JVector();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour allouer les �l�ments
	JVoid SetCount(JInt32 Count);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// la fonction pour modifier l'�l�ment du curseur
	JItem & GetItem();

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	JInt32 GetIndex() const;
	const JItem & GetItem() const;

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(JInt32 Index) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JVector<JItem> & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JVector();

private:
	// on d�finit les noeuds du vecteur
	class JNode
	{
	public:
		JItem m_Item; // l'�l�ment du noeud
	};

private:
	// la fonction pour allouer les noeuds
	JBool OnAddNodes(JInt32 Count);

	// la fonction pour lib�rer les noeuds
	JVoid OnRemNodes();

private:
	// l'op�rateur d'affectation
	JVector<JItem> & operator =(const JVector<JItem> & Source);

	// le constructeur copie
	JVector(const JVector<JItem> & Source);

private:
	mutable JNode* m_pCursor; // le curseur

private:
	JNode* m_pFNode; // le premier noeud
	JNode* m_pLNode; // le dernier noeud
	JInt32 m_Count;  // la taille
};

////////////////////
// les constructeurs

template <class JItem> JVector<JItem>::JVector()
{
	// on initialise la taille et les noeuds
	m_Count = 0; m_pFNode = m_pLNode = m_pCursor = 0;
}

template <class JItem> JVector<JItem>::JVector(const JVector<JItem> & Source)
{
	// on g�n�re une exception
	throw JInternalError::GetInstance();
}

////////////////////////////
// l'op�rateur d'affectation

template <class JItem> JVector<JItem> & JVector<JItem>::operator =(const JVector<JItem> & Source)
{
	// on g�n�re une exception
	throw JInternalError::GetInstance();
}

//////////////////////////////////////
// la fonction pour allouer les noeuds

template <class JItem> JBool JVector<JItem>::OnAddNodes(JInt32 Count)
{
	// on teste si on a des noeuds � allouer
	if (Count > 0)
	{
		// on alloue les noeuds
		JNode* pNode = new JNode[Count]; if (pNode == 0) return (false);

		// on met � jour les noeuds
		m_pLNode = pNode + (Count - 1);
		m_pFNode = pNode;
	}

	// on met � jour la taille
	m_Count = Count;

	// on quitte
	return (true);
}

//////////////////////////////////////
// la fonction pour lib�rer les noeuds

template <class JItem> JVoid JVector<JItem>::OnRemNodes()
{
	// on m�morise le premier �l�ment
	JNode* pNode = m_pFNode;

	// on r�initialise la taille et les noeuds
	m_Count = 0; m_pFNode = m_pLNode = m_pCursor = 0;

	// on supprime les �l�ments
	delete [] pNode;
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

template <class JItem> JInt32 JVector<JItem>::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_Count);
}

////////////////////////////////////////
// la fonction pour allouer les �l�ments

template <class JItem> JVoid JVector<JItem>::SetCount(JInt32 Count)
{
	// on teste le nombre d'�l�ments
	if (Count < 0) throw JInvalidCall::GetInstance();

	// on teste si on a d�j� des �l�ments
	if (m_Count > 0) throw JInvalidCall::GetInstance();

	// on alloue les �l�ments
	if (!this->OnAddNodes(Count)) throw JMemoryFull::GetInstance();
}

/////////////////////////////////////
// la fonction pour tester le curseur

template <class JItem> JBool JVector<JItem>::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_pCursor != 0);
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

template <class JItem> JItem & JVector<JItem>::GetItem()
{
	// on v�rifie la validit� du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on renvoie l'�l�ment du noeud
	return (m_pCursor->m_Item);
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

template <class JItem> JInt32 JVector<JItem>::GetIndex() const
{
	// on v�rifie la validit� du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on renvoie l'indice du curseur
	return (m_pCursor - m_pFNode);
}

template <class JItem> const JItem & JVector<JItem>::GetItem() const
{
	// on v�rifie la validit� du curseur
	if (m_pCursor == 0) throw JInvalidCall::GetInstance();

	// on renvoie l'�l�ment du noeud
	return (m_pCursor->m_Item);
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

template <class JItem> JVoid JVector<JItem>::MoveTo(JInt32 Index) const
{
	// on d�place le curseur sur l'�l�ment
	if (Index >= 0 && Index < m_Count) m_pCursor = m_pFNode + Index; else m_pCursor = 0;
}

template <class JItem> JVoid JVector<JItem>::MoveFirst() const
{
	// on d�place le curseur sur le premier noeud
	m_pCursor = m_pFNode;
}

template <class JItem> JVoid JVector<JItem>::MoveNext() const
{
	// on d�place le curseur sur le noeud suivant
	if (m_pCursor != 0 && m_pCursor != m_pLNode) m_pCursor += 1; else m_pCursor = 0;
}

template <class JItem> JVoid JVector<JItem>::MovePrev() const
{
	// on d�place le curseur sur le noeud pr�c�dent
	if (m_pCursor != 0 && m_pCursor != m_pFNode) m_pCursor -= 1; else m_pCursor = 0;
}

template <class JItem> JVoid JVector<JItem>::MoveLast() const
{
	// on d�place le curseur sur le dernier noeud
	m_pCursor = m_pLNode;
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

template <class JItem> JVoid JVector<JItem>::Swap(JVector<JItem> & Source)
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
// la fonction pour lib�rer les �l�ments

template <class JItem> JVoid JVector<JItem>::Reset()
{
	// on lib�re les noeuds
	this->OnRemNodes();
}

/////////////////
// le destructeur

template <class JItem> JVector<JItem>::~JVector()
{
	// on lib�re les noeuds
	this->OnRemNodes();
}

// on finit l'inclusion conditionnelle
#endif
