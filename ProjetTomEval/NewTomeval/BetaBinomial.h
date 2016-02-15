//
// Fichier: CBetaBinomial.h
// Auteur:  Sylvain SAMMURI
// Date:    09/05/2003
//

// on définit l'inclusion conditionnelle
#ifndef BETABINOMIAL_H
#define BETABINOMIAL_H

class CBetaBinomial
{
public:
	// le constructeur
	CBetaBinomial();

	// les fonctions pour calculer les éléments
	void SetItems(double A, double B, long N);
	void SetItems(double A, double B, double N); // avec interpolation linéraire

	// la fonction pour récupérer les éléments
	double Item(long K) const;

	// la fonction pour libérer les éléments
	void Reset();

	// le destructeur
	~CBetaBinomial();

private:
	// les fonctions pour calculer les éléments
	void OnMakeItems(double A, double B, long N, CArray<double, double &> & Items) const;
	void OnMakeItems(double A, double B, long N1, long N2, double N, CArray<double, double &> & Items) const;

private:
	// l'opérateur pour recopier les éléments
	CBetaBinomial & operator =(const CBetaBinomial & Source);

	// le constructeur copie
	CBetaBinomial(const CBetaBinomial & Source);

private:
	CArray<double, double &> m_Items; // les éléments
};

// on met fin à l'inclusion conditionnelle
#endif
