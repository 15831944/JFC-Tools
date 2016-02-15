// ArbreREGIONS.h: interface for the CArbreREGIONS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARBREREGIONS_H__2113CC03_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_ARBREREGIONS_H__2113CC03_5C6A_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CArbreREGIONS  
{
public:
	CArbreREGIONS();
	virtual ~CArbreREGIONS();
	CArbreREGIONS & operator=(const CArbreREGIONS &Data);
	CArbreREGIONS(const CArbreREGIONS &Data);

	CArbreREGIONS * m_Tronc;  // NULL si Région mère
	CString m_LibRegionCourt;
	CString m_LibRegion;
	CString m_LibRegionLong;
	CString m_CodeZone; //'0' c'est la GGGGG.Mère
	short m_IndexRegion; // numéro Dynamique dans tables Région,Stations...(NE PAS MEMORISER)
	short m_NbBranche;
	CArbreREGIONS ** m_Branches; // alloué si NbBranche>0
	short m_IndexRegionReduit; // le ^m dans l'univers réduit... ou -1
};

typedef CArray<CArbreREGIONS,CArbreREGIONS&> CArbreRegionsArray;
#endif // !defined(AFX_ARBREREGIONS_H__2113CC03_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
