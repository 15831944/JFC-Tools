//
// Fichier: JATPEchelleSupports.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JATPEchelleSupports.h"

////////////////////
// les constructeurs

JATPEchelleSupports::JATPEchelleSupports()
{
	// on ne fait rien
}

JATPEchelleSupports::JATPEchelleSupports(const JATPEchelleSupports & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JATPEchelleSupports & JATPEchelleSupports::operator =(const JATPEchelleSupports & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JATPEchelleSupports::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_IdSupports.GetCount());
}

////////////////////////////////////////
// la fonction pour d�finir les �l�ments

JVoid JATPEchelleSupports::SetIdSupports(const JList<JUnt32> & ListIdSupports, JBool KeepOrder)
{
	// on cr�e les nouveaux �l�ments
	JArray<JUnt32> IdSupports;

	// on cr�e l'ensemble des �l�ments
	JMap<JUnt32, JBool> MapIds;

	// on boucle sur tous les nouveaux �l�ments
	for (ListIdSupports.MoveFirst(); ListIdSupports.IsValid(); ListIdSupports.MoveNext())
	{
		// on r�cup�re l'�l�ment courant
		const JUnt32 & IdItem = ListIdSupports.GetItem(); if (!IdItem) throw JInvalidCall::GetInstance();

		// on ajoute l'�l�ment
		MapIds.MoveTo(IdItem); if (!MapIds.IsValid()) MapIds.Add(IdItem) = false;
	}

	// on initialise l'indice
	JInt32 Idx = 0;

	// on alloue les nouveaux �l�ments
	IdSupports.SetCount(MapIds.GetCount());

	// on teste si on doit conserver l'ordre des anciens �l�ments
	if (KeepOrder)
	{
		// on r�cup�re le nombre d'anciens �l�ments
		JInt32 Count = m_IdSupports.GetCount();

		// on boucle sur tous les anciens �l�ments
		for (JInt32 Jdx = 0; Jdx < Count; Jdx += 1)
		{
			// on recherche l'�l�ment courant
			MapIds.MoveTo(m_IdSupports.Item(Jdx));

			// on ajoute l'�l�ment si on doit le garder
			if (MapIds.IsValid())
			{	
				if (MapIds.GetItem() == false)
				{ 
					IdSupports.Item(Idx) = MapIds.GetKey();
					MapIds.GetItem() = true;
					Idx += 1;
				}
			}
		}
	}

	// on boucle sur tous les nouveaux �l�ments
	for (ListIdSupports.MoveFirst(); ListIdSupports.IsValid(); ListIdSupports.MoveNext())
	{
		// on recherche l'�l�ment courant
		MapIds.MoveTo(ListIdSupports.GetItem());

		// on ajoute l'�l�ment si on ne l'a pas d�j� pris
		if (MapIds.GetItem() == false) { IdSupports.Item(Idx) = MapIds.GetKey(); MapIds.GetItem() = true; Idx += 1; }
	}

	// on permute les �l�ments
	m_IdSupports.Swap(IdSupports);
}

/////////////////////////////////////////
// la fonction pour rechercher un �l�ment

JInt32 JATPEchelleSupports::FindItem(JUnt32 IdSupport) const
{
	// on initialise l'indice
	JInt32 Idx = -1;
	JInt32 Jdx =  0;

	// on r�cup�re le nombre d'�l�ments
	JInt32 Count = m_IdSupports.GetCount();

	// on boucle sur tous les �l�ments
	while (Idx < 0 && Jdx < Count)
	{
		// on r�cup�re l'�l�ment courant
		const JUnt32 & IdItem = m_IdSupports.Item(Jdx);

		// on teste si on trouv� l'�l�ment
		if (IdItem == IdSupport) Idx = Jdx;

		// on passe � l'�l�ment suivant
		Jdx += 1;
	}

	// on renvoie l'indice de l'�l�ment
	return (Idx);
}

/////////////////////////////////////////
// la fonction pour ordonner les �l�ments

JVoid JATPEchelleSupports::SwapItems(JInt32 Index1, JInt32 Index2)
{
	// on r�cup�re les �l�ments
	JUnt32 & Ref1 = m_IdSupports.Item(Index1);
	JUnt32 & Ref2 = m_IdSupports.Item(Index2);

	// on permute les �l�ments
	JUnt32 Tmp = Ref1; Ref1 = Ref2; Ref2 = Tmp;
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

const JUnt32 & JATPEchelleSupports::Item(JInt32 Index) const
{
	// on renvoie l'�l�ment
	return (m_IdSupports.Item(Index));
}

/////////////////////////////////////////////
// les fonctions pour s�rialiser les �l�ments

JVoid JATPEchelleSupports::Send(JStream & Stream) const
{
	// on s�rialise le nombre d'�l�ments
	JInt32 Count = m_IdSupports.GetCount(); Stream.Send(Count);

	// on boucle sur tous les �l�ments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1)
	{
		// on r�cup�re l'�l�ment courant
		const JUnt32 & IdItem = m_IdSupports.Item(Idx);

		// on s�rialise l'�l�ment
		Stream.Send(IdItem);
	}
}

JVoid JATPEchelleSupports::Recv(JStream & Stream)
{
	// on cr�e les nouveaux �l�ments
	JArray<JUnt32> IdSupports;

	// on s�rialise le nombre d'�l�ments
	JInt32 Count; Stream.Recv(Count); if (Count < 0) throw JBadSchema::GetInstance();

	// on alloue les nouveaux �l�ments
	IdSupports.SetCount(Count);

	// on boucle sur tous les �l�ments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1)
	{
		// on r�cup�re l'�l�ment courant
		JUnt32 & IdItem = IdSupports.Item(Idx);

		// on s�rialise l'�l�ment
		Stream.Recv(IdItem); if (!IdItem) throw JBadSchema::GetInstance();
	}

	// on permute les �l�ments
	m_IdSupports.Swap(IdSupports);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JATPEchelleSupports::Swap(JATPEchelleSupports & Source)
{
	// on permute les �l�ments
	m_IdSupports.Swap(Source.m_IdSupports);
}

/////////////////
// le destructeur

JATPEchelleSupports::~JATPEchelleSupports()
{
	// on ne fait rien
}
