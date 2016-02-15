// TableFAMILLE.h: interface for the CTableFAMILLE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEFAMILLE_H__2113CC07_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_TABLEFAMILLE_H__2113CC07_5C6A_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTableFAMILLE  
{
public:
	CTableFAMILLE();
	virtual ~CTableFAMILLE();
	CTableFAMILLE & operator=(const CTableFAMILLE &Data);
	CTableFAMILLE(const CTableFAMILLE &Data);

	short m_Numero;
	CString m_Nom;
	CString m_Libelle;
};

typedef CArray<CTableFAMILLE, CTableFAMILLE&> CTableFamilleArray;
#endif // !defined(AFX_TABLEFAMILLE_H__2113CC07_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
