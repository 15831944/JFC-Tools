#pragma once

#include "Terrain.h"
#include "KeyTerrain.h"
#include "ClrSelComboBox.h"
#include "JFCSelListBox.h"
#include <JFC_MultiListBox.h>
#include <JFCListBox.h>
#include <ClrSelComboBox.h>
#include <Label.h>
#include <BtnST.h>
// CSrcUniqueDlg dialog

class CSrcUniqueDlg : public CDialog
{
	DECLARE_DYNAMIC(CSrcUniqueDlg)

public:
	CSrcUniqueDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSrcUniqueDlg();
	void SetData(JUnt32 SourceId, const JList<UINT> & Terrains, const JList<UINT> & Segments, JUnt32 TypePoids);
	void GetData(JUnt32 & SourceId, JList<UINT> & Terrains, JList<UINT> & Segments, JUnt32 & TypePoids);

// Dialog Data
	enum { IDD = IDD_AN_DLG_SRC_UNIQUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void FillComboSource();
	void FillListTerrain();
	void FillListSegment();
	void FillListPoids();
	void UpdateOk();

	CTerrain * GetTerrain(CKeyTerrain & Key);
	void SetColors();
	void SetupControls();

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeComboSource();
	afx_msg void OnLbnSelchangeListTerrain();
	afx_msg void OnLbnSelchangeListSegment();
	afx_msg void OnLbnSelchangeListPoids();

private:
	CLabel								m_bkgSrc;
	CLabel								m_ttlSource;
	CClrSelComboBox						m_comboSource;
	CLabel								m_ttlTerrain;
	JFCSelListBox						m_lstTerrain;
	CLabel								m_ttlSegment;
	JFCSelListBox						m_lstSegment;
	CLabel								m_ttlPoids;
	JFCSelListBox						m_lstPoids;
	CButtonST							m_But_Ok;
	CButtonST							m_But_Cancel;

	JUnt32								m_TerrainId;
	JMap<CKeyTerrain,CTerrain *>		m_Terrains;
	JList<UINT>							m_SelTerrains;
	JList<UINT>							m_SelSegments;
	JUnt32								m_SourceId;
	JUnt32								m_TypePoidsId;

	JSRCTables*							m_srcTables;
};
