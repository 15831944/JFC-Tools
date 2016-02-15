// TableCIBLE.h: interface for the CTableCIBLE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLECIBLE_H__593EDCE1_5D29_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_TABLECIBLE_H__593EDCE1_5D29_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTableCIBLE  
{
public:
	CTableCIBLE();
	virtual ~CTableCIBLE();
	CTableCIBLE & operator=(const CTableCIBLE &Data);
	CTableCIBLE(const CTableCIBLE &Data);

	short m_Numero;
	short m_NrSource;
	short m_NrVague;
	short m_Occupe;
	short m_temoin;
	CString m_Blob;
	CString m_Nom;
};

typedef CArray<CTableCIBLE,CTableCIBLE&> CTableCibleArray;
#endif // !defined(AFX_TABLECIBLE_H__593EDCE1_5D29_11D2_B3F9_006052017E36__INCLUDED_)
