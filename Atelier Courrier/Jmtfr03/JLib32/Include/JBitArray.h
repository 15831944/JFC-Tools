//
// Fichier: JBitArray.h
// Auteur:  Sylvain SAMMURI
// Date:    03/03/2003
//

// on débute l'inclusion conditionnelle
#ifndef JLIB_BITARRAY_H
#define JLIB_BITARRAY_H

// on inclut les définitions nécessaires
#include "..\Include\JTypes.h"

// on inclut les définitions nécessaires
#include "..\Include\JMsk32.h"

class JBitArray
{
public:
	// les constructeurs
	JBitArray();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour allouer les éléments
	JVoid SetCount(JInt32 Count);

	// les fonctions pour manipuler les éléments
	JVoid SetAt(JInt32 Index, JBool State);
	JBool GetAt(JInt32 Index) const;

	// les fonctions pour manipuler les éléments
	JVoid  SetMaskAt(JInt32 Index, JMsk32 Mask);
	JMsk32 GetMaskAt(JInt32 Index) const;

	// les opérateurs de comparaison
	JBool operator ==(const JBitArray & Reference) const;
	JBool operator !=(const JBitArray & Reference) const;
	
	// les opérateurs logiques
	JBitArray & operator &=(const JBitArray & Source);
	JBitArray & operator |=(const JBitArray & Source);
	JBitArray & operator ^=(const JBitArray & Source);

	// les opérateurs logiques
	JBitArray & operator &=(JBool Source);
	JBitArray & operator |=(JBool Source);
	JBitArray & operator ^=(JBool Source);

	// la fonction pour permuter les éléments
	JVoid Swap(JBitArray & Source);

	// la fonction pour réinitialiser les éléments
	JVoid Reset();

	// le destructeur
	~JBitArray();

private:
	// on définit les noeuds du tableau
	class JNode
	{
	public:
		JMsk32 m_Item; // l'élément du noeud
	};

private:
	// la fonction pour calculer le nombre d'éléments
	JInt32 OnGetItemCount(JInt32 Count) const;

private:
	// les fonctions pour manipuler les noeuds
	JBool OnAddNodes(JInt32 Count);
	JVoid OnRemNodes();
	
private:
	// l'opérateur d'affectation
	JBitArray & operator =(const JBitArray & Source);

	// le constructeur copie
	JBitArray(const JBitArray & Source);

private:
	JNode* m_pFNode; // les noeuds
	JInt32 m_Count;  // la taille
};

// on finit l'inclusion conditionnelle
#endif
