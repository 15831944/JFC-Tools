//
// Fichier: JFCArray.h
// Date:    02/02/2000
// Auteur:  Sylvain SAMMURI
//

// on v�rifie que le fichier n'a pas d�j� �t� inclu
#ifndef ARRAY_H

// on d�finit la macro pour �viter les inclusions multiples
#define ARRAY_H

// on inclut les d�finitions n�cessaires
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

	// la fonction pour manipuler l'�l�ment courant
	long  GetKey();
	Item* GetItem();

	// l'op�rateur d'indexation
	Item & operator[](long indice);

	// la fonction pour r�initialiser le tableau
	void Reset();

	// le destructeur
	~Array();

protected:
	long  m_Taille;  // la taille du tableau
	Item* m_Curseur; // le pointeur sur le curseur
	Item* m_Premier; // le pointeur sur le premier �l�ment
	Item* m_Dernier; // le pointeur sur le dernier �l�ment
};

//////////////////////////////////////////////////////////////////////////////
// le constructeur
//////////////////////////////////////////////////////////////////////////////

template <class Item> Array<Item>::Array()
{
	// on initialise la taille et les pointeurs sur les �l�ments
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

		// on calcule le dernier �l�ment
		dernier = premier + (taille - 1);
	}

	// on teste les exceptions
	try
	{
		// on lib�re l'ancien bloc
		delete [] ancien;
	}
	catch (...)
	{
		// on ne fait rien
	}

	// on met � jour la taille et les pointeurs sur les �l�ments
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
	// on teste la validit� du curseur
	return (m_Curseur != NULL);
}

template <class Item> void Array<Item>::Move(long indice)
{
	// on v�rifie la validit� de l'indice
	if ((indice < 0) || (indice >= m_Taille))
	{
		// on invalide le curseur
		m_Curseur = NULL;
	}
	else
	{
		// on place le curseur sur l'�l�ment d�sir�
		m_Curseur = m_Premier + indice;
	}
}

template <class Item> void Array<Item>::MoveFirst()
{
	// on place le curseur sur le premier �l�ment
	m_Curseur = m_Premier;
}

template <class Item> void Array<Item>::MoveNext()
{
	// on teste si le curseur est valide
	if (m_Curseur != NULL)
	{
		// on teste si on est sur le dernier �l�ment
		if (m_Curseur == m_Dernier)
		{
			// on invalide le curseur
			m_Curseur = NULL;
		}
		else
		{
			// on incr�mente le curseur
			m_Curseur += 1;
		}
	}
}

template <class Item> void Array<Item>::MovePrev()
{
	// on teste si le curseur est valide
	if (m_Curseur != NULL)
	{
		// on teste si on est sur le premier �l�ment
		if (m_Curseur == m_Premier)
		{
			// on invalide le curseur
			m_Curseur = NULL;
		}
		else
		{
			// on d�cr�mente le curseur
			m_Curseur -= 1;
		}
	}
}

template <class Item> void Array<Item>::MoveLast()
{
	// on place le curseur sur le dernier �l�ment
	m_Curseur = m_Dernier;
}

//////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler l'�l�ment courant
//////////////////////////////////////////////////////////////////////////////

template <class Item> long Array<Item>::GetKey()
{
	long indice;

	// on v�rifie la validit� du curseur
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
// l'op�rateur d'indexation
//////////////////////////////////////////////////////////////////////////////

template <class Item> Item & Array<Item>::operator[](long indice)
{
	// on v�rifie la validit� de l'indice
	if ((indice < 0) || (indice >= m_Taille)) throw Exception();

	// on renvoie une r�f�rence sur l'�l�ment demand�
	return (m_Premier[indice]);
}

//////////////////////////////////////////////////////////////////////////////
// la fonction pour r�initialiser le tableau
//////////////////////////////////////////////////////////////////////////////

template <class Item> void Array<Item>::Reset()
{
	// on teste les exceptions
	try
	{
		// on lib�re le bloc
		delete [] m_Premier;
	}
	catch (...)
	{
		// on ne fait rien
	}

	// on r�initialise la taille et les pointeurs sur les �l�ments
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
		// on lib�re le bloc
		delete [] m_Premier;
	}
	catch (...)
	{
		// on ne fait rien
	}
}

// fin de l'inclusion conditionnelle
#endif
