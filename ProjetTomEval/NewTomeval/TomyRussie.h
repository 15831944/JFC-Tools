// TomyRussie.h: interface for the CTomyRussie class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMYRUSSIE_H__E53DA6C6_4035_451A_B5CD_579317BEC4EE__INCLUDED_)
#define AFX_TOMYRUSSIE_H__E53DA6C6_4035_451A_B5CD_579317BEC4EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "tomy.h"

class CTomyRussie : public InterfaceTomy
{
public:
	CTomyRussie();

public:
	virtual void SetParametres(CTomyParam * param);
	virtual	void InitialisePanel(LPCSTR panel,LPCSTR etude,int NbIndiv);
	virtual	void InitialiseCible(int NrCible);
	virtual void InitialiseCible(long * CoeffExCible);
	virtual void InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2=-1,int NrModalite2=-1);
	virtual int GetNbCas();
	virtual int GetPopulation();
	virtual void InitialiseSpot();
	virtual double LanceEvalDistrib();
	virtual double GetAsymptote();

#ifdef OPTION_DUREEECOUTE
	void AddSpot(short Station, int JourDate,short JourNomme,short QHeure,double GRP,short NbInsertion);
	void AddEvenement(short Station, int JourDate,short JourNomme,short HoraireDebut,short Duree,double GRP);
	void UpdateSpot(short Station, int JourDate, short JourNomme,short QHeure,double GRP,short NbInsertion);
#else
	void AddSpot(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion=1);
	void UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion);
#endif

	virtual	int GetNbSpot();
	virtual	long GetNbInsertions();
	virtual	double GetCouverture();
	virtual	double GetCouvertureValideBeta();
	virtual	double GetCouvertureValideMinMax();
	virtual double GetGRP();
	virtual int GetNbClasseResultat();
	virtual double GetExactementTouche(int categorie);
	virtual double GetCourbeReponse(int nbfoistouche);
	virtual void ActiveFiltreIndividusStrateConsomation(int NrStrate);
	virtual double LanceEval();
	virtual void SavePlanBeforeZoom();
	virtual void RestorePlanAfterZoom();

public:
	virtual ~CTomyRussie();

private:
	static CPanelLoader m_PanelLoader;

private:
	class CElement
	{
	public:
		short  m_NoStationJFC;
		int    m_JourDate;
		short  m_JourNomme;
		short  m_QHeure;
		bool   m_OptionAncrage;
		double m_GRP;
		short  m_NbInsertion;

	public:
		bool IsEquivalent(CElement * E)
		{
			if (m_NoStationJFC  != E->m_NoStationJFC)  return (false);
			if (m_JourDate      != E->m_JourDate)      return (false);
			if (m_JourNomme     != E->m_JourNomme)     return (false);
			if (m_QHeure        != E->m_QHeure)        return (false);
			if (m_OptionAncrage != E->m_OptionAncrage) return (false);
			if (m_GRP           != E->m_GRP)           return (false);
			return true;
		};
	};

	typedef CArray<CElement,CElement &> CTabElement;

private:
	class CAbaque
	{
	public:
		double m_Delta;
		double m_Alpha;
		double m_Beta;
		double m_Prob;
		double m_Disp;
	};

private:
	class CSupport
	{
	public:
		int m_IdxStation;
		int m_IdxQuartHeure;
	};

private:
	CFicProba* LoadPanel(LPCSTR Panel, LPCSTR Etude,int NbIndiv);

private:
	void BuildAsymptotes();
	void BuildMapStation();

private:
	void AllocIndivPourPanel();
	void CalculPopulation();
	void FreeAllocIndiv();
	void FreePanel();

private:
	bool AddMaille(CList<CSupport, CSupport &> & Maille, int IdxStation, int IdxQuartHeure);

private:
	double ExtrapoleAudience(double VolContactsDLY, double VolContactsWLY, double VolContactsMLY, double AudienceDLY, double AudienceWLY, double AudienceMLY, double VolContacts);

private:
	void DoEtalonnage(CAbaque & Abaque, double Asymptote, double Audience, double Audience7Jrs, double VolContacts, double* pDistribution, long KMax);

private:
	void DoFiche(int NbCas, double Effectif, double AudienceDLY, double AudienceWLY, double AudienceMLY, double Asymptote, double Audience, double VolContacts, double* pDistribution, long KMax, const CAbaque & Abaque);

private:
	CMap<short,short,int,int> m_MapStation;

private:
	long** m_AudienceDLY;
	bool** m_AudienceWLY;
	bool** m_AudienceMLY;

private:
	CTomyParam    m_Param;
	CString       m_CodePanel;
	CString       m_Etude;
	CFicProba*    m_FicProba;
	int           m_NbIndiv;
	int           m_NrCible;
	double*       m_CoeffEx;
	int           m_NbCas;
	double        m_Population;
	CTabElement   m_Plan;
	long          m_NbInsertions;
	double        m_GRP;
	double        m_Couverture;
	CDistribution m_Distribution;
	CTabElement*  m_PlanWhileZoom;
};

#endif // !defined(AFX_TOMYRUSSIE_H__E53DA6C6_4035_451A_B5CD_579317BEC4EE__INCLUDED_)
