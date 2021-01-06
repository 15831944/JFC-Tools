//==============================
// fichier: JFCGraduation.cpp
//
// date: 18/02/2002
// auteur: JB
//==============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCGraduation.h"
#include <float.h>
#include <math.h>

//====================================================================================
// le constructeur:
//------------------------------------------------------------------------------------
JFCGraduation::JFCGraduation()
{
	// on initialise le nombre de décimales
	m_Decimale = 0;

	// on fixe les valeurs des bornes
	m_RangeMax = 0.0;
	m_RangeMin = 0.0;
}

//====================================================================================
// la fonction pour fixer les bornes:
//------------------------------------------------------------------------------------
void JFCGraduation::SetRange(double RangeMin, double RangeMax)
{
	// on calcule l'intervalle
	double intervalle = RangeMax - RangeMin;

	if (intervalle == 0.0)
	{
		m_RangeMin = RangeMin;
		m_RangeMax = RangeMax;
		return;
	}

	// on calcule l'exposant
	long exposant = (long) (floor (log10(intervalle)));

	// on calcule l'échelle
	double echelle = 1; 
	if (exposant > 0) 
	{
		for (long i = 0; i < exposant; i++) echelle *= 10.0;
		m_Decimale = 0;
	}
	else if (exposant < 0)
	{
		for (long i = exposant; i < 0 ; i++) echelle /= 10.0;
		m_Decimale = (-exposant);
	}
	else
	{
		m_Decimale = 0;
	}
	// on calcule le pas
	double pas = 0.0;

	if ((intervalle/(10.0 * echelle) + 0.5) >= 1.0) pas = 1.0;
	else pas = 0.5;

	// les balises
	double BInf;
	double BSup;

	// on teste le pas
	if (pas < 1.0)
	{
		// on calcule les balises
		BInf = floor((RangeMin / echelle) * 2.0 + 0.5) / 2.0;
		BSup = floor((RangeMax / echelle) * 2.0 + 0.5) / 2.0;
	}
	else
	{
		// on calcule les balises
		BInf = floor((RangeMin / echelle) + 0.5);
		BSup = floor((RangeMax / echelle) + 0.5);
	}
	// on récupère le nombre de pas de la graduation
	long NbPas = (long)floor((BSup - BInf)/(pas));

	// on teste le nombre de pas
	if ((NbPas % 2) == 0)
	{
		// on teste le nombre de pas
		if (NbPas > 2)
		{
			// on fixe la taille du tableau
			m_Graduation.SetSize(5);

			// on remplit les éléments du tableau
			m_Graduation[0] = BInf * echelle;
			m_Graduation[1] = (BInf + (BSup - BInf) / 4.0) * echelle;
			m_Graduation[2] = ((BInf + BSup) / 2.0) * echelle;
			m_Graduation[3] = (BInf + (BSup - BInf) * 3.0 / 4.0) * echelle;
			m_Graduation[4] = BSup * echelle;

			// on met à jour les bornes
			if (RangeMin < m_Graduation[0]) m_RangeMin = RangeMin; else m_RangeMin = m_Graduation[0];
			if (RangeMax > m_Graduation[4]) m_RangeMax = RangeMax; else m_RangeMax = m_Graduation[4];

			// on teste le nombre de décimales
			long reste = (((long)(NbPas*pas)) % 4);
			
			if ( reste != 0 ) 
			{
				exposant -= 1;
				if ((reste % 2) != 0) exposant -= 1;
			}
		}
		else
		{
			// on fixe la taille du tableau
			m_Graduation.SetSize(3);

			// on remplit les éléments du tableau
			m_Graduation[0] = BInf * echelle;
			m_Graduation[1] = ((BInf + BSup) / 2.0) * echelle;
			m_Graduation[2] = BSup * echelle;

			if (RangeMin < m_Graduation[0]) m_RangeMin = RangeMin; else m_RangeMin = m_Graduation[0];
			if (RangeMax > m_Graduation[2]) m_RangeMax = RangeMax; else m_RangeMax = m_Graduation[2];

			// on teste le nombre de décimales
			if ((((long)(NbPas*pas)) % 2) > 0 ) exposant-= 1;
		}
	}
	else
	{
		// on fixe le nombre de graduations
		m_Graduation.SetSize(NbPas+1);

		// on boucle sur les graduations
		for (long i = 0; i <= NbPas; i++)
		{
			// on ajoute la graduation
			m_Graduation[i] = (BInf + i * pas) * echelle;
		}
		// on met à jour les bornes
		if (RangeMin < m_Graduation[0])     m_RangeMin = RangeMin; else m_RangeMin = m_Graduation[0];
		if (RangeMax > m_Graduation[NbPas]) m_RangeMax = RangeMax; else m_RangeMax = m_Graduation[NbPas];

		// on teste le nombre de décimales
		if ((pas < 1.0)) exposant -= 1;
	}
	// on met à jour le nombre de décimales
	if (exposant < 0) m_Decimale = -exposant;
	
}

//====================================================================================
// la fonction pour récupérer le nombre de graduations:
//------------------------------------------------------------------------------------
long JFCGraduation::GetNbGraduation()
{
	// on renvoie la taille du tableau
	return (m_Graduation.GetSize());
}

//====================================================================================
// la fonction pour récupérer une graduation par son indice:
//------------------------------------------------------------------------------------
double JFCGraduation::GetGraduation(long indice)
{
	// on renvoie l'élément indicé
	return (m_Graduation[indice]);
}

//====================================================================================
// la fonction pour récupérer le libellé de la  graduation d'aprés son indice:
//------------------------------------------------------------------------------------
CString JFCGraduation::GetLibGraduation(long indice)
{
	// le libellé
	CString Libelle;

	// on formate le libellé
	Libelle.Format("%0.*f", m_Decimale, m_Graduation[indice]);

	// on renvoie le libelle
	return (Libelle);
}

//====================================================================================
// les fonctions pour renvoyer les bornes:
//------------------------------------------------------------------------------------
double JFCGraduation::GetMin()
{
	// on renvoie la borne inférieure
	return (m_RangeMin);
}

double JFCGraduation::GetMax()
{
	// on renvoie la borne supérieure
	return (m_RangeMax);
}

//====================================================================================
// la fonction pour vider le tableau des graduations:
//------------------------------------------------------------------------------------
void JFCGraduation::Reset()
{
	// on vide le tableau des graduations
	m_Graduation.RemoveAll();

	// on remet le nombre de décimale à 0
	m_Decimale = 0;
}

//====================================================================================
// le destructeur:
//------------------------------------------------------------------------------------
JFCGraduation::~JFCGraduation()
{
	// on ne fait rien
}