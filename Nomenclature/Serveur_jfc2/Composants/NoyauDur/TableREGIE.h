// TableREGIE.h: interface for the CTableREGIE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEREGIE_H__2113CC05_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_TABLEREGIE_H__2113CC05_5C6A_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTableREGIE  
{
public:
	CTableREGIE();
	virtual ~CTableREGIE();
	CTableREGIE & operator=(const CTableREGIE &Data);
	CTableREGIE(const CTableREGIE &Data);
	
	short m_NrUnique; // numéro unique dans le marché
	CString m_Nom;
	CString m_Libelle;
	short m_Remise; // 0:NON 1:OUI (Remise professionnelle déduite du tarif Facial)
	short m_NrOrdre;
	CString m_Logo;
	COleDateTime m_DateDebut; // date du début de validité en nb de jours depuis 010180
	COleDateTime m_DateFin;   // date de fin de validité en nb de jours depuis 010180
};

typedef CArray<CTableREGIE, CTableREGIE&> CTableRegieArray;
#endif // !defined(AFX_TABLEREGIE_H__2113CC05_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
