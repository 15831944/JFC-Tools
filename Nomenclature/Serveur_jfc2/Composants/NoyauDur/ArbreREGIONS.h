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

	CArbreREGIONS * m_Tronc;  // NULL si R�gion m�re
	CString m_LibRegionCourt;
	CString m_LibRegion;
	CString m_LibRegionLong;
	CString m_CodeZone; //'0' c'est la GGGGG.M�re
	short m_IndexRegion; // num�ro Dynamique dans tables R�gion,Stations...(NE PAS MEMORISER)
	short m_NbBranche;
	CArbreREGIONS ** m_Branches; // allou� si NbBranche>0
	short m_IndexRegionReduit; // le ^m dans l'univers r�duit... ou -1
};

typedef CArray<CArbreREGIONS,CArbreREGIONS&> CArbreRegionsArray;
#endif // !defined(AFX_ARBREREGIONS_H__2113CC03_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
