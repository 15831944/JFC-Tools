// TomyConsolideMediadataMediacabsat.h: interface for the CTomyConsolideMediadataMediacabsat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMYCONSOLIDEMEDIADATAMEDIACABSAT_H__4F9EBFBC_581C_4A39_ABFF_9A2C0DDD8BA9__INCLUDED_)
#define AFX_TOMYCONSOLIDEMEDIADATAMEDIACABSAT_H__4F9EBFBC_581C_4A39_ABFF_9A2C0DDD8BA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TomyMediadata.h"
#include "TomyCroiseeMediacabsat.h"
#include "TomyConsolide.h"
#include "LiaisonChaineTeleFrance.h"


class CTomyConsolideMediadataMediacabsat  : public CTomyConsolide
{
public:
	void EtalonneSousGRPCabsatAvecLesSousGRPHertzien();
	void CalculCoeffCorrectionOffreElargie(int popEquipeTV, int popEquipeCabsat);
	CTomyConsolideMediadataMediacabsat();
	virtual ~CTomyConsolideMediadataMediacabsat();

	// Implémentation de l'interface Tomy lorsque CTomyConsolide ne convient pas
	void InitialisePanel(LPCSTR panel,LPCSTR etude);
	double LanceEvalDistrib();
	double LanceEval(){return LanceEvalDistrib();};

	// ----------------------------------------------
	// Cas particulier, il ne faut pas compter 2 fois les spots hertzien
	void InitialiseSpot(); 
	int GetNbSpot(){return m_NbSpot;};
	// ----------------------------------------------
	// La population de la cible Médiacabsat est celle des initialisés Médiadata dans la cas du sur-écantillon
	void InitialiseCible(int NrCible);
	void InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2=-1,int NrModalite2=-1);
	// ----------------------------------------------
	// Le GRP des hertzien ne doit pas être compté 2 fois
	double GetGRP();
	double GetGRPOffreElargie();
	double GetGRPOffreRestreinte();
	double GetCouvertureOffreElargie();
	double GetCouvertureOffreRestreinte();
	double GetCouvertureValideMinMaxOffreElargie();
	double GetCouvertureValideMinMaxOffreRestreinte();
	// ----------------------------------------------
	// il faut calculer les PMG sur le Médiadata, puis appliquer les ratios sur le Médiacabsat
	void ActiveFiltreIndividusStrateConsomation(int NrStrate);
	// ----------------------------------------------
	// Le nombre de cas (cas particulier)
	int GetNbCas();
	int GetNbCasMediadata();
	int GetNbCasMediacabsat();
	// ----------------------------------------------
	// Lecture des liaisons
	void SetParametres(CTomyParam * param);
	// ----------------------------------------------
	void InitialisePanelMediadata(LPCSTR panel);
	void InitialisePanelMediacabsat(LPCSTR panel);
	void InitialisePanelMediadataETMediacabsat(LPCSTR panelMediadata,LPCSTR panelMediacabsat);
	// ----------------------------------------------
	void AddSpot(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion=1);
	void UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion);
	// ----------------------------------------------


	private:
	CTomyMediadata m_TomyMediadata;
	CTomyCroiseeMediacabsat m_TomyMediacabsat;
	bool m_flagMediadata;
	bool m_flagMediacabsat;
	CLiaisonChaineTeleFrance m_LienChaine;
	int m_NbSpot;

	bool m_flagCorrectionPopulationReferenceCabsat;
	double m_popEquipeTV;
	double m_popEquipeCabsat;
	CArray<double,double> m_ProportionRatioConsoDansOffreElargie;
};

#endif // !defined(AFX_TOMYCONSOLIDEMEDIADATAMEDIACABSAT_H__4F9EBFBC_581C_4A39_ABFF_9A2C0DDD8BA9__INCLUDED_)
