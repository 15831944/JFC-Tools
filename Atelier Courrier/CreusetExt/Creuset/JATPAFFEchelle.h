//
// Fichier: JATPAFFEchelle.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    04/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JATP_AFF_ECHELLE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JATP_AFF_ECHELLE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JATPAFFEchelle
{
public:
	// le constructeur
	JATPAFFEchelle();

	// les fonctions pour manipuler le tri par Libell�
	JVoid SetSortBySupports();
	JBool IsSortBySupportsUp() const;
	JBool IsSortBySupportsDn() const;

	// les fonctions pour manipuler le tri par P�riodicit�
	JVoid SetSortByPeriodicite();
	JBool IsSortByPeriodiciteUp() const;
	JBool IsSortByPeriodiciteDn() const;

	// les fonctions pour manipuler le tri par Tarif
	JVoid SetSortByTarifs();
	JBool IsSortByTarifsUp() const;
	JBool IsSortByTarifsDn() const;

	// les fonctions pour manipuler le tri par GRP
	JVoid SetSortByGRP();
	JBool IsSortByGRPUp() const;
	JBool IsSortByGRPDn() const;

	// les fonctions pour manipuler le tri par C% 1+
	JVoid SetSortByCouvertures();
	JBool IsSortByCouverturesUp() const;
	JBool IsSortByCouverturesDn() const;

	// les fonctions pour manipuler le tri par Co�t / GRP
	JVoid SetSortByCoutsGRP();
	JBool IsSortByCoutsGRPUp() const;
	JBool IsSortByCoutsGRPDn() const;

	// les fonctions pour manipuler le tri par R�p�tition
	JVoid SetSortByRepetitions();
	JBool IsSortByRepetitionsUp() const;
	JBool IsSortByRepetitionsDn() const;

	// les fonctions pour manipuler le tri par Nombre d'individus
	JVoid SetSortByNbIndividus();
	JBool IsSortByNbIndividusUp() const;
	JBool IsSortByNbIndividusDn() const;

	// les fonctions pour manipuler le tri par Nombre de contacts
	JVoid SetSortByNbContacts();
	JBool IsSortByNbContactsUp() const;
	JBool IsSortByNbContactsDn() const;

	// les fonctions pour manipuler le tri par Indice Affinit�
	JVoid SetSortByIndAffinite();
	JBool IsSortByIndAffiniteUp() const;
	JBool IsSortByIndAffiniteDn() const;

	// les fonctions pour manipuler le tri par Pourcentage Affinit�
	JVoid SetSortByPrcAffinite();
	JBool IsSortByPrcAffiniteUp() const;
	JBool IsSortByPrcAffiniteDn() const;

	// les fonctions pour manipuler le tri par LDP
	JVoid SetSortByLDP();
	JBool IsSortByLDPUp() const;
	JBool IsSortByLDPDn() const;

	// les fonctions pour manipuler le tri par Reprise
	JVoid SetSortByReprise();
	JBool IsSortByRepriseUp() const;
	JBool IsSortByRepriseDn() const;

	// les fonctions pour manipuler le tri par Apport
	JVoid SetSortByApport();
	JBool IsSortByApportUp() const;
	JBool IsSortByApportDn() const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JATPAFFEchelle & Source);
	JVoid	Send(JStream & Stream); 
	JVoid	Recv(JStream & Stream);

	// le destructeur
	~JATPAFFEchelle();

private:
	// l'op�rateur pour recopier l'�l�ment
	JATPAFFEchelle & operator =(const JATPAFFEchelle & Source);

	// le constructeur copie
	JATPAFFEchelle(const JATPAFFEchelle & Source);

private:
	JInt32 m_TriGrandeur; // la grandeur de tri
};

// fin de l'inclusion conditionnelle
#endif
