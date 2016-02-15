// TomyNet.h: interface for the CTomyNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMYNET_H__C0EE3F1D_E6F9_4BEC_B5D1_45CFC6F795A9__INCLUDED_)
#define AFX_TOMYNET_H__C0EE3F1D_E6F9_4BEC_B5D1_45CFC6F795A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cible.h"
#include "panelinternet.h"
#ifdef MODE_BANDEAUX
//#include "c:\dev\zoom\semainetypeInternet.h"

class CTomyNet  
{
public:

	// Initialisation des GRP de la semaine Internet
	void InitGRP(CSemaineTypeInternet * semtype);

	// GRP et Debit max pour un support
	double CalculGRPMax(int nrsite,int idxjour);
	double CalculDebitMax(int nrsite,int jour);

	double * ConstruitVecteurB(double probatouche, double nbcontactmoyen, double PdV);
	double * ConstruitVecteurBCaping(double probatouche, double nbcontactmoyen, double PdV);
	void CalculProduitVecteurB(double * couverture, double * VBf);
	double ** CalculDistributionIndividuelle(bool flag_Caping);
	double CalculGRPPlan();
	double CalculCouverture(bool flag_Caping);
	double CalculNbBandeauxCible();

	// GRP cible
	double CalculGRPSupport(int nrsite, int idxjour);
	double CalculGRPSupport(int nrsite);

	// Débits cible
	double CalculDebitCible();
	double CalculDebitCible(int nrsite);
	double CalculDebitCible(int nrsite, int idxjour);

	bool AjouteBandeauxIndexJour(int NrSite, int Jour,int NombreAchete);
	bool AjouteBandeauxMasqueJour(int NrSite, int Jour,int NombreAchete);

	// Initialisation plan
	bool InitialiseBandeaux();
	bool InitialiseNombreSemaine(int NbSemaine);
	bool InitialiseCibleEnsemble();
	bool InitialiseCible(int NumeroCible);
	bool InitialiseParametre(LPCSTR CheminData,int NbClasseMax=10);
	bool InitialisePanel(LPCSTR CodePanel);

	CTomyNet();
	virtual ~CTomyNet();

private:
	double CalculPdV(int NrSupport);
	CString m_CheminData;
	int m_NbClasseMax;
	CPanelInternet m_Panel;
	CCible  * m_Cible;
	CCible  * m_CibleEnsemble;

	class CSupportBandeaux:public CSupportInternet
	{
	public:
		int m_NbAchete;
		CPanelInternet::CSupportProba * m_SupportProba;

		// Operateur d'affectation
		CSupportBandeaux & operator=(const CSupportBandeaux & Data)
		{
			m_NbAchete = Data.m_NbAchete;
			m_SupportProba = Data.m_SupportProba;
			m_NrSite=Data.m_NrSite;
			m_Jour=Data.m_Jour;

			return * this;
		}

		// Copy constructor
		CSupportBandeaux(const  CSupportBandeaux & Data)
		{	
				*this=Data;
		};

		CSupportBandeaux(){};

	};

	class CPlanInternet
	{
	public:
		int m_NbSemaine;
		CArray<CSupportBandeaux,CSupportBandeaux&> m_ListeSupport;
		void Initialise(){m_ListeSupport.RemoveAll();};
		void AddBandeaux(CSupportBandeaux support)
		{
			m_ListeSupport.Add(support);
		};
		int GetNbSupport(){return m_ListeSupport.GetSize();};
	};

	CPlanInternet m_Plan;
	int m_NrInitialisation;
	double ** m_Distrib; // [Indiv][class]
};
#endif
#endif // !defined(AFX_TOMYNET_H__C0EE3F1D_E6F9_4BEC_B5D1_45CFC6F795A9__INCLUDED_)
