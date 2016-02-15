// TableMEDIA.h: interface for the CTableMEDIA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEMEDIA_H__2113CC08_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_TABLEMEDIA_H__2113CC08_5C6A_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTableMEDIA  
{
public:
	CTableMEDIA();
	virtual ~CTableMEDIA();
	CTableMEDIA & operator=(const CTableMEDIA &Data);
	CTableMEDIA(const CTableMEDIA &Data);

	char m_Nom; // T, R, M, C .....
	CString m_Libelle;
};

typedef CArray<CTableMEDIA, CTableMEDIA&> CTableMediaArray;
#endif // !defined(AFX_TABLEMEDIA_H__2113CC08_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
