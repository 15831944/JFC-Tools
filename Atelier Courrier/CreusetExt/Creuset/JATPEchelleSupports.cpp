//
// Fichier: JATPEchelleSupports.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on inclut les définitions nécessaires
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
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JATPEchelleSupports & JATPEchelleSupports::operator =(const JATPEchelleSupports & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JATPEchelleSupports::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_IdSupports.GetCount());
}

////////////////////////////////////////
// la fonction pour définir les éléments

JVoid JATPEchelleSupports::SetIdSupports(const JList<JUnt32> & ListIdSupports, JBool KeepOrder)
{
	// on crée les nouveaux éléments
	JArray<JUnt32> IdSupports;

	// on crée l'ensemble des éléments
	JMap<JUnt32, JBool> MapIds;

	// on boucle sur tous les nouveaux éléments
	for (ListIdSupports.MoveFirst(); ListIdSupports.IsValid(); ListIdSupports.MoveNext())
	{
		// on récupère l'élément courant
		const JUnt32 & IdItem = ListIdSupports.GetItem(); if (!IdItem) throw JInvalidCall::GetInstance();

		// on ajoute l'élément
		MapIds.MoveTo(IdItem); if (!MapIds.IsValid()) MapIds.Add(IdItem) = false;
	}

	// on initialise l'indice
	JInt32 Idx = 0;

	// on alloue les nouveaux éléments
	IdSupports.SetCount(MapIds.GetCount());

	// on teste si on doit conserver l'ordre des anciens éléments
	if (KeepOrder)
	{
		// on récupère le nombre d'anciens éléments
		JInt32 Count = m_IdSupports.GetCount();

		// on boucle sur tous les anciens éléments
		for (JInt32 Jdx = 0; Jdx < Count; Jdx += 1)
		{
			// on recherche l'élément courant
			MapIds.MoveTo(m_IdSupports.Item(Jdx));

			// on ajoute l'élément si on doit le garder
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

	// on boucle sur tous les nouveaux éléments
	for (ListIdSupports.MoveFirst(); ListIdSupports.IsValid(); ListIdSupports.MoveNext())
	{
		// on recherche l'élément courant
		MapIds.MoveTo(ListIdSupports.GetItem());

		// on ajoute l'élément si on ne l'a pas déjà pris
		if (MapIds.GetItem() == false) { IdSupports.Item(Idx) = MapIds.GetKey(); MapIds.GetItem() = true; Idx += 1; }
	}

	// on permute les éléments
	m_IdSupports.Swap(IdSupports);
}

/////////////////////////////////////////
// la fonction pour rechercher un élément

JInt32 JATPEchelleSupports::FindItem(JUnt32 IdSupport) const
{
	// on initialise l'indice
	JInt32 Idx = -1;
	JInt32 Jdx =  0;

	// on récupère le nombre d'éléments
	JInt32 Count = m_IdSupports.GetCount();

	// on boucle sur tous les éléments
	while (Idx < 0 && Jdx < Count)
	{
		// on récupère l'élément courant
		const JUnt32 & IdItem = m_IdSupports.Item(Jdx);

		// on teste si on trouvé l'élément
		if (IdItem == IdSupport) Idx = Jdx;

		// on passe à l'élément suivant
		Jdx += 1;
	}

	// on renvoie l'indice de l'élément
	return (Idx);
}

/////////////////////////////////////////
// la fonction pour ordonner les éléments

JVoid JATPEchelleSupports::SwapItems(JInt32 Index1, JInt32 Index2)
{
	// on récupère les éléments
	JUnt32 & Ref1 = m_IdSupports.Item(Index1);
	JUnt32 & Ref2 = m_IdSupports.Item(Index2);

	// on permute les éléments
	JUnt32 Tmp = Ref1; Ref1 = Ref2; Ref2 = Tmp;
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

const JUnt32 & JATPEchelleSupports::Item(JInt32 Index) const
{
	// on renvoie l'élément
	return (m_IdSupports.Item(Index));
}

/////////////////////////////////////////////
// les fonctions pour sérialiser les éléments

JVoid JATPEchelleSupports::Send(JStream & Stream) const
{
	// on sérialise le nombre d'éléments
	JInt32 Count = m_IdSupports.GetCount(); Stream.Send(Count);

	// on boucle sur tous les éléments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1)
	{
		// on récupère l'élément courant
		const JUnt32 & IdItem = m_IdSupports.Item(Idx);

		// on sérialise l'élément
		Stream.Send(IdItem);
	}
}

JVoid JATPEchelleSupports::Recv(JStream & Stream)
{
	// on crée les nouveaux éléments
	JArray<JUnt32> IdSupports;

	// on sérialise le nombre d'éléments
	JInt32 Count; Stream.Recv(Count); if (Count < 0) throw JBadSchema::GetInstance();

	// on alloue les nouveaux éléments
	IdSupports.SetCount(Count);

	// on boucle sur tous les éléments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1)
	{
		// on récupère l'élément courant
		JUnt32 & IdItem = IdSupports.Item(Idx);

		// on sérialise l'élément
		Stream.Recv(IdItem); if (!IdItem) throw JBadSchema::GetInstance();
	}

	// on permute les éléments
	m_IdSupports.Swap(IdSupports);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JATPEchelleSupports::Swap(JATPEchelleSupports & Source)
{
	// on permute les éléments
	m_IdSupports.Swap(Source.m_IdSupports);
}

/////////////////
// le destructeur

JATPEchelleSupports::~JATPEchelleSupports()
{
	// on ne fait rien
}
