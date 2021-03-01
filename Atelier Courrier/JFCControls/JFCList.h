//
// Fichier: JFCList.h
// Date:    02/02/2000
// Auteur:  Sylvain SAMMURI
//

// on v�rifie que le fichier n'a pas d�j� �t� inclu
#ifndef LIST_H

// on d�finit la macro pour �viter les inclusions multiples
#define LIST_H

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JFCException.h"

template <class Item> class List  
{
public:
	// le constructeur
	List();

	// les fonctions pour ajouter des �l�ments
	Item* AddHead();
	Item* AddTail();

	// la fonction pour obtenir le nombre d'�l�ments
	long GetSize() const;

	// les fonctions pour manipuler le curseur
	bool IsValid() const;
	void Move(long indice) const;
	void MoveFirst() const;
	void MoveNext() const;
	void MovePrev() const;
	void MoveLast() const;

	// les fonctions pour manipuler l'�l�ment courant
	const Item* GetItem() const;
	Item* GetItem();

	// la fonction pour supprimer l'�l�ment courant
	void Remove(long methode = 0);

	// la fonction pour r�initialiser la liste
	void Reset();

	// le destructeur
	~List();

protected:
	// un noeud de la liste
	class Link
	{
	public:
		Item  m_Element;   // l'�l�ment du noeud
		Link* m_Precedent; // le pointeur sur le noeud pr�c�dent
		Link* m_Suivant;   // le pointeur sur le noeud suivant
	};

protected:
	mutable Link* m_Curseur; // le pointeur sur le curseur

protected:
	long  m_Taille;  // la taille de la liste
	Link* m_Premier; // le pointeur sur le premier noeud
	Link* m_Dernier; // le pointeur sur le dernier noeud
};

//////////////////////////////////////////////////////////////////////////////
// le constructeur
//////////////////////////////////////////////////////////////////////////////

template <class Item> List<Item>::List()
{
	// on initialise la taille et les pointeurs sur les noeuds
	m_Taille  = 0;
	m_Curseur = NULL;
	m_Premier = NULL;
	m_Dernier = NULL;
}

//////////////////////////////////////////////////////////////////////////////
// les fonctions pour ajouter des �l�ments
//////////////////////////////////////////////////////////////////////////////

template <class Item> Item* List<Item>::AddHead()
{
	Link* noeud;

	// on alloue un nouveau noeud pour l'�l�ment
	noeud = new Link;
	if (noeud == NULL) throw Exception();

	// on ins�re le nouveau noeud en d�but de liste
	noeud->m_Suivant   = m_Premier;
	noeud->m_Precedent = NULL;

	// on teste si on a un premier noeud
	if (m_Premier != NULL)
	{
		// on place le nouveau noeud avant le premier noeud
		m_Premier->m_Precedent = noeud;
	}

	// on met � jour le premier noeud de la liste
	m_Premier = noeud;

	// on teste si on a un dernier noeud
	if (m_Dernier == NULL)
	{
		// le nouveau noeud devient le dernier noeud
		m_Dernier = noeud;
	}

	// on positionne le curseur sur le nouveau noeud
	m_Curseur = noeud;

	// on incr�mente la taille de la liste
	m_Taille += 1;

	// on renvoie un pointeur sur le nouvel �l�ment
	return (&noeud->m_Element);
}

template <class Item> Item* List<Item>::AddTail()
{
	Link* noeud;

	// on alloue un nouveau noeud pour l'�l�ment
	noeud = new Link;
	if (noeud == NULL) throw Exception();

	// on ins�re le nouveau noeud en fin de liste
	noeud->m_Suivant   = NULL;
	noeud->m_Precedent = m_Dernier;

	// on teste si on a un dernier noeud
	if (m_Dernier != NULL)
	{
		// on place le nouveau noeud apr�s le dernier noeud
		m_Dernier->m_Suivant = noeud;
	}

	// on met � jour le dernier noeud de la liste
	m_Dernier = noeud;

	// on teste si on a un premier noeud
	if (m_Premier == NULL)
	{
		// le nouveau noeud devient le premier noeud
		m_Premier = noeud;
	}

	// on positionne le curseur sur le nouveau noeud
	m_Curseur = noeud;

	// on incr�mente la taille de la liste
	m_Taille += 1;

	// on renvoie un pointeur sur le nouvel �l�ment
	return (&noeud->m_Element);
}

//////////////////////////////////////////////////////////////////////////////
// la fonction pour obtenir le nombre d'�l�ments
//////////////////////////////////////////////////////////////////////////////

template <class Item> long List<Item>::GetSize() const
{
	// on renvoie la taille de la liste
	return (m_Taille);
}

//////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le curseur
//////////////////////////////////////////////////////////////////////////////

template <class Item> bool List<Item>::IsValid() const
{
	// on teste si le curseur est valide
	return (m_Curseur != NULL);
}

template <class Item> void List<Item>::Move(long indice) const
{
	long debut;
	long fin;
	long saut;

	// on v�rifie la validit� de l'indice
	if ((indice < 0) || (indice >= m_Taille))
	{
		// on invalide le curseur
		m_Curseur = NULL;
	}
	else
	{
		// on calcule le nombre de saut en partant du premier et du dernier noeud
		debut  = indice;
		fin    = m_Taille - indice - 1;

		// on teste s'il vaut mieux partir du d�but ou de la fin
		if (debut <= fin)
		{
			// on part du premier noeud
			m_Curseur = m_Premier;

			// on avance jusqu'au noeud d�sir�
			for (saut = 0; saut < debut; saut += 1)
			{
				// on passe au noeud suivant
				m_Curseur = m_Curseur->m_Suivant;
			}
		}
		else
		{
			// on part du dernier noeud
			m_Curseur = m_Dernier;

			// on recule jusqu'au noeud d�sir�
			for (saut = 0; saut < fin; saut += 1)
			{
				// on passe au noeud pr�c�dent
				m_Curseur = m_Curseur->m_Precedent;
			}
		}
	}
}

template <class Item> void List<Item>::MoveFirst() const
{
	// on place le curseur sur le premier noeud
	m_Curseur = m_Premier;
}

template <class Item> void List<Item>::MoveNext() const
{
	// on v�rifie que le curseur est valide
	if (m_Curseur != NULL)
	{
		// on place le curseur sur le noeud suivant
		m_Curseur = m_Curseur->m_Suivant;
	}
}

template <class Item> void List<Item>::MovePrev() const
{
	// on v�rifie que le curseur est valide
	if (m_Curseur != NULL)
	{
		// on place le curseur sur le noeud pr�c�dent
		m_Curseur = m_Curseur->m_Precedent;
	}
}

template <class Item> void List<Item>::MoveLast() const
{
	// on place le curseur sur le dernier noeud
	m_Curseur = m_Dernier;
}

//////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler l'�l�ment courant
//////////////////////////////////////////////////////////////////////////////

template <class Item> const Item* List<Item>::GetItem() const
{
	// on teste si le curseur est valide
	if (m_Curseur == NULL) throw Exception();

	// on renvoie l'adresse de l'�l�ment courant
	return (&m_Curseur->m_Element);
}

template <class Item> Item* List<Item>::GetItem()
{
	// on teste si le curseur est valide
	if (m_Curseur == NULL) throw Exception();

	// on renvoie l'adresse de l'�l�ment courant
	return (&m_Curseur->m_Element);
}

//////////////////////////////////////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment courant
//////////////////////////////////////////////////////////////////////////////

template <class Item> void List<Item>::Remove(long methode)
{
	Link* noeud;
	Link* precedent;
	Link* suivant;
	
	// on note le noeud � supprimer
	noeud = m_Curseur;

	// on v�rifie l'existence du noeud
	if (noeud != NULL)
	{
		// on recup�re le prochain curseur
		switch (methode)
		{
		case (-1):
			// on place le curseur sur le noeud pr�c�dent
			m_Curseur = m_Curseur->m_Precedent;
			break;
		case  (0):
			// on invalide le curseur
			m_Curseur = NULL;
			break;
		case (+1):
			// on place le curseur sur le noeud suivant
			m_Curseur = m_Curseur->m_Suivant;
			break;
		default:
			// on g�n�re une exception
			throw Exception();
		}

		// on note le noeud pr�c�dent et le noeud suivant
		precedent = noeud->m_Precedent;
		suivant   = noeud->m_Suivant;
	
		// on teste si on a un noeud pr�c�dent
		if (precedent != NULL)
		{
			// on relie le noeud pr�c�dent au noeud suivant
			precedent->m_Suivant = suivant;
		}

		// on teste si on a un noeud suivant
		if (suivant != NULL)
		{
			// on relie le noeud suivant au noeud pr�c�dent
			suivant->m_Precedent = precedent;
		}
	
		// on teste si on est en train de supprimer le premier noeud de la liste
		if (noeud == m_Premier)
		{
			// le noeud suivant devient le premier noeud
			m_Premier = suivant;
		}

		// on teste si on est en train de supprimer le dernier noeud de la liste
		if (noeud == m_Dernier)
		{
			// le noeud pr�c�dent devient le dernier noeud
			m_Dernier = precedent;
		}

		// on teste les exceptions
		try
		{
			// on lib�re le bloc du noeud
			delete noeud;
		}
		catch (...)
		{
			// on ne fait rien
		}

		// on d�cr�mente la taille de la liste
		m_Taille -= 1;
	}
}

//////////////////////////////////////////////////////////////////////////////
// la fonction pour r�initialiser la liste
//////////////////////////////////////////////////////////////////////////////

template <class Item> void List<Item>::Reset()
{
	Link* noeud;
	Link* suivant;

	// on se place sur le premier noeud
	noeud = m_Premier;

	// on lib�re les noeuds un par un
	while (noeud != NULL)
	{
		// on note le noeud suivant
		suivant = noeud->m_Suivant;

		// on teste les exceptions
		try
		{
			// on lib�re le noeud
			delete noeud;
		}
		catch (...)
		{
			// on ne fait rien
		}

		// on se place sur le noeud suivant
		noeud = suivant;
	}

	// on r�initialise la taille et les pointeurs sur les noeuds
	m_Taille  = 0;
	m_Premier = NULL;
	m_Dernier = NULL;
	m_Curseur = NULL;
}

//////////////////////////////////////////////////////////////////////////////
// le destructeur
//////////////////////////////////////////////////////////////////////////////

template <class Item> List<Item>::~List()
{
	Link* noeud;
	Link* suivant;

	// on se place sur le premier noeud
	noeud = m_Premier;

	// on lib�re les noeuds un par un
	while (noeud != NULL)
	{
		// on note le noeud suivant
		suivant = noeud->m_Suivant;

		// on teste les exceptions
		try
		{
			// on lib�re le noeud
			delete noeud;
		}
		catch (...)
		{
			// on ne fait rien
		}

		// on se place sur le noeud suivant
		noeud = suivant;
	}
}

// fin de l'inclusion conditionnelle
#endif
