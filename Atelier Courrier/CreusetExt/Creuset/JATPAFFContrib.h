//
// Fichier: JATPAFFContrib.h
// Auteur:  Jérémy Devilers
// Date:    19/12/2003
//

#pragma once

// on inclut les définitions nécessaires
#include "JLib.h"
#include "PlanInsertions.h"

class JATPAFFContrib
{
public:
	// le constructeur
	JATPAFFContrib();

	//void SetData( JFlt64 contrib1, JFlt64 xGRP, JFlt64 coutGRP);

	void SetVisible(JBool bVisible);
	JBool IsVisible();

	// la fonction pour réinitialiser les éléments
	JVoid Reset();

	// le destructeur
	~JATPAFFContrib();

private:
	// l'opérateur pour recopier l'élément
	JATPAFFContrib & operator =(const JATPAFFContrib & Source);

	// le constructeur copie
	JATPAFFContrib(const JATPAFFContrib & Source);

friend class CATPContrib_A1;
friend class CATPContrib_A2;
friend class CATPCreusetHypo_C2;

	//les données
	
	JFlt64 m_ContribAdd;
	JFlt64 m_XGRPAdd;
	JFlt64 m_coutGRPAdd;
	JFlt64 m_repriseAdd;

	JFlt64 m_ContribSupp;
	JFlt64 m_XGRPSupp;
	JFlt64 m_coutGRPSupp;
	JFlt64 m_repriseSupp;

	JFlt64 m_PartBudget;
	JFlt64 m_Prix;
	JFlt64 m_PartXGRP;
	JFlt64 m_XGRP;


	JBool m_bVisible;

	int m_idHypo;
	int	m_idFormat;
	int m_idSupport;

};

class CKeyContrib
{
public:
	// les constructeurs
	CKeyContrib();
	CKeyContrib(const CKeyContrib & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	CKeyContrib & operator =(const CKeyContrib & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const CKeyContrib & Reference) const;
	JBool operator >(const CKeyContrib & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~CKeyContrib();

public:
	JInt32	m_Id1;	// l'identifiant du Support
	JInt32	m_Id2;	// l'Identifiant du format	
	JInt32	m_Id3;	// l'Identifiant du Hypo	
};


class JATPAFFContrib_New
{
public:
	// le constructeur
	JATPAFFContrib_New();

	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);
	JVoid Update(const JFCUpdate & Message);

	void SetVisible(JBool bVisible);
	JBool IsVisible();

	// la fonction pour réinitialiser les éléments
	JVoid Reset();

	// le destructeur
	~JATPAFFContrib_New();

	// Type de contribution
	enum TYPE{COUV=0, GRP, PARTGRP, BUDGET, PARTBUDGET, CTCOUV};
	void SetType(TYPE type);
	TYPE m_type;

	// Mode affichage contribution (Par Blocs de supports, ou par blocs de grandeur identiques)
	enum MODEAFFICH{AFFICH_BLOC_SUPPORTS = 0, AFFICH_BLOC_GRANDEURS};
	void SetModeAffich(MODEAFFICH ModeAffichage);
	int GetModeAffich();
	MODEAFFICH m_ModeAffich;
	
	CString GetValue(int idHypo, int idFormat, int idSupport) const;

	// Renvoie le map des différentes valeurs
	JMap<JUnt32, CString> & GetMapValue(int idHypo, int idFormat, int idSupport);

	CString GetTypeStringContrib() const;
	CString GetTypeString() const;
	CString ExportContrib(int nHypo);

	CString GetValue(int idHypo, int idFormat, int idSupport, TYPE type) const;

	void ShowCouv(bool bShow);
	void ShowGRP(bool bShow);
	void ShowPGRP(bool bShow);
	void ShowBudg(bool bShow);
	void ShowPBudg(bool bShow);
	void ShowCtCouv(bool bShow);

	bool CanCouv();
	bool CanGRP();
	bool CanPGRP();
	bool CanBudg();
	bool CanPBudg();
	bool CanCtCouv();

	int NbValue() const;

private:

	// l'opérateur pour recopier l'élément
	JATPAFFContrib_New & operator =(const JATPAFFContrib_New & Source);

	// le constructeur copie
	JATPAFFContrib_New(const JATPAFFContrib_New & Source);

	//les données
	struct Contrib
	{
		double nbInd;
		double nbIndTot;
		double cts;
		double ctsTot;
		double effectif;
		double budget;
		double budgetTot;
		
		void Reset() {
			nbInd = nbIndTot = cts = ctsTot = effectif = budget = budgetTot = -1.0;
		}

		void SetNotDefined() {
			nbInd = nbIndTot = cts = ctsTot = effectif = budget = budgetTot = -999.0;
		}

		double Couv() const {
			if(effectif > 0.0)
				return 100.0 * nbInd / effectif ;
			if (effectif < -1)
				return effectif;

			return -1.0;
		};

		double GRP() const {
			if(effectif > 0.0)
				return 100.0 * cts / effectif;
			if (effectif < -1)
				return effectif;

			return -1.0;
		};

		double PartGRP() const{
			if(ctsTot > 0.0)
				return 100.0 * cts / ctsTot;
			if (ctsTot < -1)
				return ctsTot;
			return -1.0;
		};
		double Budget() const{
			return budget;
		};
		double PartBudget() const{
			if(budgetTot > 0.0)
				return 100.0 * budget / budgetTot;
			if(budgetTot < -1.0)
				return budgetTot;
			return -1.0;
		};
		double CoutPtCouv() const{
			if(Couv() > 0.0)
				return budget/ Couv();
			if(Couv() < -1.0)
				return Couv();
			return -1.0;
		};
		
	};

	void ComputeContrib(LONG i, LONG ii, LONG j, Contrib &contrib);
	JMap<CKeyContrib, Contrib> m_Contrib;
	
	// Map result par grandeur
	JMap<JUnt32, CString> m_MapTxtResult;

	JATPDocCreuset* m_pATPDocCreuset; // le document
	JMap<int,int> m_mapHypo;

	JBool m_bVisible;
	bool m_bUpdAll;
	bool m_bUpd;

	bool m_bShowCouv;
	bool m_bShowGRP;
	bool m_bShowPGRP;
	bool m_bShowBudg;
	bool m_bShowPBudg;
	bool m_bShowCtCouv;
};

