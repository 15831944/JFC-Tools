// PanelInternet.h: interface for the CPanelInternet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANELINTERNET_H__41CDAD94_67A1_464A_B5E6_8532E0FE284C__INCLUDED_)
#define AFX_PANELINTERNET_H__41CDAD94_67A1_464A_B5E6_8532E0FE284C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "supportinternet.h"


class CPanelInternet  
{
public:
	bool LoadPanel(LPCSTR CheminData,LPCSTR CodeVague);
	bool LoadPanel(LPCSTR NomFic);
	CPanelInternet();
	virtual ~CPanelInternet();
	// Destruction des variables
	void UnloadPanel();

	class CProbaIndiv
{
public:
	unsigned short  m_NrIndiv;
	unsigned short  m_Quantum;

	CProbaIndiv & operator=(const  CProbaIndiv & Data)
	{
		m_NrIndiv=Data.m_NrIndiv;
		m_Quantum=Data.m_Quantum;
		return *this;
	};

	// copy constructor
	CProbaIndiv(const CProbaIndiv & Data)
	{
		*this=Data;
	};

	CProbaIndiv()
	{
	};

	unsigned short GetNbSemaineTouche()
	{
		return ((m_Quantum&57344)/8192);
	};
	unsigned short GetNbContact()
	{
		return (m_Quantum&8191);
	};
};

	class CSupportProba: public CSupportInternet
	{
	public:
		short m_NbSemaine;

		CArray<CProbaIndiv,CProbaIndiv&> m_Probabilite;

		CSupportProba & operator=(const  CSupportProba & Data)
		{
			m_NrSite=Data.m_NrSite;
			m_Jour=Data.m_Jour;
			m_NbSemaine=Data.m_NbSemaine;
			m_Probabilite.Copy(Data.m_Probabilite);
			return *this;
		};

		CSupportProba()
		{
			int h=0;
		};

		// copy constructor
		CSupportProba(const  CSupportProba & Data)
		{	
				*this=Data;
		};

		int GetNbIndivTouche()
		{
			return m_Probabilite.GetSize();
		};
		unsigned short GetNrIndivTouche(unsigned short individu)
		{
			return m_Probabilite[individu].m_NrIndiv;
		};
		double GetProbabiliteTouche(int individu)
		{
			double nbsemainetouche = m_Probabilite[individu].GetNbSemaineTouche();

			if (nbsemainetouche>m_NbSemaine)
				nbsemainetouche=m_NbSemaine;

			return (double)nbsemainetouche/(double)m_NbSemaine;
		};

		double GetNbContactProbabilise(int individu)
		{
			return (double)m_Probabilite[individu].GetNbContact()/(double)m_NbSemaine;
		};

		double GetNbFoisToucheMoyen(int individu)
		{
			double proba=GetProbabiliteTouche(individu);
			// TODO MODIF DONNEES
			if (proba==0) 
			{
				return 0.0;
				ASSERT(0);
			}
			if(proba>1)
			{
				proba=1;
				ASSERT(0);
			}
			return GetNbContactProbabilise(individu)/proba;
		};
	};
	CSupportProba * RechercheSupportProba(CSupportInternet support);

	// Les données des sites (les supports internet)
	CArray<CSupportProba,CSupportProba &> m_SupportProba;

	CString m_CodePanel;
	int m_fVagueLoaded;
	int m_NbIndiv;

private:
	bool ReadNbIndividuPanel(LPCSTR CheminData);
};

#endif // !defined(AFX_PANELINTERNET_H__41CDAD94_67A1_464A_B5E6_8532E0FE284C__INCLUDED_)
