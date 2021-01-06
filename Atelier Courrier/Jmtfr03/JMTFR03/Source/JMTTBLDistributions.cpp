//
// Fichier: JMTTBLDistributions.cpp
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTTBLDistributions.h"

// on inclut les d�finitions n�cessaires
#include "JTrusquin.h"

////////////////////
// les constructeurs

JMTTBLDistributions::JMTTBLDistributions()
{
	// on ne fait rien
}

JMTTBLDistributions::JMTTBLDistributions(const JMTTBLDistributions & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTTBLDistributions & JMTTBLDistributions::operator =(const JMTTBLDistributions & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer le nombre d'�l�ments

JInt32 JMTTBLDistributions::GetRowCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetRowCount());
}

JInt32 JMTTBLDistributions::GetColCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetColCount());
}

////////////////////////////////////////////////
// la fonction pour d�finir le nombre d'�l�ments

JVoid JMTTBLDistributions::SetCount(JInt32 NbIndividus, JInt32 NbClasses)
{
	// on v�rifie le nombre de classes
	if (NbClasses < 1) throw JInvalidCall::GetInstance();

	// on cr�e les nouveaux �l�ments
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

	// on permute les �l�ments
	m_TBLItems.Swap(TBLItems);
	m_TBLMax  .Swap(TBLMax);
}

//////////////////////////////////////////
// la fonction pour accumuler les �l�ments

JVoid JMTTBLDistributions::AddItems(const JMTFR03DATVariable & DATVariable, JInt32 NbInsertions, JFlt64 CoefTrusquin)
{
	// on v�rifie la validit� du nombre d'insertions
	if (NbInsertions < 1) throw JInvalidCall::GetInstance();

	// on r�cup�re le nombre d'individus et le nombre de classes
	JInt32 NbIndividus = m_TBLItems.GetRowCount();
	JInt32 NbClasses   = m_TBLItems.GetColCount(); if (NbClasses < 1) throw JInvalidCall::GetInstance();

	// on v�rifie le nombre d'individus
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
			// on r�cup�re la valeur de la variable
			JInt32 ValVariable = DATVariable.m_ItemVariables.Item(IdxVariable).AsJInt32();

			// on teste la valeur de la variable
			if (ValVariable > 0)
			{
				// on cr�e le trusquin
				JTrusquin Trusquin;

				// on calcule le trusquin
				Trusquin.SetItems((JFlt64)ValVariable / (JFlt64)DivVariable, CoefTrusquin);

				// on r�cup�re le nombre de probabilit�s du trusquin
				JInt32 NbProbabilites = Trusquin.GetCount();

				// on teste si on a quelque chose � faire
				if (NbProbabilites > 0)
				{
					// on r�cup�re la derni�re classe non nulle
					JInt32 & KMax = m_TBLMax.Item(IdxIndividu);

					// on incr�mente la derni�re classe
					KMax += NbInsertions * NbProbabilites; if (KMax > NbClasses) KMax = NbClasses;

					// on boucle sur toutes les classes 
					for (JInt32 IdxClasse = KMax - 1; IdxClasse >= 0; IdxClasse -= 1)
					{
						// on r�cup�re la probabilit� de la classe courante
						JFlt64 & ProbaK = m_TBLItems.Item(IdxIndividu, IdxClasse);

						// on calcule le premier terme de la probabilit�
						ProbaK = (1.0 - Trusquin.Item(0)) * ProbaK;

						// on initialise l'indice de la probabilit� et de la classe
						JInt32 JdxProba  = 0;
						JInt32 JdxClasse = IdxClasse - NbInsertions;

						// on boucle sur toutes les probabilit�s
						for (JInt32 IdxProbabilite = 0; (JdxClasse >= 0) && (IdxProbabilite < NbProbabilites); IdxProbabilite += 1)
						{
							// on r�cup�re la probabilit� de la classe courante
							JFlt64 & ProbaKM1 = m_TBLItems.Item(IdxIndividu, JdxClasse);

							// on teste si on a une probabilit� sup�rieure � soustraire
							if ((NbProbabilites - IdxProbabilite) > 1)
							{
								// on incr�mente la probabilit�
								ProbaK += (Trusquin.Item(IdxProbabilite) - Trusquin.Item(IdxProbabilite + 1)) * ProbaKM1;
							}
							else
							{
								// on incr�mente la probabilit�
								ProbaK += Trusquin.Item(IdxProbabilite) * ProbaKM1;
							}

							// on d�cr�mente les indices
							JdxClasse -= NbInsertions;
							JdxProba  += 1;
						}

						// on ajoute le dernier terme, s'il existe
						if (JdxProba < NbProbabilites) ProbaK += Trusquin.Item(JdxProba);

						// on corrige la probabilit�
						if (ProbaK > 1.0) ProbaK = 1.0;
					}
				}
			}

			// on incr�mente l'indice de la variable
			IdxVariable += 1;
		}
	}
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

const JFlt64 & JMTTBLDistributions::Item(JInt32 IdxIndividu, JInt32 IdxClasse) const
{
	// on renvoie les �l�ments
	return (m_TBLItems.Item(IdxIndividu, IdxClasse));
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTTBLDistributions::Swap(JMTTBLDistributions & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
	m_TBLMax  .Swap(Source.m_TBLMax);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTTBLDistributions::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
	m_TBLMax  .Reset();
}

/////////////////
// le destructeur

JMTTBLDistributions::~JMTTBLDistributions()
{
	// on ne fait rien
}
