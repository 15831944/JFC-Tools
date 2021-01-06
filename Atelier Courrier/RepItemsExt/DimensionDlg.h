#pragma once

#include "resource.h"
#include "IModalite.h"
#include "IBloc.h"
#include "ListBoxDimensions.h"
#include "label.h"
#include "BtnST.h"
#include "afxwin.h"
//#include "SupportTree.h"

#define WM_USER_DIMENSION_X_DLG_UPDATED (WM_USER + 100)
#define WM_USER_DIMENSION_X_DLG_DIMSEL (WM_USER + 102)

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
class AFX_EXT_CLASS CDimensionDlg : public CDialog
{
/// Construction
public:
	/// Set the data for this dialog
	void SetData(IQuestion::DIMTYPE Type, int Index, JVector<const IModalite *> & Dimension);
	void SetData(IQuestion::DIMTYPE Type, int Index, JList<const IModalite *> & Dimension);
	CDimensionDlg(bool bIcones = false ,bool bOperators = true, CWnd* pParent = NULL);   /// standard constructor
	void SetDimensionOperator(IBloc::OPERATOR Operator);
	IBloc::OPERATOR GetDimensionOperator();
	void GetSelectedID(JList<JInt32> & ID) { m_List_Dimension.GetSelectedID(ID); } ;
	void GetSelectedID(JVector<JInt32> & ID) { m_List_Dimension.GetSelectedID(ID); } ;
	void GetSelectedItems(JList<const IModalite *> & Items) { m_List_Dimension.GetSelectedItems(Items); } ;
	void GetSelectedItems(JVector<const IModalite *> & Items) { m_List_Dimension.GetSelectedItems(Items); } ;
	bool SetSelectedID(JList<JInt32> & ID) { return m_List_Dimension.SetSelectedID(ID); } ;
	bool SetSelectedID(JVector<JInt32> & ID) { return m_List_Dimension.SetSelectedID(ID); } ;
	void SetDimMode(bool bDimMode, int DimModeIndex, CString & DimText);
	bool IsDimSelected() { return (m_Button_Dimension.GetCheck() != 0)?true:false; } ;
	/// Sets the colors of this dialog
	void SetColors();
	void SetIcones(bool bIcones);
	void EnableOperators(BOOL bOperators);

	// Rend actif ou pas l'opérateur dimension 3
	void SetOpeDimension3(bool Actif, bool ModeIcone);

	CATPColors::APPCOLOR			m_CATPColorsApp;
	bool					m_bIcones;
	BOOL					m_bOperators;

protected:
	void									UpdateTitle();
	bool									m_bDimMode;
	int										m_DimModeIndex;
	CString									m_DimText;

	/// Type of Dialog 
	IQuestion::DIMTYPE						m_Type;
	/// Dialog Index number 1>
	int										m_Index;
	/// Modalité list box
	CListBoxDimensions						m_List_Dimension;

	CLabel									m_Label;
	CButtonST								m_Button_Dimension;
	CLabel									m_Box;

	// Les boutons opérateurs
	CButtonST								m_Button_Menu_Ope_Moda2;
	CButtonST								m_Button_Dim1_ET;
	CButtonST								m_Button_Dim1_OU;
	CButtonST								m_Button_Dim1_NI;
	CButtonST								m_Button_Dim1_XOR;
	CButtonST								m_Button_Dim1_RAZ;

	//CSupportTree* m_pTree_Support;

public:
/// Dialog Data
	//{{AFX_DATA(CDimensionDlg)
	enum { IDD = IDD_RI_DLG_DIMENSION };
		/// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


/// Overrides
	/// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDimensionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    /// DDX/DDV support
	//}}AFX_VIRTUAL

/// Implementation
protected:

	/// Generated message map functions
	//{{AFX_MSG(CDimensionDlg)
	afx_msg void OnChange_Dim_1_PaletteSel();
	afx_msg void OnDim_1_Et();
	afx_msg void OnDim_1_Ni();
	afx_msg void OnDim_1_Ou();
	afx_msg void OnDim_1_XOr();
	afx_msg void OnDim_1_Raz();
	afx_msg void OnSelchangeListDimension();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Rend visible ou pas pour la sélection des opérateurs sur la boite modalité 2
	void SetVisibleOpeModa2(bool Visible);

public:
	afx_msg void OnBnClickedRiButMenuOpeModa2();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
