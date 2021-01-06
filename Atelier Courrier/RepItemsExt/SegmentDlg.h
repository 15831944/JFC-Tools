#pragma once
//#include "afxwin.h"
#include "Terrain.h"
#include "label.h"
#include "BtnST.h"
#include "JFCListBox.h"


/// Population Segment dialog
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
 * \date 07-11-2003
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
class AFX_EXT_CLASS CSegmentDlg : public CDialog
{
	DECLARE_DYNAMIC(CSegmentDlg)

public:
	CSegmentDlg(CWnd* pParent = NULL);   ///< standard constructor
	virtual ~CSegmentDlg();

/// Dialog Data
	enum { IDD = IDD_RI_DLG_SEGMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    ///< DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnOk();
	bool SetData(CTerrain * pTerrain, JVector<JInt32> & IdSegments);

	CATPColors::APPCOLOR		m_CATPColorsApp;
	CTerrain *			m_pTerrain; ///< Terrain Pointer
	JUnt32				m_ContingentMask;///< The contingent Mask
    JVector<JInt32>		m_Id_PopSegments;///< The Selected Segment Ids
	CBitArray			m_PopulationSeg;///< The Population Segment

	CLabel				m_Label;
	CLabel				m_Box;
	CButtonST			m_Button_OK;
	CButtonST			m_Button_Cancel;
	JFCListBox			m_ListSegments;

};
