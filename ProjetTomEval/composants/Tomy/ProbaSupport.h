// ProbaSupport.h: interface for the CProbaSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROBASUPPORT_H__5E587943_726B_11D3_989D_F2A7B13AF30A__INCLUDED_)
#define AFX_PROBASUPPORT_H__5E587943_726B_11D3_989D_F2A7B13AF30A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef DllExport
#define DllExport
#endif

class DllExport CProbaSupport  
{
public:
	CProbaSupport();

	void AddNouveauCas(unsigned short NrIndividu,unsigned short Proba);
	void SetNbCas(unsigned short NbCas);

	virtual ~CProbaSupport();

public:
	unsigned short  m_NbCas;
	unsigned short* m_NrIndividu;
	unsigned short* m_Proba;

private:
	unsigned short m_NbCasAlloue;
};

class DllExport CEverReach  
{
public:
	CEverReach();

	void SetNbCas(unsigned short NbCas,unsigned short NrSta);
	
	virtual ~CEverReach();

public:
	unsigned short  m_NbCas;
	unsigned short* m_NrIndividu;

private:
	unsigned short m_NrStation;
};

#endif // !defined(AFX_PROBASUPPORT_H__5E587943_726B_11D3_989D_F2A7B13AF30A__INCLUDED_)
