//
// Fichier: JMTResultats.cpp
// Auteur:  Sylvain SAMMURI
// Date:    30/06/2004
//

// on inclut les définitions nécessaires
#include "JMTResultats.h"

// on inclut les définitions nécessaires
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
	// on vérifie la validité de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie la valeur
	m_Value = Source.m_Value;
}

///////////////////////////////////////
// la fonction pour récupérer l'élément

JFlt64 JMTResultats::AsJFlt64() const
{
	// on renvoie la valeur
	return (m_Value);
}

///////////////////////////////////////
// les fonctions pour définir l'élément

JVoid JMTResultats::SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations)
{
	// on initialise le résultat
	JFlt64 Resultat = 0.0;

	// on récupère le nombre d'individus
	JInt32 NbIndividus = ItemIndividus.GetCount();

	// on vérifie le nombre d'individus de la cible
	if (DATCible.m_TBLProbas.GetCount() != NbIndividus) { JInt32 LOG_A_FAIRE; throw JMTFR03Exception::GetInstance(); }

	// on récupère le facteur d'homothétie et le diviseur des poids
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
			// Ajout contribution résultat individu
			JFlt64 ContribIndiv = (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64();

			// On pondére la contribution indiv (suite à certaines pondérations cibles)
			ContribIndiv *= TBLPonderations.GetPonderations(IdxIndividu).AsJFlt64(); 

			// on incrémente le résultat
			// Resultat += (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64();
			Resultat += ContribIndiv;
		}
	}

	// on mémorise le résultat
	m_Value = (Resultat * (JFlt64)Homothetie) / ((JFlt64)DiviseurH * (JFlt64)DiviseurP);
}

JVoid JMTResultats::SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations, const JMTTBLAudiences & TBLAudiences)
{
	// on initialise le résultat
	JFlt64 Resultat = 0.0;

	// on récupère le nombre d'individus
	JInt32 NbIndividus = ItemIndividus.GetCount();

	// on vérifie le nombre d'individus de la cible
	if (DATCible.m_TBLProbas.GetCount() != NbIndividus) { JInt32 LOG_A_FAIRE; throw JMTFR03Exception::GetInstance(); }

	// on récupère le facteur d'homothétie et le diviseur des poids
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
			// Ajout contribution résultat individu
			JFlt64 ContribIndiv = (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64() * TBLAudiences.Item(IdxIndividu);

			// On pondére la contribution indiv (suite à certaines pondérations cibles)
			ContribIndiv *= TBLPonderations.GetPonderations(IdxIndividu).AsJFlt64(); 

			// on incrémente le résultat
			Resultat += ContribIndiv;
		}
	}

	// on mémorise le résultat
	m_Value = (Resultat * (JFlt64)Homothetie) / ((JFlt64)DiviseurH * (JFlt64)DiviseurP);
}

JVoid JMTResultats::SetItem(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations, const JMTTBLVolContacts & TBLVolContacts)
{
	// on initialise le résultat
	JFlt64 Resultat = 0.0;

	// on récupère le nombre d'individus
	JInt32 NbIndividus = ItemIndividus.GetCount();

	// on vérifie le nombre d'individus de la cible
	if (DATCible.m_TBLProbas.GetCount() != NbIndividus) { JInt32 LOG_A_FAIRE; throw JMTFR03Exception::GetInstance(); }

	// on récupère le facteur d'homothétie et le diviseur des poids
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
			// Ajout contribution résultat individu
			JFlt64 ContribIndiv = (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64() * TBLVolContacts.Item(IdxIndividu);

			// On pondére la contribution indiv (suite à certaines pondérations cibles)
			ContribIndiv *= TBLPonderations.GetPonderations(IdxIndividu).AsJFlt64(); 

			// on incrémente le résultat
			Resultat += ContribIndiv;
		}
	}

	// on mémorise le résultat
	m_Value = (Resultat * (JFlt64)Homothetie) / ((JFlt64)DiviseurH * (JFlt64)DiviseurP);
}

JVoid JMTResultats::SetItem(const JBitArray & ItemIndividus, const JMTFR03KEYPoids & KEYPoids, const JMTFR03DATPoids & DATPoids, const JMTFR03TBLEchantillons & TBLEchantillons, const JMTFR03TBLHomotheties & TBLHomotheties, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations)
{
	// on initialise le résultat
	JFlt64 Resultat = 0.0;

	// on récupère le nombre d'individus
	JInt32 NbIndividus = ItemIndividus.GetCount();

	// on vérifie le nombre d'individus de la cible
	if (DATCible.m_TBLProbas.GetCount() != NbIndividus) { JInt32 LOG_A_FAIRE; throw JMTFR03Exception::GetInstance(); }

	// on initialise le nombre d'échantillons
	JInt32 NbEchantillons = 0;

	// on alloue l'item de l'effectif
	JBitArray ItemEffectif; ItemEffectif.SetCount(NbIndividus);

	// on boucle sur tous les échantillons
	for (TBLEchantillons.MoveFirst(); TBLEchantillons.IsValid(); TBLEchantillons.MoveNext())
	{
		// on récupère l'échantillon courant
		const JMTFR03KEYEchantillon & KEYEchantillon = TBLEchantillons.GetKey();
		const JMTFR03DATEchantillon & DATEchantillon = TBLEchantillons.GetItem();

		// on crée la clé du facteur d'homothétie
		JMTFR03KEYHomothetie KEYHomothetie;

		// on initialise la clé du facteur d'homothétie
		KEYHomothetie.m_IdEchantillon = KEYEchantillon.m_IdEchantillon;
		KEYHomothetie.m_IdUnivers     = KEYPoids      .m_IdUnivers;

		// on recherche le facteur d'homothétie
		TBLHomotheties.MoveTo(KEYHomothetie); if (!TBLHomotheties.IsValid()) { JInt32 LOG_A_FAIRE; throw JMTFR03Exception::GetInstance(); }

		// on récupère le facteur d'homothétie
		const JMTFR03DATHomothetie & DATHomothetie = TBLHomotheties.GetItem();

		// on récupère le facteur d'homothétie et le diviseur des poids
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
				// Ajout contribution résultat individu
				JFlt64 ContribIndiv = (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64();

				// On pondére la contribution indiv (suite à certaines pondérations cibles)
				ContribIndiv *= TBLPonderations.GetPonderations(IdxIndividu).AsJFlt64();

				// on calcule le poids de l'individu
				Effectif += ContribIndiv;
			}
		}

		// on met à jour l'effectif du résultat
		Resultat += (Effectif * (JFlt64)Homothetie) / ((JFlt64)DiviseurH * (JFlt64)DiviseurP);

		// on réinitialise l'item de l'effectif
		ItemEffectif &= false;

		// on incrémente le nombre d'échantillons
		NbEchantillons += 1;
	}

	// on teste le nombre d'échantillons
	if (NbEchantillons > 1)
	{
		// on calcule les résultats moyens par échantillon
		Resultat /= (JFlt64)NbEchantillons;
	}

	// on mémorise le résultat
	m_Value = Resultat;
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTResultats & JMTResultats::operator =(const JMTResultats & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie la valeur
		m_Value = Source.m_Value;
	}

	// on renvoie notre référence
	return (*this);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JMTResultats::Reset()
{
	// on réinitialise la valeur
	m_Value = 0.0;
}

/////////////////
// le destructeur

JMTResultats::~JMTResultats()
{
	// on ne fait rien
}
