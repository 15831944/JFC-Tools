#if !defined(AFX_DLGREPCIBLE_H__BB9437A2_1510_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGREPCIBLE_H__BB9437A2_1510_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRepCible.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRepCible dialog

#include "obj_edit.h"
#include "dlgfonctioncible.h"

class CDlgRepCible : public CDialog
{
// Construction
public:
	void SetTitre();
	void AfficheResultatCible();
	void UpdateList1();
	void UpdateClasses();
	void UpdateSegment();
		
	CDlgRepCible(CWnd* pParent = NULL);   // standard constructor

	char fMemorise; // 1 si mémorisée


	int	NumCible;
	CString Nom;
	
	TabSegment Segment;
	short NrSegmentActif;
	
	int m_NrCritActif;



// Dialog Data
	//{{AFX_DATA(CDlgRepCible)
	enum { IDD = IDD_CIBLE };
	CObj_Edit	m_Edit1;
	CObj_Gray	m_Cadre3;
	CObj_Gray	m_Cadre2;
	CObj_Gray	m_Cadre1;
	CObj_LBox	m_List3;
	CListBox	m_List2;
	CListBox	m_List1;
	CString	m_PctUniCib;
	UINT	m_NbCasCib;
	CString	m_PctUniSeg;
	UINT	m_NbCasSeg;
	CString	m_Titre;
	CString	m_PopPhyCib;
	CString	m_PopPonCib;
	CString	m_PopPhySeg;
	CString	m_PopPonSeg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRepCible)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRepCible)
	afx_msg void OnCibleFermer();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList2();
	afx_msg void OnSelchangeList3();
	afx_msg void OnSelchangeList1();
	afx_msg void OnSegmentMoins();
	afx_msg void OnSegmentPlus();
	afx_msg void OnButtonEffacer();
	afx_msg void OnCibleNouvelle();
	afx_msg void OnCibleCharger();
	afx_msg void OnCibleMmoriser();
	afx_msg void OnCibleRenomer();
	afx_msg void OnCibleSupprimer();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkList2();
	afx_msg void OnDblclkList3();
	afx_msg void OnCibleMemoriserSous();
	afx_msg void OnUpdateCibleMemoriserSous(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HICON m_hIcon;
	bool MemoriseCible();
	bool IsMemoOK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREPCIBLE_H__BB9437A2_1510_11D4_989E_004095420987__INCLUDED_)
