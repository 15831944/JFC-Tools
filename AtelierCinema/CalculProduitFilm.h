// CalculProduitFilm.h: interface for the CCalculProduitFilm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCULPRODUITFILM_H__81D21BF6_804E_4222_9C45_A5E4341F082C__INCLUDED_)
#define AFX_CALCULPRODUITFILM_H__81D21BF6_804E_4222_9C45_A5E4341F082C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CalculMemoDememo.h"

class CCalculProduitFilm  
{
public:
	CCampagneFilm * m_pCampagne;
	class CResultatPermormances
	{
	public :
		// Résultats calculés
		double m_Population;
		double m_BudgetBrut;
		double m_BudgetNet;

		// tickets
		double m_NbContacts;
		double GRP()
		{
			if (m_Population>0)return 100.0*m_NbContacts/m_Population;
			return 0;
		};
		double CPMbrut()
		{
			if (m_NbContacts>0)return 1000.0*m_BudgetBrut/m_NbContacts;
			return 0;
		};
		double CPMnet()
		{
			if (m_NbContacts>0)return 1000.0*m_BudgetNet/m_NbContacts;
			return 0;
		};

		// résultat de la couverture 
		double m_NbIndiv;
		double m_Couverture()
		{
			if (m_Population>0)return 100.0*m_NbIndiv/m_Population;
			return 0;
		};
		double m_Repetition()
		{
			if (m_NbIndiv>0)return m_NbContacts/m_NbIndiv;
			return 0;
		};

		// Résultats de la mémo démémo
		// ---------------------------
		CCalculMemoDememo::CResultat m_pResultatMemoDememo;

	};

	// Objet résultat pour le calcul du nombre d'entrée
	class CResultatNbEntree
	{
	public:
		// le nombre d'entrée par film et par semaine + totaux
		// Seul les individus appartenant à la cible sont considérés
		// m_NbEntreeCible[idxfilm][nbsemaine] = TOTAL PAR FILM
		// m_NbEntreeCible[NBfilm][idxsemaine] = TOTAL PAR SEMAINE
		// m_NbEntreeCible[NBfilm][nbsemaine] = TOTAL GENERAL
		
		double ** m_NbEntreeCible; //[film+total,semaine+total]

		// le nombre d'entrée par film
		// Tous les individus sont considérés
		// m_NbEntreeCible[NBfilm]= TOTAL DES ENTREES

		double * m_NbEntreeFilm; //[film+total]

		// Tarif pour chacun des films
		// m_Tarif[NBfilm]= TOTAL DES TARIFS

		double * m_Tarif; //[film+total]
	};

	// calcul de la couverture + mémo-démémo
	void CalculPerformances();

	// Calcul du nombre d'entrée
	CResultatNbEntree * CalculNbEntre();

	CCalculProduitFilm(CCampagneFilm * cmp/* pointeur sur la campagne*/);
	virtual ~CCalculProduitFilm();


	CResultatNbEntree m_ResultatNbEntree;
	CResultatPermormances m_ResultatPermormances;


private:
	double GetPopulationCibleBase(int base);
	// Matrice pour le calcul pour la mémo-démémo
	double ** m_CouvertureSemaine; //[semaine][classe]
	double ** m_CouvertureCumul;   //[semaine][classe]
	double ** m_NbTickets; //[film][semaine]

	// Calcul de la couverture 
	void CalculCouvertures();

	// Calcul du vecteur résultat: probabilité d'avoir vu exactement 0...20 films.
	void CalculProduitStatistique(double*** proba,double** couverture);

	// Calcul le produit de deux vecteurs binomiaux
	void CalculProduit(double  *VBf,double * couverture);

	// Calcul population cible
	int CalculPopCible(int IndexCible);
	int CalculPopulationSousCibleGeo (CByteArray * listebac,int idxsouscible);
	int CalculPopulationSegmentGeo (CByteArray * listebac,int idxsouscible);

	// Permet d'établir la liste des bacs actifs en analysant la composition en bac du produit
	// de chacun des produits film.
	// l'index de chaque bac utile pour le calcul est ajouté à cette liste
	void ConstruitListeBacsActifs(int idxfilm, CByteArray * listebac);

	// Construit la liste des bacs utilisés dans la campagne film
	// Utilisé pour la base de calcul régionale
	void ConstruitListeBacsActifs(CByteArray * listebac);

	// Calcul de la fréquentation sur la cible ensemble Fc = Somme(proba(i)*poids(i))
	double CalculFrequentationTotale(CByteArray * listebac);

	// Calcul de la fréquentation d'un segment
	double CalculFrequentationSegment(CByteArray * listebac,int idxsegment);

	// Calcul de la fréquentation de la cible active
	double CalculFrequentationCible(CByteArray * listebac);

	// Calcul de la fréquentation de la sous cible active
	// Seul les individus compris dans une modalité du critère courant sont pris en compte
	double CalculFrequentationSouscible (CByteArray * listebac,CProduitFilm * prodfilm,int idxsouscible);

	// Calcul de la somme des coefficients de sur fréquentation 
	double CalculSommeCoeffSouscible(CProduitFilm * filmprod);

	// Calcul de la proportion de sur fréquentation seconde par sous cible
	void CalculSurFrequentationParSousCible(CProduitFilm * filmprod, CArray<double,double&> * SFseconde,CByteArray * listeBacActif);

	// Calcul de la proportion de sur fréquentation seconde par segment
	void CalculSurFrequentationParSegment(CProduitFilm * filmprod, CArray<double,double&> * SFseconde,CByteArray * listeBacActif);

	// Calcul de la proportion de sur fréquentation seconde par segment ciblé
	void CalculSurFrequentationParSegmentCible(CProduitFilm * filmprod, CArray<double,double&> * SFseconde,CByteArray * listeBacActif);


};

#endif // !defined(AFX_CALCULPRODUITFILM_H__81D21BF6_804E_4222_9C45_A5E4341F082C__INCLUDED_)

