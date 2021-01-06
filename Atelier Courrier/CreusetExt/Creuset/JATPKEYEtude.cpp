//
// Fichier: JATPKEYEtude.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "resource.h"
#include "JATPKEYEtude.h"

////////////////////
// les constructeurs

JATPKEYEtude::JATPKEYEtude()
{
	m_EtudeName.LoadString(IDS_CR_NONAME);
	m_path = "";
	m_IdProduit	=	0;
}

JATPKEYEtude::JATPKEYEtude(const JATPKEYEtude & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JATPKEYEtude & JATPKEYEtude::operator =(const JATPKEYEtude & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////////
// les fonctions pour manipuler le nom de l'étude
CString JATPKEYEtude::GetLabel() const
{
	return m_EtudeName;
}

////////////////////////////////////////////////////////
// les fonctions pour manipuler le nom de l'étude
JVoid JATPKEYEtude::SetPath(CString path)
{
	m_path = path;
	int idx = path.ReverseFind(0x5C);//On recherche le  '\'
	int idx2 = path.Find(".atp", 0);//On recherche l'extension
	if(idx2==-1)
		idx2 = path.Find(".qp", 0);//On recherche l'extension
	if(idx2==-1)
		idx2 = path.GetLength();
	
	m_EtudeName = path.Mid(idx+1, idx2-idx-1);

}

CString JATPKEYEtude::GetPath() const
{
	return m_path;
}

////////////////////////////////////////////////////////
// les fonctions pour manipuler l'identifiant du produit

JVoid JATPKEYEtude::SetIdProduit(JUnt32 IdProduit)
{
	// on vérifie la validité de l'identifiant
	if (!IdProduit) throw JInvalidCall::GetInstance();

	// on vérifie que l'identifiant n'a pas déjà été défini
	if (m_IdProduit) { if (IdProduit != m_IdProduit) throw JInvalidCall::GetInstance(); }

	// on mémorise l'identifiant
	m_IdProduit = IdProduit;
}

JUnt32 JATPKEYEtude::GetIdProduit() const
{
	// on renvoie l'identifiant du produit
	return (m_IdProduit);
}

///////////////////////////////////////////////////////////
// les fonctions pour manipuler la date de début de l'étude

JVoid JATPKEYEtude::SetDateD(JDate DateD)
{
	// on vérifie la validité de la date de début
	if (!DateD.IsValid()) throw JInvalidCall::GetInstance();

	// on vérifie la cohérence des nouvelles dates
	if      (m_DateD.IsValid()) { if (DateD != m_DateD) throw JInvalidCall::GetInstance(); }
	else if (m_DateF.IsValid()) { if (DateD >  m_DateF) throw JInvalidCall::GetInstance(); }

	// on mémorise la date de début
	m_DateD = DateD;
}

JDate JATPKEYEtude::GetDateD() const
{
	// on renvoie la date de début
	return (m_DateD);
}

/////////////////////////////////////////////////////////
// les fonctions pour manipuler la date de fin de l'étude

JVoid JATPKEYEtude::SetDateF(JDate DateF)
{
	// on vérifie la validité de la date de fin
	if (!DateF.IsValid()) throw JInvalidCall::GetInstance();

	// on vérifie la cohérence des nouvelles dates
	if (m_DateD.IsValid()) { if (DateF < m_DateD) throw JInvalidCall::GetInstance(); }

	// on mémorise la date de fin
	m_DateF = DateF;
}

JDate JATPKEYEtude::GetDateF() const
{
	// on renvoie la date de fin
	return (m_DateF);
}

//////////////////////////////////////////
// les fonctions pour sérialiser l'élément

JVoid JATPKEYEtude::Send(JStream & Stream) const
{
	// on sérialise l'élément
	Stream.Send(m_IdProduit);
	m_DateD    .Send(Stream);
	m_DateF    .Send(Stream);
}

JVoid JATPKEYEtude::Recv(JStream & Stream)
{
	// on sérialise l'élément
	JUnt32 IdProduit; Stream.Recv(IdProduit);
	JDate  DateD;     DateD    .Recv(Stream);
	JDate  DateF;     DateF    .Recv(Stream);

	// on vérifie la cohérence des dates
	if (DateD.IsValid() && DateF.IsValid()) { if (DateF < DateD) throw JBadSchema::GetInstance(); }

	// on recopie l'élément
	m_IdProduit = IdProduit;
	m_DateD     = DateD;
	m_DateF     = DateF;
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JATPKEYEtude::Swap(JATPKEYEtude & Source)
{
	// on permute l'élément
	CString EtudeName	= m_EtudeName;
	m_EtudeName			= Source.m_EtudeName;
	Source.m_EtudeName	= EtudeName;

	CString Path		= m_path;
	m_path				= Source.m_path;
	Source.m_path		= Path;

	JUnt32 IdProduit	= m_IdProduit;
	m_IdProduit			= Source.m_IdProduit;
	Source.m_IdProduit	= IdProduit;

	JDate  DateD		= m_DateD;
	m_DateD				= Source.m_DateD;
	Source.m_DateD		= DateD;

	JDate  DateF		= m_DateF;
	m_DateF				= Source.m_DateF;
	Source.m_DateF		= DateF;
}

/////////////////
// le destructeur

JATPKEYEtude::~JATPKEYEtude()
{
	// on ne fait rien
}
