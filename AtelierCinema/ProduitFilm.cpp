// ProduitFilm.cpp: implementation of the CProduitFilm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cinema.h"
#include "ProduitFilm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProduitFilm::CProduitFilm()
{

	// S�lection par d�faut TRUE
	m_Select = TRUE;

	// Nom du film
	m_Libelle = "";

	// Code produit
	m_CodeProduit = "";

	// Actif par d�faut
	//m_Actif = TRUE;
	
	// Tarif Film
	m_Tarif = 0;

	// D�but Film
	m_Debut = COleDateTime::GetCurrentTime();

	// Nb semaines analyse
	m_NbSemaine = 0;

	// Donn�es CNC
	m_EntreeCNC = 0; 

	// Nom crit�re pour coeff fr�q
	m_NomCritCoeff = "";

	// Code crit�re
	m_CodeCritCoeff = 0;
	
}

CProduitFilm::~CProduitFilm()
{

}

