//
// Fichier: JATPEchelle_Tri.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    04/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JATPECHELLE_TRI_H

// on définit la macro pour éviter les inclusions multiples
#define JATPECHELLE_TRI_H

// on inclut les définitions nécessaires
#include "JATPEchelleSupports.h"
#include "JITPTBLEchelle.h"

class JATPEchelle_Tri
{
public:
	// les fonctions pour trier les échelles par Supportx
	static JVoid SortBySupportsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortBySupportsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par Périodicité
	static JVoid SortByPeriodiciteDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByPeriodiciteUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par Tarifs
	static JVoid SortByTarifsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByTarifsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par GRP
	static JVoid SortByGRPDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByGRPUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par Couvertures
	static JVoid SortByCouverturesDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByCouverturesUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par Coût / GRP
	static JVoid SortByCoutsGRPDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByCoutsGRPUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par Répétition
	static JVoid SortByRepetitionsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByRepetitionsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par Nombre d'individus
	static JVoid SortByNbIndividusDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByNbIndividusUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par Nombre de contacts
	static JVoid SortByNbContactsDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByNbContactsUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par Indice Affinité
	static JVoid SortByIndAffiniteDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByIndAffiniteUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par Pourcentage Affinité
	static JVoid SortByPrcAffiniteDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByPrcAffiniteUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par LDP
	static JVoid SortByLDPDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByLDPUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par Reprise
	static JVoid SortByRepriseDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByRepriseUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

	// les fonctions pour trier les échelles par Apports
	static JVoid SortByApportDn(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);
	static JVoid SortByApportUp(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports);

private:
	// on définit les éléments à trier
	class JItem
	{
	public:
//		const JFR05Support*       m_pPSupport; // le Support des tarifs
		const JITPDATEchelle* m_pDATEchelle; // les grandeurs de l'échelle
	};

private:
	// la fonction pour trier tous les éléments
	static JVoid SortAllItems(const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports, JInt32 (*pCompare)(const JATPEchelleSupports &, const JArray<JItem> &, JInt32, JInt32), JVoid (*pExchange)(JATPEchelleSupports &, JArray<JItem> &, JInt32, JInt32), JInt32 Comp);

private:
	// la fonction pour trier les éléments
	static JVoid SortItems(JATPEchelleSupports & EchelleSupports, JArray<JItem> & Items, JInt32 (*pCompare)(const JATPEchelleSupports &, const JArray<JItem> &, JInt32, JInt32), JVoid (*pExchange)(JATPEchelleSupports &, JArray<JItem> &, JInt32, JInt32), JInt32 Comp);

	// la fonction pour ordonner les éléments
	static JVoid OrderItems(JATPEchelleSupports & EchelleSupports, JArray<JItem> & Items, JInt32 (*pCompare)(const JATPEchelleSupports &, const JArray<JItem> &, JInt32, JInt32), JVoid (*pExchange)(JATPEchelleSupports &, JArray<JItem> &, JInt32, JInt32), JInt32 Index, JInt32 Max, JInt32 Comp);

private:
	// les fonctions pour comparer les éléments
	static JInt32 CompareBySupports		(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByPeriodicite  (const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByTarifs		(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByGRP			(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByCouvertures	(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByCoutsGRP		(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByRepetitions	(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByNbIndividus	(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByNbContacts	(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByIndAffinite	(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByPrcAffinite	(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByLDP			(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByReprise		(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
	static JInt32 CompareByApport		(const JATPEchelleSupports & EchelleSupports, const JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);

private:
	// la fonction pour échanger les éléments
	static JVoid Exchange(JATPEchelleSupports & EchelleSupports, JArray<JItem> & Items, JInt32 IdxL, JInt32 IdxR);
};

// fin de l'inclusion conditionnelle
#endif
