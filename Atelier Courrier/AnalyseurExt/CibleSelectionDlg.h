#pragma once

//#include "ItemConstruit.h"
#include "Terrain.h"
#include <Label.h>
#include "BtnST.h"
#include "JFCSelListBox.h"
#include "AnalyseurDoc.h"

// CCibleSelectionDlg dialog

class CCibleSelectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CCibleSelectionDlg)

public:
	CCibleSelectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCibleSelectionDlg();
	virtual void SetupControls();
	virtual void SetColors();
	bool SetData(CAnalyseurDoc * pDoc);
//	bool GetData(JList<const CItemConstruit *> & ListItems);

// Dialog Data
	enum { IDD = IDD_AN_DLG_CIBLE };

protected:
	CAnalyseurDoc *					m_pDoc;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool							m_bCible;
	CTerrain *						m_pTerrain;
	CLabel							m_bkgCible;
	CLabel							m_ttlCible;
	JFCSelListBox 					m_List_Selected;
	JFCSelListBox					m_List_Cibles;
	CButtonST						m_But_Ok;
	CButtonST						m_But_Cancel;
	JList<const CItemConstruit *>	m_SelectedItems;
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLbnSelchangeAnListSelcible();
	afx_msg void OnLbnSelchangeAnListCibles();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
