//==============================
// fichier: JFCGraduation.h
// 
// date: 18/02/2002
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est déjà définie
#pragma once 


class AFX_EXT_CLASS JFCGraduation
{
public:
	// le constructeur
	JFCGraduation();

	// la fonction pour fixer les bornes
	//void SetRange(double RangeMin, double RangeMax, long nbgraduation);
	void SetRange(double RangeMin, double RangeMax);

	// la fonction pour récupérer le nombre de graduations
	long GetNbGraduation();

	// la fonction pour récupérer une graduation d'aprés son indice
	double GetGraduation(long indice);

	// la fonction pour renvoyer le libelle de la graduation
	CString GetLibGraduation(long indice);

	// les fonctions pour renvoyer les bornes
	double GetMax();
	double GetMin();

	// la fonction pour vider le tableau des graduations
	void Reset();

	// le destructeur
	~JFCGraduation();

protected:
	// le tableau de graduations
	CArray<double, double &> m_Graduation;

	// la borne supérieure
	double m_RangeMax;

	// la borne inférieure
	double m_RangeMin;

private:
	// le nombre de décimales
	long m_Decimale;
};
