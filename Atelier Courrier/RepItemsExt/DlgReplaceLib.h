#pragma once

#include "label.h"
#include "BtnST.h"


// Boîte de dialogue CDlgReplaceLib
// 
// Pour remplacer des chaines de caractères dans 1 ou plusieurs libellés items

class CDlgReplaceLib : public CDialog
{
	DECLARE_DYNAMIC(CDlgReplaceLib)

public:
	// Constructeur
	CDlgReplaceLib(CWnd* pParent = NULL);  

	// Initialisation boite de dialogue
	virtual BOOL OnInitDialog();

	CATPColors::APPCOLOR		m_CATPColorsApp;

	/// Dialog Data
	//{{AFX_DATA(CDlgLibelle)
	enum { IDD = IDD_RI_DLG_REMPLACE};
	
	CString		m_EditBoxRemplace;
	CString		m_EditBoxRemplacePar;
	//}}AFX_DATA


	// Destructeur
	virtual ~CDlgReplaceLib();

	// Liste des idents items à remplacer
	JList <JInt32> *m_pListIDs;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()

	// Les élmts controles
	CLabel		m_LabelRemplace;
	CLabel		m_LabelRemplacePar;
	CLabel		m_FondBox;

	// Les boutons de contrôles
	CButtonST	m_Button_OK;
	CButtonST	m_Button_Cancel;
	
protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
