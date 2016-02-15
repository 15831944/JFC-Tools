// TableVAGUE.h: interface for the CTableVAGUE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLEVAGUE_H__2113CC0B_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_TABLEVAGUE_H__2113CC0B_5C6A_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTableVAGUE  
{
public:
	CTableVAGUE();
	virtual ~CTableVAGUE();
	CTableVAGUE & operator=(const CTableVAGUE &Data);
	CTableVAGUE(const CTableVAGUE &Data);

	short	m_Num;
	short	m_SourceT;
	long	m_NbInd;
	long	m_NbIndS;
	CString m_Nom;
	CString m_Titre;
	short	m_MaxCriteres;
	short	m_Occupe;
	COleDateTime m_DateDebut;		//debut du mois et annee de la vague
};

typedef CArray<CTableVAGUE, CTableVAGUE&> CTableVagueArray;
#endif // !defined(AFX_TABLEVAGUE_H__2113CC0B_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
