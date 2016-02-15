// SupportInternet.h: interface for the CSupportInternet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPPORTINTERNET_H__2CCFC61A_2FD7_4503_8552_CDB7444066B7__INCLUDED_)
#define AFX_SUPPORTINTERNET_H__2CCFC61A_2FD7_4503_8552_CDB7444066B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSupportInternet  
{
public:
	CSupportInternet();
	virtual ~CSupportInternet();

	const int operator==(const  CSupportInternet & Data)
	{
		if((m_NrSite==Data.m_NrSite)&&(m_Jour=Data.m_Jour))return 1;
		else return 0;
	};
	CSupportInternet & operator = (const CSupportInternet & Data)
	{
		m_NrSite = Data.m_NrSite;
		m_Jour = Data.m_Jour;
		return *this;
	}
	CSupportInternet(const  CSupportInternet & Data)
	{
		*this=Data;
	};

	short m_NrSite;
	short m_Jour;
};

#endif // !defined(AFX_SUPPORTINTERNET_H__2CCFC61A_2FD7_4503_8552_CDB7444066B7__INCLUDED_)
