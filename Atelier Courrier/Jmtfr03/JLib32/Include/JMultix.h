//
// Fichier: JMultix.h
// Auteur:  Sylvain SAMMURI
// Date:    15/11/2002
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_MULTIX_H
#define JLIB_MULTIX_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JInternalError.h"
#include "..\Include\JInvalidCall.h"
#include "..\Include\JMemoryFull.h"

template <class JItem> class JMultix
{
public:
	// le constructeur
	JMultix();

	// les fonctions pour récupérer le nombre d'éléments
	JInt32 GetRowCount() const;
	JInt32 GetColCount(JInt32 RowIndex) const;

	// les fonctions pour allouer les éléments
	JVoid SetRowCount(JInt32 RowCount);
	JVoid SetColCount(JInt32 RowIndex, JInt32 ColCount);

	// la fonction pour récupérer les éléments
	const JItem & Item(JInt32 RowIndex, JInt32 ColIndex) const;

	// la fonction pour modifier les éléments
	JItem & Item(JInt32 RowIndex, JInt32 ColIndex);

	// la fonction pour permuter les éléments
	JVoid Swap(JMultix<JItem> & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMultix();

private:
	// on définit les noeuds de la matrice
	class JNode
	{
	public:
		JItem m_Item; // l'élément du noeud
	};

private:
	// on définit les entrées de la table
	class JTable
	{
	public:
		JNode* m_pFNode; // les noeuds de l'entrée
		JInt32 m_CountC; // la taille en colonne
	};

private:
	// la fonction pour allouer la table
	JBool OnMakeTable(JInt32 CountR);

	// la fonction pour allouer les noeuds
	JBool OnAddNodes(JInt32 IdxRow, JInt32 CountC);

	// la fonction pour libérer les noeuds
	JVoid OnRemNodes();

private:
	// l'opérateur d'affectation
	JMultix<JItem> & operator =(const JMultix<JItem> & Source);

	// le constructeur copie
	JMultix(const JMultix<JItem> & Source);

private:
	JTable* m_pFTable; // l'entrée de la table
	JInt32  m_CountR;  // la taille en ligne
};

////////////////////
// les constructeurs

template <class JItem> JMultix<JItem>::JMultix()
{
	// on initialise la taille et la table
	m_CountR = 0; m_pFTable = 0;
}

template <class JItem> JMultix<JItem>::JMultix(const JMultix<JItem> & Source)
{
	// on génère une exception
	throw JInternalError::GetInstance();
}

////////////////////////////
// l'opérateur d'affectation

template <class JItem> JMultix<JItem> & JMultix<JItem>::operator =(const JMultix<JItem> & Source)
{
	// on génère une exception
	throw JInternalError::GetInstance();
}

////////////////////////////////////
// la fonction pour allouer la table

template <class JItem> JBool JMultix<JItem>::OnMakeTable(JInt32 CountR)
{
	// on teste si on a des noeuds à allouer
	if (CountR > 0)
	{
		// on crée l'indice
		JInt32 Idx = 0;

		// on alloue la table
		JTable* pTable = new JTable[CountR]; if (pTable == 0) return (false);

		// on initialise les entrées de la table
		JTable* pCTable = pTable; while (Idx < CountR) { pCTable->m_pFNode = 0; pCTable->m_CountC = 0; pCTable += 1; Idx += 1; }

		// on met à jour la table
		m_pFTable = pTable;
	}

	// on met à jour la taille
	m_CountR = CountR;

	// on quitte
	return (true);
}

//////////////////////////////////////
// la fonction pour allouer les noeuds

template <class JItem> JBool JMultix<JItem>::OnAddNodes(JInt32 IdxRow, JInt32 CountC)
{
	// on teste si on a des noeuds à allouer
	if (CountC > 0)
	{
		// on alloue la table
		JNode* pNode = new JNode[CountC]; if (pNode == 0) return (false);

		// on met à jour la table
		(m_pFTable + IdxRow)->m_pFNode = pNode;
	}

	// on met à jour la taille
	(m_pFTable + IdxRow)->m_CountC = CountC;

	// on quitte
	return (true);
}

//////////////////////////////////////
// la fonction pour libérer les noeuds

template <class JItem> JVoid JMultix<JItem>::OnRemNodes()
{
	// on crée l'indice
	JInt32 Idx = 0;

	// on récupère la taille et la table
	JInt32  CountR = m_CountR;
	JTable* pTable = m_pFTable;

	// on réinitialise la taille et la table
	m_CountR = 0; m_pFTable = 0;

	// on supprime tous les noeuds
	JTable* pCTable = pTable; while (Idx < CountR) { delete [] pCTable->m_pFNode; pCTable += 1; Idx += 1; }

	// on supprime la table
	delete [] pTable;
}

////////////////////////////////////////////////////
// les fonctions pour récupérer le nombre d'éléments

template <class JItem> JInt32 JMultix<JItem>::GetRowCount() const
{
	// on renvoie le nombre d'éléments
	return (m_CountR);
}

template <class JItem> JInt32 JMultix<JItem>::GetColCount(JInt32 RowIndex) const
{
	// on vérifie la validité de l'indice
	if (RowIndex < 0 || RowIndex >= m_CountR) throw JInvalidCall::GetInstance();

	// on renvoie le nombre d'éléments
	return ((m_pFTable + RowIndex)->m_CountC);
}

//////////////////////////////////////////
// les fonctions pour allouer les éléments

template <class JItem> JVoid JMultix<JItem>::SetRowCount(JInt32 RowCount)
{
	// on teste le nombre d'éléments
	if (RowCount < 0) throw JInvalidCall::GetInstance();

	// on teste si on a déjà des éléments
	if (m_CountR > 0) throw JInvalidCall::GetInstance();

	// on alloue la table
	if (!this->OnMakeTable(RowCount)) throw JMemoryFull::GetInstance();
}

template <class JItem> JVoid JMultix<JItem>::SetColCount(JInt32 RowIndex, JInt32 ColCount)
{
	// on teste le nombre d'éléments
	if (ColCount < 0) throw JInvalidCall::GetInstance();

	// on vérifie la validité de l'indice
	if (RowIndex < 0 || RowIndex >= m_CountR) throw JInvalidCall::GetInstance();

	// on teste si on a déjà des éléments
	if ((m_pFTable + RowIndex)->m_CountC > 0) throw JInvalidCall::GetInstance();

	// on alloue les éléments
	if (!this->OnAddNodes(RowIndex, ColCount)) throw JMemoryFull::GetInstance();
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

template <class JItem> const JItem & JMultix<JItem>::Item(JInt32 RowIndex, JInt32 ColIndex) const
{
	// on vérifie la validité de la ligne
	if (RowIndex < 0 || RowIndex >= m_CountR) throw JInvalidCall::GetInstance();

	// on vérifie la validité de la colonne
	if (ColIndex < 0 || ColIndex >= (m_pFTable + RowIndex)->m_CountC) throw JInvalidCall::GetInstance();

	// on renvoie l'élément
	return (((m_pFTable + RowIndex)->m_pFNode + ColIndex)->m_Item);
}

/////////////////////////////////////////
// la fonction pour modifier les éléments

template <class JItem> JItem & JMultix<JItem>::Item(JInt32 RowIndex, JInt32 ColIndex)
{
	// on vérifie la validité de la ligne
	if (RowIndex < 0 || RowIndex >= m_CountR) throw JInvalidCall::GetInstance();

	// on vérifie la validité de la colonne
	if (ColIndex < 0 || ColIndex >= (m_pFTable + RowIndex)->m_CountC) throw JInvalidCall::GetInstance();

	// on renvoie l'élément
	return (((m_pFTable + RowIndex)->m_pFNode + ColIndex)->m_Item);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

template <class JItem> JVoid JMultix<JItem>::Swap(JMultix<JItem> & Source)
{
	// on permute les tables
	JTable* pFTable = m_pFTable; m_pFTable = Source.m_pFTable; Source.m_pFTable = pFTable;

	// on permute les tailles
	JInt32 CountR = m_CountR; m_CountR = Source.m_CountR; Source.m_CountR = CountR;
}

////////////////////////////////////////
// la fonction pour libérer les éléments

template <class JItem> JVoid JMultix<JItem>::Reset()
{
	// on libère les noeuds
	this->OnRemNodes();
}

/////////////////
// le destructeur

template <class JItem> JMultix<JItem>::~JMultix()
{
	// on libère les noeuds
	this->OnRemNodes();
}

// on finit l'inclusion conditionnelle
#endif
