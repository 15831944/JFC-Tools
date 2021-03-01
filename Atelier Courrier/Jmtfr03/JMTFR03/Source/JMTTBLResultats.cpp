//
// Fichier: JMTTBLResultats.cpp
// Auteur:  Sylvain SAMMURI
// Date:    30/06/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTTBLResultats.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03Exception.h"

////////////////////
// les constructeurs

JMTTBLResultats::JMTTBLResultats()
{
	// on ne fait rien
}

JMTTBLResultats::JMTTBLResultats(const JMTTBLResultats & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTTBLResultats & JMTTBLResultats::operator =(const JMTTBLResultats & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTTBLResultats::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLValues.GetCount());
}

////////////////////////////////////////
// la fonction pour d�finir les �l�ments

JVoid JMTTBLResultats::SetItems(const JBitArray & ItemIndividus, const JMTFR03DATPoids & DATPoids, const JMTFR03DATEchantillon & DATEchantillon, const JMTFR03DATHomothetie & DATHomothetie, const JMTFR03DATCible & DATCible, const JMTFR03TBLPonderations & TBLPonderations, const JMTTBLDistributions & TBLDistributions)
{
	// on r�cup�re le nombre d'individus et le nombre de classes
	JInt32 NbIndividus = ItemIndividus   .GetCount();
	JInt32 NbClasses   = TBLDistributions.GetColCount();

	// on v�rifie le nombre d'individus de la cible
	if (DATCible.m_TBLProbas.GetCount() != NbIndividus || TBLDistributions.GetRowCount() != NbIndividus) { JInt32 LOG_A_FAIRE; throw JMTFR03Exception::GetInstance(); }

	// on cr�e les nouveaux �l�ments
	JArray<JFlt64> TBLValues; TBLValues.SetCount(NbClasses);

	// on initialise les �l�ments
	for (JInt32 IdxClasse = 0; IdxClasse < NbClasses; IdxClasse += 1) TBLValues.Item(IdxClasse) = 0.0;

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
			// on boucle sur toutes les classes
			for (JInt32 JdxClasse = 0; JdxClasse < NbClasses; JdxClasse += 1)
			{
				// Ajout pour chaque classe
				JFlt64 ContribClasse = (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64() * TBLDistributions.Item(IdxIndividu, JdxClasse);

				// On pond�re la contribution indiv (suite � certaines pond�rations cibles)
				ContribClasse *= TBLPonderations.GetPonderations(IdxIndividu).AsJFlt64();

				// on incr�mente le r�sultat
				// TBLValues.Item(JdxClasse) += (JFlt64)DATPoids.m_ItemPoids.Item(IdxIndividu).AsJInt32() * DATCible.m_TBLProbas.Item(IdxIndividu).AsJFlt64() * TBLDistributions.Item(IdxIndividu, JdxClasse);
				TBLValues.Item(JdxClasse) += ContribClasse;
			}
		}
	}

	// on boucle sur toutes les classes
	for (JInt32 KdxClasse = 0; KdxClasse < NbClasses; KdxClasse += 1)
	{
		// on m�morise le r�sultat
		TBLValues.Item(KdxClasse) = (TBLValues.Item(KdxClasse) * (JFlt64)Homothetie) / ((JFlt64)DiviseurH * (JFlt64)DiviseurP);
	}

	// on permute les �l�ments
	m_TBLValues.Swap(TBLValues);
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

const JFlt64 & JMTTBLResultats::Item(JInt32 Index) const
{
	// on corrige l'index de l'�l�ment
	if (Index >= 0) Index -= 1;

	// on renvoie l'�l�ment
	return (m_TBLValues.Item(Index));
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTTBLResultats::Swap(JMTTBLResultats & Source)
{
	// on permute les �l�ments
	m_TBLValues.Swap(Source.m_TBLValues);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTTBLResultats::Reset()
{
	// on lib�re les �l�ments
	m_TBLValues.Reset();
}

/////////////////
// le destructeur

JMTTBLResultats::~JMTTBLResultats()
{
	// on ne fait rien
}
