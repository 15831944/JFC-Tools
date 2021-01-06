//
// Fichier: JBetaBinomial.cpp
// Auteur:  Sylvain SAMMURI
// Date:    09/05/2003
//

// on inclut les d�finitions n�cessaires
#include "JBetaBinomial.h"

////////////////////
// les constructeurs

JBetaBinomial::JBetaBinomial()
{
	// on ne fait rien
}

JBetaBinomial::JBetaBinomial(const JBetaBinomial & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JBetaBinomial & JBetaBinomial::operator =(const JBetaBinomial & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// la fonction pour calculer les �l�ments

JVoid JBetaBinomial::OnMakeItems(JFlt64 A, JFlt64 B, JInt32 N, JArray<JFlt64> & Items) const
{
	// on alloue les �l�ments
	Items.SetCount(N);

	// on initialise les r�sultats
	JFlt64 Cum = 1.0;
	JFlt64 Res = 1.0;

	// on boucle sur tous les lambdas
	for (JInt32 Idx = 0; Idx < N; Idx += 1)
	{
		// on calcule le premier terme de la suite
		Res = (Res * (B + (JFlt64)Idx)) / (A + B + (JFlt64)Idx);
	}

	// on initialise le premier �l�ment
	Cum = Cum - Res; if (Cum < 0.0) Items.Item(0) = Cum = 0.0; else Items.Item(0) = Cum;

	// on calcule tous les termes suivants de la suite
	for (JInt32 Jdx = 1; Jdx < N; Jdx += 1)
	{
		// on calcule le terme courant
		Res = (Res * (A + (JFlt64)(Jdx - 1)) * (JFlt64)(1 + N - Jdx)) / ((B + (JFlt64)(N - Jdx)) * (JFlt64)(Jdx));

		// on initialise l'�l�ment courant
		Cum = Cum - Res; if (Cum < 0.0) Items.Item(Jdx) = Cum = 0.0; else Items.Item(Jdx) = Cum;
	}
}

JVoid JBetaBinomial::OnMakeItems(JFlt64 A, JFlt64 B, JInt32 N1, JInt32 N2, JFlt64 N, JArray<JFlt64> & Items) const
{
	// on alloue les �l�ments
	Items.SetCount(N2);

	// on initialise les r�sultats
	JFlt64 Cum1 = 1.0; JFlt64 Cum2 = 1.0;
	JFlt64 Res1 = 1.0; JFlt64 Res2 = 1.0;

	// on boucle sur les premiers lambdas
	for (JInt32 Idx = 0; Idx < N1; Idx += 1)
	{
		// on calcule le premier terme de la suite 1
		Res1 = (Res1 * (B + (JFlt64)Idx)) / (A + B + (JFlt64)Idx);
	}

	// on reporte le premier terme de la suite 1
	Res2 = Res1;

	// on boucle sur les derniers lambdas
	for (JInt32 Jdx = N1; Jdx < N2; Jdx += 1)
	{
		// on calcule le premier terme de la suite 2
		Res2 = (Res2 * (B + (JFlt64)Jdx)) / (A + B + (JFlt64)Jdx);
	}

	// on initialise les premiers �l�ments
	Cum1 = Cum1 - Res1; if (Cum1 < 0.0) Cum1 = 0.0;
	Cum2 = Cum2 - Res2; if (Cum2 < 0.0) Cum2 = 0.0;

	// on calcule le premier �l�ment par une interpolation lin�aire
	Items.Item(0) = (((Cum2 - Cum1) * (N - (JFlt64)N1)) / ((JFlt64)N2 - (JFlt64)N1)) + Cum1;

	// on calcule tous les termes suivants de la suite
	for (JInt32 Kdx = 1; Kdx < N2; Kdx += 1)
	{
		// on teste le calcul � faire
		if (Kdx < N1)
		{
			// on calcule le terme courant
			Res1 = (Res1 * (A + (JFlt64)(Kdx - 1)) * (JFlt64)(1 + N1 - Kdx)) / ((B + (JFlt64)(N1 - Kdx)) * (JFlt64)(Kdx));
			Res2 = (Res2 * (A + (JFlt64)(Kdx - 1)) * (JFlt64)(1 + N2 - Kdx)) / ((B + (JFlt64)(N2 - Kdx)) * (JFlt64)(Kdx));
		}
		else
		{
			// on calcule le terme courant
			Res2 = (Res2 * (A + (JFlt64)(Kdx - 1)) * (JFlt64)(1 + N2 - Kdx)) / ((B + (JFlt64)(N2 - Kdx)) * (JFlt64)(Kdx));
			Res1 = Cum1 = 0.0;
		}

		// on intialise les �l�ments courants
		Cum1 = Cum1 - Res1; if (Cum1 < 0.0) Cum1 = 0.0;
		Cum2 = Cum2 - Res2; if (Cum2 < 0.0) Cum2 = 0.0;

		// on calcule l'�l�ment courant
		Items.Item(Kdx) = (((Cum2 - Cum1) * (N - (JFlt64)N1)) / ((JFlt64)N2 - (JFlt64)N1)) + Cum1;
	}
}

///////////////////////////////////////////
// les fonctions pour calculer les �l�ments

JVoid JBetaBinomial::SetItems(JFlt64 A, JFlt64 B, JInt32 N)
{
	// on initialise le Fpu
	JFpu Fpu;

	// on cr�e les �l�ments
	JArray<JFlt64> Items;

	// on v�rifie les param�tres
	if (A <= 0.0 || B <= 0.0 || N < 0) throw JInvalidCall::GetInstance();

	// on calcule les nouveaux �l�ments
	if (N > 0) this->OnMakeItems(A, B, N, Items);

	// on permute les �l�ments
	m_Items.Swap(Items);
}

JVoid JBetaBinomial::SetItems(JFlt64 A, JFlt64 B, JFlt64 N)
{
	// on initialise le Fpu
	JFpu Fpu;

	// on cr�e les �l�ments
	JArray<JFlt64> Items;

	// on v�rifie les param�tres
	if (A <= 0.0 || B <= 0.0 || N < 0.0 || N > (JFlt64)JINT32_MAX) throw JInvalidCall::GetInstance();

	// on calcule les bornes de l'intervalle
	Fpu.SetRoundingModeDown(); JInt32 N1 = (JInt32)JMath::Round(N);
	Fpu.SetRoundingModeUp();   JInt32 N2 = (JInt32)JMath::Round(N); Fpu.SetRoundingModeNear();

	// on teste le traitement � faire
	if (N1 != N2)
	{
		// on ordonne les �l�ments
		if (N1 > N2) { JInt32 Tmp = N1; N1 = N2; N2 = Tmp; }

		// on calcule les nouveaux �l�ments
		if (N2 > 0) this->OnMakeItems(A, B, N1, N2, N, Items);
	}
	else
	{
		// on calcule les nouveaux �l�ments
		if (N2 > 0) this->OnMakeItems(A, B, N2, Items);
	}

	// on permute les �l�ments
	m_Items.Swap(Items);
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

JFlt64 JBetaBinomial::Item(JInt32 K) const
{
	// on teste et on corrige le param�tre
	if (K > 0) K -= 1; else if (K == 0) return (1.0);

	// on v�rifie le nombre d'�l�ments
	JInt32 Count = m_Items.GetCount(); if (K >= Count) return (0.0);

	// on renvoie l'�l�ment
	return (m_Items.Item(K));
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JBetaBinomial::Swap(JBetaBinomial & Source)
{
	// on permute les �l�ments
	m_Items.Swap(Source.m_Items);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JBetaBinomial::Reset()
{
	// on lib�re les �l�ments
	m_Items.Reset();
}

/////////////////
// le destructeur

JBetaBinomial::~JBetaBinomial()
{
	// on ne fait rien
}
