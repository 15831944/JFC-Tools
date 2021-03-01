//
// Fichier: JMTTBLDistributions.cpp
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on inclut les définitions nécessaires
#include "JMTTBLDistributions.h"

// on inclut les définitions nécessaires
#include "JTrusquin.h"

////////////////////
// les constructeurs

JMTTBLDistributions::JMTTBLDistributions()
{
	// on ne fait rien
}

JMTTBLDistributions::JMTTBLDistributions(const JMTTBLDistributions & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTTBLDistributions & JMTTBLDistributions::operator =(const JMTTBLDistributions & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////
// les fonctions pour récupérer le nombre d'éléments

JInt32 JMTTBLDistributions::GetRowCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetRowCount());
}

JInt32 JMTTBLDistributions::GetColCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetColCount());
}

////////////////////////////////////////////////
// la fonction pour définir le nombre d'éléments

JVoid JMTTBLDistributions::SetCount(JInt32 NbIndividus, JInt32 NbClasses)
{
	// on vérifie le nombre de classes
	if (NbClasses < 1) throw JInvalidCall::GetInstance();

	// on crée les nouveaux éléments
	JArray<JInt32>  TBLMax;   TBLMax  .SetCount(NbIndividus);
	JMatrix<JFlt64> TBLItems; TBLItems.SetCount(NbIndividus, NbClasses);

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on initialise toutes les classes suivantes
		for (JInt32 IdxClasse = 0; IdxClasse < NbClasses; IdxClasse += 1) TBLItems.Item(IdxIndividu, IdxClasse) = 0.0;

		// on initialise le maximum
		TBLMax.Item(IdxIndividu) = 0;
	}

	// on permute les éléments
	m_TBLItems.Swap(TBLItems);
	m_TBLMax  .Swap(TBLMax);
}

//////////////////////////////////////////
// la fonction pour accumuler les éléments

JVoid JMTTBLDistributions::AddItems(const JMTFR03DATVariable & DATVariable, JInt32 NbInsertions, JFlt64 CoefTrusquin)
{
	// on vérifie la validité du nombre d'insertions
	if (NbInsertions < 1) throw JInvalidCall::GetInstance();

	// on récupère le nombre d'individus et le nombre de classes
	JInt32 NbIndividus = m_TBLItems.GetRowCount();
	JInt32 NbClasses   = m_TBLItems.GetColCount(); if (NbClasses < 1) throw JInvalidCall::GetInstance();

	// on vérifie le nombre d'individus
	if (DATVariable.m_ItemIndividus.GetCount() != NbIndividus) throw JInternalError::GetInstance();

	// on initialise l'indice et le diviseur de la variable
	JInt32 IdxVariable = 0;
	JInt32 DivVariable = DATVariable.m_Diviseur.AsJInt32();

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on teste si l'individu a de l'audience
		if (DATVariable.m_ItemIndividus.GetAt(IdxIndividu))
		{
			// on récupère la valeur de la variable
			JInt32 ValVariable = DATVariable.m_ItemVariables.Item(IdxVariable).AsJInt32();

			// on teste la valeur de la variable
			if (ValVariable > 0)
			{
				// on crée le trusquin
				JTrusquin Trusquin;

				// on calcule le trusquin
				Trusquin.SetItems((JFlt64)ValVariable / (JFlt64)DivVariable, CoefTrusquin);

				// on récupère le nombre de probabilités du trusquin
				JInt32 NbProbabilites = Trusquin.GetCount();

				// on teste si on a quelque chose à faire
				if (NbProbabilites > 0)
				{
					// on récupère la dernière classe non nulle
					JInt32 & KMax = m_TBLMax.Item(IdxIndividu);

					// on incrémente la dernière classe
					KMax += NbInsertions * NbProbabilites; if (KMax > NbClasses) KMax = NbClasses;

					// on boucle sur toutes les classes 
					for (JInt32 IdxClasse = KMax - 1; IdxClasse >= 0; IdxClasse -= 1)
					{
						// on récupère la probabilité de la classe courante
						JFlt64 & ProbaK = m_TBLItems.Item(IdxIndividu, IdxClasse);

						// on calcule le premier terme de la probabilité
						ProbaK = (1.0 - Trusquin.Item(0)) * ProbaK;

						// on initialise l'indice de la probabilité et de la classe
						JInt32 JdxProba  = 0;
						JInt32 JdxClasse = IdxClasse - NbInsertions;

						// on boucle sur toutes les probabilités
						for (JInt32 IdxProbabilite = 0; (JdxClasse >= 0) && (IdxProbabilite < NbProbabilites); IdxProbabilite += 1)
						{
							// on récupère la probabilité de la classe courante
							JFlt64 & ProbaKM1 = m_TBLItems.Item(IdxIndividu, JdxClasse);

							// on teste si on a une probabilité supérieure à soustraire
							if ((NbProbabilites - IdxProbabilite) > 1)
							{
								// on incrémente la probabilité
								ProbaK += (Trusquin.Item(IdxProbabilite) - Trusquin.Item(IdxProbabilite + 1)) * ProbaKM1;
							}
							else
							{
								// on incrémente la probabilité
								ProbaK += Trusquin.Item(IdxProbabilite) * ProbaKM1;
							}

							// on décrémente les indices
							JdxClasse -= NbInsertions;
							JdxProba  += 1;
						}

						// on ajoute le dernier terme, s'il existe
						if (JdxProba < NbProbabilites) ProbaK += Trusquin.Item(JdxProba);

						// on corrige la probabilité
						if (ProbaK > 1.0) ProbaK = 1.0;
					}
				}
			}

			// on incrémente l'indice de la variable
			IdxVariable += 1;
		}
	}
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

const JFlt64 & JMTTBLDistributions::Item(JInt32 IdxIndividu, JInt32 IdxClasse) const
{
	// on renvoie les éléments
	return (m_TBLItems.Item(IdxIndividu, IdxClasse));
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTTBLDistributions::Swap(JMTTBLDistributions & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
	m_TBLMax  .Swap(Source.m_TBLMax);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTTBLDistributions::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
	m_TBLMax  .Reset();
}

/////////////////
// le destructeur

JMTTBLDistributions::~JMTTBLDistributions()
{
	// on ne fait rien
}
