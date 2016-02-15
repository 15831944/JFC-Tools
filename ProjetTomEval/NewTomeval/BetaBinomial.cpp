//
// Fichier: CBetaBinomial.cpp
// Auteur:  Sylvain SAMMURI
// Date:    09/05/2003
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"

// on inclut les d�finitions n�cessaires
#include "BetaBinomial.h"

// on inclut les d�finitions n�cessaires
#include "math.h"

// on d�finit le message de l'exception
static const char* g_pException = "Internal Error";

////////////////////
// les constructeurs

CBetaBinomial::CBetaBinomial()
{
	// on ne fait rien
}

CBetaBinomial::CBetaBinomial(const CBetaBinomial & Source)
{
	// on g�n�re une exception
	throw g_pException;
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

CBetaBinomial & CBetaBinomial::operator =(const CBetaBinomial & Source)
{
	// on g�n�re une exception
	throw g_pException;
}

/////////////////////////////////////////
// la fonction pour calculer les �l�ments

void CBetaBinomial::OnMakeItems(double A, double B, long N, CArray<double, double &> & Items) const
{
	// on alloue les �l�ments
	Items.SetSize(N);

	// on initialise les r�sultats
	double Cum = 1.0;
	double Res = 1.0;

	// on boucle sur tous les lambdas
	for (long Idx = 0; Idx < N; Idx += 1)
	{
		// on calcule le premier terme de la suite
		Res = (Res * (B + (double)Idx)) / (A + B + (double)Idx);
	}

	// on initialise le premier �l�ment
	Cum = Cum - Res; if (Cum < 0.0) Items[0] = Cum = 0.0; else Items[0] = Cum;

	// on calcule tous les termes suivants de la suite
	for (long Jdx = 1; Jdx < N; Jdx += 1)
	{
		// on calcule le terme courant
		Res = (Res * (A + (double)(Jdx - 1)) * (double)(1 + N - Jdx)) / ((B + (double)(N - Jdx)) * (double)(Jdx));

		// on initialise l'�l�ment courant
		Cum = Cum - Res; if (Cum < 0.0) Items[Jdx] = Cum = 0.0; else Items[Jdx] = Cum;
	}
}

void CBetaBinomial::OnMakeItems(double A, double B, long N1, long N2, double N, CArray<double, double &> & Items) const
{
	// on alloue les �l�ments
	Items.SetSize(N2);

	// on initialise les r�sultats
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

	// on initialise les premiers �l�ments
	Cum1 = Cum1 - Res1; if (Cum1 < 0.0) Cum1 = 0.0;
	Cum2 = Cum2 - Res2; if (Cum2 < 0.0) Cum2 = 0.0;

	// on calcule le premier �l�ment par une interpolation lin�aire
	Items[0] = (((Cum2 - Cum1) * (N - (double)N1)) / ((double)N2 - (double)N1)) + Cum1;

	// on calcule tous les termes suivants de la suite
	for (long Kdx = 1; Kdx < N2; Kdx += 1)
	{
		// on teste le calcul � faire
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

		// on intialise les �l�ments courants
		Cum1 = Cum1 - Res1; if (Cum1 < 0.0) Cum1 = 0.0;
		Cum2 = Cum2 - Res2; if (Cum2 < 0.0) Cum2 = 0.0;

		// on calcule l'�l�ment courant
		Items[Kdx] = (((Cum2 - Cum1) * (N - (double)N1)) / ((double)N2 - (double)N1)) + Cum1;
	}
}

///////////////////////////////////////////
// les fonctions pour calculer les �l�ments

void CBetaBinomial::SetItems(double A, double B, long N)
{
	// on lib�re les �l�ments
	m_Items.RemoveAll();

	// on v�rifie les param�tres
	if (A <= 0.0 || B <= 0.0 || N < 0) throw g_pException;

	// on calcule les nouveaux �l�ments
	if (N > 0) this->OnMakeItems(A, B, N, m_Items);
}

void CBetaBinomial::SetItems(double A, double B, double N)
{
	// on lib�re les �l�ments
	m_Items.RemoveAll();

	// on v�rifie les param�tres
	if (A <= 0.0 || B <= 0.0 || N < 0.0 || N > (double)LONG_MAX) throw g_pException;

	// on calcule les bornes de l'intervalle
	long N1 = (long)floor(N);
	long N2 = (long)ceil (N);

	// on teste le traitement � faire
	if (N1 != N2)
	{
		// on ordonne les �l�ments
		if (N1 > N2) { long Tmp = N1; N1 = N2; N2 = Tmp; }

		// on calcule les nouveaux �l�ments
		if (N2 > 0) this->OnMakeItems(A, B, N1, N2, N, m_Items);
	}
	else
	{
		// on calcule les nouveaux �l�ments
		if (N2 > 0) this->OnMakeItems(A, B, N2, m_Items);
	}
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

double CBetaBinomial::Item(long K) const
{
	// on teste et on corrige le param�tre
	if (K > 0) K -= 1; else if (K == 0) return (1.0);

	// on v�rifie le nombre d'�l�ments
	long Count = m_Items.GetSize(); if (K >= Count) return (0.0);

	// on renvoie l'�l�ment
	return (m_Items[K]);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

void CBetaBinomial::Reset()
{
	// on lib�re les �l�ments
	m_Items.RemoveAll();
}

/////////////////
// le destructeur

CBetaBinomial::~CBetaBinomial()
{
	// on ne fait rien
}
