// ResultatJour.h: interface for the CResultatJour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULTATJOUR_H__1DBDE455_5D9A_4E72_B8D4_2519547873A8__INCLUDED_)
#define AFX_RESULTATJOUR_H__1DBDE455_5D9A_4E72_B8D4_2519547873A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=3D9AB7D5003D
class CResultatJour  
{
public:
	//##ModelId=3D9AB7D50047
	CResultatJour()
	{
			m_Individu=0;
	};
	//##ModelId=3D9AB7D50051
	virtual ~CResultatJour(){};

	//##ModelId=3D9AB7D50053
	CString m_Libelle;
	//##ModelId=3D9AB7D5005B
	CString m_LibJour;
	
	//##ModelId=3D9AB7D5005C
	double m_Individu;

	//##ModelId=3D9AB7D50065
	CResultatJour& operator=(const CResultatJour & Data)
	{
		m_Libelle=Data.m_Libelle;
		m_LibJour=Data.m_LibJour;
		m_Individu=Data.m_Individu;
		return *this;
	};


private:

};

//##ModelId=3D9AB7D5006F
typedef CArray<CResultatJour,CResultatJour&>CTabResultatJour;

#endif // !defined(AFX_RESULTATJOUR_H__1DBDE455_5D9A_4E72_B8D4_2519547873A8__INCLUDED_)
