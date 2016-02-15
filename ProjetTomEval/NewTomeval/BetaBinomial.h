//
// Fichier: CBetaBinomial.h
// Auteur:  Sylvain SAMMURI
// Date:    09/05/2003
//

// on d�finit l'inclusion conditionnelle
#ifndef BETABINOMIAL_H
#define BETABINOMIAL_H

class CBetaBinomial
{
public:
	// le constructeur
	CBetaBinomial();

	// les fonctions pour calculer les �l�ments
	void SetItems(double A, double B, long N);
	void SetItems(double A, double B, double N); // avec interpolation lin�raire

	// la fonction pour r�cup�rer les �l�ments
	double Item(long K) const;

	// la fonction pour lib�rer les �l�ments
	void Reset();

	// le destructeur
	~CBetaBinomial();

private:
	// les fonctions pour calculer les �l�ments
	void OnMakeItems(double A, double B, long N, CArray<double, double &> & Items) const;
	void OnMakeItems(double A, double B, long N1, long N2, double N, CArray<double, double &> & Items) const;

private:
	// l'op�rateur pour recopier les �l�ments
	CBetaBinomial & operator =(const CBetaBinomial & Source);

	// le constructeur copie
	CBetaBinomial(const CBetaBinomial & Source);

private:
	CArray<double, double &> m_Items; // les �l�ments
};

// on met fin � l'inclusion conditionnelle
#endif
