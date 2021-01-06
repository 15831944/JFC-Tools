//
// Fichier: JMTTBLAudiences.cpp
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on inclut les définitions nécessaires
#include "JMTTBLAudiences.h"

////////////////////
// les constructeurs

JMTTBLAudiences::JMTTBLAudiences()
{
	// on ne fait rien
}

JMTTBLAudiences::JMTTBLAudiences(const JMTTBLAudiences & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTTBLAudiences & JMTTBLAudiences::operator =(const JMTTBLAudiences & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTTBLAudiences::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////////////
// la fonction pour définir le nombre d'éléments

JVoid JMTTBLAudiences::SetCount(JInt32 NbIndividus)
{
	// on crée les nouveaux éléments
	JArray<JFlt64> TBLItems; TBLItems.SetCount(NbIndividus);

	// on initialise les éléments
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1) TBLItems.Item(IdxIndividu) = 0.0;

	// on permute les éléments
	m_TBLItems.Swap(TBLItems);
}

////////////////////////////////////////////
// les fonctions pour accumuler les éléments

JVoid JMTTBLAudiences::AddItems(const JMTFR03DATLectorat & DATLectorat)
{
	// on récupère le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

	// on vérifie le nombre d'individus
	if (DATLectorat.m_ItemIndividus.GetCount() != NbIndividus) throw JInternalError::GetInstance();

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on teste si l'individu a de l'audience
		if (DATLectorat.m_ItemIndividus.GetAt(IdxIndividu))
		{
			// on cumule l'audience de l'individu
			m_TBLItems.Item(IdxIndividu) = 1.0;
		}
	}
}

JVoid JMTTBLAudiences::AddItems(const JMTFR03DATVariable & DATVariable, JInt32 NbInsertions)
{
	// on vérifie la validité du nombre d'insertions
	if (NbInsertions < 1) throw JInvalidCall::GetInstance();

	// on récupère le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

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
				// on récupère l'audience de l'individu
				JFlt64 & Audience = m_TBLItems.Item(IdxIndividu);

				// on cumule l'audience de l'individu
				Audience = 1.0 - ((1.0 - Audience) * JMath::Pow(((JFlt64)DivVariable - (JFlt64)ValVariable) / (JFlt64)DivVariable, NbInsertions));
			}

			// on incrémente l'indice de la variable
			IdxVariable += 1;
		}
	}
}

JVoid JMTTBLAudiences::AddItems(const JMTFR03DATVariable & DATVariable, JInt32 PtEtalonnage, JInt32 NbJours)
{
	// on vérifie la validité du nombre d'insertions
	if (PtEtalonnage < 1 || NbJours < 1) throw JInvalidCall::GetInstance();

	// on récupère le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

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
				// on récupère l'audience de l'individu
				JFlt64 & Audience = m_TBLItems.Item(IdxIndividu);

				// on cumule l'audience de l'individu
				Audience = 1.0 - ((1.0 - Audience) * JMath::Pow(((JFlt64)DivVariable - (JFlt64)ValVariable) / (JFlt64)DivVariable, (JFlt64)NbJours / (JFlt64)PtEtalonnage));
			}

			// on incrémente l'indice de la variable
			IdxVariable += 1;
		}
	}
}

JVoid JMTTBLAudiences::AddItems(const JMTTBLVolContacts & TBLVolContacts)
{
	// on récupère le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

	// on vérifie le nombre d'individus
	if (TBLVolContacts.GetCount() != NbIndividus) throw JInternalError::GetInstance();

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on récupère l'audience de l'individu
		JFlt64 & Audience = m_TBLItems.Item(IdxIndividu);

		// on récupère le volume de contacts de l'individu
		JFlt64 VolContacts = TBLVolContacts.Item(IdxIndividu); if (VolContacts >= 1.0) VolContacts = 1.0;

		// on cumule l'audience de l'individu
		Audience = 1.0 - ((1.0 - Audience) * (1.0 - VolContacts));
	}
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

const JFlt64 & JMTTBLAudiences::Item(JInt32 IdxIndividu) const
{
	// on renvoie l'élément
	return (m_TBLItems.Item(IdxIndividu));
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTTBLAudiences::Swap(JMTTBLAudiences & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTTBLAudiences::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTTBLAudiences::~JMTTBLAudiences()
{
	// on ne fait rien
}
