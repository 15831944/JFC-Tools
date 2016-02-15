// DistributionCroiseeRedresse.cpp: implementation of the CDistributionCroiseeEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DistributionCroiseeEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDistributionCroiseeEx::CDistributionCroiseeEx()
{

}

CDistributionCroiseeEx::~CDistributionCroiseeEx()
{

}

/*void CDistributionCroiseeEx::RedresseDistributionMarginale0(CDistributionCroisee & crossdistrib,CDistribution & distribMarginale)
{
	crossdistrib.RedresseDistributionMarginale0(distribMarginale);
}*/

void CDistributionCroiseeEx::RedresseDistributionMarginale0(CDistribution & distribMarginaleRedresse)
{
	// Calcul de la distribution Marginale sur la dimension 0 et 1
	CDistribution DM0,DM1;
	CalculDistrubitionMarginale(DM0,0);
	CalculDistrubitionMarginale(DM1,1);
	
	// Calcul de la matrice stochastique finale
	CDistributionCroiseeEx DXTheoriqueFinale;
	DXTheoriqueFinale.CreateNewDistributionCroiseeTheorique(distribMarginaleRedresse,DM1);

	// Calcul de la matrice stochastique Initiale (1)
	CDistributionCroiseeEx DXTheorique1;
	DXTheorique1.CreateNewDistributionCroiseeTheorique(DM0,DM1);

	// Calcul des coefficients relatifs (DXTheoriqueFinale/DXTheorique1)
	CalculDistributionCroiseeRedressementRelatif(DXTheorique1,DXTheoriqueFinale);

	// Calcul de la matrice stochastique Intermédiaire (2)
	CalculDistrubitionMarginale(DM0,0);
	CalculDistrubitionMarginale(DM1,1);
	DXTheorique1.CreateNewDistributionCroiseeTheorique(DM0,DM1);

	// Calcul des coefficients Absolu(DXTheoriqueFinale-DXTheorique1)
	CalculDistributionCroiseeRedressementAbsolu(DXTheorique1,DXTheoriqueFinale);
}

void CDistributionCroiseeEx::RedresseDistributionMarginale1(CDistribution & distribMarginaleRedresse)
{
	// Calcul de la distribution Marginale sur la dimension 0 et 1
	CDistribution DM0,DM1;
	CalculDistrubitionMarginale(DM0,0);
	CalculDistrubitionMarginale(DM1,1);
	
	// Calcul de la matrice stochastique finale
	CDistributionCroiseeEx DXTheoriqueFinale;
	DXTheoriqueFinale.CreateNewDistributionCroiseeTheorique(DM0,distribMarginaleRedresse);

	// Calcul de la matrice stochastique Initiale (1)
	CDistributionCroiseeEx DXTheorique1;
	DXTheorique1.CreateNewDistributionCroiseeTheorique(DM0,DM1);

	// Calcul des coefficients relatifs (DXTheoriqueFinale/DXTheorique1)
	CalculDistributionCroiseeRedressementRelatif(DXTheorique1,DXTheoriqueFinale);

	// Calcul de la matrice stochastique Intermédiaire (2)
	CalculDistrubitionMarginale(DM0,0);
	CalculDistrubitionMarginale(DM1,1);
	DXTheorique1.CreateNewDistributionCroiseeTheorique(DM0,DM1);

	// Calcul des coefficients Absolu(DXTheoriqueFinale-DXTheorique1)
	CalculDistributionCroiseeRedressementAbsolu(DXTheorique1,DXTheoriqueFinale);
}


void CDistributionCroiseeEx::RedresseDistributionMarginale(CDistribution & distribMarginaleRedresse0,CDistribution & distribMarginaleRedresse1)
{
	// Calcul de la distribution Marginale sur la dimension 0 et 1
	CDistribution DM0,DM1;
	CalculDistrubitionMarginale(DM0,0);
	CalculDistrubitionMarginale(DM1,1);
	
	// Calcul de la matrice stochastique finale
	CDistributionCroiseeEx DXTheoriqueFinale;
	DXTheoriqueFinale.CreateNewDistributionCroiseeTheorique(distribMarginaleRedresse0,distribMarginaleRedresse1);

	// Calcul de la matrice stochastique Initiale (1)
	CDistributionCroiseeEx DXTheorique1;
	DXTheorique1.CreateNewDistributionCroiseeTheorique(DM0,DM1);

	// Calcul des coefficients relatifs (DXTheoriqueFinale/DXTheorique1)
	CalculDistributionCroiseeRedressementRelatif(DXTheorique1,DXTheoriqueFinale);

	// Calcul de la matrice stochastique Intermédiaire (2)
	CalculDistrubitionMarginale(DM0,0);
	CalculDistrubitionMarginale(DM1,1);
	DXTheorique1.CreateNewDistributionCroiseeTheorique(DM0,DM1);

	// Calcul des coefficients Absolu(DXTheoriqueFinale-DXTheorique1)
	CalculDistributionCroiseeRedressementAbsolu(DXTheorique1,DXTheoriqueFinale);
}

void CDistributionCroiseeEx::CalculDistributionCroiseeRedressementRelatif(CDistributionCroiseeEx & DXTheoriqueInitiale,CDistributionCroiseeEx & DXTheoriqueFinale)
{
	// pour chaque cellule de la matrice, on applique un coefficient
	for(int x=0;x<m_NbClasse1;x++)
	{
		for(int y=0;y<m_NbClasse2;y++)
		{
			double v=DXTheoriqueInitiale.GetValue(x,y);
			if(v>0)
				m_Distribution[x][y]*=DXTheoriqueFinale.GetValue(x,y)/v;
		}
	}
}

void CDistributionCroiseeEx::CalculDistributionCroiseeRedressementAbsolu(CDistributionCroiseeEx & DXTheoriqueInitiale,CDistributionCroiseeEx & DXTheoriqueFinale)
{
	// pour chaque cellule de la matrice, on applique une correction
	for(int x=0;x<m_NbClasse1;x++)
	{
		for(int y=0;y<m_NbClasse2;y++)
		{
			m_Distribution[x][y]+=DXTheoriqueFinale.GetValue(x,y)-DXTheoriqueInitiale.GetValue(x,y);
		}
	}
}


// Calcul d'un distribution croisée théorique
// méthode STOCHASTIQUE (epérance mathématique du modèle)
void CDistributionCroiseeEx::CreateNewDistributionCroiseeTheorique(CDistribution &DistributionMarginale0, CDistribution &DistributionMarginale1)
{
	CreateNewDistribution(DistributionMarginale0.GetNbElement(),DistributionMarginale1.GetNbElement());

	for(int x=0;x<DistributionMarginale0.GetNbElement();x++)
	for(int y=0;y<DistributionMarginale1.GetNbElement();y++)
	{
		SetValue(x,y,DistributionMarginale0.GetValue(x)*DistributionMarginale1.GetValue(y));
	}
	
	// Si l'on n'est pas en base 1 il faut redresser
	double sigma=DistributionMarginale0.CalculCouverture_nPlus(0);
	// On peut aussi vérifier que c'est la même base dans les deux distributions
	ASSERT(sigma>DistributionMarginale1.CalculCouverture_nPlus(0)-0.00001);
	ASSERT(sigma<DistributionMarginale1.CalculCouverture_nPlus(0)+0.00001);

	if(sigma<.999999999999999 || sigma>1.000000000001)
		(*this)/=sigma;
}

void CDistributionCroiseeEx::CalculDistrubitionMarginale(CDistribution & distribmarginale,const int Dimension)
{
	ASSERT(Dimension==0 || Dimension==1);

	// On se prépare une distribution marginale de la bonne taille
	distribmarginale.CreateNewDistribution(GetNbElement(Dimension));

	// Initialisation (Mise à 0) des éléments
	distribmarginale.Initialise();

	if(Dimension==0)
	{
		for(int x=0;x<m_NbClasse1;x++)
		{
			for(int y=0;y<m_NbClasse2;y++)
			{
				distribmarginale.AddValue(x,m_Distribution[x][y]);
			}
		}
	}
	else
	{
		for(int x=0;x<m_NbClasse1;x++)
		{
			for(int y=0;y<m_NbClasse2;y++)
			{
				distribmarginale.AddValue(y,m_Distribution[x][y]);
			}
		}
	}
}

