#pragma once

#include "Resizer.h"
#include "ColSortKey.h"
#include "label.h"
#include "BtnST.h"
#include "QuickList.h"
#include "AnalyseurEnums.h"
#include "IItemTable.h"
#include "CrossTable.h"
#include "AnalyseurDoc.h"
#include <afxadv.h>      // For CSharedFile

// CDlgSurlignage dialog

class CDlgSurlignage : public CDialog
{
	DECLARE_DYNAMIC(CDlgSurlignage)

public:
	CDlgSurlignage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSurlignage();
// Dialog Data
	enum { IDD = IDD_AN_DLG_SURLIGNAGE };

	virtual BOOL OnInitDialog();
	/// Sets up colors for all controls
	virtual void SetColors();

	// virtual void SetButtons(CButtonST * pBut_Surlignage, CButtonST * pBut_Base100);
	void SetButtons(CButtonST * pBut_Surlignage, BOOL Base100);

	virtual void SetData(JVector<CTableKey> & Items, IItemTable * pIItemTable, RESULTTYPE SurlignageCalc, RESULTSTYLE SurlignageStyle, CAnalyseurDoc * pDoc);

	CLabel					m_Box;
	CLabel					m_Label;
	CQuickList				m_List;
	CLabel					m_InfoText;
	CButtonST				m_But_Exit;
	CToolTipCtrl			m_ToolTip;
	/// Resize class
	CResizer				m_Resizer;

	// The surlignage calculation base
	RESULTTYPE				m_SurlignageCalc;
	// The surlignage style
	RESULTSTYLE				m_SurlignageStyle;
	JVector<CTableKey>		m_Items;

	IItemTable *			m_pIItemTable;
		
	CAnalyseurDoc *			m_pDoc;

	CButtonST*				m_pBut_Surlignage;

	BOOL					m_Base100;

	CButtonST				m_But_Export;

	// Export format texte
	void ExportFormatTexte(CFile & exportFile);

	// Export des lignes dans fichier texte
	void ExportLignes(CFile * pFile);

	// Export dans clipboard
	bool CopyAllToClipboard();

	// Export pour macro exel
	bool Export(CFile & exportFile);

	// Mise à jour titre surlignage si fenêtre surlignage ouverte
	void UpdateTitre();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	/// Sets up control fonts etc...
	virtual void SetupControls();
	virtual void SetWindowRelations();

	// Renvoi la grandeur utilsée pour affichage lignes surlignages
	CString GrandeurCalcul();

	// Indicateur de calcul
	CString GetStrIndicateur();

	// Renvoi le style utilisé pour affichage lignes surlignages
	CString GetStrStyleCalcul();

	// Retaillage colonnes
	virtual void RetailleColonne();

	// Flags positions tris
	bool	m_Sort;

public:
	afx_msg void OnBnClickedAnButExit();
	afx_msg void OnSysColorChange();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnGetListItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedAnButExport();
	afx_msg void OnLvnColumnclickAnListItems1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT nIDEvent);
};
