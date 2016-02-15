// TableSOURCE.h: interface for the CTableSOURCE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLESOURCE_H__2113CC0A_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_TABLESOURCE_H__2113CC0A_5C6A_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTableSOURCE  
{
public:
	CTableSOURCE();
	virtual ~CTableSOURCE();
	CTableSOURCE & operator=(const CTableSOURCE &Data);
	CTableSOURCE(const CTableSOURCE &Data);

	short m_Num;
	CString m_Rep;
	CString m_Structure;
	CString m_Media;
	CString m_Role;
	CString m_Nature;
	CString m_Type;
	CString m_Date;
	CString m_Grp;
	CString m_NomLong;
	CString m_NomCourt;
//	REGION	*Region;
	short	m_Occupe;
	// ajouts remi:
	short m_fTypeUtilisation; 	// 1:Etalon 1/4h
							// 2:Référence 1/4h
							// 4:Etalon écrans
							// 8:Référence écrans
};

typedef CArray<CTableSOURCE, CTableSOURCE&> CTableSourceArray;
#endif // !defined(AFX_TABLESOURCE_H__2113CC0A_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
