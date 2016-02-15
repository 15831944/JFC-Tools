// FicIND.h: interface for the CFicIND class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICIND_H__4A3748A0_FE67_11D3_989E_004095420987__INCLUDED_)
#define AFX_FICIND_H__4A3748A0_FE67_11D3_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFicIND  
{
public:
	CFicIND();
	virtual ~CFicIND();
	char m_fVagueLoaded;
	short LoadVague(LPCSTR NomFic,int NbIndiv);
	void FreeVague();
	CDWordArray m_NrIndiv;
};

#endif // !defined(AFX_FICIND_H__4A3748A0_FE67_11D3_989E_004095420987__INCLUDED_)
