//
// Fichier: JITPDATEchelle.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    29/08/2003
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JITPDATEchelle.h"

////////////////////
// les constructeurs

JITPDATEchelle::JITPDATEchelle()
{
	m_Tarif			= -1.0;     // le tarif
	m_TarifManuel	= false;	// par d�faut c'est un tarif source
	m_Periodicite	= -1;		// la p�riodicit�
	m_AudienceRef	= -1.0;     // l'audience Ref
	m_AudienceProba = -1.0;     // l'audience Proba
	m_VolLecture	= -1.0;		// le volume de lecture dans le mode en cours
	m_Effectif		= -1.0;     // l'effectif
	m_IndAffinite	= -1.0;		// l'indice d'affinit�
	m_PrcAffinite	= -1.0;		// le pourcentage d''affinit�
	m_Apport		= -1.0;		// l'apport via plan courant
}

JITPDATEchelle::JITPDATEchelle(const JITPDATEchelle& Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JITPDATEchelle & JITPDATEchelle::operator =(const JITPDATEchelle & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JITPDATEchelle::~JITPDATEchelle()
{
	// on ne fait rien
}
