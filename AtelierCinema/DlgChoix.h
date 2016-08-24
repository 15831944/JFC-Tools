#if !defined(AFX_DLGCHOIX_H__8FF00AE8_855F_4485_8FEC_B8029E51A297__INCLUDED_)
#define AFX_DLGCHOIX_H__8FF00AE8_855F_4485_8FEC_B8029E51A297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ListBoxSelect.h"

// DlgChoix.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChoix dialog

class CDlgChoix : public CDialog
{
// Construction
public:
	bool SetListe(const CString regie="");
	CStringArray& GetLibelleAjoute();
	void SetLibelleAjoute(CStringArray& libelle);
	void SetType(int type);

	// Chargement de la liste des groupes de bacs
	bool SetListeGrpBacs();

	CDlgChoix(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChoix)
	enum { IDD = IDD_SELPRODBACCOMPLEXE };
	CButton			m_Seltout;
	CButton			m_Inverser;
	CButton			m_Cancel;
	CButton			m_OK;
	CStatic			m_Label;
	CStatic			m_Barre;
	CComboBox		m_ComboRegie;
	CObj_Gray		m_Cadre2;
	CObj_Gray		m_Cadre;
	CObj_Gray		m_Cadre7;
	CListBoxSelect	m_List;
	CListBoxSelect	m_List_Grp_Bacs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChoix)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChoix)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSeltout();
	afx_msg void OnInverser();
	afx_msg void OnFiltreRegie();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_Type;
	CStringArray m_LibelleAjoute;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHOIX_H__8FF00AE8_855F_4485_8FEC_B8029E51A297__INCLUDED_)
