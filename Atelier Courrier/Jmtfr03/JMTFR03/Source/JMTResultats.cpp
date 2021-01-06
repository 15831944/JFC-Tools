//
// Fichier: JMTResultats.cpp
// Auteur:  Sylvain SAMMURI
// Date:    30/06/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTResultats.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03Exception.h"

////////////////////
// les constructeurs

JMTResultats::JMTResultats()
{
	// on initialise la valeur
	m_Value = 0.0;
}

JMTResultats::JMTResultats(const JMTResultats & Source)
{
	// on v�rifie la validit� de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie la valeur
	m_Value = Source.m_Value;
}

///////////////////////////////////////
// la fonction pour r�cup�rer l'�l�ment

JFlt64 JMTResultats::AsJFlt64() const
{
	// on renvoie la valeur
	return (m_Value);
}

///////////////////////////////////////
// les fonctions pour d�finir l'�l�ment

JVoid JMTResultats::SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations)
{
	// on initialise le r�sultat
	JFlt64 Resultat = 0.0;

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = ItemIndividus.GetCount();

	// on v�rifie le nombre d'individus de la cible
	if (DATCible.m_TBLProbas.GetCount() != NbIndividus) { JInt32 LOG_A_FAIRE; throw JMTFR03Exception::GetInstance(); }

	// on r�cup�re le facteur d'homoth�tie et le diviseur des poids
	JInt32 Homothetie = DATHomothetie.m_Homothetie.AsJInt32();
	JInt32 DiviseurH  = DATHomothetie.m_Diviseur  .AsJInt32();
	JInt32 DiviseurP  = DATPoids     .m_Diviseur  .AsJInt32();

	// on alloue l'item de l'effectif
	JBitArray ItemEffectif; ItemEffectif.SetCount(NbIndividus);

	// on initialise l'item de l'effectif
	ItemEffectif |= ItemIndividus;
	ItemEffectif &= DATEchantillon.m_ItemIndividus;

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on teste si on doit prendre l'individu
		if (ItemEffectif.GetAt(IdxIndividu))
		{
			// Ajout contribution r�sultat individu
			JFlt64 ContribIndiv = (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64();

			// On pond�re la contribution indiv (suite � certaines pond�rations cibles)
			ContribIndiv *= TBLPonderations.GetPonderations(IdxIndividu).AsJFlt64(); 

			// on incr�mente le r�sultat
			// Resultat += (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64();
			Resultat += ContribIndiv;
		}
	}

	// on m�morise le r�sultat
	m_Value = (Resultat * (JFlt64)Homothetie) / ((JFlt64)DiviseurH * (JFlt64)DiviseurP);
}

JVoid JMTResultats::SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations, const JMTTBLAudiences & TBLAudiences)
{
	// on initialise le r�sultat
	JFlt64 Resultat = 0.0;

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = ItemIndividus.GetCount();

	// on v�rifie le nombre d'individus de la cible
	if (DATCible.m_TBLProbas.GetCount() != NbIndividus) { JInt32 LOG_A_FAIRE; throw JMTFR03Exception::GetInstance(); }

	// on r�cup�re le facteur d'homoth�tie et le diviseur des poids
	JInt32 Homothetie = DATHomothetie.m_Homothetie.AsJInt32();
	JInt32 DiviseurH  = DATHomothetie.m_Diviseur  .AsJInt32();
	JInt32 DiviseurP  = DATPoids     .m_Diviseur  .AsJInt32();

	// on alloue l'item de l'audience
	JBitArray ItemAudience; ItemAudience.SetCount(NbIndividus);

	// on initialise l'item de l'effectif
	ItemAudience |= ItemIndividus;
	ItemAudience &= DATEchantillon.m_ItemIndividus;

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on teste si on doit prendre l'individu
		if (ItemAudience.GetAt(IdxIndividu))
		{
			// Ajout contribution r�sultat individu
			JFlt64 ContribIndiv = (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64() * TBLAudiences.Item(IdxIndividu);

			// On pond�re la contribution indiv (suite � certaines pond�rations cibles)
			ContribIndiv *= TBLPonderations.GetPonderations(IdxIndividu).AsJFlt64(); 

			// on incr�mente le r�sultat
			Resultat += ContribIndiv;
		}
	}

	// on m�morise le r�sultat
	m_Value = (Resultat * (JFlt64)Homothetie) / ((JFlt64)DiviseurH * (JFlt64)DiviseurP);
}

JVoid JMTResultats::SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations, const JMTTBLVolContacts & TBLVolContacts)
{
	// on initialise le r�sultat
	JFlt64 Resultat = 0.0;

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = ItemIndividus.GetCount();

	// on v�rifie le nombre d'individus de la cible
	if (DATCible.m_TBLProbas.GetCount() != NbIndividus) { JInt32 LOG_A_FAIRE; throw JMTFR03Exception::GetInstance(); }

	// on r�cup�re le facteur d'homoth�tie et le diviseur des poids
	JInt32 Homothetie = DATHomothetie.m_Homothetie.AsJInt32();
	JInt32 DiviseurH  = DATHomothetie.m_Diviseur  .AsJInt32();
	JInt32 DiviseurP  = DATPoids     .m_Diviseur  .AsJInt32();

	// on alloue l'item de l'audience
	JBitArray ItemAudience; ItemAudience.SetCount(NbIndividus);

	// on initialise l'item de l'effectif
	ItemAudience |= ItemIndividus;
	ItemAudience &= DATEchantillon.m_ItemIndividus;

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on teste si on doit prendre l'individu
		if (ItemAudience.GetAt(IdxIndividu))
		{
			// Ajout contribution r�sultat individu
			JFlt64 ContribIndiv = (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64() * TBLVolContacts.Item(IdxIndividu);

			// On pond�re la contribution indiv (suite � certaines pond�rations cibles)
			ContribIndiv *= TBLPonderations.GetPonderations(IdxIndividu).AsJFlt64(); 

			// on incr�mente le r�sultat
			Resultat += ContribIndiv;
		}
	}

	// on m�morise le r�sultat
	m_Value = (Resultat * (JFlt64)Homothetie) / ((JFlt64)DiviseurH * (JFlt64)DiviseurP);
}

JVoid JMTResultats::SetItem(const JBitArray & ItemIndividus, const JMTFR03KEYPoids & KEYPoids, const JMTFR03DATPoids & DATPoids, const JMTFR03TBLEchantillons & TBLEchantillons, const JMTFR03TBLHomotheties & TBLHomotheties, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations)
{
	// on initialise le r�sultat
	JFlt64 Resultat = 0.0;

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = ItemIndividus.GetCount();

	// on v�rifie le nombre d'individus de la cible
	if (DATCible.m_TBLProbas.GetCount() != NbIndividus) { JInt32 LOG_A_FAIRE; throw JMTFR03Exception::GetInstance(); }

	// on initialise le nombre d'�chantillons
	JInt32 NbEchantillons = 0;

	// on alloue l'item de l'effectif
	JBitArray ItemEffectif; ItemEffectif.SetCount(NbIndividus);

	// on boucle sur tous les �chantillons
	for (TBLEchantillons.MoveFirst(); TBLEchantillons.IsValid(); TBLEchantillons.MoveNext())
	{
		// on r�cup�re l'�chantillon courant
		const JMTFR03KEYEchantillon & KEYEchantillon = TBLEchantillons.GetKey();
		const JMTFR03DATEchantillon & DATEchantillon = TBLEchantillons.GetItem();

		// on cr�e la cl� du facteur d'homoth�tie
		JMTFR03KEYHomothetie KEYHomothetie;

		// on initialise la cl� du facteur d'homoth�tie
		KEYHomothetie.m_IdEchantillon = KEYEchantillon.m_IdEchantillon;
		KEYHomothetie.m_IdUnivers     = KEYPoids      .m_IdUnivers;

		// on recherche le facteur d'homoth�tie
		TBLHomotheties.MoveTo(KEYHomothetie); if (!TBLHomotheties.IsValid()) { JInt32 LOG_A_FAIRE; throw JMTFR03Exception::GetInstance(); }

		// on r�cup�re le facteur d'homoth�tie
		const JMTFR03DATHomothetie & DATHomothetie = TBLHomotheties.GetItem();

		// on r�cup�re le facteur d'homoth�tie et le diviseur des poids
		JInt32 Homothetie = DATHomothetie.m_Homothetie.AsJInt32();
		JInt32 DiviseurH  = DATHomothetie.m_Diviseur  .AsJInt32();
		JInt32 DiviseurP  = DATPoids     .m_Diviseur  .AsJInt32();

		// on initialise l'item de l'effectif
		ItemEffectif |= ItemIndividus;
		ItemEffectif &= DATEchantillon.m_ItemIndividus;

		// on initialise l'effectif
		JFlt64 Effectif = 0.0;

		// on boucle sur tous les individus
		for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
		{
			// on teste si on doit prendre l'individu
			if (ItemEffectif.GetAt(IdxIndividu))
			{
				// Ajout contribution r�sultat individu
				JFlt64 ContribIndiv = (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64();

				// On pond�re la contribution indiv (suite � certaines pond�rations cibles)
				ContribIndiv *= TBLPonderations.GetPonderations(IdxIndividu).AsJFlt64();

				// on calcule le poids de l'individu
				Effectif += ContribIndiv;
			}
		}

		// on met � jour l'effectif du r�sultat
		Resultat += (Effectif * (JFlt64)Homothetie) / ((JFlt64)DiviseurH * (JFlt64)DiviseurP);

		// on r�initialise l'item de l'effectif
		ItemEffectif &= false;

		// on incr�mente le nombre d'�chantillons
		NbEchantillons += 1;
	}

	// on teste le nombre d'�chantillons
	if (NbEchantillons > 1)
	{
		// on calcule les r�sultats moyens par �chantillon
		Resultat /= (JFlt64)NbEchantillons;
	}

	// on m�morise le r�sultat
	m_Value = Resultat;
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTResultats & JMTResultats::operator =(const JMTResultats & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie la valeur
		m_Value = Source.m_Value;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JMTResultats::Reset()
{
	// on r�initialise la valeur
	m_Value = 0.0;
}

/////////////////
// le destructeur

JMTResultats::~JMTResultats()
{
	// on ne fait rien
}
