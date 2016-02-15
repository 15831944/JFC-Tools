//
// Fichier : GestionCouleurs.h
// Date    : 29/01/2002
// Auteur  : Jeremie
//

// on verifie que le fichier n'a pas deja été inclu
#ifndef GESTION_COULEURS_H

#define GESTION_COULEURS_H

class GestionCouleurs
{
public:
	// constructeur de recopie
	GestionCouleurs(const GestionCouleurs & source);

	// operateur d'affectation
	GestionCouleurs & operator = (const GestionCouleurs & operande);

	// operateur de converstion (casting de const --> long)
	operator long() const;

	// destructeur
	~GestionCouleurs();

public:
	// Fonction retournant la valeur Min du Contraste
	static long GetMinContraste();
	// Fonction retournant la valeur Max du Contraste
	static long GetMaxContraste();
	// Fonction retournant la valeur Defaut du Contraste
	static long GetDefautContraste();

	// Fonction qui modifie la valeur du Contraste
	static void SetContraste(long contraste);
	// Fonction qui recupere la valeur du Contraste
	static long GetContraste();

	// fonction d'ajout de couleurs au lancement de l'application
	static const GestionCouleurs & GetColor(long indice);
	static void SetColor(long indice,int Rouge,int Vert,int Bleu);

private:
	// La valeur du contraste
	static long m_Contraste;
	
private:
	// Les différentes couleurs utilisées
/*
	static  const GestionCouleurs  BLANC;
	static  const GestionCouleurs  GRIS;
	static  const GestionCouleurs  NOIR;
	static  const GestionCouleurs  GRIS_1; // 210
	static  const GestionCouleurs  GRIS_2; // 220
	static  const GestionCouleurs  GRIS_3; // 230
	static  const GestionCouleurs  GRIS_4; // 240
	static  const GestionCouleurs  GRIS_5; // 220
*/
	static  GestionCouleurs  BLANC;	// 2
	static  GestionCouleurs  GRIS;	// 1
	static  GestionCouleurs  NOIR;	// 0
	static  GestionCouleurs  GRIS_1; // 11
	static  GestionCouleurs  GRIS_2; // 12
	static  GestionCouleurs  GRIS_3; // 13
	static  GestionCouleurs  GRIS_4; // 14
	static  GestionCouleurs  GRIS_5; // 15
	static  GestionCouleurs  GRIT; // 3
	static  GestionCouleurs  GRIT_1; // 31
	static  GestionCouleurs  GRIT_2; // 32
	static  GestionCouleurs  GRIT_3; // 33
	static  GestionCouleurs  GRIT_4; // 34
	static  GestionCouleurs  GRIT_5; // 35

	static  GestionCouleurs  PALETTE_1;
	static  GestionCouleurs  PALETTE_2;
	static  GestionCouleurs  PALETTE_3;
	static  GestionCouleurs  PALETTE_4;
	static  GestionCouleurs  PALETTE_5;
	static  GestionCouleurs  PALETTE_6;
	static  GestionCouleurs  PALETTE_7;
	static  GestionCouleurs  PALETTE_8;
	static  GestionCouleurs  PALETTE_9;
	static  GestionCouleurs  PALETTE_10;
	static  GestionCouleurs  PALETTE_11;
	static  GestionCouleurs  PALETTE_12;
	static  GestionCouleurs  PALETTE_13;
	static  GestionCouleurs  PALETTE_14;
	static  GestionCouleurs  PALETTE_15;
	static  GestionCouleurs  PALETTE_16;


private:
	// le constructeur
	GestionCouleurs(unsigned char Rouge, unsigned char Vert, unsigned char Bleu);

	// la couleur utilisée
	long m_couleur;
};

#endif