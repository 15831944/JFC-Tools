// Restom.h: interface for the CRestom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESTOM_H__A28C480A_82B0_4C39_B7ED_B7EB4DAAE55C__INCLUDED_)
#define AFX_RESTOM_H__A28C480A_82B0_4C39_B7ED_B7EB4DAAE55C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=3D9AB7D5007A
typedef CArray <double,double> CDoubleArray;
typedef CArray <int,int>       CIntArray;

#include "ResultatJour.h"
#include "Tomy.h"

//##ModelId=3D9AB7D500B5
class CRestom  
{
public:
	//##ModelId=3D9AB7D500BF
	void UpdateResultat(InterfaceTomy * pTomy,int Population);
	//##ModelId=3D9AB7D500CB
	bool EffaceFichier(LPCSTR NomFic);
	// Constructeur
	//##ModelId=3D9AB7D500D4
	CRestom();
	//##ModelId=3D9AB7D500D5
	virtual ~CRestom();

	// Opérations

	// résultat global

		// Résultat de la cible
	int m_Population;

	CIntArray m_NbCas;

	// Courbe de réponse utilisée
	CWordArray m_CourbeReponse;


	//##ModelId=3D9AB7D500D7
	int m_NbInsertion;
	//##ModelId=3D9AB7D500DD
	int m_Audience000;
	//##ModelId=3D9AB7D500E7
	double m_Couverture;
	//##ModelId=3D9AB7D500E8
	double m_CouvertureValideBeta;
	//##ModelId=3D9AB7D500F1
	double m_CouvertureValideXY;
	//##ModelId=3D9AB7D500FB
	int m_NbContacts000;
	//##ModelId=3D9AB7D500FC
	double m_GRP;
	//##ModelId=3D9AB7D50105
	double m_Repetition;
	//##ModelId=3D9AB7D5010F
	double m_GRPmoyen;

	// Résultat de la distribution des contacts
	//##ModelId=3D9AB7D5011A
	CDoubleArray m_DistributionExacte;
	//##ModelId=3D9AB7D5012E
	CDoubleArray m_DistributionCumulee;

	// Résultat des ventilations
	//##ModelId=3D9AB7D50137
	void InitResultatVentile(int NbVentilation);
	//##ModelId=3D9AB7D50139
	int m_NbVentilation;
	//##ModelId=3D9AB7D50142
	CDoubleArray * m_Population_Ventile;//[critère][modalite]
	CDoubleArray * m_NbCas_Ventile;//[critère][modalite]
	CStringArray m_LibelleCritere;//[critère]
	CStringArray * m_LibelleModalite;//[critère][modalite]
	//##ModelId=3D9AB7D5014D
	CDoubleArray * m_Couverture_Ventile; //[critère][modalite]
	//##ModelId=3D9AB7D50158
	CDoubleArray * m_GRP_Ventile;//[critère][modalite]
};


//##ModelId=3D9AB7D50173
class CRestomZoom:public CRestom
{
public:
	//##ModelId=3D9AB7D5017E
	void UpdateResultat(InterfaceTomy * pTomy,CRestom * pRestomAvant,double Population);
	//##ModelId=3D9AB7D50188
	bool EcritureFichier(LPCSTR NomFic);
private:
	//##ModelId=3D9AB7D50192
	double m_Population;
};

//##ModelId=3D9AB7D501AF
#ifdef OPTION_CARAT
#include "TomyConsolideMediadataMediacabsat.h"
class CRestomCarat:public CRestom
{
public :
	// PMG
	//##ModelId=3D9AB7D501D8
	CDoubleArray m_GRPPMG;
	//##ModelId=3D9AB7D501E2
	CDoubleArray m_CouverturePMG;

	// Apport par Jour semaine et semaine glissante
	//##ModelId=3D9AB7D501EC
	CTabResultatJour * m_TabJour;
	//##ModelId=3D9AB7D501F7
	CTabResultatJour * m_TabSemaine;
	//##ModelId=3D9AB7D50201
	CTabResultatJour * m_TabSemaineGlissante;

	//##ModelId=3D9AB7D5020A
	bool EcritureFichier(LPCSTR NomFic);
	void UpdateResultat(CTomyConsolideMediadataMediacabsat * pTomy,int Population);

	double m_GRP_offreEllargie;
	double m_Couverture_offreEllargie;
	double m_CouvValideXY_offreEllargie;
	double m_GRP_offreRestreinte;
	double m_Couverture_offreRestreinte;
	double m_CouvValideXY_offreRestreinte;

};
#endif
//##ModelId=3D9AB7D50232
class CRestomJFC
{
public:
	//##ModelId=3D9AB7D5023C
	bool EcritureFichier(LPCSTR NomFic,bool flagFormatMarginal=false);
	//##ModelId=3D9AB7D5023E
	void SetSize(int nbplan)
	{
		FreeMemory();
		m_TabRestom=new CRestom[nbplan];
		m_NbRestom=nbplan;
	};
	//##ModelId=3D9AB7D50247
	CRestomJFC()
	{
		m_TabRestom=NULL;
		m_NbRestom=0;
	};
	//##ModelId=3D9AB7D50248
	~CRestomJFC()
	{
		FreeMemory();
	};
	//##ModelId=3D9AB7D50249
	bool EffaceFichier(LPCSTR NomFic)
	{
		CRestom obj;
		return(obj.EffaceFichier(NomFic));
	};
	
	//##ModelId=3D9AB7D50251
	CRestom * GetRestom(int idx)
	{
		return &(m_TabRestom[idx]);
	};

	
public:
	int m_Population;
	CIntArray  m_NbCas;
	CWordArray m_CourbeReponse;


private:
	//##ModelId=3D9AB7D5026E
	int m_NbRestom;
	//##ModelId=3D9AB7D50279
	CRestom * m_TabRestom;

	//##ModelId=3D9AB7D50282
	void FreeMemory()
	{
		if(m_TabRestom)
			delete [] m_TabRestom;
		m_TabRestom=NULL;
		m_NbRestom=0;
	};
};


#endif // !defined(AFX_RESTOM_H__A28C480A_82B0_4C39_B7ED_B7EB4DAAE55C__INCLUDED_)
