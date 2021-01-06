//
// Fichier: JFR03TBLSources.cpp
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on inclut les d�finitions n�cessaires
#include "JFR03TBLSources.h"

////////////////////
// les constructeurs

JFR03TBLSources::JFR03TBLSources()
{
	// on ne fait rien
}

JFR03TBLSources::JFR03TBLSources(const JFR03TBLSources & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JFR03TBLSources & JFR03TBLSources::operator =(const JFR03TBLSources & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JFR03TBLSources::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

///////////////////////////////////////////
// la fonction pour rechercher les �l�ments

JInt32 JFR03TBLSources::FindItem(JRef32 IdSource) const
{
	// on initialise l'indice
	JInt32 Idx = -1;

	// on initialise les bornes de recherche
	JInt32 Min = 0;
	JInt32 Max = m_TBLItems.GetCount() - 1;

	// on boucle tant qu'on a pas trouv� l'�l�ment
	while (Idx < 0 && Min <= Max)
	{
		// on calcule l'indice m�dian
		JInt32 Med = ((Max - Min) / 2) + Min;

		// on r�cup�re l'identifiant de l'�l�ment
		const JRef32 & IdRef = m_TBLItems.Item(Med).m_IdSource;

		// on compare les r�f�rences
		if      (IdRef < IdSource) Min = Med + 1;
		else if (IdRef > IdSource) Max = Med - 1;
		else                       Idx = Med;
	}

	// on renvoie l'indice
	return (Idx);
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

const JFR03Source & JFR03TBLSources::Item(JInt32 Index) const
{
	// on renvoie l'�l�ment
	return (m_TBLItems.Item(Index));
}

////////////////////////////////////////////////////
// les fonctions pour importer/exporter les �l�ments

JVoid JFR03TBLSources::Import(JStream* pStream)
{
	// on cr�e les �l�ments
	JArray<JFR03Source> TBLItems;

	// on cr�e le parseur
	JParser* pParser = JParser::Create(pStream);

	// on teste les exceptions
	try
	{
		// on ajoute le s�parateur
		pParser->AddOperator(",", 9);

		// on cr�e les �l�ments temporaires
		JMap<JRef32, JFR03Source> TmpItems;

		// on boucle sur tous les lex�mes
		while (!pParser->IsEmpty())
		{
			// on r�cup�re le lex�me suivant
			JLexeme Lex01 = pParser->Recv();

			// on teste si la ligne est vide
			if (Lex01.GetType() != 1)
			{
				// on cr�e un �l�ment
				JFR03Source Item;

				// on teste si on est sur un entier
				if (Lex01.GetType() != 3) throw JBadSchema::GetInstance();

				// on r�cup�re les lex�mes suivants
				JLexeme Lex02 = pParser->Recv(); if (Lex02.GetType() != 9) throw JBadSchema::GetInstance();
				JLexeme Lex03 = pParser->Recv(); if (Lex03.GetType() != 3) throw JBadSchema::GetInstance();
				JLexeme Lex04 = pParser->Recv(); if (Lex04.GetType() != 9) throw JBadSchema::GetInstance();
				JLexeme Lex05 = pParser->Recv(); if (Lex05.GetType() != 5) throw JBadSchema::GetInstance();
				JLexeme Lex06 = pParser->Recv(); if (Lex06.GetType() != 1) throw JBadSchema::GetInstance();

				// on effectue les conversions des lex�mes
				JInt32 Val01; if (!JConvert::ToBinary(Lex01.AsJCharPtr(), Val01)) throw JBadSchema::GetInstance();
				JInt32 Val03; if (!JConvert::ToBinary(Lex03.AsJCharPtr(), Val03)) throw JBadSchema::GetInstance();

				// on initialise l'�l�ment
				Item.m_IdSource = Val01;
				Item.m_Libelle  = Lex05.AsJCharPtr();
				Item.m_IdMedia  = Val03;

				// on v�rifie la validit� de l'�l�ment
				if (!Item.IsValid()) throw JBadSchema::GetInstance();

				// on v�rifie si l'�l�ment existe d�j�
				TmpItems.MoveTo(Item.m_IdSource); if (TmpItems.IsValid()) throw JBadSchema::GetInstance();

				// on ajoute l'�l�ment
				TmpItems.Add(Item.m_IdSource) = Item;
			}
		}

		// on cr�e les nouveaux �l�ments
		TBLItems.SetCount(TmpItems.GetCount());

		// on se place sur les premiers �l�ments
		JInt32 Idx = 0; TmpItems.MoveFirst();

		// on boucle sur tous les �l�ments
		while (TmpItems.IsValid())
		{
			// on recopie l'�l�ment courant
			TBLItems.Item(Idx) = TmpItems.GetItem();

			// on passe aux �l�ments suivants
			TmpItems.MoveNext(); Idx += 1;
		}
	}
	catch (...)
	{
		// on lib�re le parseur
		delete pParser;

		// on propage l'exception
		throw;
	}

	// on permute les �l�ments
	m_TBLItems.Swap(TBLItems);

	// on lib�re le parseur
	delete pParser;
}

JVoid JFR03TBLSources::Export(JStream* pStream) const
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JFR03TBLSources::Swap(JFR03TBLSources & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JFR03TBLSources::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JFR03TBLSources::~JFR03TBLSources()
{
	// on ne fait rien
}
