//
// Fichier: JMTTBLVolContacts.cpp
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTTBLVolContacts.h"

////////////////////
// les constructeurs

JMTTBLVolContacts::JMTTBLVolContacts()
{
	// on ne fait rien
}

JMTTBLVolContacts::JMTTBLVolContacts(const JMTTBLVolContacts & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTTBLVolContacts & JMTTBLVolContacts::operator =(const JMTTBLVolContacts & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTTBLVolContacts::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////////////
// la fonction pour d�finir le nombre d'�l�ments

JVoid JMTTBLVolContacts::SetCount(JInt32 NbIndividus)
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

JVoid JMTTBLVolContacts::AddItems(const JMTFR03DATReprise & DATReprise, JInt32 NbInsertions)
{
	// on v�rifie la validit� du nombre d'insertions
	if (NbInsertions < 1) throw JInvalidCall::GetInstance();

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

	// on v�rifie le nombre d'individus
	if (DATReprise.m_ItemIndividus.GetCount() != NbIndividus) throw JInternalError::GetInstance();

	// on initialise l'indice et le diviseur de la reprise
	JInt32 IdxReprise = 0;
	JInt32 DivReprise = DATReprise.m_Diviseur.AsJInt32();

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on teste si l'individu a des reprises
		if (DATReprise.m_ItemIndividus.GetAt(IdxIndividu))
		{
			// on r�cup�re la valeur de la reprise
			JInt32 ValReprise = DATReprise.m_ItemReprises.Item(IdxReprise).AsJInt32();

			// on teste la valeur de la reprise
			if (ValReprise > 0)
			{
				// on incr�mente les contacts de l'individu
				m_TBLItems.Item(IdxIndividu) += ((JFlt64)ValReprise * (JFlt64)NbInsertions) / (JFlt64)DivReprise;
			}

			// on incr�mente l'indice de la reprise
			IdxReprise += 1;
		}
	}
}

JVoid JMTTBLVolContacts::AddItems(const JMTFR03DATVariable & DATVariable, JInt32 NbInsertions)
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
		// on teste si l'individu a des contacts
		if (DATVariable.m_ItemIndividus.GetAt(IdxIndividu))
		{
			// on r�cup�re la valeur de la variable
			JInt32 ValVariable = DATVariable.m_ItemVariables.Item(IdxVariable).AsJInt32();

			// on teste la valeur de la variable
			if (ValVariable > 0)
			{
				// on incr�mente les contacts de l'individu
				m_TBLItems.Item(IdxIndividu) += ((JFlt64)ValVariable * (JFlt64)NbInsertions) / (JFlt64)DivVariable;
			}

			// on incr�mente l'indice de la variable
			IdxVariable += 1;
		}
	}
}

JVoid JMTTBLVolContacts::AddItems(const JMTFR03DATLectorat & DATLectorat, JInt32 PtEtalonnage)
{
	// on v�rifie la validit� du point d'�talonnage
	if (PtEtalonnage < 1) throw JInvalidCall::GetInstance();

	// on r�cup�re le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

	// on v�rifie le nombre d'individus
	if (DATLectorat.m_ItemIndividus.GetCount() != NbIndividus) throw JInternalError::GetInstance();

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on teste si l'individu a des contacts
		if (DATLectorat.m_ItemIndividus.GetAt(IdxIndividu))
		{
			// on incr�mente les contacts de l'individu
			m_TBLItems.Item(IdxIndividu) += 1.0 / (JFlt64)PtEtalonnage;
		}
	}
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

const JFlt64 & JMTTBLVolContacts::Item(JInt32 IdxIndividu) const
{
	// on renvoie l'�l�ment
	return (m_TBLItems.Item(IdxIndividu));
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTTBLVolContacts::Swap(JMTTBLVolContacts & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTTBLVolContacts::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTTBLVolContacts::~JMTTBLVolContacts()
{
	// on ne fait rien
}
