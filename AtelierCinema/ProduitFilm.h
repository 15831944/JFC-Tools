// ProduitFilm.h: interface for the CProduitFilm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRODUITFILM_H__40E00B25_0BC6_40CA_93DB_750ACB7E4B71__INCLUDED_)
#define AFX_PRODUITFILM_H__40E00B25_0BC6_40CA_93DB_750ACB7E4B71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CProduitFilm  
{
public:

	CProduitFilm();
	virtual ~CProduitFilm();
	CProduitFilm(const CProduitFilm &ProduitFilm){ 
		*this = ProduitFilm;
	}		;
	CProduitFilm & operator=(const CProduitFilm &ProduitFilm){
		
		// Recopie des �l�ment Produit films
		m_Select			= ProduitFilm.m_Select;
		m_Libelle			= ProduitFilm.m_Libelle;
		m_CodeProduit		= ProduitFilm.m_CodeProduit;
		//m_Actif				= ProduitFilm.m_Actif; 
		m_Tarif				= ProduitFilm.m_Tarif;
		m_Debut				= ProduitFilm.m_Debut;
		m_NbSemaine			= ProduitFilm.m_NbSemaine; 
		m_EntreeCNC			= ProduitFilm.m_EntreeCNC ; 
		m_NomCritCoeff		= ProduitFilm.m_NomCritCoeff ;
		m_CodeCritCoeff		= ProduitFilm.m_CodeCritCoeff ;

		// Les tableaux
		m_TabCoeffUsure.Copy(ProduitFilm.m_TabCoeffUsure); 
		m_TabCoeffFreq.Copy(ProduitFilm.m_TabCoeffFreq); 
		m_TabCoeffPluriFreq.Copy(ProduitFilm.m_TabCoeffPluriFreq);

		return *this;
	}

	///////////////////// Informations concernant un produit film ////////////////////////
	//
	// Produit s�lectionn� Oui/Non
	BOOL m_Select;
	
	// Nom du produit film (en g�n�ral nom du film)
	CString m_Libelle;

	// Code produit (code du produit associ� au film)
	CString m_CodeProduit;

	// Film actif (s�lectionn� ou pas pour l'analyse / par d�faut Oui)
	BOOL m_Actif;

	// Tarif Film
	double m_Tarif;

	// D�but film
	COleDateTime m_Debut;

	// Nb de semaines
	int m_NbSemaine;

	// Donn�es Entr�es CNC
	long m_EntreeCNC; 

	// Tableau des coeffs usure (de 0 � 100%)
	CArray <float,float&> m_TabCoeffUsure; 

	// Nom crit�re pour coeff fr�q
	CString m_NomCritCoeff;

	// Code crit�re
	int m_CodeCritCoeff;

	// Tableau des coeffs fr�q. modalit�s
	CArray <float,float&> m_TabCoeffFreq; 

	// Tableau des coeffs fr�q. modalit�s
	CArray <float,float&> m_TabCoeffPluriFreq; 

};

typedef CArray<CProduitFilm,CProduitFilm&> CTableProduitFilmMere;

class CTableProduitFilm : public CTableProduitFilmMere
{
public :
	
	// Initialisation du tableau des produits films
	CTableProduitFilm();

	void InitTabProduitFilm();

	// Ajout d'un produit films
	bool AddFilm(CProduitFilm ProduitFilm);

	// Suppression d'un produit film
	void SuppFilm(CString LibelleFilm);

	// destruction tab produit films
	~CTableProduitFilm();

	
public :

	// N� crit�re courant pour application coeff Fr�q et Pluri Freq Tous Produits confondus
	int m_NrCritCoeff;

	// Nb modalit�s --- pour le crit�re ci-dessous
	int m_NbModaliteCoeff;
};	



#endif // !defined(AFX_PRODUITFILM_H__40E00B25_0BC6_40CA_93DB_750ACB7E4B71__INCLUDED_)
