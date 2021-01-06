// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JATPEchelle_Tri.h"

////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par supports
JVoid JATPEchelle_Tri::SortBySupportsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareBySupports, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortBySupportsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareBySupports, JATPEchelle_Tri::Exchange, -1);
}

////////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par P�riodicit�
JVoid JATPEchelle_Tri::SortByPeriodiciteDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByPeriodicite, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByPeriodiciteUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByPeriodicite, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////
// les fonctions pour trier les �chelles par Tarifs
JVoid JATPEchelle_Tri::SortByTarifsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByTarifs, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByTarifsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByTarifs, JATPEchelle_Tri::Exchange, -1);
}

////////////////////////////////////////////////
// les fonctions pour trier les �chelles par GRP
JVoid JATPEchelle_Tri::SortByGRPDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByGRP, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByGRPUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByGRP, JATPEchelle_Tri::Exchange, -1);
}

////////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par Couvertures
JVoid JATPEchelle_Tri::SortByCouverturesDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByCouvertures, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByCouverturesUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByCouvertures, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par Co�t / GRP
JVoid JATPEchelle_Tri::SortByCoutsGRPDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByCoutsGRP, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByCoutsGRPUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByCoutsGRP, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par R�p�tition
JVoid JATPEchelle_Tri::SortByRepetitionsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByRepetitions, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByRepetitionsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByRepetitions, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par Nombre d'individus
JVoid JATPEchelle_Tri::SortByNbIndividusDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByNbIndividus, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByNbIndividusUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByNbIndividus, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par Nombre de contacts
JVoid JATPEchelle_Tri::SortByNbContactsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByNbContacts, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByNbContactsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByNbContacts, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par Indice Affinit�
JVoid JATPEchelle_Tri::SortByIndAffiniteDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByIndAffinite, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByIndAffiniteUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByIndAffinite, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par Pourcentage Affinit�
JVoid JATPEchelle_Tri::SortByPrcAffiniteDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByPrcAffinite, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByPrcAffiniteUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByPrcAffinite, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par LDP
JVoid JATPEchelle_Tri::SortByLDPDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByLDP, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByLDPUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByLDP, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par Reprise
JVoid JATPEchelle_Tri::SortByRepriseDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByReprise, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByRepriseUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByReprise, JATPEchelle_Tri::Exchange, -1);
}

///////////////////////////////////////////////////////////////
// les fonctions pour trier les �chelles par Apport
JVoid JATPEchelle_Tri::SortByApportDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByApport, JATPEchelle_Tri::Exchange, +1);
}
JVoid JATPEchelle_Tri::SortByApportUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports)
{
	// on trie les �l�ments
	JATPEchelle_Tri::SortAllItems(ITREchelle, EchelleSupports, JATPEchelle_Tri::CompareByApport, JATPEchelle_Tri::Exchange, -1);
}

//////////////////////////////////////
// la fonction pour trier les �l�ments

JVoid JATPEchelle_Tri::SortAllItems(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports, JInt32 (*pCompare)(const JATPEchelleSupports &, const JArray<JItem> &, JInt32, JInt32), JVoid (*pExchange)(JATPEchelleSupports &, JArray<JItem> &, JInt32, JInt32), JInt32 Comp)
{
	// TODO :: on r�cup�re les tarifs
	// const JFR05Tarifs & PTarifs = JFR05Tarifs::GetInstance();

	// on boucle sur toutes les �chelles

	// on r�cup�re le nombre de supports
	JInt32 NbSupports = EchelleSupports.GetCount();

	// on alloue les �l�ments des supports
	JArray<JItem> Items; Items.SetCount(NbSupports);

	// on boucle sur tous les supports
	for (JInt32 IdxSupport = 0; IdxSupport < NbSupports; IdxSupport += 1)
	{
		// on r�cup�re l'�l�ment courant
		JItem & Item = Items.Item(IdxSupport);

		// on r�cup�re l'identifiant du Support
		const JUnt32 & IdSupport = EchelleSupports.Item(IdxSupport);

		// TODO :: on recherche le Support dans les tarifs
		// JInt32 PIdxSupport = PTarifs.m_TBLSupports.FindItem(IdSupport);

		// on teste si on a trouv� le Support
		if (1)//PIdxSupport >= 0)
		{
			// TODO :: on r�cup�re le Support
			// Item.m_pPSupport = &PTarifs.m_TBLSupports.Item(PIdxSupport);
		}
		else
		{
			// on invalide le Support
			//Item.m_pPSupport = 0;
		}

		// recherche les grandeurs du Support
		ITREchelle.MoveTo(IdSupport);

		// on teste si on a trouv� le Support
		if (ITREchelle.IsValid())
		{
			// on r�cup�re les grandeurs
			Item.m_pDATEchelle = &ITREchelle.GetItem();
		}
		else
		{
			// on invalide les grandeurs
			Item.m_pDATEchelle = 0;
		}
	}

	// on trie les �l�ments
	JATPEchelle_Tri::SortItems(EchelleSupports, Items, pCompare, pExchange, Comp);
}

//////////////////////////////////////
// la fonction pour trier les �l�ments

JVoid JATPEchelle_Tri::SortItems(JATPEchelleSupports & EchelleSupports, JArray<JItem> & Items, JInt32 (*pCompare)(const JATPEchelleSupports &, const JArray<JItem> &, JInt32, JInt32), JVoid (*pExchange)(JATPEchelleSupports &, JArray<JItem> &, JInt32, JInt32), JInt32 Comp)
{
	// on calcule l'indice du dernier �l�ment et l'indice de l'�l�ment m�dian
	JInt32 Max = EchelleSupports.GetCount() - 1;
	JInt32 Med = Max / 2;

	// on ordonne tous les �l�ments pour les transformer en tas
	for (JInt32 Idx = Med; Idx >= 0; Idx -= 1)
	{ 
		JATPEchelle_Tri::OrderItems(EchelleSupports, Items, pCompare, pExchange, Idx, Max, Comp);
	}

	// on extrait un par un tous les �l�ments du tas en le r�ordonnant � chaque fois
	for (JInt32 Jdx = Max; Jdx >= 1; Jdx -= 1)
	{ 
		pExchange(EchelleSupports, Items, Jdx, 0); JATPEchelle_Tri::OrderItems(EchelleSupports, Items, pCompare, pExchange, 0, Jdx - 1, Comp);
	}
}

/////////////////////////////////////////
// la fonction pour ordonner les �l�ments

JVoid JATPEchelle_Tri::OrderItems(JATPEchelleSupports & EchelleSupports, JArray<JItem> & Items, JInt32 (*pCompare)(const JATPEchelleSupports &, const JArray<JItem> &, JInt32, JInt32), JVoid (*pExchange)(JATPEchelleSupports &, JArray<JItem> &, JInt32, JInt32), JInt32 Index, JInt32 Max, JInt32 Comp)
{
	// on calcule l'indice de d�part et l'indice du fils gauche
	JInt32 Zdx = Index;
	JInt32 Ldx = (Zdx * 2) + 1;

	// on descend l'�l�ment jusqu'au bas de l'arbre
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

		// on permute les �l�ments courants
		pExchange(EchelleSupports, Items, Zdx, Kdx);

		// on met � jour les indices
		Ldx = (Kdx * 2) + 1;
		Zdx = (Kdx);
	}
}

///////////////////////////////////////////
// les fonctions pour comparer les �l�ments
JInt32 JATPEchelle_Tri::CompareBySupports(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// Partie � Virer, je la laisse en attendant les tests
	// Ne concerne que les supports existants (pas de gestion des titres apparies ou autres) 
	// CString Label1  = JSRCPresse::GetInstance().m_TBLTitres.GetLabelByID(IdL);
	// CString Label2  = JSRCPresse::GetInstance().m_TBLTitres.GetLabelByID(IdR);

	// Les 2 labels � comparer
	CString LabelLeft  = "";
	CString LabelRight = "";

	// R�cup infos regroupement
	CRegroupements * pReg = CRegroupements::GetInstance();
	IElem * pElem1= pReg->GetIElemByID(IdL);
	if(pElem1)
	{
		// on r�cup�re le libell� du Support Gauche
		LabelLeft = pElem1->GetLabel().AsJCharPtr();
	}

	IElem * pElem2= pReg->GetIElemByID(IdR);
	if(pElem2)
	{
		// on r�cup�re le libell� du Support Droit
		LabelRight = pElem2->GetLabel().AsJCharPtr();
	}

	// on compare directement les libell�s et renvoi le code r�sultat comparaison
	/*
	if (Label1 < Label2) return (-1);
	if (Label1 > Label2) return (+1);
	*/

	// Ou alors on garde la m�thode pr�c�dente mais en renvoyant cette fois les valeurs attendues (-1, 0 ou +1)
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
// Comparaison sur les p�riodicit�s (ici on opte pour tri selon la dur�e p�riodicit�, et non pas par libell� p�riodicit�
JInt32 JATPEchelle_Tri::CompareByPeriodicite(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// Tri par bn jours p�riodicit�
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (+1);

		// on r�cup�re la p�riodicit� gauche et droite
		JInt32 PeriodiciteL = ItemL.m_pDATEchelle->m_Periodicite;
		JInt32 PeriodiciteR = ItemR.m_pDATEchelle->m_Periodicite;

		// on compare la p�riodicit� gauche et droite
		if (PeriodiciteL < PeriodiciteR) return (-1);
		if (PeriodiciteL > PeriodiciteR) return (+1);
		
		/* Ici au cas o� on veut un tri par libell� (� voir)
		// on r�cup�re la p�riodicit� gauche et droite
		JInt32 PeriodiciteL = ItemL.m_pDATEchelle->m_Periodicite;
		JInt32 PeriodiciteR = ItemR.m_pDATEchelle->m_Periodicite;

		// On r�cup�re les libell�s droite et gauche
		const JSRCPresse & SrcPresse = JSRCPresse::GetInstance();
		CString TextPeriodL = SrcPresse.m_TBLPeriodicites.GetLabelCourtByID(PeriodiciteL);
		CString TextPeriodR = SrcPresse.m_TBLPeriodicites.GetLabelCourtByID(PeriodiciteR);

		if (TextPeriodL < TextPeriodR) return (-1);
		if (TextPeriodL > TextPeriodR) return (+1);
		*/

	}
	else
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (-1);
	}

	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);
	
	// les �l�ments sont �gaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByTarifs(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (+1);

		// on r�cup�re les tarifs gauche et droite
		JFlt32 TarifsL = ItemL.m_pDATEchelle->m_Tarif;
		JFlt32 TarifsR = ItemR.m_pDATEchelle->m_Tarif;

		// on compare le tarif des �l�ments
		if (TarifsL < TarifsR) return (-1);
		if (TarifsL > TarifsR) return (+1);
	}
	else
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (-1);
	}

	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByGRP(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (+1);

		// on r�cup�re les effectifs et les nombres de contacts
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

			// on compare les �l�ments
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
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (-1);
	}
	
	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByCouvertures(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (+1);

		// on r�cup�re les couvertures � 1+ gauche et droite
		JFlt64 CouvL = ItemL.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 CouvR = ItemR.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;

		// on compare les couvertures
		if (CouvL < CouvR) return (-1);
		if (CouvL > CouvR) return (+1);
	}
	else
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (-1);
	}
	
	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByCoutsGRP(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on r�cup�re les tarifs, les nombres de contacts et les effectifs
		JFlt32 TarifL      = ItemL.m_pDATEchelle->m_Tarif;
		JFlt32 TarifR      = ItemR.m_pDATEchelle->m_Tarif;
		JFlt64 EffectifL   = ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 EffectifR   = ItemR.m_pDATEchelle->m_Effectif;
		JFlt64 NbContactsL = ItemL.m_pDATEchelle->m_AudienceRef;
		JFlt64 NbContactsR = ItemR.m_pDATEchelle->m_AudienceRef;

		// on teste si on peut calculer le Co�t / GRP gauche
		if (EffectifL > 0.0 && NbContactsL > 0.0)
		{
			// on teste si on peut calculer le Co�t / GRP droite
			if (EffectifR <= 0.0 || NbContactsR <= 0.0) return (-1);

			// on calcule les Co�ts / GRP gauche et droite
			JFlt64 CoutGRPL = (TarifL * EffectifL) / (NbContactsL * 100.0);
			JFlt64 CoutGRPR = (TarifR * EffectifR) / (NbContactsR * 100.0);

			// on compare les Co�ts / GRP
			if (CoutGRPL < CoutGRPR) return (-1);
			if (CoutGRPL > CoutGRPR) return (+1);
		}
		else
		{
			// on teste si on peut calculer le Co�t / GRP droite
			if (EffectifR > 0.0 && NbContactsR > 0.0) return (+1);
		}
	}
	else
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}
	
	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByRepetitions(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on r�cup�re les Nombres de contacts, les effectifs et les couvertures
		JFlt64 NbContactsL = ItemL.m_pDATEchelle->m_VolLecture;
		JFlt64 NbContactsR = ItemR.m_pDATEchelle->m_VolLecture;
		JFlt64 EffectifL   = ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 EffectifR   = ItemR.m_pDATEchelle->m_Effectif;
		JFlt64 CouvL       = ItemL.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 CouvR       = ItemR.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;

		// on teste si on peut calculer la R�p�tition gauche
		if (EffectifL > 0.0 && CouvL > 0.0)
		{
			// on teste si on peut calculer la R�p�tition droite
			if (EffectifR <= 0.0 || CouvR <= 0.0) return (+1);

			// on calcule les R�p�titions gauche et droite
			JFlt64 RepetitionL = NbContactsL / (EffectifL * CouvL);
			JFlt64 RepetitionR = NbContactsR / (EffectifR * CouvR);

			// on compare les R�p�titions
			if (RepetitionL < RepetitionR) return (-1);
			if (RepetitionL > RepetitionR) return (+1);
		}
		else
		{
			// on teste si on peut calculer la R�p�tition droite
			if (EffectifR > 0.0 && CouvR > 0.0) return (-1);
		}
	}
	else
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}
	
	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByNbIndividus(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on r�cup�re les couvertures � 1+ et les effectifs
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
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}
	
	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByNbContacts(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on compare le nombre de contacts des �l�ments
		if (ItemL.m_pDATEchelle->m_VolLecture < ItemR.m_pDATEchelle->m_VolLecture) return (-1);
		if (ItemL.m_pDATEchelle->m_VolLecture > ItemR.m_pDATEchelle->m_VolLecture) return (+1);
	}
	else
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}

	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByIndAffinite(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on compare le nombre de contacts des �l�ments
		if (ItemL.m_pDATEchelle->m_IndAffinite < ItemR.m_pDATEchelle->m_IndAffinite) return (-1);
		if (ItemL.m_pDATEchelle->m_IndAffinite > ItemR.m_pDATEchelle->m_IndAffinite) return (+1);
	}
	else
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}

	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByPrcAffinite(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on compare le nombre de contacts des �l�ments
		if (ItemL.m_pDATEchelle->m_PrcAffinite < ItemR.m_pDATEchelle->m_PrcAffinite) return (-1);
		if (ItemL.m_pDATEchelle->m_PrcAffinite > ItemR.m_pDATEchelle->m_PrcAffinite) return (+1);
	}
	else
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}

	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByLDP(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on compare le nombre de contacts des �l�ments
		if (ItemL.m_pDATEchelle->m_AudienceRef < ItemR.m_pDATEchelle->m_AudienceRef) return (-1);
		if (ItemL.m_pDATEchelle->m_AudienceRef > ItemR.m_pDATEchelle->m_AudienceRef) return (+1);
	}
	else
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}

	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

JInt32 JATPEchelle_Tri::CompareByReprise(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (-1);

		// on r�cup�re les Nombres de contacts, les effectifs et les couvertures
		JFlt64 NbContactsL = ItemL.m_pDATEchelle->m_VolLecture;
		JFlt64 NbContactsR = ItemR.m_pDATEchelle->m_VolLecture;
		JFlt64 EffectifL   = ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 EffectifR   = ItemR.m_pDATEchelle->m_Effectif;
		JFlt64 CouvL       = ItemL.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;
		JFlt64 CouvR       = ItemR.m_pDATEchelle->m_Distribution.Item(1).AsJFlt64() / ItemL.m_pDATEchelle->m_Effectif;

		// on teste si on peut calculer la R�p�tition gauche
		if (EffectifL > 0.0 && CouvL > 0.0)
		{
			// on teste si on peut calculer la R�p�tition droite
			if (EffectifR <= 0.0 || CouvR <= 0.0) return (+1);

			// on calcule les R�p�titions gauche et droite
			JFlt64 RepetitionL = NbContactsL / (EffectifL * CouvL);
			JFlt64 RepetitionR = NbContactsR / (EffectifR * CouvR);

			// on compare les R�p�titions
			if (RepetitionL < RepetitionR) return (-1);
			if (RepetitionL > RepetitionR) return (+1);
		}
		else
		{
			// on teste si on peut calculer la R�p�tition droite
			if (EffectifR > 0.0 && CouvR > 0.0) return (-1);
		}
	}
	else
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (+1);
	}
	
	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Comparaison des apports
JInt32 JATPEchelle_Tri::CompareByApport(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	const JItem & ItemL = Items.Item(IdxL);
	const JItem & ItemR = Items.Item(IdxR);

	// on v�rifie la validit� de l'�l�ment gauche
	if (ItemL.m_pDATEchelle != 0)
	{
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle == 0) return (+1);

		// Apports droite et gauche
		JFlt64 ApportL = - 1; 
		JFlt64 ApportR = - 1; 

		// on r�cup�re les apports gauche et droite
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
		// on v�rifie la validit� de l'�l�ment droite
		if (ItemR.m_pDATEchelle != 0) return (-1);
	}
	
	// on r�cup�re les identifiants
	const JUnt32 & IdL = EchelleSupports.Item(IdxL);
	const JUnt32 & IdR = EchelleSupports.Item(IdxR);

	// on compare les identifiants
	if (IdL < IdR) return (-1);
	if (IdL > IdR) return (+1);

	// les �l�ments sont �gaux
	return (0);
}

/////////////////////////////////////////
// la fonction pour �changer les �l�ments

JVoid JATPEchelle_Tri::Exchange(JATPEchelleSupports & EchelleSupports, JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR)
{
	// on r�cup�re les �l�ments
	JItem & ItemL = Items.Item(IdxL);
	JItem & ItemR = Items.Item(IdxR);

	// TODO : on permute les donn�es des �l�ments
//	const JFR05Support*       pPSupport = ItemL.m_pPSupport; ItemL.m_pPSupport = ItemR.m_pPSupport; ItemR.m_pPSupport = pPSupport;
	const JITPDATEchelle* pDATEchelle = ItemL.m_pDATEchelle; ItemL.m_pDATEchelle = ItemR.m_pDATEchelle; ItemR.m_pDATEchelle = pDATEchelle;

	// on permute les supports de l'�chelle
	EchelleSupports.SwapItems(IdxL, IdxR);
}
