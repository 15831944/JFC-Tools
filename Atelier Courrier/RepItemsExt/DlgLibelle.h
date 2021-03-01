#pragma once

#include "label.h"
#include "BtnST.h"

/**
 * \ingroup RepItems
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-09-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class CDlgLibelle : public CDialog
{
/// Construction
public:
	CDlgLibelle(CWnd* pParent = NULL);   /// standard constructor
	virtual BOOL OnInitDialog();

	CATPColors::APPCOLOR		m_CATPColorsApp;

/// Dialog Data
	//{{AFX_DATA(CDlgLibelle)
	enum { IDD = IDD_RI_DLG_LIBELLE };
	CString	m_Libelle;
	//}}AFX_DATA


/// Overrides
	/// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLibelle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    /// DDX/DDV support
	//}}AFX_VIRTUAL

/// Implementation
protected:
	CLabel		m_Label;
	CLabel		m_Box;
	CButtonST	m_Button_OK;
	CButtonST	m_Button_Cancel;

	/// Generated message map functions
	//{{AFX_MSG(CDlgLibelle)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
