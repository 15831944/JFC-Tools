#pragma once
#include "afxwin.h"

#include "resource.h"
#include "IModalite.h"
#include "label.h"
#include "BtnST.h"
// #include "JFCSelListCtrl.h"
// #include "JFCListBox.h"
#include "JFCSelListBox.h"

// CDlgFiltre dialog

class CDlgFiltre : public CDialog
{
	DECLARE_DYNAMIC(CDlgFiltre)

public:
	CDlgFiltre(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFiltre();

	// Initialisation boite de dialogue
	virtual BOOL OnInitDialog();

	CATPColors::APPCOLOR		m_CATPColorsApp;
	CRect						m_PosDlg;

	// Liste des elmts en cours de filtrage
	JMap<JUnt32, JBool>			m_MapCodeFiltre;

	// Map des libellés secteurs
	JMap<JUnt32, CString>		m_MapLibFiltre; 

// Dialog Data
	enum { IDD = IDD_RI_FILTRE_MODA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CButtonST			m_ButtonCancel;
	CButtonST			m_ButtonOK;
	
	// CListBox			m_ListFiltre;
	// JFCListBox		m_ListFiltre;
	JFCSelListBox		m_ListFiltre;

	// JFCSelListCtrl		m_ListFiltreModa;
	CLabel				m_BoxFiltre;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRaz();
};
