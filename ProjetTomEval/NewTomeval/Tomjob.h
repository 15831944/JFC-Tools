// Tomjob.h: interface for the CTomjob class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMJOB_H__0250D9C9_FC07_4F5D_B9C4_672F526C807C__INCLUDED_)
#define AFX_TOMJOB_H__0250D9C9_FC07_4F5D_B9C4_672F526C807C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=3D9AB7D303A1
class CTomjob  
{
public:
	double CalculGRPDansJob(int NrJob);
	long CalculNbSpotDansJob(int NrJob);
	// Opérations
	//##ModelId=3D9AB7D303AB
	bool LectureFichier(LPCSTR NomFic);
	//##ModelId=3D9AB7D303AD
	bool LectureFichierCarat();
	//##ModelId=3D9AB7D303B5
	bool LectureFichierJFC();
	//##ModelId=3D9AB7D303B6
	bool LectureFichierMarginal(LPCSTR NomFic);
	//##ModelId=3D9AB7D303BF
	CTomjob();
	//##ModelId=3D9AB7D303C0
	virtual ~CTomjob();
	
	// Atributs
	//##ModelId=3D9AB7D303C2
	int m_NumeroCible;
	
	//##ModelId=3D9AB7D303C9
	int m_Beta;
	//##ModelId=3D9AB7D303CA
	int m_Mini;
	//##ModelId=3D9AB7D303D3
	int m_Maxi;

	DWORD m_PosFichierCIBJOB; // Exclusif format radio
	int m_NbIndiv; // Exclusif format radio

	//##ModelId=3D9AB7D303DD
	int m_NbVentilation; // Nombre de ventilations
	//##ModelId=3D9AB7D303E8
	CWordArray m_Ventilation; // index des critères ventilés

	//##ModelId=3D9AB7D40009
	CString m_CheminData;

	//##ModelId=3D9AB7D4000A
	int m_NbPlan;
	//##ModelId=3D9AB7D40013
	CString m_CodePanelMediadata; 
	//##ModelId=3D9AB7D40014
	CString m_CodePanelMediaCabSat;
	//##ModelId=3D9AB7D4001D
	int m_Population;
	//##ModelId=3D9AB7D40027
	int m_PopulationOffreElargie;

	//##ModelId=3D9AB7D40082
	class CSpotTomjob
	{
	public:
		//##ModelId=3D9AB7D4008C
		int m_Station;
		//##ModelId=3D9AB7D4008D
		int m_JourN;
		//##ModelId=3D9AB7D40096
		CString m_JourD;
		//##ModelId=3D9AB7D40097
		int m_horaire;
		//##ModelId=3D9AB7D400A0
		double m_GRP;
				
		bool m_OptionAncrage;
		//##ModelId=3D9AB7D400AB
		CWordArray m_NbInsertion;
		//##ModelId=3D9AB7D400B4
		CSpotTomjob & operator=(const CSpotTomjob & Data)
		{
			m_Station=Data.m_Station;
			m_JourN=Data.m_JourN;
			m_JourD=Data.m_JourD;
			m_horaire=Data.m_horaire;
			m_OptionAncrage=Data.m_OptionAncrage;
			m_GRP=Data.m_GRP;
			m_NbInsertion.Copy(Data.m_NbInsertion);
			return *this;
		};

	};
	//##ModelId=3D9AB7D400BE
	typedef CArray<CSpotTomjob,CSpotTomjob&> CPlanTomjob;
	//##ModelId=3D9AB7D40032
	CPlanTomjob m_Plan;

private:
	//##ModelId=3D9AB7D40036
	bool OuvertureFichier(LPCSTR NomFic);
	//##ModelId=3D9AB7D4003C
	bool LecturePlan();

	bool LecturePlanFormatRadioCIM();

	//##ModelId=3D9AB7D40045
	int GetIdxJourNomme(int jour);
	//##ModelId=3D9AB7D4004F
	bool ReadLine();
	//##ModelId=3D9AB7D40051
	CStdioFile m_File;
	//##ModelId=3D9AB7D40064
	CStringArray m_TabElement;
	//##ModelId=3D9AB7D4006E
	LPCSTR GetElement(LPCSTR Chaine,int position);
};

#endif // !defined(AFX_TOMJOB_H__0250D9C9_FC07_4F5D_B9C4_672F526C807C__INCLUDED_)
