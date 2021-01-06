// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JATPEchelle_Tri.h"

////////////////////////////////////////////////////
// les fonctions pour trier les échelles par supports
JVoid JATPEchelle_Tri::SortBySupportsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareBySupports, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortBySupportsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareBySupports, JATPEchelle_Tri::Exchange, -1);
}

////////////////////////////////////////////////////////
// les fonctions pour trier les échelles par Périodicité
JVoid JATPEchelle_Tri::SortByPeriodiciteDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByPeriodicite, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByPeriodiciteUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByPeriodicite, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////
// les fonctions pour trier les échelles par Tarifs
JVoid JATPEchelle_Tri::SortByTarifsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByTarifs, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByTarifsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByTarifs, JATPEchelle_Tri::Exchange, -1);
}

////////////////////////////////////////////////
// les fonctions pour trier les échelles par GRP
JVoid JATPEchelle_Tri::SortByGRPDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByGRP, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByGRPUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByGRP, JATPEchelle_Tri::Exchange, -1);
}

////////////////////////////////////////////////////////
// les fonctions pour trier les échelles par Couvertures
JVoid JATPEchelle_Tri::SortByCouverturesDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByCouvertures, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByCouverturesUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByCouvertures, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////
// les fonctions pour trier les échelles par Coût / GRP
JVoid JATPEchelle_Tri::SortByCoutsGRPDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByCoutsGRP, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByCoutsGRPUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByCoutsGRP, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////
// les fonctions pour trier les échelles par Répétition
JVoid JATPEchelle_Tri::SortByRepetitionsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByRepetitions, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByRepetitionsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByRepetitions, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les échelles par Nombre d'individus
JVoid JATPEchelle_Tri::SortByNbIndividusDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByNbIndividus, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByNbIndividusUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByNbIndividus, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les échelles par Nombre de contacts
JVoid JATPEchelle_Tri::SortByNbContactsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByNbContacts, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByNbContactsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByNbContacts, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les échelles par Indice Affinité
JVoid JATPEchelle_Tri::SortByIndAffiniteDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByIndAffinite, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByIndAffiniteUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByIndAffinite, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les échelles par Pourcentage Affinité
JVoid JATPEchelle_Tri::SortByPrcAffiniteDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByPrcAffinite, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByPrcAffiniteUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByPrcAffinite, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les échelles par LDP
JVoid JATPEchelle_Tri::SortByLDPDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByLDP, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByLDPUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByLDP, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les échelles par Reprise
JVoid JATPEchelle_Tri::SortByRepriseDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByReprise, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByRepriseUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByReprise, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les échelles par Apport
JVoid JATPEchelle_Tri::SortByApportDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByApport, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByApportUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les éléments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByApport, JATPEchelle_Tri::Exchange, -1);
}

//////////////////////////////////////
// la fonction pour trier les éléments

JVoid JATPEchelle_Tri::SortAllItems(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports, JInt32 (*pCompare)(const JATPEchelleSupports &, const JArray<JItem> &, JInt32, JInt32), JVoid (*pExchange)(JATPEchelleSupports &, JArray<JItem> &, JInt32, JInt32), JInt32 Comp)
{
	// TODO :: on récupère les tarifs
	// const JFR05Tarifs & PTarifs = JFR05Tarifs::GetInstance();

	// on boucle sur toutes les échelles

	// on récupère le nombre de supports
	JInt32 NbSupports = EchelleSupports.GetCount();

	// on alloue les éléments des supports
	JArray<JItem> Items; Items.SetCount(NbSupports);

	// on boucle sur tous les supports
	for (JInt32 IdxSupport = 0; IdxSupport < NbSupports; IdxSupport += 1)
	{
		// on récupère l'élément courant
		JItem & Item = Items.Item(IdxSupport);

		// on récupère l'identifiant du Support
		const JUnt32 & IdSupport = EchelleSupports.Item(IdxSupport);

		// TODO :: on recherche le Support dans les tarifs
		// JInt32 PIdxSupport = PTarifs.m_TBLSupports.FindItem(IdSupport);

		// on teste si on a trouvé le Support
		if (1)//PIdxSupport >= 0)
		{
			// TODO :: on récupère le Support
			// Item.m_pPSupport = &PTarifs.m_TBLSupports.Item(PIdxSupport);
		}
		else
		{
			// on invalide le Support
			//Item.m_pPSupport = 0;
		}

		// recherche les grandeurs du Support
		ITREchelle.MoveTo(IdSupport);

		// on teste si on a trouvé le Support
		if (ITREchelle.IsValid())
		{
			// on récupère les grandeurs
			Item.m_pDATEchelle = &ITREchelle.GetItem();
		}
		else
		{
			// on invalide les grandeurs
			Item.m_pDATEchelle = 0;
		}
	}

	// on trie les éléments
	JATPEchelle_Tri::SortItems(EchelleSupports, Items, pCompare, pExchange, Comp);
}

//////////////////////////////////////
// la fonction pour trier les éléments

JVoid JATPEchelle_Tri::SortItems(JATPEchelleSupports & EchelleSupports, JArray<JItem> & Items, JInt32 (*pCompare)(const JATPEchelleSupports &, const JArray<JItem> &, JInt32, JInt32), JVoid (*pExchange)(JATPEchelleSupports &, JArray<JItem> &, JInt32, JInt32), JInt32 Comp)
{
	// on calcule l'indice du dernier élément et l'indice de l'élément médian
	JInt32 Max = EchelleSupports.GetCount() - 1;
	JInt32 Med = Max / 2;

	// on ordonne tous les éléments pour les transformer en tas
	for (JInt32 Idx = Med; Idx >= 0; Idx -= 1)
	{ 
		JATPEchelle_Tri::OrderItems(EchelleSupports, Items, pCompare, pExchange, Idx, Max, Comp);
	}

	// on extrait un par un tous les éléments du tas en le réordonnant à chaque fois
	for (JInt32 Jdx = Max; Jdx >= 1; Jdx -= 1)
	{ 
		pExchange(EchelleSupports, Items, Jdx, 0); JATPEchelle_Tri::OrderItems(EchelleSupports, Items, pCompare, pExchange, 0, Jdx - 1, Comp);
	}
}

/////////////////////////////////////////
// la fonction pour ordonner les éléments

JVoid JATPEchelle_Tri::OrderItems(JATPEchelleSupports & EchelleSupports, JArray<JItem> & Items, JInt32 (*pCompare)(const JATPEchelleSupports &, const JArray<JItem> &, JInt32, JInt32), JVoid (*pExchange)(JATPEchelleSupports &, JArray<JItem> &, JInt32, JInt32), JInt32 Index, JInt32 Max, JInt32 Comp)
{
	// on calcule l'indice de départ et l'indice du fils gauche
	JInt32 Zdx = Index;
	JInt32 Ldx = (Zdx * 2) + 1;

	// on descend l'élément jusqu'au bas de l'arbre
	while (Ldx <= Max)
	{
		// on calcule l'indice du fils droit
		JInt32 Rdx = Ldx + 1;

		// on prend soit le fils gauche, soit le fils droite
		JInt32 Kdx = Ldx; 
		if (Rdx <= Max) 
		{ 
			if (Comp == pCompare(EchelleSupports, Items, Ldx, Rdx)) 
				Kdx = Rdx; 
		}

		// on teste si on a fini l'ordonnancement
		if (Comp == pCompare(EchelleSupports, Items, Kdx, Zdx)) 
			return;

		// on permute les éléments courants
		pExchange(EchelleSupports, Items, Zdx, Kdx);

		// on met à jour les indices
		Ldx = (Kdx * 2) + 1;
		Zdx = (Kdx);
	}
}

///////////////////////////////////////////
// les fonctions pour comparer les éléments
JInt32 JATPEchelle_Tri::CompareBySupports(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// Partie à Virer, je la laisse en attendant les tests
	// Ne concerne que les supports existants (pas de gestion des titres apparies ou autres) 
	// CString Label1  = JSRCPresse::GetInstance().m_TBLTitres.GetLabelByID(IdL);
	// CString Label2  = JSRCPresse::GetInstance().m_TBLTitres.GetLabelByID(IdR);

	// Les 2 labels à comparer
	CString LabelLeft  = "";
	CString LabelRight = "";

	// Récup infos regroupement
	CRegroupements * pReg = CRegroupements::GetInstance();
	IElem * pElem1= pReg->GetIElemByID(IdL);
	if(pElem1)
	{
		// on récupére le libellé du Support Gauche
		LabelLeft = pElem1->GetLabel().AsJCharPtr();
	}

	IElem * pElem2= pReg->GetIElemByID(IdR);
	if(pElem2)
	{
		// on récupére le libellé du Support Droit
		LabelRight = pElem2->GetLabel().AsJCharPtr();
	}

	// on compare directement les libellés et renvoi le code résultat comparaison
	/*
	if (Label1 < Label2) return (-1);
	if (Label1 > Label2) return (+1);
	*/

	// Ou alors on garde la méthode précédente mais en renvoyant cette fois les valeurs attendues (-1, 0 ou +1)
	if(CCompareTitre::CompareTitre(LabelLeft, LabelRight) < 0)
		return (-1);
	else if (CCompareTitre::CompareTitre(LabelLeft, LabelRight) > 0)
		return (+1);
	return 0;

	// Pas valide, renvoi d'autres valeurs que +1, 0, -1 style -2, -8, +8 etc....
	/*
	return CCompareTitre::CompareTitre(
		JSRCPresse::GetInstance().m_TBLTitres.GetLabelByID(IdL),
		JSRCPresse::GetInstance().m_TBLTitres.GetLabelByID(IdR));
	*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Comparaison sur les périodicités (ici on opte pour tri selon la durée périodicité, et non pas par libellé périodicité
JInt32 JATPEchelle_Tri::CompareByPeriodicite(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// Tri par bn jours périodicité
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (+1);

		// on récupère la périodicité gauche et droite
		JInt32 PeriodiciteL = ItemL.m_pDATEchelle->m_Periodicite;
		JInt32 PeriodiciteR = ItemR.m_pDATEchelle->m_Periodicite;

		// on compare la périodicité gauche et droite
		if (PeriodiciteL < PeriodiciteR) return (-1);
		if (PeriodiciteL > PeriodiciteR) return (+1);
		
		/* Ici au cas où on veut un tri par libellé (à voir)
		// on récupère la périodicité gauche et droite
		JInt32 PeriodiciteL = ItemL.m_pDATEchelle->m_Periodicite;
		JInt32 PeriodiciteR = ItemR.m_pDATEchelle->m_Periodicite;

		// On récupère les libellés droite et gauche
		const JSRCPresse & SrcPresse = JSRCPresse::GetInstance();
		CString TextPeriodL = SrcPresse.m_TBLPeriodicites.GetLabelCourtByID(PeriodiciteL);
		CString TextPeriodR = SrcPresse.m_TBLPeriodicites.GetLabelCourtByID(PeriodiciteR);

		if (TextPeriodL < TextPeriodR) return (-1);
		if (TextPeriodL > TextPeriodR) return (+1);
		*/

	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (-1);
	}

	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);
	
	// les éléments sont égaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByTarifs(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (+1);

		// on récupère les tarifs gauche et droite
		JFlt32 TarifsL = ItemL.m_pDATEchelle->m_Tarif;
		JFlt32 TarifsR = ItemR.m_pDATEchelle->m_Tarif;

		// on compare le tarif des éléments
		if (TarifsL < TarifsR) return (-1);
		if (TarifsL > TarifsR) return (+1);
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (-1);
	}

	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByGRP(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (+1);

		// on récupère les effectifs et les nombres de contacts
		JFlt64 EffectifL   = ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 EffectifR   = ItemR.m_pDATEchelle->m_Effectif;
		JFlt64 NbContactsL = ItemL.m_pDATEchelle->m_VolLecture;
		JFlt64 NbContactsR = ItemR.m_pDATEchelle->m_VolLecture;

		// on teste si on peut calculer le GRP gauche
		if (EffectifL > 0.0)
		{
			// on teste si on peut calculer le GRP droite
			if (EffectifR <= 0.0) return (-1);

			// on calcule les GRP gauche et droite
			JFlt64 GRPL = (NbContactsL * 100.0) / EffectifL;
			JFlt64 GRPR = (NbContactsR * 100.0) / EffectifR;

			// on compare les éléments
			if (GRPL < GRPR) return (-1);
			if (GRPL > GRPR) return (+1);
		}
		else
		{
			// on teste si on peut calculer le GRP droite
			if (EffectifR > 0.0) return (+1);
		}
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (-1);
	}
	
	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByCouvertures(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (+1);

		// on récupère les couvertures à 1+ gauche et droite
		JFlt64 CouvL = ItemL.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 CouvR = ItemR.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;

		// on compare les couvertures
		if (CouvL < CouvR) return (-1);
		if (CouvL > CouvR) return (+1);
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (-1);
	}
	
	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByCoutsGRP(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on récupère les tarifs, les nombres de contacts et les effectifs
		JFlt32 TarifL      = ItemL.m_pDATEchelle->m_Tarif;
		JFlt32 TarifR      = ItemR.m_pDATEchelle->m_Tarif;
		JFlt64 EffectifL   = ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 EffectifR   = ItemR.m_pDATEchelle->m_Effectif;
		JFlt64 NbContactsL = ItemL.m_pDATEchelle->m_AudienceRef;
		JFlt64 NbContactsR = ItemR.m_pDATEchelle->m_AudienceRef;

		// on teste si on peut calculer le Coût / GRP gauche
		if (EffectifL > 0.0 && NbContactsL > 0.0)
		{
			// on teste si on peut calculer le Coût / GRP droite
			if (EffectifR <= 0.0 || NbContactsR <= 0.0) return (-1);

			// on calcule les Coûts / GRP gauche et droite
			JFlt64 CoutGRPL = (TarifL * EffectifL) / (NbContactsL * 100.0);
			JFlt64 CoutGRPR = (TarifR * EffectifR) / (NbContactsR * 100.0);

			// on compare les Coûts / GRP
			if (CoutGRPL < CoutGRPR) return (-1);
			if (CoutGRPL > CoutGRPR) return (+1);
		}
		else
		{
			// on teste si on peut calculer le Coût / GRP droite
			if (EffectifR > 0.0 && NbContactsR > 0.0) return (+1);
		}
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}
	
	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByRepetitions(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on récupère les Nombres de contacts, les effectifs et les couvertures
		JFlt64 NbContactsL = ItemL.m_pDATEchelle->m_VolLecture;
		JFlt64 NbContactsR = ItemR.m_pDATEchelle->m_VolLecture;
		JFlt64 EffectifL   = ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 EffectifR   = ItemR.m_pDATEchelle->m_Effectif;
		JFlt64 CouvL       = ItemL.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 CouvR       = ItemR.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;

		// on teste si on peut calculer la Répétition gauche
		if (EffectifL > 0.0 && CouvL > 0.0)
		{
			// on teste si on peut calculer la Répétition droite
			if (EffectifR <= 0.0 || CouvR <= 0.0) return (+1);

			// on calcule les Répétitions gauche et droite
			JFlt64 RepetitionL = NbContactsL / (EffectifL * CouvL);
			JFlt64 RepetitionR = NbContactsR / (EffectifR * CouvR);

			// on compare les Répétitions
			if (RepetitionL < RepetitionR) return (-1);
			if (RepetitionL > RepetitionR) return (+1);
		}
		else
		{
			// on teste si on peut calculer la Répétition droite
			if (EffectifR > 0.0 && CouvR > 0.0) return (-1);
		}
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}
	
	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByNbIndividus(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on récupère les couvertures à 1+ et les effectifs
		JFlt64 CouvL     = ItemL.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 CouvR     = ItemR.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 EffectifL = ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 EffectifR = ItemR.m_pDATEchelle->m_Effectif;

		// on calcule les nombre d'individus gauche et droite
		JFlt64 NbIndividusL = EffectifL * CouvL;
		JFlt64 NbIndividusR = EffectifR * CouvR;

		// on compare les Nombres d'individus
		if (NbIndividusL < NbIndividusR) return (-1);
		if (NbIndividusL > NbIndividusR) return (+1);
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}
	
	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByNbContacts(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on compare le nombre de contacts des éléments
		if (ItemL.m_pDATEchelle->m_VolLecture < ItemR.m_pDATEchelle->m_VolLecture) return (-1);
		if (ItemL.m_pDATEchelle->m_VolLecture > ItemR.m_pDATEchelle->m_VolLecture) return (+1);
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}

	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByIndAffinite(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on compare le nombre de contacts des éléments
		if (ItemL.m_pDATEchelle->m_IndAffinite < ItemR.m_pDATEchelle->m_IndAffinite) return (-1);
		if (ItemL.m_pDATEchelle->m_IndAffinite > ItemR.m_pDATEchelle->m_IndAffinite) return (+1);
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}

	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByPrcAffinite(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on compare le nombre de contacts des éléments
		if (ItemL.m_pDATEchelle->m_PrcAffinite < ItemR.m_pDATEchelle->m_PrcAffinite) return (-1);
		if (ItemL.m_pDATEchelle->m_PrcAffinite > ItemR.m_pDATEchelle->m_PrcAffinite) return (+1);
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}

	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByLDP(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on compare le nombre de contacts des éléments
		if (ItemL.m_pDATEchelle->m_AudienceRef < ItemR.m_pDATEchelle->m_AudienceRef) return (-1);
		if (ItemL.m_pDATEchelle->m_AudienceRef > ItemR.m_pDATEchelle->m_AudienceRef) return (+1);
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}

	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByReprise(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on récupère les Nombres de contacts, les effectifs et les couvertures
		JFlt64 NbContactsL = ItemL.m_pDATEchelle->m_VolLecture;
		JFlt64 NbContactsR = ItemR.m_pDATEchelle->m_VolLecture;
		JFlt64 EffectifL   = ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 EffectifR   = ItemR.m_pDATEchelle->m_Effectif;
		JFlt64 CouvL       = ItemL.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 CouvR       = ItemR.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;

		// on teste si on peut calculer la Répétition gauche
		if (EffectifL > 0.0 && CouvL > 0.0)
		{
			// on teste si on peut calculer la Répétition droite
			if (EffectifR <= 0.0 || CouvR <= 0.0) return (+1);

			// on calcule les Répétitions gauche et droite
			JFlt64 RepetitionL = NbContactsL / (EffectifL * CouvL);
			JFlt64 RepetitionR = NbContactsR / (EffectifR * CouvR);

			// on compare les Répétitions
			if (RepetitionL < RepetitionR) return (-1);
			if (RepetitionL > RepetitionR) return (+1);
		}
		else
		{
			// on teste si on peut calculer la Répétition droite
			if (EffectifR > 0.0 && CouvR > 0.0) return (-1);
		}
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}
	
	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Comparaison des apports
JInt32 JATPEchelle_Tri::CompareByApport(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on vérifie la validité de l'élément gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle == 0) return (+1);

		// Apports droite et gauche
		JFlt64 ApportL = - 1; 
		JFlt64 ApportR = - 1; 

		// on récupère les apports gauche et droite
		if (ItemL.m_pDATEchelle->m_Effectif > 0.0)
			ApportL = ItemL.m_pDATEchelle->m_Apport / ItemL.m_pDATEchelle->m_Effectif;

		if (ItemR.m_pDATEchelle->m_Effectif > 0.0)
			ApportR = ItemR.m_pDATEchelle->m_Apport / ItemL.m_pDATEchelle->m_Effectif;

		// on compare les couvertures
		if (ApportL < ApportR) return (-1);
		if (ApportL > ApportR) return (+1);
	}
	else
	{
		// on vérifie la validité de l'élément droite
		if (ItemR.m_pDATEchelle != 0) return (-1);
	}
	
	// on récupère les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les éléments sont égaux
	return (0);
}

/////////////////////////////////////////
// la fonction pour échanger les éléments

JVoid JATPEchelle_Tri::Exchange(JATPEchelleSupports & EchelleSupports, JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on récupère les éléments
	JItem & ItemL = Items.Item(IdxL);
	JItem & ItemR = Items.Item(IdxR);

	// TODO : on permute les données des éléments
//	const JFR05Support*       pPSupport = ItemL.m_pPSupport; ItemL.m_pPSupport = ItemR.m_pPSupport; ItemR.m_pPSupport = pPSupport;
	const JITPDATEchelle* pDATEchelle = ItemL.m_pDATEchelle; ItemL.m_pDATEchelle = ItemR.m_pDATEchelle; ItemR.m_pDATEchelle = pDATEchelle;

	// on permute les supports de l'échelle
	EchelleSupports.SwapItems(IdxL, IdxR);
}
