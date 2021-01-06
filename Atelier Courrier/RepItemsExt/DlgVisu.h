#pragma once

#include "Resizer.h"
#include "QuickList.h"
#include "Defines.h"
#include "label.h"
#include "BtnST.h"

class CDlgItems;

#define WM_USER_VISU_CLOSE (WM_USER + 111)

// CDlgVisu dialog
class CVisuItems
{
public:
	CVisuItems() { m_NbCas = -1; } ;
	int m_Level;
	int m_NbCas;
	double m_Effective;
	double m_EffectiveTotal;
	CString m_Label;
};

class CDlgVisu : public CDialog
{
	DECLARE_DYNAMIC(CDlgVisu)

public:
	// CDlgVisu(int ShowResults, CWnd* pParent = NULL);   // standard constructor
	CDlgVisu(int ShowResults, CDlgItems * pParent);

	virtual ~CDlgVisu();
	virtual BOOL OnInitDialog();
	/// Sets up colors for all controls
	virtual void SetColors();
	virtual void SetData(JList<CVisuItems> &Items);

// Dialog Data
	enum { IDD = IDD_RI_DLG_VISU };

protected:
	JList<CVisuItems>		m_Items;
	CLabel					m_Box;
	CLabel					m_Label;
	CQuickList				m_List;
	CButtonST				m_But_Exit;
	CButtonST				m_But_Imp;
	CToolTipCtrl			m_ToolTip;
	/// Resize class
	CResizer				m_Resizer;
	/// Color Scheme
//	CATPColors::APPCOLOR			m_CATPColorsApp;
	int                     m_ShowResults;

	// Pointeur sur fenêtre des items en construction
	CDlgItems				* m_pDlgItems;
	
	// Export Construction Item Echelle dans un fichier
	void OnExportItem(JList<JInt32> &Values);

	// Export de l'entete création d'un item
	void ExportEntete(JList<JInt32> &Values, CString &TxtMacro);

	// Export des composants de  l'item
	void ExportItem(CString &TxtMacro);

	// Passe le descriptif item dans le clipboard
	bool Clipboard(CString DescriptifItem);

	// Impression possible
	bool ImpressionPossible();

	// Sets up control fonts etc...
	virtual void SetupControls();
	virtual void SetWindowRelations();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnExit();
	afx_msg void OnSysColorChange();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnGetListItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedRiBtnImpitem();
	afx_msg void OnLvnItemchangedRiListItems(NMHDR *pNMHDR, LRESULT *pResult);
};
