//
// Fichier: JFCArray.h
// Date:    02/02/2000
// Auteur:  Sylvain SAMMURI
//

// on vérifie que le fichier n'a pas déjà été inclu
#ifndef ARRAY_H

// on définit la macro pour éviter les inclusions multiples
#define ARRAY_H

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCException.h"

template <class Item> class Array  
{
public:
	// le constructeur
	Array();

	// les fonctions pour manipuler la taille du tableau
	void SetSize(long taille);
	long GetSize();

	// les fonctions pour manipuler le curseur
	bool IsValid();
	void Move(long indice);
	void MoveFirst();
	void MoveNext();
	void MovePrev();
	void MoveLast();

	// la fonction pour manipuler l'élément courant
	long  GetKey();
	Item* GetItem();

	// l'opérateur d'indexation
	Item & operator[](long indice);

	// la fonction pour réinitialiser le tableau
	void Reset();

	// le destructeur
	~Array();

protected:
	long  m_Taille;  // la taille du tableau
	Item* m_Curseur; // le pointeur sur le curseur
	Item* m_Premier; // le pointeur sur le premier élément
	Item* m_Dernier; // le pointeur sur le dernier élément
};

//////////////////////////////////////////////////////////////////////////////
// le constructeur
//////////////////////////////////////////////////////////////////////////////

template <class Item> Array<Item>::Array()
{
	// on initialise la taille et les pointeurs sur les éléments
	m_Taille  = 0;
	m_Curseur = NULL;
	m_Premier = NULL;
	m_Dernier = NULL;
}

//////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler la taille du tableau
//////////////////////////////////////////////////////////////////////////////

template <class Item> void Array<Item>::SetSize(long taille)
{
	Item* ancien;
	Item* premier;
	Item* dernier;

	// on initialise les pointeurs
	ancien  = m_Premier;
	premier = NULL;
	dernier = NULL;

	// on corrige la taille
	taille = (taille < 0) ? 0 : taille;

	// on teste si on doit allouer un nouveau bloc
	if (taille > 0)
	{
		// on alloue un bloc
		premier = new Item[taille];
		if (premier == NULL) throw Exception();

		// on calcule le dernier élément
		dernier = premier + (taille - 1);
	}

	// on teste les exceptions
	try
	{
		// on libère l'ancien bloc
		delete [] ancien;
	}
	catch (...)
	{
		// on ne fait rien
	}

	// on met à jour la taille et les pointeurs sur les éléments
	m_Taille  = taille;
	m_Curseur = NULL;
	m_Premier = premier;
	m_Dernier = dernier;
}

template <class Item> long Array<Item>::GetSize()
{
	// on renvoie la taille
	return (m_Taille);
}

//////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le curseur
//////////////////////////////////////////////////////////////////////////////

template <class Item> bool Array<Item>::IsValid()
{
	// on teste la validité du curseur
	return (m_Curseur != NULL);
}

template <class Item> void Array<Item>::Move(long indice)
{
	// on vérifie la validité de l'indice
	if ((indice < 0) || (indice >= m_Taille))
	{
		// on invalide le curseur
		m_Curseur = NULL;
	}
	else
	{
		// on place le curseur sur l'élément désiré
		m_Curseur = m_Premier + indice;
	}
}

template <class Item> void Array<Item>::MoveFirst()
{
	// on place le curseur sur le premier élément
	m_Curseur = m_Premier;
}

template <class Item> void Array<Item>::MoveNext()
{
	// on teste si le curseur est valide
	if (m_Curseur != NULL)
	{
		// on teste si on est sur le dernier élément
		if (m_Curseur == m_Dernier)
		{
			// on invalide le curseur
			m_Curseur = NULL;
		}
		else
		{
			// on incrémente le curseur
			m_Curseur += 1;
		}
	}
}

template <class Item> void Array<Item>::MovePrev()
{
	// on teste si le curseur est valide
	if (m_Curseur != NULL)
	{
		// on teste si on est sur le premier élément
		if (m_Curseur == m_Premier)
		{
			// on invalide le curseur
			m_Curseur = NULL;
		}
		else
		{
			// on décrémente le curseur
			m_Curseur -= 1;
		}
	}
}

template <class Item> void Array<Item>::MoveLast()
{
	// on place le curseur sur le dernier élément
	m_Curseur = m_Dernier;
}

//////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler l'élément courant
//////////////////////////////////////////////////////////////////////////////

template <class Item> long Array<Item>::GetKey()
{
	long indice;

	// on vérifie la validité du curseur
	if ((m_Curseur == NULL) || (m_Curseur < m_Premier) || (m_Curseur > m_Dernier)) throw Exception();

	// on calcule l'indice courant
	indice = m_Curseur - m_Premier;

	// on renvoie l'indice
	return (indice);
}

template <class Item> Item* Array<Item>::GetItem()
{
	// on teste si le curseur est valide
	if (m_Curseur == NULL) throw Exception();

	// on renvoie le curseur
	return (m_Curseur);
}

//////////////////////////////////////////////////////////////////////////////
// l'opérateur d'indexation
//////////////////////////////////////////////////////////////////////////////

template <class Item> Item & Array<Item>::operator[](long indice)
{
	// on vérifie la validité de l'indice
	if ((indice < 0) || (indice >= m_Taille)) throw Exception();

	// on renvoie une référence sur l'élément demandé
	return (m_Premier[indice]);
}

//////////////////////////////////////////////////////////////////////////////
// la fonction pour réinitialiser le tableau
//////////////////////////////////////////////////////////////////////////////

template <class Item> void Array<Item>::Reset()
{
	// on teste les exceptions
	try
	{
		// on libère le bloc
		delete [] m_Premier;
	}
	catch (...)
	{
		// on ne fait rien
	}

	// on réinitialise la taille et les pointeurs sur les éléments
	m_Taille  = 0;
	m_Curseur = NULL;
	m_Premier = NULL;
	m_Dernier = NULL;
}

//////////////////////////////////////////////////////////////////////////////
// le destructeur
//////////////////////////////////////////////////////////////////////////////

template <class Item> Array<Item>::~Array()
{
	// on teste les exceptions
	try
	{
		// on libère le bloc
		delete [] m_Premier;
	}
	catch (...)
	{
		// on ne fait rien
	}
}

// fin de l'inclusion conditionnelle
#endif
