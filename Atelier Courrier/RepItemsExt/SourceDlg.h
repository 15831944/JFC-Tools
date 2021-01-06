#pragma once

//#include "Terrain.h"
//#include "JSrcPresse.h"
//#include "afxwin.h"
#include "label.h"
#include "BtnST.h"
#include "ClrSelComboBox.h"
#include "JFCListBox.h"

/// Source & Terrain Selection dialog
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
 * \date 07-18-2003
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
class AFX_EXT_CLASS CSourceDlg : public CDialog
{
	DECLARE_DYNAMIC(CSourceDlg)

public:
	CSourceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSourceDlg();
	/// Dialog initialisation
	virtual BOOL OnInitDialog();
	/// Dialog Data
	enum { IDD = IDD_RI_DLG_SOURCE };

	/// Source Id
	int m_Id_Source;
	/// Terrain Id
	int m_Id_Terrain;

protected:
	/// Data exchange
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	/// Selection changed in combo source
	afx_msg void OnCbnSelchangeComboSource();
	/// Selection changed in List Terrain
	afx_msg void OnLbnSelchangeListTerrain();

	CATPColors::APPCOLOR		m_CATPColorsApp;
	/// The Terrain listbox object
	JFCListBox 			m_ListTerrain;
	/// The Source combobox object
	CClrSelComboBox		m_ComboSource;
	CLabel				m_Label_Source;
	CLabel				m_Label_Terrain;
	CLabel				m_Box;
	CButtonST			m_Button_OK;
	CButtonST			m_Button_Cancel;
	CButtonST			m_Button_Property;
	afx_msg void OnOk();
	afx_msg void OnProperty();
};
