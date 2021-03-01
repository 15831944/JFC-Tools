//
// Fichier: JMTFR03Options.cpp
// Auteur:  Sylvain SAMMURI
// Date:    14/01/2004
//

// on inclut les définitions nécessaires
#include "JMTFR03Options.h"

// on définit les identifiants Oui/Non des options
static const JRef32 g_IdOui = 1;
static const JRef32 g_IdNon = 2;

////////////////////
// les constructeurs

JMTFR03Options::JMTFR03Options()
{
	// on ne fait rien
}

JMTFR03Options::JMTFR03Options(const JMTFR03Options & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie les éléments
	m_LDPQuotidiens       = Source.m_LDPQuotidiens;
	m_LNMMagazines        = Source.m_LNMMagazines;
	m_LDP30JrsBimestriels = Source.m_LDP30JrsBimestriels;
	m_Reprises            = Source.m_Reprises;
}

/////////////////////////////////////////
// les fonctions pour tester les éléments

JBool JMTFR03Options::IsValid() const
{
	// on renvoie la validité des éléments
	return (m_LDPQuotidiens.IsValid() && m_LNMMagazines.IsValid() && m_LDP30JrsBimestriels.IsValid() && m_Reprises.IsValid());
}

////////////////////////////////////////////////////////////
// les fonctions pour privilégier la LDP pour les quotidiens

JVoid JMTFR03Options::SetLDPQuotidiens(JBool Etat)
{
	// on modifie le mode des quotidiens
	if (Etat) m_LDPQuotidiens = g_IdOui; else m_LDPQuotidiens = g_IdNon;
}

JBool JMTFR03Options::GetLDPQuotidiens() const
{
	// on vérifie la validité du mode des quotidiens
	if (!m_LDPQuotidiens.IsValid()) throw JInvalidCall::GetInstance();

	// on renvoie le mode des quotidiens
	return (m_LDPQuotidiens == g_IdOui);
}

///////////////////////////////////////////////////////////
// les fonctions pour privilégier la LNM pour les magazines

JVoid JMTFR03Options::SetLNMMagazines(JBool Etat)
{
	// on modifie le mode des magazines
	if (Etat) m_LNMMagazines = g_IdOui; else m_LNMMagazines = g_IdNon;
}

JBool JMTFR03Options::GetLNMMagazines() const
{
	// on vérifie la validité du mode des magazines
	if (!m_LNMMagazines.IsValid()) throw JInvalidCall::GetInstance();

	// on renvoie le mode des magazines
	return (m_LNMMagazines == g_IdOui);
}

/////////////////////////////////////////////////////////////////////
// les fonctions pour utiliser la LDP à 30 jours pour les bimestriels

JVoid JMTFR03Options::SetLDP30JrsBimestriels(JBool Etat)
{
	// on modifie le mode des bimestriels
	if (Etat) m_LDP30JrsBimestriels = g_IdOui; else m_LDP30JrsBimestriels = g_IdNon;
}

JBool JMTFR03Options::GetLDP30JrsBimestriels() const
{
	// on vérifie la validité du mode des bimestriels
	if (!m_LDP30JrsBimestriels.IsValid()) throw JInvalidCall::GetInstance();

	// on renvoie le mode des bimestriels
	return (m_LDP30JrsBimestriels == g_IdOui);
}

/////////////////////////////////////////////////////////////
// les fonctions pour prendre en compte les reprises en mains

JVoid JMTFR03Options::SetReprises(JBool Etat)
{
	// on modifie le mode des reprises en mains
	if (Etat) m_Reprises = g_IdOui; else m_Reprises = g_IdNon;
}

JBool JMTFR03Options::GetReprises() const
{
	// on vérifie la validité du mode des reprises en mains
	if (!m_Reprises.IsValid()) throw JInvalidCall::GetInstance();

	// on renvoie le mode des reprises en mains
	return (m_Reprises == g_IdOui);
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03Options & JMTFR03Options::operator =(const JMTFR03Options & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie les éléments
		m_LDPQuotidiens       = Source.m_LDPQuotidiens;
		m_LNMMagazines        = Source.m_LNMMagazines;
		m_LDP30JrsBimestriels = Source.m_LDP30JrsBimestriels;
		m_Reprises            = Source.m_Reprises;
	}

	// on renvoie notre référence
	return (*this);
}

//////////////////////////////////////////////
// la fonction pour réinitialiser les éléments

JVoid JMTFR03Options::Reset()
{
	// on réinitialise les éléments
	m_LDPQuotidiens      .Reset();
	m_LNMMagazines       .Reset();
	m_LDP30JrsBimestriels.Reset();
	m_Reprises           .Reset();
}

/////////////////
// le destructeur

JMTFR03Options::~JMTFR03Options()
{
	// on ne fait rien
}
