//
// Fichier: JFR03TBLTerrains.cpp
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on inclut les définitions nécessaires
#include "JFR03TBLTerrains.h"

////////////////////
// les constructeurs

JFR03TBLTerrains::JFR03TBLTerrains()
{
	// on ne fait rien
}

JFR03TBLTerrains::JFR03TBLTerrains(const JFR03TBLTerrains & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JFR03TBLTerrains & JFR03TBLTerrains::operator =(const JFR03TBLTerrains & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JFR03TBLTerrains::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

///////////////////////////////////////////
// la fonction pour rechercher les éléments

JInt32 JFR03TBLTerrains::FindItem(JRef32 IdTerrain) const
{
	// on initialise l'indice
	JInt32 Idx = -1;

	// on initialise les bornes de recherche
	JInt32 Min = 0;
	JInt32 Max = m_TBLItems.GetCount() - 1;

	// on boucle tant qu'on a pas trouvé l'élément
	while (Idx < 0 && Min <= Max)
	{
		// on calcule l'indice médian
		JInt32 Med = ((Max - Min) / 2) + Min;

		// on récupère l'identifiant de l'élément
		const JRef32 & IdRef = m_TBLItems.Item(Med).m_IdTerrain;

		// on compare les références
		if      (IdRef < IdTerrain) Min = Med + 1;
		else if (IdRef > IdTerrain) Max = Med - 1;
		else                        Idx = Med;
	}

	// on renvoie l'indice
	return (Idx);
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

const JFR03Terrain & JFR03TBLTerrains::Item(JInt32 Index) const
{
	// on renvoie l'élément
	return (m_TBLItems.Item(Index));
}

////////////////////////////////////////////////////
// les fonctions pour importer/exporter les éléments

JVoid JFR03TBLTerrains::Import(JStream* pStream)
{
	// on crée les éléments
	JArray<JFR03Terrain> TBLItems;

	// on crée le parseur
	JParser* pParser = JParser::Create(pStream);

	// on teste les exceptions
	try
	{
		// on ajoute le séparateur
		pParser->AddOperator(",", 9);

		// on crée les éléments temporaires
		JMap<JRef32, JFR03Terrain> TmpItems;

		// on boucle sur tous les lexèmes
		while (!pParser->IsEmpty())
		{
			// on récupère le lexème suivant
			JLexeme Lex01 = pParser->Recv();

			// on teste si la ligne est vide
			if (Lex01.GetType() != 1)
			{
				// on crée un élément
				JFR03Terrain Item;

				// on teste si on est sur un entier
				if (Lex01.GetType() != 3) throw JBadSchema::GetInstance();

				// on récupère les lexèmes suivants
				JLexeme Lex02 = pParser->Recv(); if (Lex02.GetType() != 9) throw JBadSchema::GetInstance();
				JLexeme Lex03 = pParser->Recv(); if (Lex03.GetType() != 3) throw JBadSchema::GetInstance();
				JLexeme Lex04 = pParser->Recv(); if (Lex04.GetType() != 9) throw JBadSchema::GetInstance();
				JLexeme Lex05 = pParser->Recv(); if (Lex05.GetType() != 5) throw JBadSchema::GetInstance();
				JLexeme Lex06 = pParser->Recv(); if (Lex06.GetType() != 9) throw JBadSchema::GetInstance();
				JLexeme Lex07 = pParser->Recv(); if (Lex07.GetType() != 5) throw JBadSchema::GetInstance();

				// Les valeurs seuils en +
				JLexeme Lex08 = pParser->Recv(); if (Lex08.GetType() != 9) throw JBadSchema::GetInstance();
				JLexeme Lex09 = pParser->Recv(); if (Lex09.GetType() != 3) throw JBadSchema::GetInstance();
				JLexeme Lex10 = pParser->Recv(); if (Lex10.GetType() != 9) throw JBadSchema::GetInstance();
				JLexeme Lex11 = pParser->Recv(); if (Lex11.GetType() != 3) throw JBadSchema::GetInstance();

				// Fin de ligne
				JLexeme Lex12 = pParser->Recv(); if (Lex12.GetType() != 1) throw JBadSchema::GetInstance();

				// on effectue les conversions des lexèmes
				JInt32 Val01; if (!JConvert::ToBinary(Lex01.AsJCharPtr(), Val01)) throw JBadSchema::GetInstance();
				JInt32 Val03; if (!JConvert::ToBinary(Lex03.AsJCharPtr(), Val03)) throw JBadSchema::GetInstance();

				// on initialise l'élément
				Item.m_IdTerrain = Val01;
				Item.m_Libelle   = Lex07.AsJCharPtr();
				Item.m_IdSource  = Val03;
				Item.m_Racine    = Lex05.AsJCharPtr();

				// on vérifie la validité de l'élément
				if (!Item.IsValid()) throw JBadSchema::GetInstance();

				// on vérifie si l'élément existe déjà
				TmpItems.MoveTo(Item.m_IdTerrain); if (TmpItems.IsValid()) throw JBadSchema::GetInstance();

				// on ajoute l'élément
				TmpItems.Add(Item.m_IdTerrain) = Item;
			}
		}

		// on crée les nouveaux éléments
		TBLItems.SetCount(TmpItems.GetCount());

		// on se place sur les premiers éléments
		JInt32 Idx = 0; TmpItems.MoveFirst();

		// on boucle sur tous les éléments
		while (TmpItems.IsValid())
		{
			// on recopie l'élément courant
			TBLItems.Item(Idx) = TmpItems.GetItem();

			// on passe aux éléments suivants
			TmpItems.MoveNext(); Idx += 1;
		}
	}
	catch (...)
	{
		// on libère le parseur
		delete pParser;

		// on propage l'exception
		throw;
	}

	// on permute les éléments
	m_TBLItems.Swap(TBLItems);

	// on libère le parseur
	delete pParser;
}

JVoid JFR03TBLTerrains::Export(JStream* pStream) const
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JFR03TBLTerrains::Swap(JFR03TBLTerrains & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JFR03TBLTerrains::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JFR03TBLTerrains::~JFR03TBLTerrains()
{
	// on ne fait rien
}
