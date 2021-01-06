//
// Fichier: JATPPlateauSupports.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on inclut les d�finitions n�cessaires
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
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JATPPlateauSupports & JATPPlateauSupports::operator =(const JATPPlateauSupports & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JATPPlateauSupports::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_KeySupports.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JInt32 JATPPlateauSupports::Add(CKeySupport & KeySupport)
{
	// on cr�e les nouveaux �l�ments
	JArray<CKeySupport> Supports;

	// on v�rifie la validit� de l'�l�ment
	if (!KeySupport.m_IdSupport)
		throw JInvalidCall::GetInstance();

	// on r�cup�re le nombre d'�l�ments et on teste les limites de l'impl�mentation
	JInt32 Count = m_KeySupports.GetCount();
	if (Count == JINT32_MAX)
		throw JInternalError::GetInstance();

	// on teste si l'�l�ment existe d�j�
	for (JInt32 Idx = 0; Idx < Count; Idx += 1)
		if (KeySupport == m_KeySupports.Item(Idx))
			throw JInvalidCall::GetInstance();

	// on alloue les nouveaux �l�ments
	Supports.SetCount(Count + 1);

	// on r�cup�re tous les anciens �l�ments
	for (JInt32 Jdx = 0; Jdx < Count; Jdx += 1)
		Supports.Item(Jdx) = m_KeySupports.Item(Jdx);

	// on initialise le nouvel �l�ment
	Supports.Item(Count) = KeySupport;

	// on permute les �l�ments
	m_KeySupports.Swap(Supports);

	// on renvoie l'indice de l'�l�ment
	return (Count);
}

////////////////////////////////////////
// la fonction pour d�finir les �l�ments

JVoid JATPPlateauSupports::SetKeySupports(const JList<CKeySupport> & ListKeySupport)
{
	// on remove existing items �l�ments
	m_KeySupports.Reset();

	// on alloue les nouveaux �l�ments
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
// la fonction pour rechercher un �l�ment

JInt32 JATPPlateauSupports::FindItem(CKeySupport & KeySupport) const
{
	// on initialise l'indice
	JInt32 Idx = -1;

	// on r�cup�re le nombre d'�l�ments
	JInt32 Count = m_KeySupports.GetCount();

	// on boucle sur tous les �l�ments
	for(JInt32 Jdx = 0; Jdx < Count; Jdx++)
	{
		// on r�cup�re l'�l�ment courant
		// on teste si on trouv� l'�l�ment
		if (m_KeySupports.Item(Jdx) == KeySupport)
		{
			Idx = Jdx;
			break;
		}

	}
	// on renvoie l'indice de l'�l�ment
	return (Idx);
}

///////////////////////////////////////////
// les fonctions pour ordonner les �l�ments

JVoid JATPPlateauSupports::MoveUp(JInt32 Index)
{
	// on r�cup�re le nombre d'�l�ments
	JInt32 Count = m_KeySupports.GetCount();

	// on r�cup�re l'�l�ment � monter
	CKeySupport & KeyItem = m_KeySupports.Item(Index);

	// on teste si on a quelque chose � faire
	if (Count > 1 && Index > 0)
	{
		// r�cup�re l'�l�ment � descendre
		CKeySupport & KeyPrev = m_KeySupports.Item(Index - 1);

		// on permute les �l�ments
		CKeySupport Tmp = KeyItem; KeyItem = KeyPrev; KeyPrev = Tmp;
	}
}

JVoid JATPPlateauSupports::MoveDn(JInt32 Index)
{
	// on r�cup�re le nombre d'�l�ments
	JInt32 Count = m_KeySupports.GetCount();

	// on r�cup�re l'�l�ment � descendre
	CKeySupport & KeyItem = m_KeySupports.Item(Index);

	// on teste si on a quelque chose � faire
	if (Count > 1 && Index < Count - 1)
	{
		// r�cup�re l'�l�ment � monter
		CKeySupport & KeyPrev = m_KeySupports.Item(Index + 1);

		// on permute les �l�ments
		CKeySupport Tmp = KeyItem; KeyItem = KeyPrev; KeyPrev = Tmp;
	}
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

const CKeySupport & JATPPlateauSupports::Item(JInt32 Index) const
{
	// on renvoie l'�l�ment
	return (m_KeySupports.Item(Index));
}

/////////////////////////////////////////////
// les fonctions pour s�rialiser les �l�ments

JVoid JATPPlateauSupports::Send(JStream & Stream) const
{
	// on s�rialise le nombre d'�l�ments
	JInt32 Count = m_KeySupports.GetCount();
	Stream.Send(Count);

	// on boucle sur tous les �l�ments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1)
	{
		// on r�cup�re l'�l�ment courant
		const CKeySupport & KeyItem = m_KeySupports.Item(Idx);

		// on s�rialise l'�l�ment
		KeyItem.Send(Stream);
	}
}

JVoid JATPPlateauSupports::Recv(JStream & Stream)
{
	// on cr�e les nouveaux �l�ments
	JArray<CKeySupport> KeySupports;

	// on s�rialise le nombre d'�l�ments
	JInt32 Count; 
	Stream.Recv(Count); 
	if (Count < 0) throw JBadSchema::GetInstance();

	// on alloue les nouveaux �l�ments
	KeySupports.SetCount(Count);

	// on boucle sur tous les �l�ments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1)
	{
		// on r�cup�re l'�l�ment courant
		CKeySupport & KeyItem = KeySupports.Item(Idx);

		// on s�rialise l'�l�ment
		KeyItem.Recv(Stream); 
		if (!KeyItem.m_IdSupport) 
			throw JBadSchema::GetInstance();
	}

	// on permute les �l�ments
	m_KeySupports.Swap(KeySupports);

}

//////////////////////////////////////////
// la fonction pour supprimer les �l�ments

JVoid JATPPlateauSupports::Remove(JInt32 Index)
{
	// List des support a garder
	JList<CKeySupport> tmp;

	// on r�cup�re l'�l�ment
	CKeySupport & KeySupport = m_KeySupports.Item(Index);

	// on r�cup�re le nombre d'�l�ments
	JInt32 Count = m_KeySupports.GetCount();

	// on boucle sur tous les �l�ments
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
// la fonction pour permuter les �l�ments

JVoid JATPPlateauSupports::Swap(JATPPlateauSupports & Source)
{
	// on permute les �l�ments
	m_KeySupports.Swap(Source.m_KeySupports);
}

/////////////////
// le destructeur

JATPPlateauSupports::~JATPPlateauSupports()
{
	// on ne fait rien
}
