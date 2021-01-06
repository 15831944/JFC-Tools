//
// Fichier: JArray.h
// Auteur:  Sylvain SAMMURI
// Date:    09/10/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_ARRAY_H
#define JLIB_ARRAY_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JInternalError.h"
#include "..\Include\JInvalidCall.h"
#include "..\Include\JMemoryFull.h"

template <class JItem> class JArray
{
public:
	// le constructeur
	JArray();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour allouer les �l�ments
	JVoid SetCount(JInt32 Count);

	// la fonction pour r�cup�rer les �l�ments
	const JItem & Item(JInt32 Index) const;

	// la fonction pour modifier les �l�ments
	JItem & Item(JInt32 Index);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JArray<JItem> & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JArray();

private:
	// on d�finit les noeuds du tableau
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
	// on g�n�re une exception
	throw JInternalError::GetInstance();
}

////////////////////////////
// l'op�rateur d'affectation

template <class JItem> JArray<JItem> & JArray<JItem>::operator =(const JArray<JItem> & Source)
{
	// on g�n�re une exception
	throw JInternalError::GetInstance();
}

//////////////////////////////////////
// la fonction pour allouer les noeuds

template <class JItem> JBool JArray<JItem>::OnAddNodes(JInt32 Count)
{
	// on teste si on a des noeuds � allouer
	if (Count > 0)
	{
		// on alloue les noeuds
		JNode* pNode = new JNode[Count]; if (pNode == 0) return (false);

		// on met � jour les noeuds
		m_pFNode = pNode;
	}

	// on met � jour la taille
	m_Count = Count;

	// on quitte
	return (true);
}

//////////////////////////////////////
// la fonction pour lib�rer les noeuds

template <class JItem> JVoid JArray<JItem>::OnRemNodes()
{
	// on m�morise le noeud
	JNode* pNode = m_pFNode;

	// on r�initialise la taille et les noeuds
	m_Count = 0; m_pFNode = 0;

	// on supprime les �l�ments
	delete [] pNode;
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

template <class JItem> JInt32 JArray<JItem>::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_Count);
}

////////////////////////////////////////
// la fonction pour allouer les �l�ments

template <class JItem> JVoid JArray<JItem>::SetCount(JInt32 Count)
{
	// on teste le nombre d'�l�ments
	if (Count < 0) throw JInvalidCall::GetInstance();

	// on teste si on a d�j� des �l�ments
	if (m_Count > 0) throw JInvalidCall::GetInstance();

	// on alloue les �l�ments
	if (!this->OnAddNodes(Count)) throw JMemoryFull::GetInstance();
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

template <class JItem> const JItem & JArray<JItem>::Item(JInt32 Index) const
{
	// on v�rifie la validit� de l'indice
	if (Index < 0 || Index >= m_Count) throw JInvalidCall::GetInstance();

	// on renvoie l'�l�ment
	return ((m_pFNode + Index)->m_Item);
}

/////////////////////////////////////////
// la fonction pour modifier les �l�ments

template <class JItem> JItem & JArray<JItem>::Item(JInt32 Index)
{
	// on v�rifie la validit� de l'indice
	if (Index < 0 || Index >= m_Count) throw JInvalidCall::GetInstance();

	// on renvoie l'�l�ment
	return ((m_pFNode + Index)->m_Item);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

template <class JItem> JVoid JArray<JItem>::Swap(JArray<JItem> & Source)
{
	// on permute les noeuds
	JNode* pFNode = m_pFNode; m_pFNode = Source.m_pFNode; Source.m_pFNode = pFNode;

	// on permute la taille
	JInt32 Count = m_Count; m_Count = Source.m_Count; Source.m_Count = Count;
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

template <class JItem> JVoid JArray<JItem>::Reset()
{
	// on lib�re les noeuds
	this->OnRemNodes();
}

/////////////////
// le destructeur

template <class JItem> JArray<JItem>::~JArray()
{
	// on lib�re les noeuds
	this->OnRemNodes();
}

// on finit l'inclusion conditionnelle
#endif
