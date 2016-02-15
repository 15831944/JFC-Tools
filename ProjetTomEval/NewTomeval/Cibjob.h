// Cibjob.h: interface for the CCibjob class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIBJOB_H__7E300ACF_3766_4F10_943B_999A6A556E34__INCLUDED_)
#define AFX_CIBJOB_H__7E300ACF_3766_4F10_943B_999A6A556E34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCibjob  
{
public:
	int LoadFile(LPCSTR FileName,DWORD position,int NbIndiv,int NbVentil);
	CCibjob();
	virtual ~CCibjob();
private:
	void FreeFile();

public:
	int m_NbIndiv;
	int m_NbVentil;
	long * m_CoeffExCible; //[indiv]
	long * m_CoeffExEnsemble; //[indiv]
	char * m_NrRegion; //[indiv]
	char ** m_NrModalite;//[ventilation][indiv]

private:
	bool m_FileLoaded;
};

#endif // !defined(AFX_CIBJOB_H__7E300ACF_3766_4F10_943B_999A6A556E34__INCLUDED_)
