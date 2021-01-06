//
// Fichier: JMTTBLVolContacts.cpp
// Auteur:  Sylvain SAMMURI
// Date:    07/04/2004
//

// on inclut les définitions nécessaires
#include "JMTTBLVolContacts.h"

////////////////////
// les constructeurs

JMTTBLVolContacts::JMTTBLVolContacts()
{
	// on ne fait rien
}

JMTTBLVolContacts::JMTTBLVolContacts(const JMTTBLVolContacts & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTTBLVolContacts & JMTTBLVolContacts::operator =(const JMTTBLVolContacts & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTTBLVolContacts::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////////////
// la fonction pour définir le nombre d'éléments

JVoid JMTTBLVolContacts::SetCount(JInt32 NbIndividus)
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

JVoid JMTTBLVolContacts::AddItems(const JMTFR03DATReprise & DATReprise, JInt32 NbInsertions)
{
	// on vérifie la validité du nombre d'insertions
	if (NbInsertions < 1) throw JInvalidCall::GetInstance();

	// on récupère le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

	// on vérifie le nombre d'individus
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
			// on récupère la valeur de la reprise
			JInt32 ValReprise = DATReprise.m_ItemReprises.Item(IdxReprise).AsJInt32();

			// on teste la valeur de la reprise
			if (ValReprise > 0)
			{
				// on incrémente les contacts de l'individu
				m_TBLItems.Item(IdxIndividu) += ((JFlt64)ValReprise * (JFlt64)NbInsertions) / (JFlt64)DivReprise;
			}

			// on incrémente l'indice de la reprise
			IdxReprise += 1;
		}
	}
}

JVoid JMTTBLVolContacts::AddItems(const JMTFR03DATVariable & DATVariable, JInt32 NbInsertions)
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
		// on teste si l'individu a des contacts
		if (DATVariable.m_ItemIndividus.GetAt(IdxIndividu))
		{
			// on récupère la valeur de la variable
			JInt32 ValVariable = DATVariable.m_ItemVariables.Item(IdxVariable).AsJInt32();

			// on teste la valeur de la variable
			if (ValVariable > 0)
			{
				// on incrémente les contacts de l'individu
				m_TBLItems.Item(IdxIndividu) += ((JFlt64)ValVariable * (JFlt64)NbInsertions) / (JFlt64)DivVariable;
			}

			// on incrémente l'indice de la variable
			IdxVariable += 1;
		}
	}
}

JVoid JMTTBLVolContacts::AddItems(const JMTFR03DATLectorat & DATLectorat, JInt32 PtEtalonnage)
{
	// on vérifie la validité du point d'étalonnage
	if (PtEtalonnage < 1) throw JInvalidCall::GetInstance();

	// on récupère le nombre d'individus
	JInt32 NbIndividus = m_TBLItems.GetCount();

	// on vérifie le nombre d'individus
	if (DATLectorat.m_ItemIndividus.GetCount() != NbIndividus) throw JInternalError::GetInstance();

	// on boucle sur tous les individus
	for (JInt32 IdxIndividu = 0; IdxIndividu < NbIndividus; IdxIndividu += 1)
	{
		// on teste si l'individu a des contacts
		if (DATLectorat.m_ItemIndividus.GetAt(IdxIndividu))
		{
			// on incrémente les contacts de l'individu
			m_TBLItems.Item(IdxIndividu) += 1.0 / (JFlt64)PtEtalonnage;
		}
	}
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

const JFlt64 & JMTTBLVolContacts::Item(JInt32 IdxIndividu) const
{
	// on renvoie l'élément
	return (m_TBLItems.Item(IdxIndividu));
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTTBLVolContacts::Swap(JMTTBLVolContacts & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTTBLVolContacts::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTTBLVolContacts::~JMTTBLVolContacts()
{
	// on ne fait rien
}
