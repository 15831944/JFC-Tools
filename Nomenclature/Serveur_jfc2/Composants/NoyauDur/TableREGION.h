// TableREGION.h: interface for the CTableREGION class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEREGION_H__2113CC09_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_TABLEREGION_H__2113CC09_5C6A_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTableREGION  
{
public:
	CTableREGION();
	virtual ~CTableREGION();
	CTableREGION & operator=(const CTableREGION &Data);
	CTableREGION(const CTableREGION &Data);

	short m_NrUnique; // pour DLL
	CString m_Code;
	CString m_Libelle;
	CString m_LibelleLong;
	CString m_LibelleCourt;
	CString m_LibelleMini;
};

typedef CArray<CTableREGION, CTableREGION&> CTableRegionArray;
#endif // !defined(AFX_TABLEREGION_H__2113CC09_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
