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

	// Sélection par défaut TRUE
	m_Select = TRUE;

	// Nom du film
	m_Libelle = "";

	// Code produit
	m_CodeProduit = "";

	// Actif par défaut
	//m_Actif = TRUE;
	
	// Tarif Film
	m_Tarif = 0;

	// Début Film
	m_Debut = COleDateTime::GetCurrentTime();

	// Nb semaines analyse
	m_NbSemaine = 0;

	// Données CNC
	m_EntreeCNC = 0; 

	// Nom critère pour coeff fréq
	m_NomCritCoeff = "";

	// Code critère
	m_CodeCritCoeff = 0;
	
}

CProduitFilm::~CProduitFilm()
{

}

