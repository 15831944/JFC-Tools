//
// Fichier: JBitArray.h
// Auteur:  Sylvain SAMMURI
// Date:    03/03/2003
//

// on d�bute l'inclusion conditionnelle
#ifndef JLIB_BITARRAY_H
#define JLIB_BITARRAY_H

// on inclut les d�finitions n�cessaires
#include "..\Include\JTypes.h"

// on inclut les d�finitions n�cessaires
#include "..\Include\JMsk32.h"

class JBitArray
{
public:
	// les constructeurs
	JBitArray();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour allouer les �l�ments
	JVoid SetCount(JInt32 Count);

	// les fonctions pour manipuler les �l�ments
	JVoid SetAt(JInt32 Index, JBool State);
	JBool GetAt(JInt32 Index) const;

	// les fonctions pour manipuler les �l�ments
	JVoid  SetMaskAt(JInt32 Index, JMsk32 Mask);
	JMsk32 GetMaskAt(JInt32 Index) const;

	// les op�rateurs de comparaison
	JBool operator ==(const JBitArray & Reference) const;
	JBool operator !=(const JBitArray & Reference) const;
	
	// les op�rateurs logiques
	JBitArray & operator &=(const JBitArray & Source);
	JBitArray & operator |=(const JBitArray & Source);
	JBitArray & operator ^=(const JBitArray & Source);

	// les op�rateurs logiques
	JBitArray & operator &=(JBool Source);
	JBitArray & operator |=(JBool Source);
	JBitArray & operator ^=(JBool Source);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JBitArray & Source);

	// la fonction pour r�initialiser les �l�ments
	JVoid Reset();

	// le destructeur
	~JBitArray();

private:
	// on d�finit les noeuds du tableau
	class JNode
	{
	public:
		JMsk32 m_Item; // l'�l�ment du noeud
	};

private:
	// la fonction pour calculer le nombre d'�l�ments
	JInt32 OnGetItemCount(JInt32 Count) const;

private:
	// les fonctions pour manipuler les noeuds
	JBool OnAddNodes(JInt32 Count);
	JVoid OnRemNodes();
	
private:
	// l'op�rateur d'affectation
	JBitArray & operator =(const JBitArray & Source);

	// le constructeur copie
	JBitArray(const JBitArray & Source);

private:
	JNode* m_pFNode; // les noeuds
	JInt32 m_Count;  // la taille
};

// on finit l'inclusion conditionnelle
#endif
