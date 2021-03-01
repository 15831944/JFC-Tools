//
// Fichier: JTrusquin.cpp
// Auteur:  Sylvain SAMMURI
// Date:    01/03/2005
//

// on inclut les d�finitions n�cessaires
#include "JTrusquin.h"

// on inclut les d�finitions n�cessaires
#include "float.h"
#include "math.h"

////////////////////
// les constructeurs

JTrusquin::JTrusquin()
{
	// on ne fait rien
}

JTrusquin::JTrusquin(const JTrusquin & Source)
{
	// on g�n�re une exception
	throw JInternalError::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JTrusquin & JTrusquin::operator =(const JTrusquin & Source)
{
	// on g�n�re une exception
	throw JInternalError::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JTrusquin::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour d�finir les �l�ments

JVoid JTrusquin::SetItems(JFlt64 P, JFlt64 N)
{
	// on v�rifie le type des param�tres
	if (::_finite(P) == 0 || ::_finite(N) == 0) throw JInternalError::GetInstance();

	// on v�rifie la validit� des param�tres
	if (P < 0.0 || P > 1.0 || N < 0.0) throw JInternalError::GetInstance();

	// on calcule les coefficients
	JInt32 N1 = (JInt32)floor(N);
	JInt32 N2 = (JInt32)ceil (N);

	// on cr�e les nouveaux �l�ments
	JArray<JFlt64> TBLItems;

	// on teste le cas � traiter
	if (N1 <= 1 && N2 == 1)
	{
		// on alloue et on initialise l'unique �l�ment
		TBLItems.SetCount(1); TBLItems.Item(0) = P * N;
	}
	else if (N1 >= 1 && N2 > 1)
	{
		// on alloue les nouveaux �l�ments
		TBLItems.SetCount(N2);

		// on boucle sur toutes les classes
		for (JInt32 Jdx = 0; Jdx < N1; Jdx += 1)
		{
			// on initialise la classe courante
			TBLItems.Item(Jdx) = 0.0;

			// on remonte toutes les classes
			for (JInt32 Kdx = Jdx; Kdx >= 0; Kdx -= 1)
			{
				// on r�cup�re la probabilit� de la classe courante
				JFlt64 & ProbaK = TBLItems.Item(Kdx);

				// on teste si on r�cup�rer la probabilit� de la classe pr�c�dente
				if (Kdx > 0)
				{
					// on r�cup�re la probabilit� de la classe pr�c�dente
					const JFlt64 & ProbaKM1 = TBLItems.Item(Kdx - 1);

					// on incr�mente la probabilit� de la classe courante
					if (ProbaK <= ProbaKM1) ProbaK += (ProbaKM1 - ProbaK) * P;
				}
				else
				{
					// on incr�mente la probabilit� de la classe courante
					ProbaK += (1.0 - ProbaK) * P;
				}

				// on corrige la probabilit� de la classe courante
				if (ProbaK > 1.0) ProbaK = 1.0;
			}
		}

		// on teste si on doit faire une extrapolation
		if (N1 < N2)
		{
			// on initialise la classe courante
			TBLItems.Item(N2 - 1) = 0.0;

			// on boucle sur toutes les classes
			for (JInt32 Ldx = N2 - 1; Ldx >= 0; Ldx -= 1)
			{
				// on r�cup�re la probabilit� de la classe courante
				JFlt64 & ProbaK = TBLItems.Item(Ldx);

				// on teste si on r�cup�rer la probabilit� de la classe pr�c�dente
				if (Ldx > 0)
				{
					// on r�cup�re la probabilit� de la classe pr�c�dente
					const JFlt64 & ProbaKM1 = TBLItems.Item(Ldx - 1);

					// on calcule la probabilit� de la classe courante
					if (ProbaK <= ProbaKM1) ProbaK += (ProbaKM1 - ProbaK) * (N - (JFlt64)N1) * P;
				}
				else
				{
					// on calcule la probabilit� de la classe courante
					ProbaK += (1.0 - ProbaK) * (N - (JFlt64)N1) * P;
				}

				// on corrige la probabilit� de la classe courante
				if (ProbaK > 1.0) ProbaK = 1.0;
			}
		}
	}

	// on permute les �l�ments
	m_TBLItems.Swap(TBLItems);
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

const JFlt64 & JTrusquin::Item(JInt32 Index) const
{
	// on renvoie l'�l�ment
	return (m_TBLItems.Item(Index));
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JTrusquin::Swap(JTrusquin & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JTrusquin::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JTrusquin::~JTrusquin()
{
	// on ne fait rien
}
