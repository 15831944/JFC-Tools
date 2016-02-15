// Nomenclature.h : main header file for the NOMENCLATURE application
//

#if !defined(AFX_NOMENCLATURE_H__FD540A72_7E06_48DA_BC8D_987260763D83__INCLUDED_)
#define AFX_NOMENCLATURE_H__FD540A72_7E06_48DA_BC8D_987260763D83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "LockAccess.h"	// Added by ClassView

// Cryptage et decryptage mot de passe utilisateur
CString CodageMP(CString MotDePasse);
CString DecodageMP(CString MotCode);


/////////////////////////////////////////////////////////////////////////////
// CNomenclatureApp:
// See Nomenclature.cpp for the implementation of this class
//

class CNomenclatureApp : public CWinApp
{
public:
	bool m_ModeAjoutSeul;
	CString m_AnneeNomencl;
	CString m_PathNomenclAnnee;
	CString m_PathArtRadio;
	CString m_PathArtTele;
	CString m_TypeNomencl;

	// Infos pour impression
	CString m_PrintUser;
	CString m_PrintRaison;
	CString m_PrintLogo;

	CString m_PathNomencl;
	CString m_UtilTauxCur;

	// Les différents niveaux d'accés entrée informations
	int m_NiveauAcces;
	int m_NiveauEDI;

	CLockAccess m_Lock;
	CNomenclatureApp();

	CString GetRegData(CString key, CString value);

	enum {NBCODESTATION = 5};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNomenclatureApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNomenclatureApp)
	afx_msg void OnAppAbout();
	afx_msg void OnInfosAproposdenomenclature();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOMENCLATURE_H__FD540A72_7E06_48DA_BC8D_987260763D83__INCLUDED_)
