// TomyCIMRusse.h: interface for the CTomyCIMRusse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMYCIMRUSSE_H__6670E8C6_F014_45AE_A692_CD7A2EFF36BD__INCLUDED_)
#define AFX_TOMYCIMRUSSE_H__6670E8C6_F014_45AE_A692_CD7A2EFF36BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "tomy.h"

class CTomyCIMRusse : public InterfaceTomy
{
public:
	CTomyCIMRusse();

public:
	virtual void SetParametres(CTomyParam * param);
	virtual	void InitialisePanel(LPCSTR panel,LPCSTR etude);
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
	virtual ~CTomyCIMRusse();

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
	CFicProba* LoadPanel(LPCSTR Panel, LPCSTR Etude, int NbIndividu);

private:
	int GetNoSemaine(int JourDate);

private:
	int* CalculerSemaines(int & NbActives);

private:
	double fZeroFrequence(double x,double y,short z);

private:
	double  CalculerCouverture  (int N, double M, double W, double* SW);
	double* CalculerDistribution(int N, double SE, double SR);

private:
	void BuildMapStation();
#ifdef OPTION_NOVA_RUSSE
void CTomyCIMRusse::BuildAsymptotes();
#endif
private:
	void AllocIndivPourPanel();
	void CalculPopulation();
	void FreeAllocIndiv();
	void FreePanel();

private:
	CMap<short,short,int,int> m_MapStation;
	bool** m_AudienceWLY;
#ifdef OPTION_NOVA_RUSSE
	unsigned char * m_AudienceJRS;
	unsigned char *GetEcoutesJRS();
#endif


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

#endif // !defined(AFX_TOMYCIMRUSSE_H__6670E8C6_F014_45AE_A692_CD7A2EFF36BD__INCLUDED_)
