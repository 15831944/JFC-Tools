//==========================
// fichier: JFCDecimilToUnit.h
//
// date: 29/08/2001
// auteur: JB
//==========================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDECIMILTOUNIT_H_

// on définit une macro pour identifier le fichier
#define _JFCDECIMILTOUNIT_H_

// on inclus les fichiers nécessaires
#include <windows.h>


class DecimilToUnit
{
public:
	// les constructeurs
	DecimilToUnit(long widthunit, long heightunit, long widthmm, long heightmm, long zoom, long offsetX, long offsetY);

	// le constucteur de recopie
	DecimilToUnit(const DecimilToUnit & source);

	// l'opérateur d'égalité
	DecimilToUnit & operator=(const DecimilToUnit & operande);

	// les fonctions pour modifier les paramètres
	void SetScale(long ScaleRatio);
	void SetOffsetX(long offsetX);
	void SetOffsetY(long offsetY);

	// la fonction pour récupérer la taille en pixels
	long GetUnitPosX(long coordXmm);
	long GetUnitPosY(long coordYmm);

	// la fonction pour récupérer une dimension en pixels
	long GetDimX(long dim);
	long GetDimY(long dim);

	// le destructeur
	~DecimilToUnit();

public:
	// le zoom en pourcent
	long m_Scale;

	// le déplacement (en mm)
	long m_OffsetX;
	long m_OffsetY;

private:
	// la résolution de l'écran en largeur
	long m_UnitWidth;

	// la largeur en mm de l'écran physique
	long m_MMWidth;

	// la résolution de l'écran en hauteur
	long m_UnitHeight;

	// la hauteur en mm de l'écran physique
	long m_MMHeight;
};

// fin du test sur la macro
#endif
