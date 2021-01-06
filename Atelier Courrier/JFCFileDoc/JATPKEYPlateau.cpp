//
// Fichier: JATPKEYPlateau.cpp
// Auteur:  Alain Chambard
// Date:    08/04/2004
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "resource.h"
#include "JATPKEYPlateau.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// Le constructeur
JATPKEYPlateau::JATPKEYPlateau(void)
{
	m_NomPlateau.LoadString(IDS_FD_NONAME);
	m_DateTar.Reset(); 
	m_path = "";
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Le destructeur
JATPKEYPlateau::~JATPKEYPlateau(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
JATPKEYPlateau::JATPKEYPlateau(const JATPKEYPlateau & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// l'opérateur pour recopier les éléments du plateau
JATPKEYPlateau & JATPKEYPlateau::operator =(const JATPKEYPlateau & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le nom du plateau
CString JATPKEYPlateau::GetLabel() const
{
	return m_NomPlateau;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler le nom du plateau
JVoid JATPKEYPlateau::SetPath(CString path)
{
	// Sauve path courant
	m_path = path;

	//On recherche le  '\'
	int idx = path.ReverseFind(0x5C);

	//On recherche l'extension plateau "ofr"
	int idx2 = path.Find(".ofr", 0);
	
	// Reconstitue le nom du plateau
	m_NomPlateau = path.Mid(idx+1, idx2-idx-1);

}

////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère path courant fichiers plateau
CString JATPKEYPlateau::GetPath() const
{
	return m_path;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// les fonctions pour manipuler la date de tarification
JVoid JATPKEYPlateau::SetDateTar(JDate DateTar)
{
	// on vérifie la validité de la date de tarification
	if (!DateTar.IsValid()) throw JInvalidCall::GetInstance();

	// on vérifie la cohérence des nouvelles dates
	/*
	if (m_DateTar.IsValid())
	{ 
		if (DateTar != m_DateTar) 
			throw JInvalidCall::GetInstance();
	}
	*/

	// on mémorise la date de tarification
	m_DateTar = DateTar;
}

JDate JATPKEYPlateau::GetDateTar() const
{
	// on renvoie la date de tarification
	return (m_DateTar);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// les fonctions pour sérialiser l'élément
JVoid JATPKEYPlateau::Send(JStream & Stream) const
{
	// on sérialise l'élément
	m_DateTar  .Send(Stream);
}

JVoid JATPKEYPlateau::Recv(JStream & Stream)
{
	// on sérialise l'élément
	JDate  DateTar;   DateTar  .Recv(Stream);

	// on vérifie la cohérence de la date tarification
	if (!DateTar.IsValid())	throw JBadSchema::GetInstance();

	// on recopie l'élément
	m_DateTar     = DateTar;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour permuter les éléments
JVoid JATPKEYPlateau::Swap(JATPKEYPlateau & Source)
{
	// on permute nom du plateau
	CString NomPlateau	= m_NomPlateau;
	m_NomPlateau		= Source.m_NomPlateau;
	Source.m_NomPlateau = NomPlateau;

	// on permute path répertoire 
	CString Path		= m_path;
	m_path				= Source.m_path;
	Source.m_path		= Path;

	// on permute date de tarification
	JDate  DateTar		= m_DateTar;
	m_DateTar			= Source.m_DateTar;
	Source.m_DateTar	= DateTar;
}
