//
// Fichier: JITPDATEchelle.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    29/08/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JITPDATEchelle.h"

////////////////////
// les constructeurs

JITPDATEchelle::JITPDATEchelle()
{
	m_Tarif			= -1.0;     // le tarif
	m_TarifManuel	= false;	// par défaut c'est un tarif source
	m_Periodicite	= -1;		// la périodicité
	m_AudienceRef	= -1.0;     // l'audience Ref
	m_AudienceProba = -1.0;     // l'audience Proba
	m_VolLecture	= -1.0;		// le volume de lecture dans le mode en cours
	m_Effectif		= -1.0;     // l'effectif
	m_IndAffinite	= -1.0;		// l'indice d'affinité
	m_PrcAffinite	= -1.0;		// le pourcentage d''affinité
	m_Apport		= -1.0;		// l'apport via plan courant
}

JITPDATEchelle::JITPDATEchelle(const JITPDATEchelle& Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JITPDATEchelle & JITPDATEchelle::operator =(const JITPDATEchelle & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JITPDATEchelle::~JITPDATEchelle()
{
	// on ne fait rien
}
