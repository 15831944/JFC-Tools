//
// Fichier: JATPKEYEtude.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    20/08/2003
//

// on inclut les d�finitions n�cessaires
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
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JATPKEYEtude & JATPKEYEtude::operator =(const JATPKEYEtude & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////////
// les fonctions pour manipuler le nom de l'�tude
CString JATPKEYEtude::GetLabel() const
{
	return m_EtudeName;
}

////////////////////////////////////////////////////////
// les fonctions pour manipuler le nom de l'�tude
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
	// on v�rifie la validit� de l'identifiant
	if (!IdProduit) throw JInvalidCall::GetInstance();

	// on v�rifie que l'identifiant n'a pas d�j� �t� d�fini
	if (m_IdProduit) { if (IdProduit != m_IdProduit) throw JInvalidCall::GetInstance(); }

	// on m�morise l'identifiant
	m_IdProduit = IdProduit;
}

JUnt32 JATPKEYEtude::GetIdProduit() const
{
	// on renvoie l'identifiant du produit
	return (m_IdProduit);
}

///////////////////////////////////////////////////////////
// les fonctions pour manipuler la date de d�but de l'�tude

JVoid JATPKEYEtude::SetDateD(JDate DateD)
{
	// on v�rifie la validit� de la date de d�but
	if (!DateD.IsValid()) throw JInvalidCall::GetInstance();

	// on v�rifie la coh�rence des nouvelles dates
	if      (m_DateD.IsValid()) { if (DateD != m_DateD) throw JInvalidCall::GetInstance(); }
	else if (m_DateF.IsValid()) { if (DateD >  m_DateF) throw JInvalidCall::GetInstance(); }

	// on m�morise la date de d�but
	m_DateD = DateD;
}

JDate JATPKEYEtude::GetDateD() const
{
	// on renvoie la date de d�but
	return (m_DateD);
}

/////////////////////////////////////////////////////////
// les fonctions pour manipuler la date de fin de l'�tude

JVoid JATPKEYEtude::SetDateF(JDate DateF)
{
	// on v�rifie la validit� de la date de fin
	if (!DateF.IsValid()) throw JInvalidCall::GetInstance();

	// on v�rifie la coh�rence des nouvelles dates
	if (m_DateD.IsValid()) { if (DateF < m_DateD) throw JInvalidCall::GetInstance(); }

	// on m�morise la date de fin
	m_DateF = DateF;
}

JDate JATPKEYEtude::GetDateF() const
{
	// on renvoie la date de fin
	return (m_DateF);
}

//////////////////////////////////////////
// les fonctions pour s�rialiser l'�l�ment

JVoid JATPKEYEtude::Send(JStream & Stream) const
{
	// on s�rialise l'�l�ment
	Stream.Send(m_IdProduit);
	m_DateD    .Send(Stream);
	m_DateF    .Send(Stream);
}

JVoid JATPKEYEtude::Recv(JStream & Stream)
{
	// on s�rialise l'�l�ment
	JUnt32 IdProduit; Stream.Recv(IdProduit);
	JDate  DateD;     DateD    .Recv(Stream);
	JDate  DateF;     DateF    .Recv(Stream);

	// on v�rifie la coh�rence des dates
	if (DateD.IsValid() && DateF.IsValid()) { if (DateF < DateD) throw JBadSchema::GetInstance(); }

	// on recopie l'�l�ment
	m_IdProduit = IdProduit;
	m_DateD     = DateD;
	m_DateF     = DateF;
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JATPKEYEtude::Swap(JATPKEYEtude & Source)
{
	// on permute l'�l�ment
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
