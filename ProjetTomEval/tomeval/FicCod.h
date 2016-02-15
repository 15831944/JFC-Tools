// FicCod.h: interface for the CFicCod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICCOD_H__4A3748A1_FE67_11D3_989E_004095420987__INCLUDED_)
#define AFX_FICCOD_H__4A3748A1_FE67_11D3_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CFicCOD
{

public:
	DWORD TrouveRubricaMainFrame(DWORD Station,DWORD RubricaAdBox);
	CFicCOD();
	virtual ~CFicCOD();
	char m_fVagueLoaded;
	short LoadVague(LPCSTR NomFic);
	void FreeVague();

	int m_NbRubrica;
	CDWordArray m_RubricaMainFrame;
	CDWordArray m_RubricaAdBox;
	CDWordArray m_StationAdBox;
};

#endif // !defined(AFX_FICCOD_H__4A3748A1_FE67_11D3_989E_004095420987__INCLUDED_)
