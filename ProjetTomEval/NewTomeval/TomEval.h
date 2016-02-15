// TomEval1.h: interface for the CTomEval class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMEVAL1_H__D1E2BC79_9FD2_417B_8D79_73A31B1BF566__INCLUDED_)
#define AFX_TOMEVAL1_H__D1E2BC79_9FD2_417B_8D79_73A31B1BF566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tomjob.h"
#include "restom.h"

#ifdef OPTION_CIMBELGE
#include "TomyCIMBelge.h"
#include "cibjob.h"
#elif OPTION_RUSSIE
#include "TomyRussie.h"
#include "TomyCIMRusse.h"
#include "cibjob.h"
#else
#include "tomy.h"
#include "tomyConsolideMediadataMediacabsat.h"
#include "..\COMPOSANTS\TOMY\Lien.h"	// Added by ClassView
#endif



//##ModelId=3D9AB7D402B2
class CTomEval  
{
public:
	//##ModelId=3D9AB7D402BC
	CTomEval();
	//##ModelId=3D9AB7D402BD
	virtual ~CTomEval();
	//##ModelId=3D9AB7D402C7
#ifdef OPTION_CIMBELGE
	bool LanceCalculClassique(LPCSTR NomFicTomjob,LPCSTR NomFicResTom,LPCSTR NomFicCibJob,bool flagMarginal=false);
#elif OPTION_RUSSIE
	bool LanceCalculClassique(LPCSTR NomFicTomjob,LPCSTR NomFicResTom,LPCSTR NomFicCibJob,bool flagMarginal=false);
#else
	bool LanceCalculClassique(LPCSTR NomFicTomjob,LPCSTR NomFicResTom,bool flagMarginal=false);
#endif

protected:
	int GetPopulationEquipeCabSat(LPCSTR CheminData,LPCSTR Etude,LPCSTR Vague,int NrCible);


protected:
	//##ModelId=3D9AB7D40303
	CTomjob m_Tomjob;
#ifdef OPTION_CARAT
	//##ModelId=3D9AB7D4030D
	CRestomCarat m_Restom;
	//##ModelId=3D9AB7D40317
	CLien m_LienCible;
	//##ModelId=3D9AB7D4032C
	CLien m_LienStation;
#elif OPTION_JFC
	CRestomJFC m_Restom;
#else
Erreur: option non définie
#endif

	//##ModelId=3D9AB7D40336
#ifdef OPTION_TELEFRANCE
	CTomyConsolideMediadataMediacabsat m_Tomy;
//	CTomy m_Tomy;
//	CTomyCroisee m_Tomy;
#endif
#ifdef OPTION_CIMBELGE
	CTomyCIMBelge m_Tomy;
	CCibjob m_Cibjob;
#elif OPTION_RUSSIE
	#ifdef OPTION_CIM_RUSSE
		CTomyCIMRusse m_Tomy;
		CCibjob m_Cibjob;
	#else
		CTomyRussie m_Tomy;
		CCibjob m_Cibjob;
	#endif
#endif

	//##ModelId=3D9AB7D4033F
	bool AddEcranDansTomy(int ecran,int sousplan);
	bool AddEcranSemaineDansTomy(int Semaine,int sousplan,int ecran);

};

// Un objet par calcul marginal
#ifndef OPTION_CARAT
//##ModelId=3D9AB7D40371
class CTomEvalMarginal:public CTomEval
{
public:
	//##ModelId=3D9AB7D40385
	bool EcritureRestom();
	//##ModelId=3D9AB7D40386
	int m_NrLien;
	//##ModelId=3D9AB7D4038F
	CString m_NomFicTomjob;
	//##ModelId=3D9AB7D40390
	CString m_NomFicRestom;

	CString m_NomFicCibjob;

	//##ModelId=3D9AB7D40399
	bool LanceCalculBaseMarginal();
	//##ModelId=3D9AB7D4039A
	bool LanceCalculAddSpot();
	//##ModelId=3D9AB7D403A3
	bool LanceCalculAddZoom();

private:
	//##ModelId=3D9AB7D403A4
	bool UpdateEcranDansTomy(int ecran,int sousplan);
	//##ModelId=3D9AB7D403AF
	bool LanceCalculMarginal();
};

// Cette classe gère tous les calcul "marginal" en cours
//##ModelId=3D9AB7D403CB
class CCalculMarginal
{
public:
	//##ModelId=3D9AB7D403D5
	CCalculMarginal()
	{
	};

	//##ModelId=3D9AB7D403DF
	~CCalculMarginal()
	{
		for(int i=m_TabCalculMarginal.GetSize()-1;i>=0;i--)
		{
			RemoveJobAtIndex(i);
		}
	};

public:
	//##ModelId=3D9AB7D403E0
	int GetNrLien(LPCSTR NomFicTomjob,LPCSTR NomFicRestom)
	{
		int NrLien;
		for(NrLien=1;NrLien<99;NrLien++)
		{
			if(FindIndex(NrLien)==-1)break;
		}
		ASSERT(NrLien<99);
		CTomEvalMarginal * tom=new CTomEvalMarginal;
		tom->m_NrLien=NrLien;
		tom->m_NomFicTomjob.Format("%s%02d",NomFicTomjob,NrLien);
		tom->m_NomFicRestom.Format("%s%02d",NomFicRestom,NrLien);
		tom->m_NomFicCibjob.Format("CIBJOB%02d",NrLien);
		m_TabCalculMarginal.Add(tom);
		return(NrLien);
	};
	//##ModelId=3D9AB7D50002
	void ReleaseNrLien(int NrLien)
	{
		int idx=FindIndex(NrLien);
		RemoveJobAtIndex(idx);
	};

	//##ModelId=3D9AB7D5000B
	void CalculJob(int NrLien)
	{
		int idx=FindIndex(NrLien);
		if(idx>=0)
		{
			CTomEvalMarginal * tom=m_TabCalculMarginal[idx];
			tom->LanceCalculBaseMarginal();
		}
		else ASSERT(0);
	};

	//##ModelId=3D9AB7D5000D
	void CalculAddSpot(int NrLien)
	{
		int idx=FindIndex(NrLien);
		if(idx>=0)
		{
			CTomEvalMarginal * tom=m_TabCalculMarginal[idx];
			tom->LanceCalculAddSpot();
		}
		else ASSERT(0);
	};

	//##ModelId=3D9AB7D50015
	void CalculZoom(int NrLien)
	{
		int idx=FindIndex(NrLien);
		if(idx>=0)
		{
			CTomEvalMarginal * tom=m_TabCalculMarginal[idx];
			tom->LanceCalculAddZoom();
		}
		else ASSERT(0);
	};

	//##ModelId=3D9AB7D50017
	void WriteRestom(int NrLien)
	{
		int idx=FindIndex(NrLien);
		if(idx>=0)
		{
			CTomEvalMarginal * tom=m_TabCalculMarginal[idx];
			tom->EcritureRestom();
		}
		else ASSERT(0);
	}


private:
	//##ModelId=3D9AB7D50021
	CArray<CTomEvalMarginal *,CTomEvalMarginal *>m_TabCalculMarginal;
	//##ModelId=3D9AB7D50029
	void RemoveJobAtIndex(int idx)
	{
		CTomEvalMarginal * tom=m_TabCalculMarginal[idx];
		delete tom;
		m_TabCalculMarginal.RemoveAt(idx);
	};
	//##ModelId=3D9AB7D50034
	int FindIndex(int NrLien)
	{
		for(int i=0;i<m_TabCalculMarginal.GetSize();i++)
			if(m_TabCalculMarginal[i]->m_NrLien==NrLien)return i;

		return -1;
	}
};
#endif
#endif // !defined(AFX_TOMEVAL1_H__D1E2BC79_9FD2_417B_8D79_73A31B1BF566__INCLUDED_)
