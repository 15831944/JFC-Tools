//
// Fichier: JMTTBLAudiences.cpp
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTTBLAudiences.h"

////////////////////
// les constructeurs

JMTTBLAudiences::JMTTBLAudiences()
{
	// on ne fait rien
}

JMTTBLAudiences::JMTTBLAudiences(const JMTTBLAudiences & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTTBLAudiences & JMTTBLAudiences::operator =(const JMTTBLAudiences & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTTBLAudiences::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////////////
// la fonction pour d�finir le nombre d'�l�ments

JVoid JMTTBLAudiences::SetCount(JInt32 NbIndividus)
{
	// on cr�e les nouveaux �l�ments
	JArray<JFlt64> TBLItems; TBLItems.SetCount(NbIndividus);

	// on initialise les �l�ments
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1) TBLItems.Item(IdxIndividu) = 0.0;

	// on permute les �l�ments
	m_TBLItems.Swap(TBLItems);
}

////////////////////////////////////////////
// les fonctions pour accumuler les �l�ments

JVoid JMTTBLAudiences::AddItems(const JMTFR03DATLectorat & DATLectorat)
{
	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

	// on v�rifie le nombre d'individus
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
	// on v�rifie la validit� du nombre d'insertions
	if (NbInsertions < 1) throw JInvalidCall::GetInstance();

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

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
				// on r�cup�re l'audience de l'individu
				JFlt64 & Audience = m_TBLItems.Item(IdxIndividu);

				// on cumule l'audience de l'individu
				Audience = 1.0 - ((1.0 - Audience) * JMath::Pow(((JFlt64)DivVariable - (JFlt64)ValVariable) / (JFlt64)DivVariable, NbInsertions));
			}

			// on incr�mente l'indice de la variable
			IdxVariable += 1;
		}
	}
}

JVoid JMTTBLAudiences::AddItems(const JMTFR03DATVariable & DATVariable, JInt32 PtEtalonnage, JInt32 NbJours)
{
	// on v�rifie la validit� du nombre d'insertions
	if (PtEtalonnage < 1 || NbJours < 1) throw JInvalidCall::GetInstance();

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

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
				// on r�cup�re l'audience de l'individu
				JFlt64 & Audience = m_TBLItems.Item(IdxIndividu);

				// on cumule l'audience de l'individu
				Audience = 1.0 - ((1.0 - Audience) * JMath::Pow(((JFlt64)DivVariable - (JFlt64)ValVariable) / (JFlt64)DivVariable, (JFlt64)NbJours / (JFlt64)PtEtalonnage));
			}

			// on incr�mente l'indice de la variable
			IdxVariable += 1;
		}
	}
}

JVoid JMTTBLAudiences::AddItems(const JMTTBLVolContacts & TBLVolContacts)
{
	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

	// on v�rifie le nombre d'individus
	if (TBLVolContacts.GetCount() != NbIndividus) throw JInternalError::GetInstance();

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on r�cup�re l'audience de l'individu
		JFlt64 & Audience = m_TBLItems.Item(IdxIndividu);

		// on r�cup�re le volume de contacts de l'individu
		JFlt64 VolContacts = TBLVolContacts.Item(IdxIndividu); if (VolContacts >= 1.0) VolContacts = 1.0;

		// on cumule l'audience de l'individu
		Audience = 1.0 - ((1.0 - Audience) * (1.0 - VolContacts));
	}
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

const JFlt64 & JMTTBLAudiences::Item(JInt32 IdxIndividu) const
{
	// on renvoie l'�l�ment
	return (m_TBLItems.Item(IdxIndividu));
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTTBLAudiences::Swap(JMTTBLAudiences & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTTBLAudiences::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTTBLAudiences::~JMTTBLAudiences()
{
	// on ne fait rien
}
