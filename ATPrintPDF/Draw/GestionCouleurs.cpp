//
// Fichier : GestionCouleurs.cpp
// Date    : 29/01/2002
// Auteur  : Jeremie
//

#include "stdafx.h"
#include "GestionCouleurs.h"


// ///////////////////////////////////////////////////////////////////////////
// les differentes couleurs
// ///////////////////////////////////////////////////////////////////////////
/*
const GestionCouleurs GestionCouleurs::BLANC(255,255,255);
const GestionCouleurs GestionCouleurs::GRIS(128,128,128);
const GestionCouleurs GestionCouleurs::NOIR(0,0,0);

// les nuances de gris
const GestionCouleurs GestionCouleurs::GRIS_1(240,240,240);  // du plus clair
const GestionCouleurs GestionCouleurs::GRIS_2(192,192,192);
const GestionCouleurs GestionCouleurs::GRIS_3(160,160,160);
const GestionCouleurs GestionCouleurs::GRIS_4(128,128,128);
const GestionCouleurs GestionCouleurs::GRIS_5(96,96,96);		// au plus foncé
*/
GestionCouleurs GestionCouleurs::BLANC(255,255,255);
GestionCouleurs GestionCouleurs::GRIS(128,128,128);
GestionCouleurs GestionCouleurs::NOIR(0,0,0);
GestionCouleurs GestionCouleurs::GRIT(128,128,64);  // Couleur du titre

// les nuances de gris
GestionCouleurs GestionCouleurs::GRIT_1(192,192,128);  // Couleur du titre
GestionCouleurs GestionCouleurs::GRIT_2(160,160,96);  // Couleur du titre
GestionCouleurs GestionCouleurs::GRIT_3(128,128,64);  // Couleur du titre
GestionCouleurs GestionCouleurs::GRIT_4(96,96,32);  // Couleur du titre
GestionCouleurs GestionCouleurs::GRIT_5(64,64,0);  // Couleur du titre

GestionCouleurs GestionCouleurs::GRIS_1(240,240,240);  // du plus clair
GestionCouleurs GestionCouleurs::GRIS_2(192,192,192);
GestionCouleurs GestionCouleurs::GRIS_3(160,160,160);
GestionCouleurs GestionCouleurs::GRIS_4(128,128,128);
GestionCouleurs GestionCouleurs::GRIS_5(96,96,96);		// au plus foncé
// la palette
GestionCouleurs GestionCouleurs::PALETTE_1 (0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_2 (0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_3 (0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_4 (0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_5 (0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_6 (0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_7 (0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_8 (0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_9 (0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_10(0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_11(0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_12(0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_13(0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_14(0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_15(0,0,0);
GestionCouleurs GestionCouleurs::PALETTE_16(0,0,0);



long GestionCouleurs::m_Contraste = GestionCouleurs::GetDefautContraste();

// /////////////////////////////////////////////////////////////////////////// 
// le constructeur
// ///////////////////////////////////////////////////////////////////////////

GestionCouleurs::GestionCouleurs(unsigned char Rouge, unsigned char Vert, unsigned char Bleu)
{
	//initialisation de la couleur courante
	m_couleur = RGB(Rouge, Vert, Bleu);
}

// /////////////////////////////////////////////////////////////////////////// 
// constructeur de recopie
// ///////////////////////////////////////////////////////////////////////////

GestionCouleurs::GestionCouleurs(const GestionCouleurs & source)
{
	m_couleur = source.m_couleur;
}

// ///////////////////////////////////////////////////////////////////////////
// operateur d'affectation
// ///////////////////////////////////////////////////////////////////////////

GestionCouleurs & GestionCouleurs::operator =(const GestionCouleurs & operande)
{
	// on recopie
	m_couleur = operande.m_couleur;

	// on retourne la reference de l'objet
	return (*this);
}

// ///////////////////////////////////////////////////////////////////////////
// operateur de conversion (casting de const --> long)
// ///////////////////////////////////////////////////////////////////////////

GestionCouleurs::operator long() const
{
	return (m_couleur);
}

// ///////////////////////////////////////////////////////////////////////////
// fonction d'ajout de couleurs au lancement de l'application
// ///////////////////////////////////////////////////////////////////////////

const GestionCouleurs & GestionCouleurs::GetColor(long indice)
{
	switch (indice)
	{
	case 0:
		return (NOIR);
	case 1:
		switch (GestionCouleurs::m_Contraste)
		{
		case 1 :
			return (GRIS_1);
		case 2 :
			return (GRIS_2);
		case 3 :
			return (GRIS_3);
		case 4 :
			return (GRIS_4);
		case 5 :
			return (GRIS_5);
		}
		return (GRIS_3);
	case 2:
		return (BLANC);

	case 3: 
		switch (GestionCouleurs::m_Contraste)
		{
		case 1 :
			return (GRIT_1);
		case 2 :
			return (GRIT_2);
		case 3 :
			return (GRIT_3);
		case 4 :
			return (GRIT_4);
		case 5 :
			return (GRIT_5);
		}
		return (GRIT_3);

	case 101: // Palette 1 à 16
		return PALETTE_1;
	case 102: return PALETTE_2;
	case 103: return PALETTE_3;
	case 104: return PALETTE_4;
	case 105: return PALETTE_5;
	case 106: return PALETTE_6;
	case 107: return PALETTE_7;
	case 108: return PALETTE_8;
	case 109: return PALETTE_9;
	case 110: return PALETTE_10;
	case 111: return PALETTE_11;
	case 112: return PALETTE_12;
	case 113: return PALETTE_13;
	case 114: return PALETTE_14;
	case 115: return PALETTE_15;
	case 116: return PALETTE_16;
	}
	return (BLANC);
}

// ///////////////////////////////////////////////////////////////////////////
// Fonction retournant la valeur Max du contraste
// ///////////////////////////////////////////////////////////////////////////
long GestionCouleurs::GetMinContraste()
{
	return (1); // valeur Min contraste
}
// ///////////////////////////////////////////////////////////////////////////
// Fonction retournant la valeur Max du contraste
// ///////////////////////////////////////////////////////////////////////////
long GestionCouleurs::GetMaxContraste()
{
	return (5); // valeur Max contraste
}
// ///////////////////////////////////////////////////////////////////////////
// Fonction retournant la valeur Max du contraste
// ///////////////////////////////////////////////////////////////////////////
long GestionCouleurs::GetDefautContraste()
{
	return (3); // valeur par defaut du contraste
}

// ///////////////////////////////////////////////////////////////////////////
// Fonction qui modifie la valeur du contraste
// ///////////////////////////////////////////////////////////////////////////
void GestionCouleurs::SetContraste (long contraste)
{
	if      (contraste < GestionCouleurs::GetMinContraste()) contraste = GestionCouleurs::GetMinContraste();
	else if (contraste > GestionCouleurs::GetMaxContraste()) contraste = GestionCouleurs::GetMaxContraste();
	m_Contraste = contraste;
}
// ///////////////////////////////////////////////////////////////////////////
// Fonction qui recupere la valeur du contraste
// ///////////////////////////////////////////////////////////////////////////
long GestionCouleurs::GetContraste()
{
	return (m_Contraste);
}
void GestionCouleurs::SetColor(long indice,int Rouge_,int Vert_,int Bleu_)
{
	unsigned char Rouge,Vert,Bleu;
	Rouge = max(0,min(255,Rouge_));
	Vert = max(0,min(255,Vert_));
	Bleu = max(0,min(255,Bleu_));

	switch (indice) {
		case   0:GestionCouleurs::NOIR = GestionCouleurs(Rouge, Vert, Bleu);break;

		case   1:GestionCouleurs::GRIS = GestionCouleurs(Rouge, Vert, Bleu);break;
		case  11:GestionCouleurs::GRIS_1 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case  12:GestionCouleurs::GRIS_2 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case  13:GestionCouleurs::GRIS_3 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case  14:GestionCouleurs::GRIS_4 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case  15:GestionCouleurs::GRIS_5 = GestionCouleurs(Rouge, Vert, Bleu);break;

		case   2:GestionCouleurs::BLANC = GestionCouleurs(Rouge, Vert, Bleu);break;

		case   3:GestionCouleurs::GRIT = GestionCouleurs(Rouge, Vert, Bleu);break;		// Titre
		case  31:GestionCouleurs::GRIT_1 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case  32:GestionCouleurs::GRIT_2 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case  33:GestionCouleurs::GRIT_3 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case  34:GestionCouleurs::GRIT_4 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case  35:GestionCouleurs::GRIT_5 = GestionCouleurs(Rouge, Vert, Bleu);break;

		case 101:GestionCouleurs::PALETTE_1 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 102:GestionCouleurs::PALETTE_2 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 103:GestionCouleurs::PALETTE_3 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 104:GestionCouleurs::PALETTE_4 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 105:GestionCouleurs::PALETTE_5 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 106:GestionCouleurs::PALETTE_6 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 107:GestionCouleurs::PALETTE_7 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 108:GestionCouleurs::PALETTE_8 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 109:GestionCouleurs::PALETTE_9 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 110:GestionCouleurs::PALETTE_10 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 111:GestionCouleurs::PALETTE_11 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 112:GestionCouleurs::PALETTE_12 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 113:GestionCouleurs::PALETTE_13 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 114:GestionCouleurs::PALETTE_14 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 115:GestionCouleurs::PALETTE_15 = GestionCouleurs(Rouge, Vert, Bleu);break;
		case 116:GestionCouleurs::PALETTE_16 = GestionCouleurs(Rouge, Vert, Bleu);break;
	}
}


// ///////////////////////////////////////////////////////////////////////////
// destructeur
// ///////////////////////////////////////////////////////////////////////////

GestionCouleurs::~GestionCouleurs()
{
	// suppression de m_couleur
}