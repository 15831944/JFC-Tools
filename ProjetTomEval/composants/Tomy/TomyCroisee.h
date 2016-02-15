// TomyCroisee.h: interface for the CTomyCroisee class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMYCROISEE_H__61DEFD50_A073_4D7F_B6A6_5FD4DE6124A0__INCLUDED_)
#define AFX_TOMYCROISEE_H__61DEFD50_A073_4D7F_B6A6_5FD4DE6124A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tomy.h"
#include "DistributionCroisee.h"

class CTomyCroisee : public CTomy  
{
public:
	// Constructeur Destructeur
	CTomyCroisee();
	virtual ~CTomyCroisee();

	// Opérations
public:
	double LanceEvalDistributionCroisee();
	void AddSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion,BYTE Media);
	CDistribution & GetDistributionMedia0(){return(m_Distribution0);};
	CDistribution & GetDistributionMedia1(){return(m_Distribution1);};


protected:
	double CalculExactementToucheViaDistributionCroisee();

private:
	void CalculApportDansMatricesBinomiale(CElement & E);
	inline void InitialiseMatrice(double ** Matrice);
	inline void InitialiseMatriceTemporaire();
	inline void CalculApportDansMatriceBinomiale(int & NrIndiv,BYTE & Media);
	double CalculExactementToucheViaMatriceIndividuelle();
	double CalculExactementTouche();



	// Attributs
protected:
	CDistributionCroisee m_DistributionCroisee;

private:
	CDistribution m_Distribution0;
	CDistribution m_Distribution1;
	void CalculDistributionCroisee();
	void FreeMatriceBinomiale();
	void FreeAllocIndiv();

	void AllocMatriceBinomiale();
	double *** m_MB; // Matrice Binomiale par individu [indiv][classeMédia1][classeMédia2]
	double ** m_MBt; // Matrice Binomiale temporaire pendant calcul
	// NOTE: m_MB remplace CTomy::m_VB mais ce n'est pas codable en C++ (vive VB.NET)
	int ** m_NbMB; // Plurifréquentation des individus [indiv][Média]
	// NOTE: m_NbMB remplace CTomy::m_NbVB mais ce n'est pas codable en C++ (vive VB.NET)

};

#endif // !defined(AFX_TOMYCROISEE_H__61DEFD50_A073_4D7F_B6A6_5FD4DE6124A0__INCLUDED_)
