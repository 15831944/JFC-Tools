//
// Fichier: JATPPlateauSupports.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JATPPlateauSupports.h"

////////////////////
// les constructeurs

JATPPlateauSupports::JATPPlateauSupports()
{
	// on ne fait rien
}

JATPPlateauSupports::JATPPlateauSupports(const JATPPlateauSupports & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JATPPlateauSupports & JATPPlateauSupports::operator =(const JATPPlateauSupports & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JATPPlateauSupports::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_KeySupports.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JInt32 JATPPlateauSupports::Add(CKeySupport & KeySupport)
{
	// on crée les nouveaux éléments
	JArray<CKeySupport> Supports;

	// on vérifie la validité de l'élément
	if (!KeySupport.m_IdSupport)
		throw JInvalidCall::GetInstance();

	// on récupère le nombre d'éléments et on teste les limites de l'implémentation
	JInt32 Count = m_KeySupports.GetCount();
	if (Count == JINT32_MAX)
		throw JInternalError::GetInstance();

	// on teste si l'élément existe déjà
	for (JInt32 Idx = 0; Idx < Count; Idx += 1)
		if (KeySupport == m_KeySupports.Item(Idx))
			throw JInvalidCall::GetInstance();

	// on alloue les nouveaux éléments
	Supports.SetCount(Count + 1);

	// on récupère tous les anciens éléments
	for (JInt32 Jdx = 0; Jdx < Count; Jdx += 1)
		Supports.Item(Jdx) = m_KeySupports.Item(Jdx);

	// on initialise le nouvel élément
	Supports.Item(Count) = KeySupport;

	// on permute les éléments
	m_KeySupports.Swap(Supports);

	// on renvoie l'indice de l'élément
	return (Count);
}

////////////////////////////////////////
// la fonction pour définir les éléments

JVoid JATPPlateauSupports::SetKeySupports(const JList<CKeySupport> & ListKeySupport)
{
	// on remove existing items éléments
	m_KeySupports.Reset();

	// on alloue les nouveaux éléments
	JInt32 Count  = ListKeySupport.GetCount();
	m_KeySupports.SetCount(Count);

	// on initialise l'indice
	for (JInt32 Idx = 0; Idx < Count; Idx ++)
	{
		ListKeySupport.MoveTo(Idx);
		m_KeySupports.Item(Idx) = ListKeySupport.GetItem();
	}
}

/////////////////////////////////////////
// la fonction pour rechercher un élément

JInt32 JATPPlateauSupports::FindItem(CKeySupport & KeySupport) const
{
	// on initialise l'indice
	JInt32 Idx = -1;

	// on récupère le nombre d'éléments
	JInt32 Count = m_KeySupports.GetCount();

	// on boucle sur tous les éléments
	for(JInt32 Jdx = 0; Jdx < Count; Jdx++)
	{
		// on récupère l'élément courant
		// on teste si on trouvé l'élément
		if (m_KeySupports.Item(Jdx) == KeySupport)
		{
			Idx = Jdx;
			break;
		}

	}
	// on renvoie l'indice de l'élément
	return (Idx);
}

///////////////////////////////////////////
// les fonctions pour ordonner les éléments

JVoid JATPPlateauSupports::MoveUp(JInt32 Index)
{
	// on récupère le nombre d'éléments
	JInt32 Count = m_KeySupports.GetCount();

	// on récupère l'élément à monter
	CKeySupport & KeyItem = m_KeySupports.Item(Index);

	// on teste si on a quelque chose à faire
	if (Count > 1 && Index > 0)
	{
		// récupère l'élément à descendre
		CKeySupport & KeyPrev = m_KeySupports.Item(Index - 1);

		// on permute les éléments
		CKeySupport Tmp = KeyItem; KeyItem = KeyPrev; KeyPrev = Tmp;
	}
}

JVoid JATPPlateauSupports::MoveDn(JInt32 Index)
{
	// on récupère le nombre d'éléments
	JInt32 Count = m_KeySupports.GetCount();

	// on récupère l'élément à descendre
	CKeySupport & KeyItem = m_KeySupports.Item(Index);

	// on teste si on a quelque chose à faire
	if (Count > 1 && Index < Count - 1)
	{
		// récupère l'élément à monter
		CKeySupport & KeyPrev = m_KeySupports.Item(Index + 1);

		// on permute les éléments
		CKeySupport Tmp = KeyItem; KeyItem = KeyPrev; KeyPrev = Tmp;
	}
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

const CKeySupport & JATPPlateauSupports::Item(JInt32 Index) const
{
	// on renvoie l'élément
	return (m_KeySupports.Item(Index));
}

/////////////////////////////////////////////
// les fonctions pour sérialiser les éléments

JVoid JATPPlateauSupports::Send(JStream & Stream) const
{
	// on sérialise le nombre d'éléments
	JInt32 Count = m_KeySupports.GetCount();
	Stream.Send(Count);

	// on boucle sur tous les éléments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1)
	{
		// on récupère l'élément courant
		const CKeySupport & KeyItem = m_KeySupports.Item(Idx);

		// on sérialise l'élément
		KeyItem.Send(Stream);
	}
}

JVoid JATPPlateauSupports::Recv(JStream & Stream)
{
	// on crée les nouveaux éléments
	JArray<CKeySupport> KeySupports;

	// on sérialise le nombre d'éléments
	JInt32 Count; 
	Stream.Recv(Count); 
	if (Count < 0) throw JBadSchema::GetInstance();

	// on alloue les nouveaux éléments
	KeySupports.SetCount(Count);

	// on boucle sur tous les éléments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1)
	{
		// on récupère l'élément courant
		CKeySupport & KeyItem = KeySupports.Item(Idx);

		// on sérialise l'élément
		KeyItem.Recv(Stream); 
		if (!KeyItem.m_IdSupport) 
			throw JBadSchema::GetInstance();
	}

	// on permute les éléments
	m_KeySupports.Swap(KeySupports);

}

//////////////////////////////////////////
// la fonction pour supprimer les éléments

JVoid JATPPlateauSupports::Remove(JInt32 Index)
{
	// List des support a garder
	JList<CKeySupport> tmp;

	// on récupère l'élément
	CKeySupport & KeySupport = m_KeySupports.Item(Index);

	// on récupère le nombre d'éléments
	JInt32 Count = m_KeySupports.GetCount();

	// on boucle sur tous les éléments
	for (JInt32 Idx = 0; Idx < Count; Idx ++)
	{
		// Eliminer les supports
		if(m_KeySupports.Item(Idx).m_IdSupport == KeySupport.m_IdSupport ||
			m_KeySupports.Item(Idx).m_IdParent == KeySupport.m_IdSupport)
			continue;

		// Les supports a garder
		tmp.AddTail() = m_KeySupports.Item(Idx);
	}

	// Synchro des supports plateau
	m_KeySupports.Reset();
	m_KeySupports.SetCount(tmp.GetCount ());
	for(tmp.MoveFirst(); tmp.IsValid(); tmp.MoveNext())
		m_KeySupports.Item(tmp.GetIndex ()) = tmp.GetItem ();
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JATPPlateauSupports::Swap(JATPPlateauSupports & Source)
{
	// on permute les éléments
	m_KeySupports.Swap(Source.m_KeySupports);
}

/////////////////
// le destructeur

JATPPlateauSupports::~JATPPlateauSupports()
{
	// on ne fait rien
}
