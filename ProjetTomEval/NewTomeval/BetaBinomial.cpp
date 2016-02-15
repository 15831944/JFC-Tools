//
// Fichier: CBetaBinomial.cpp
// Auteur:  Sylvain SAMMURI
// Date:    09/05/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"

// on inclut les définitions nécessaires
#include "BetaBinomial.h"

// on inclut les définitions nécessaires
#include "math.h"

// on définit le message de l'exception
static const char* g_pException = "Internal Error";

////////////////////
// les constructeurs

CBetaBinomial::CBetaBinomial()
{
	// on ne fait rien
}

CBetaBinomial::CBetaBinomial(const CBetaBinomial & Source)
{
	// on génère une exception
	throw g_pException;
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

CBetaBinomial & CBetaBinomial::operator =(const CBetaBinomial & Source)
{
	// on génère une exception
	throw g_pException;
}

/////////////////////////////////////////
// la fonction pour calculer les éléments

void CBetaBinomial::OnMakeItems(double A, double B, long N, CArray<double, double &> & Items) const
{
	// on alloue les éléments
	Items.SetSize(N);

	// on initialise les résultats
	double Cum = 1.0;
	double Res = 1.0;

	// on boucle sur tous les lambdas
	for (long Idx = 0; Idx < N; Idx += 1)
	{
		// on calcule le premier terme de la suite
		Res = (Res * (B + (double)Idx)) / (A + B + (double)Idx);
	}

	// on initialise le premier élément
	Cum = Cum - Res; if (Cum < 0.0) Items[0] = Cum = 0.0; else Items[0] = Cum;

	// on calcule tous les termes suivants de la suite
	for (long Jdx = 1; Jdx < N; Jdx += 1)
	{
		// on calcule le terme courant
		Res = (Res * (A + (double)(Jdx - 1)) * (double)(1 + N - Jdx)) / ((B + (double)(N - Jdx)) * (double)(Jdx));

		// on initialise l'élément courant
		Cum = Cum - Res; if (Cum < 0.0) Items[Jdx] = Cum = 0.0; else Items[Jdx] = Cum;
	}
}

void CBetaBinomial::OnMakeItems(double A, double B, long N1, long N2, double N, CArray<double, double &> & Items) const
{
	// on alloue les éléments
	Items.SetSize(N2);

	// on initialise les résultats
	double Cum1 = 1.0; double Cum2 = 1.0;
	double Res1 = 1.0; double Res2 = 1.0;

	// on boucle sur les premiers lambdas
	for (long Idx = 0; Idx < N1; Idx += 1)
	{
		// on calcule le premier terme de la suite 1
		Res1 = (Res1 * (B + (double)Idx)) / (A + B + (double)Idx);
	}

	// on reporte le premier terme de la suite 1
	Res2 = Res1;

	// on boucle sur les derniers lambdas
	for (long Jdx = N1; Jdx < N2; Jdx += 1)
	{
		// on calcule le premier terme de la suite 2
		Res2 = (Res2 * (B + (double)Jdx)) / (A + B + (double)Jdx);
	}

	// on initialise les premiers éléments
	Cum1 = Cum1 - Res1; if (Cum1 < 0.0) Cum1 = 0.0;
	Cum2 = Cum2 - Res2; if (Cum2 < 0.0) Cum2 = 0.0;

	// on calcule le premier élément par une interpolation linéaire
	Items[0] = (((Cum2 - Cum1) * (N - (double)N1)) / ((double)N2 - (double)N1)) + Cum1;

	// on calcule tous les termes suivants de la suite
	for (long Kdx = 1; Kdx < N2; Kdx += 1)
	{
		// on teste le calcul à faire
		if (Kdx < N1)
		{
			// on calcule le terme courant
			Res1 = (Res1 * (A + (double)(Kdx - 1)) * (double)(1 + N1 - Kdx)) / ((B + (double)(N1 - Kdx)) * (double)(Kdx));
			Res2 = (Res2 * (A + (double)(Kdx - 1)) * (double)(1 + N2 - Kdx)) / ((B + (double)(N2 - Kdx)) * (double)(Kdx));
		}
		else
		{
			// on calcule le terme courant
			Res2 = (Res2 * (A + (double)(Kdx - 1)) * (double)(1 + N2 - Kdx)) / ((B + (double)(N2 - Kdx)) * (double)(Kdx));
			Res1 = Cum1 = 0.0;
		}

		// on intialise les éléments courants
		Cum1 = Cum1 - Res1; if (Cum1 < 0.0) Cum1 = 0.0;
		Cum2 = Cum2 - Res2; if (Cum2 < 0.0) Cum2 = 0.0;

		// on calcule l'élément courant
		Items[Kdx] = (((Cum2 - Cum1) * (N - (double)N1)) / ((double)N2 - (double)N1)) + Cum1;
	}
}

///////////////////////////////////////////
// les fonctions pour calculer les éléments

void CBetaBinomial::SetItems(double A, double B, long N)
{
	// on libère les éléments
	m_Items.RemoveAll();

	// on vérifie les paramètres
	if (A <= 0.0 || B <= 0.0 || N < 0) throw g_pException;

	// on calcule les nouveaux éléments
	if (N > 0) this->OnMakeItems(A, B, N, m_Items);
}

void CBetaBinomial::SetItems(double A, double B, double N)
{
	// on libère les éléments
	m_Items.RemoveAll();

	// on vérifie les paramètres
	if (A <= 0.0 || B <= 0.0 || N < 0.0 || N > (double)LONG_MAX) throw g_pException;

	// on calcule les bornes de l'intervalle
	long N1 = (long)floor(N);
	long N2 = (long)ceil (N);

	// on teste le traitement à faire
	if (N1 != N2)
	{
		// on ordonne les éléments
		if (N1 > N2) { long Tmp = N1; N1 = N2; N2 = Tmp; }

		// on calcule les nouveaux éléments
		if (N2 > 0) this->OnMakeItems(A, B, N1, N2, N, m_Items);
	}
	else
	{
		// on calcule les nouveaux éléments
		if (N2 > 0) this->OnMakeItems(A, B, N2, m_Items);
	}
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

double CBetaBinomial::Item(long K) const
{
	// on teste et on corrige le paramètre
	if (K > 0) K -= 1; else if (K == 0) return (1.0);

	// on vérifie le nombre d'éléments
	long Count = m_Items.GetSize(); if (K >= Count) return (0.0);

	// on renvoie l'élément
	return (m_Items[K]);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

void CBetaBinomial::Reset()
{
	// on libère les éléments
	m_Items.RemoveAll();
}

/////////////////
// le destructeur

CBetaBinomial::~CBetaBinomial()
{
	// on ne fait rien
}
