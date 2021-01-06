//
// Fichier: JMatrix.h
// Auteur:  Sylvain SAMMURI
// Date:    09/10/2002
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_MATRIX_H
#define JLIB_MATRIX_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JInternalError.h"
#include "..\Include\JInvalidCall.h"
#include "..\Include\JMemoryFull.h"

template <class JItem> class JMatrix
{
public:
	// le constructeur
	JMatrix();

	// les fonctions pour r�cup�rer le nombre d'�l�ments
	JInt32 GetRowCount() const;
	JInt32 GetColCount() const;

	// la fonction pour allouer les �l�ments
	JVoid SetCount(JInt32 RowCount, JInt32 ColCount);

	// la fonction pour r�cup�rer les �l�ments
	const JItem & Item(JInt32 RowIndex, JInt32 ColIndex) const;

	// la fonction pour modifier les �l�ments
	JItem & Item(JInt32 RowIndex, JInt32 ColIndex);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMatrix<JItem> & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMatrix();

private:
	// on d�finit les noeuds de la matrice
	class JNode
	{
	public:
		JItem m_Item; // l'�l�ment du noeud
	};

private:
	// la fonction pour allouer les noeuds
	JBool OnAddNodes(JInt32 CountR, JInt32 CountC);

	// la fonction pour lib�rer les noeuds
	JVoid OnRemNodes();

private:
	// l'op�rateur d'affectation
	JMatrix<JItem> & operator =(const JMatrix<JItem> & Source);

	// le constructeur copie
	JMatrix(const JMatrix<JItem> & Source);

private:
	JNode* m_pFNode; // les noeuds
	JInt32 m_CountC; // la taille en colonne
	JInt32 m_CountR; // la taille en ligne
};

////////////////////
// les constructeurs

template <class JItem> JMatrix<JItem>::JMatrix()
{
	// on initialise les tailles et les noeuds
	m_CountR = m_CountC = 0; m_pFNode = 0;
}

template <class JItem> JMatrix<JItem>::JMatrix(const JMatrix<JItem> & Source)
{
	// on g�n�re une exception
	throw JInternalError::GetInstance();
}

////////////////////////////
// l'op�rateur d'affectation

template <class JItem> JMatrix<JItem> & JMatrix<JItem>::operator =(const JMatrix<JItem> & Source)
{
	// on g�n�re une exception
	throw JInternalError::GetInstance();
}

//////////////////////////////////////
// la fonction pour allouer les noeuds

template <class JItem> JBool JMatrix<JItem>::OnAddNodes(JInt32 CountR, JInt32 CountC)
{
	// on calcule le nombre de noeuds
	JInt32 Count = CountR * CountC;

	// on teste si on a des noeuds � allouer
	if (Count > 0)
	{
		// on alloue les noeuds
		JNode* pNode = new JNode[Count]; if (pNode == 0) return (false);

		// on met � jour les noeuds
		m_pFNode = pNode;		
	}

	// on met � jour les tailles
	m_CountR = CountR;
	m_CountC = CountC;

	// on quitte
	return (true);
}

//////////////////////////////////////
// la fonction pour lib�rer les noeuds

template <class JItem> JVoid JMatrix<JItem>::OnRemNodes()
{
	// on m�morise les noeuds
	JNode* pNode = m_pFNode;

	// on r�initialise les tailles et les noeuds
	m_CountR = m_CountC = 0; m_pFNode = 0;

	// on supprime les �l�ments
	delete [] pNode;
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer le nombre d'�l�ments

template <class JItem> JInt32 JMatrix<JItem>::GetRowCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_CountR);
}

template <class JItem> JInt32 JMatrix<JItem>::GetColCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_CountC);
}

////////////////////////////////////////
// la fonction pour allouer les �l�ments

template <class JItem> JVoid JMatrix<JItem>::SetCount(JInt32 RowCount, JInt32 ColCount)
{
	// on teste le nombre d'�l�ments
	if (RowCount < 0 || ColCount < 0) throw JInvalidCall::GetInstance();

	// on teste si on a d�j� des �l�ments
	if (m_CountR > 0 || m_CountC > 0) throw JInvalidCall::GetInstance();

	// on alloue les �l�ments
	if (!this->OnAddNodes(RowCount, ColCount)) throw JMemoryFull::GetInstance();
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

template <class JItem> const JItem & JMatrix<JItem>::Item(JInt32 RowIndex, JInt32 ColIndex) const
{
	// on v�rifie la validit� des indices
	if (RowIndex < 0 || RowIndex >= m_CountR) throw JInvalidCall::GetInstance();
	if (ColIndex < 0 || ColIndex >= m_CountC) throw JInvalidCall::GetInstance();

	// on renvoie l'�l�ment
	return ((m_pFNode + (RowIndex * m_CountC + ColIndex))->m_Item);
}

/////////////////////////////////////////
// la fonction pour modifier les �l�ments

template <class JItem> JItem & JMatrix<JItem>::Item(JInt32 RowIndex, JInt32 ColIndex)
{
	// on v�rifie la validit� des indices
	if (RowIndex < 0 || RowIndex >= m_CountR) throw JInvalidCall::GetInstance();
	if (ColIndex < 0 || ColIndex >= m_CountC) throw JInvalidCall::GetInstance();

	// on renvoie l'�l�ment
	return ((m_pFNode + (RowIndex * m_CountC + ColIndex))->m_Item);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

template <class JItem> JVoid JMatrix<JItem>::Swap(JMatrix<JItem> & Source)
{
	// on permute les noeuds
	JNode* pFNode = m_pFNode; m_pFNode = Source.m_pFNode; Source.m_pFNode = pFNode;

	// on permute les tailles
	JInt32 CountR = m_CountR; m_CountR = Source.m_CountR; Source.m_CountR = CountR;
	JInt32 CountC = m_CountC; m_CountC = Source.m_CountC; Source.m_CountC = CountC;
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

template <class JItem> JVoid JMatrix<JItem>::Reset()
{
	// on lib�re les noeuds
	this->OnRemNodes();
}

/////////////////
// le destructeur

template <class JItem> JMatrix<JItem>::~JMatrix()
{
	// on lib�re les noeuds
	this->OnRemNodes();
}

// on finit l'inclusion conditionnelle
#endif
