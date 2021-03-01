//
// Fichier: JArray.h
// Auteur:  Sylvain SAMMURI
// Date:    09/10/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_ARRAY_H
#define JLIB_ARRAY_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JInternalError.h"
#include "..\Include\JInvalidCall.h"
#include "..\Include\JMemoryFull.h"

template <class JItem> class JArray
{
public:
	// le constructeur
	JArray();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour allouer les éléments
	JVoid SetCount(JInt32 Count);

	// la fonction pour récupérer les éléments
	const JItem & Item(JInt32 Index) const;

	// la fonction pour modifier les éléments
	JItem & Item(JInt32 Index);

	// la fonction pour permuter les éléments
	JVoid Swap(JArray<JItem> & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JArray();

private:
	// on définit les noeuds du tableau
	class JNode
	{
	public:
		JItem m_Item; // l'élément du noeud
	};

private:
	// la fonction pour allouer les noeuds
	JBool OnAddNodes(JInt32 Count);

	// la fonction pour libérer les noeuds
	JVoid OnRemNodes();

private:
	// l'opérateur d'affectation
	JArray<JItem> & operator =(const JArray<JItem> & Source);

	// le constructeur copie
	JArray(const JArray<JItem> & Source);

private:
	JNode* m_pFNode; // les noeuds
	JInt32 m_Count;  // la taille
};

////////////////////
// les constructeurs

template <class JItem> JArray<JItem>::JArray()
{
	// on initialise la taille et les noeuds
	m_Count = 0; m_pFNode = 0;
}

template <class JItem> JArray<JItem>::JArray(const JArray<JItem> & Source)
{
	// on génère une exception
	throw JInternalError::GetInstance();
}

////////////////////////////
// l'opérateur d'affectation

template <class JItem> JArray<JItem> & JArray<JItem>::operator =(const JArray<JItem> & Source)
{
	// on génère une exception
	throw JInternalError::GetInstance();
}

//////////////////////////////////////
// la fonction pour allouer les noeuds

template <class JItem> JBool JArray<JItem>::OnAddNodes(JInt32 Count)
{
	// on teste si on a des noeuds à allouer
	if (Count > 0)
	{
		// on alloue les noeuds
		JNode* pNode = new JNode[Count]; if (pNode == 0) return (false);

		// on met à jour les noeuds
		m_pFNode = pNode;
	}

	// on met à jour la taille
	m_Count = Count;

	// on quitte
	return (true);
}

//////////////////////////////////////
// la fonction pour libérer les noeuds

template <class JItem> JVoid JArray<JItem>::OnRemNodes()
{
	// on mémorise le noeud
	JNode* pNode = m_pFNode;

	// on réinitialise la taille et les noeuds
	m_Count = 0; m_pFNode = 0;

	// on supprime les éléments
	delete [] pNode;
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

template <class JItem> JInt32 JArray<JItem>::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_Count);
}

////////////////////////////////////////
// la fonction pour allouer les éléments

template <class JItem> JVoid JArray<JItem>::SetCount(JInt32 Count)
{
	// on teste le nombre d'éléments
	if (Count < 0) throw JInvalidCall::GetInstance();

	// on teste si on a déjà des éléments
	if (m_Count > 0) throw JInvalidCall::GetInstance();

	// on alloue les éléments
	if (!this->OnAddNodes(Count)) throw JMemoryFull::GetInstance();
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

template <class JItem> const JItem & JArray<JItem>::Item(JInt32 Index) const
{
	// on vérifie la validité de l'indice
	if (Index < 0 || Index >= m_Count) throw JInvalidCall::GetInstance();

	// on renvoie l'élément
	return ((m_pFNode + Index)->m_Item);
}

/////////////////////////////////////////
// la fonction pour modifier les éléments

template <class JItem> JItem & JArray<JItem>::Item(JInt32 Index)
{
	// on vérifie la validité de l'indice
	if (Index < 0 || Index >= m_Count) throw JInvalidCall::GetInstance();

	// on renvoie l'élément
	return ((m_pFNode + Index)->m_Item);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

template <class JItem> JVoid JArray<JItem>::Swap(JArray<JItem> & Source)
{
	// on permute les noeuds
	JNode* pFNode = m_pFNode; m_pFNode = Source.m_pFNode; Source.m_pFNode = pFNode;

	// on permute la taille
	JInt32 Count = m_Count; m_Count = Source.m_Count; Source.m_Count = Count;
}

////////////////////////////////////////
// la fonction pour libérer les éléments

template <class JItem> JVoid JArray<JItem>::Reset()
{
	// on libère les noeuds
	this->OnRemNodes();
}

/////////////////
// le destructeur

template <class JItem> JArray<JItem>::~JArray()
{
	// on libère les noeuds
	this->OnRemNodes();
}

// on finit l'inclusion conditionnelle
#endif
