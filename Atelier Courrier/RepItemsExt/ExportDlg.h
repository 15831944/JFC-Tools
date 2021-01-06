#pragma once
#include "JFC_MultiListBox.h"
#include "label.h"
#include "BtnST.h"

// CExportDlg dialog

class CExportDlg : public CDialog
{
	DECLARE_DYNAMIC(CExportDlg)

public:
	CExportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExportDlg();

// Dialog Data
	enum { IDD = IDD_RI_DLG_EXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()

public:

	virtual BOOL OnInitDialog(void);

	CATPColors::APPCOLOR					m_CATPColorsApp;
	CLabel							m_Label;
	CLabel							m_Box;
	CButtonST						m_Button_OK;
	CButtonST						m_Button_Cancel;
	JFC_MultiListBox<IItem const *>	m_ListItems;

	afx_msg void OnOk();
};
