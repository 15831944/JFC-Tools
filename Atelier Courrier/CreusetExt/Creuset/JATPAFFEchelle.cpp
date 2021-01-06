//
// Fichier: JATPAFFEchelle.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    04/09/2003
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JATPAFFEchelle.h"

////////////////////
// les constructeurs

JATPAFFEchelle::JATPAFFEchelle()
{
	// on initialise la grandeur de tri
	m_TriGrandeur = (JInt32)+1;
}

JATPAFFEchelle::JATPAFFEchelle(const JATPAFFEchelle & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JATPAFFEchelle & JATPAFFEchelle::operator =(const JATPAFFEchelle & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// les fonctions pour manipuler le tri par Libell�

JVoid JATPAFFEchelle::SetSortBySupports()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)+1)
		m_TriGrandeur = (JInt32)-1;
	else
		m_TriGrandeur = (JInt32)+1;
}

JBool JATPAFFEchelle::IsSortBySupportsUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+1);
}

JBool JATPAFFEchelle::IsSortBySupportsDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-1);
}

//////////////////////////////////////////////////////
// les fonctions pour manipuler le tri par P�riodicit�

JVoid JATPAFFEchelle::SetSortByPeriodicite()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-2) m_TriGrandeur = (JInt32)+2; else m_TriGrandeur = (JInt32)-2;
}

JBool JATPAFFEchelle::IsSortByPeriodiciteUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+2);
}

JBool JATPAFFEchelle::IsSortByPeriodiciteDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-2);
}

////////////////////////////////////////////////
// les fonctions pour manipuler le tri par Tarif

JVoid JATPAFFEchelle::SetSortByTarifs()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-3) m_TriGrandeur = (JInt32)+3; else m_TriGrandeur = (JInt32)-3;
}

JBool JATPAFFEchelle::IsSortByTarifsUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+3);
}

JBool JATPAFFEchelle::IsSortByTarifsDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-3);
}

//////////////////////////////////////////////
// les fonctions pour manipuler le tri par GRP

JVoid JATPAFFEchelle::SetSortByGRP()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-4) m_TriGrandeur = (JInt32)+4; else m_TriGrandeur = (JInt32)-4;
}

JBool JATPAFFEchelle::IsSortByGRPUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+4);
}

JBool JATPAFFEchelle::IsSortByGRPDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-4);
}

////////////////////////////////////////////////
// les fonctions pour manipuler le tri par C% 1+

JVoid JATPAFFEchelle::SetSortByCouvertures()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-5) m_TriGrandeur = (JInt32)+5; else m_TriGrandeur = (JInt32)-5;
}

JBool JATPAFFEchelle::IsSortByCouverturesUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+5);
}

JBool JATPAFFEchelle::IsSortByCouverturesDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-5);
}

/////////////////////////////////////////////////////
// les fonctions pour manipuler le tri par Co�t / GRP

JVoid JATPAFFEchelle::SetSortByCoutsGRP()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)+6) m_TriGrandeur = (JInt32)-6; else m_TriGrandeur = (JInt32)+6;
}

JBool JATPAFFEchelle::IsSortByCoutsGRPUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+6);
}

JBool JATPAFFEchelle::IsSortByCoutsGRPDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-6);
}

/////////////////////////////////////////////////////
// les fonctions pour manipuler le tri par R�p�tition

JVoid JATPAFFEchelle::SetSortByRepetitions()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-7) m_TriGrandeur = (JInt32)+7; else m_TriGrandeur = (JInt32)-7;
}

JBool JATPAFFEchelle::IsSortByRepetitionsUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+7);
}

JBool JATPAFFEchelle::IsSortByRepetitionsDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-7);
}

/////////////////////////////////////////////////////////////
// les fonctions pour manipuler le tri par Nombre d'individus

JVoid JATPAFFEchelle::SetSortByNbIndividus()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-8) m_TriGrandeur = (JInt32)+8; else m_TriGrandeur = (JInt32)-8;
}

JBool JATPAFFEchelle::IsSortByNbIndividusUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+8);
}

JBool JATPAFFEchelle::IsSortByNbIndividusDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-8);
}

/////////////////////////////////////////////////////////////
// les fonctions pour manipuler le tri par Nombre de contacts

JVoid JATPAFFEchelle::SetSortByNbContacts()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-9) m_TriGrandeur = (JInt32)+9; else m_TriGrandeur = (JInt32)-9;
}

JBool JATPAFFEchelle::IsSortByNbContactsUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+9);
}

JBool JATPAFFEchelle::IsSortByNbContactsDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-9);
}

/////////////////////////////////////////////////////////////
// les fonctions pour manipuler le tri par Indice Affinit�

JVoid JATPAFFEchelle::SetSortByIndAffinite()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-10) m_TriGrandeur = (JInt32)+10; else m_TriGrandeur = (JInt32)-10;
}

JBool JATPAFFEchelle::IsSortByIndAffiniteUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+10);
}

JBool JATPAFFEchelle::IsSortByIndAffiniteDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-10);
}

// les fonctions pour manipuler le tri par Pourcentage Affinit�

JVoid JATPAFFEchelle::SetSortByPrcAffinite()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-11) m_TriGrandeur = (JInt32)+11; else m_TriGrandeur = (JInt32)-11;
}

JBool JATPAFFEchelle::IsSortByPrcAffiniteUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+11);
}

JBool JATPAFFEchelle::IsSortByPrcAffiniteDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-11);
}
/////////////////////////////////////////////////////////////
// les fonctions pour manipuler le tri par LDP

JVoid JATPAFFEchelle::SetSortByLDP()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-12) m_TriGrandeur = (JInt32)+12; else m_TriGrandeur = (JInt32)-12;
}

JBool JATPAFFEchelle::IsSortByLDPUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+12);
}

JBool JATPAFFEchelle::IsSortByLDPDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-12);
}

/////////////////////////////////////////////////////////////
// les fonctions pour manipuler le tri par reprise

JVoid JATPAFFEchelle::SetSortByReprise()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-13) m_TriGrandeur = (JInt32)+13; else m_TriGrandeur = (JInt32)-13;
}

JBool JATPAFFEchelle::IsSortByRepriseUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+13);
}

JBool JATPAFFEchelle::IsSortByRepriseDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-13);
}

// les fonctions pour manipuler le tri par Apport
JVoid JATPAFFEchelle::SetSortByApport()
{
	// on s�lectionne le tri
	if (m_TriGrandeur == (JInt32)-14) m_TriGrandeur = (JInt32)+14; else m_TriGrandeur = (JInt32)-14;
}

JBool JATPAFFEchelle::IsSortByApportUp() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)+14);
}

JBool JATPAFFEchelle::IsSortByApportDn() const
{
	// on renvoie l'�tat du tri
	return (m_TriGrandeur == (JInt32)-14);
}

/////////////////////////////////////////////////////////////
// Serialize
JVoid JATPAFFEchelle::Send(JStream & Stream)
{
	Stream.Send(m_TriGrandeur);
}

JVoid JATPAFFEchelle::Recv(JStream & Stream)
{
	Stream.Recv(m_TriGrandeur);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JATPAFFEchelle::Swap(JATPAFFEchelle & Source)
{
	// on permute les �l�ments
	JInt32 TriGrandeur		= m_TriGrandeur;
	m_TriGrandeur			= Source.m_TriGrandeur;
	Source.m_TriGrandeur	= TriGrandeur;
}

/////////////////
// le destructeur

JATPAFFEchelle::~JATPAFFEchelle()
{
	// on ne fait rien
}
