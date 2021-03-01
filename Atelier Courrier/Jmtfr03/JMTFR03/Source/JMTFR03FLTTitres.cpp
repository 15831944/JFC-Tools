//
// Fichier: JMTFR03FLTTitres.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/01/2004
//

// on inclut les définitions nécessaires
#include "JMTFR03FLTTitres.h"

////////////////////
// les constructeurs

JMTFR03FLTTitres::JMTFR03FLTTitres()
{
	// on ne fait rien
}

JMTFR03FLTTitres::JMTFR03FLTTitres(const JMTFR03FLTTitres & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03FLTTitres & JMTFR03FLTTitres::operator =(const JMTFR03FLTTitres & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////
// les fonctions pour ajouter les titres

JVoid JMTFR03FLTTitres::OnAddTitres(const JMTFR03TBLFiltres & TBLFiltres, JMap<JRef32, JBool> & TmpItems) const
{
	// on boucle sur tous les filtres d'audience
	for (TBLFiltres.MoveFirst(); TBLFiltres.IsValid(); TBLFiltres.MoveNext())
	{
		// on récupère le filtre d'audience courant
		const JMTFR03KEYFiltre & KEYFiltre = TBLFiltres.GetKey();
		const JMTFR03DATFiltre & DATFiltre = TBLFiltres.GetItem();

		// on ajoute le titre courant
		TmpItems.MoveTo(KEYFiltre.m_IdTitre); if (!TmpItems.IsValid()) TmpItems.Add(KEYFiltre.m_IdTitre) = true;
	}
}

JVoid JMTFR03FLTTitres::OnAddTitres(const JMTFR03TBLReprises & TBLReprises, JMap<JRef32, JBool> & TmpItems) const
{
	// on boucle sur tous les filtres d'audience
	for (TBLReprises.MoveFirst(); TBLReprises.IsValid(); TBLReprises.MoveNext())
	{
		// on récupère le filtre d'audience courant
		const JMTFR03KEYReprise & KEYReprise = TBLReprises.GetKey();
		const JMTFR03DATReprise & DATReprise = TBLReprises.GetItem();

		// on ajoute le titre courant
		TmpItems.MoveTo(KEYReprise.m_IdTitre); if (!TmpItems.IsValid()) TmpItems.Add(KEYReprise.m_IdTitre) = true;
	}
}

JVoid JMTFR03FLTTitres::OnAddTitres(const JMTFR03TBLVariables & TBLVariables, JMap<JRef32, JBool> & TmpItems) const
{
	// on boucle sur toutes les variables
	for (TBLVariables.MoveFirst(); TBLVariables.IsValid(); TBLVariables.MoveNext())
	{
		// on récupère la variable courante
		const JMTFR03KEYVariable & KEYVariable = TBLVariables.GetKey();
		const JMTFR03DATVariable & DATVariable = TBLVariables.GetItem();

		// on ajoute le titre courant
		TmpItems.MoveTo(KEYVariable.m_IdTitre); if (!TmpItems.IsValid()) TmpItems.Add(KEYVariable.m_IdTitre) = true;
	}
}

JVoid JMTFR03FLTTitres::OnAddTitres(const JMTFR03TBLLectorats & TBLLectorats, JMap<JRef32, JBool> & TmpItems) const
{
	// on boucle sur tous les lectorats
	for (TBLLectorats.MoveFirst(); TBLLectorats.IsValid(); TBLLectorats.MoveNext())
	{
		// on récupère la variable courante
		const JMTFR03KEYLectorat & KEYLectorat = TBLLectorats.GetKey();
		const JMTFR03DATLectorat & DATLectorat = TBLLectorats.GetItem();

		// on ajoute le titre courant
		TmpItems.MoveTo(KEYLectorat.m_IdTitre); if (!TmpItems.IsValid()) TmpItems.Add(KEYLectorat.m_IdTitre) = true;
	}
}

JVoid JMTFR03FLTTitres::OnAddTitres(const JMTFR03TBLAttributs & TBLAttributs, JMap<JRef32, JBool> & TmpItems) const
{
	// on boucle sur tous les attributs
	for (TBLAttributs.MoveFirst(); TBLAttributs.IsValid(); TBLAttributs.MoveNext())
	{
		// on récupère le filtre d'audience courant
		const JMTFR03KEYAttribut & KEYAttribut = TBLAttributs.GetKey();
		const JMTFR03DATAttribut & DATAttribut = TBLAttributs.GetItem();

		// on ajoute le titre courant
		TmpItems.MoveTo(KEYAttribut.m_IdTitre); if (!TmpItems.IsValid()) TmpItems.Add(KEYAttribut.m_IdTitre) = true;
	}
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03FLTTitres::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour définir les éléments

JVoid JMTFR03FLTTitres::SetItems(const JMTFR03KEYTerrain & KEYTerrain, const JMTFR03TBLTerrains & TBLTerrains)
{
	// on crée les éléments
	JArray<JItem> TBLItems;

	// on crée les éléments temporaires
	JMap<JRef32, JBool> TmpItems;

	// on recherche les éléments du terrain
	TBLTerrains.MoveTo(KEYTerrain); if (!TBLTerrains.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère les éléments du terrain
	const JMTFR03DATTerrain & DATTerrain = TBLTerrains.GetItem();

	// on ajoute les titres présents au moins une fois dans les tables
	this->OnAddTitres(DATTerrain.m_TBLFiltres,          TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLReprises,         TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesLDP,     TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesLNM,     TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLProbabilitesLDP,  TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLProbabilitesLNM,  TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_001, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_XX1, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_002, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_004, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_007, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_014, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_030, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_060, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_090, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_120, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_180, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_270, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLVariablesDDL_365, TmpItems);
	this->OnAddTitres(DATTerrain.m_TBLAttributs,        TmpItems);

	// on alloue les nouveaux éléments
	TBLItems.SetCount(TmpItems.GetCount());

	// on se place sur les premiers filtres d'audience
	JInt32 Idx = 0; TmpItems.MoveFirst();

	// on boucle sur tous les filtres d'audience
	while (TmpItems.IsValid())
	{
		// on recopie le filtre courant
		TBLItems.Item(Idx).m_IdTitre = TmpItems.GetKey();

		// on passe aux filtres suivants
		TmpItems.MoveNext(); Idx += 1;
	}

	// on permute les éléments
	m_TBLItems.Swap(TBLItems);
}

///////////////////////////////////////////
// la fonction pour rechercher les éléments

JInt32 JMTFR03FLTTitres::FindItem(JRef32 IdTitre) const
{
	// on vérifie la validité de l'identifiant
	if (!IdTitre.IsValid()) throw JInvalidCall::GetInstance();

	// on initialise l'indice
	JInt32 Idx = -1;

	// on initialise les bornes de recherche
	JInt32 Min = 0;
	JInt32 Max = m_TBLItems.GetCount() - 1;

	// on boucle tant qu'on a pas trouvé l'élément
	while (Idx < 0 && Min <= Max)
	{
		// on calcule l'indice médian
		JInt32 Med = ((Max - Min) / 2) + Min;

		// on récupère l'identifiant de l'élément
		const JRef32 & IdRef = m_TBLItems.Item(Med).m_IdTitre;

		// on compare les identifiants
		if      (IdRef < IdTitre) Min = Med + 1;
		else if (IdRef > IdTitre) Max = Med - 1;
		else                      Idx = Med;
	}

	// on renvoie l'indice
	return (Idx);
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

const JRef32 & JMTFR03FLTTitres::Item(JInt32 Index) const
{
	// on renvoie l'identifiant de l'élément
	return (m_TBLItems.Item(Index).m_IdTitre);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03FLTTitres::Swap(JMTFR03FLTTitres & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03FLTTitres::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03FLTTitres::~JMTFR03FLTTitres()
{
	// on ne fait rien
}
