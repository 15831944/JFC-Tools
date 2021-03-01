#if !defined(AFX_ATPFrameQuickPlan_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_)
#define AFX_ATPFrameQuickPlan_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "ATPBaseFrame.h"

// on inclut les définitions nécessaires
#include "ATPSplitterHypoQP.h"
#include "DialogContrib.h"

/////////////////////////////////////////////////////////////////////////////
// CATPFrameQuickPlan

class CATPFrameQuickPlan : public CATPBaseFrame
{
public:
	// le constructeur
	CATPFrameQuickPlan(JATPDocCreuset *pDoc);

	// le destructeur
	virtual ~CATPFrameQuickPlan();


	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

protected:
	//{{AFX_VIRTUAL(CATPFrameQuickPlan)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPFrameQuickPlan)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnExportTout();
	afx_msg void OnExcelTout();
	afx_msg void OnClipboardTout();
	afx_msg void OnPrintTout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	
	CATPSplitterHypoQP* m_pSplitterHypo;
    BOOL m_bFirstTime;
    static const CRect s_rectDefault;

	bool ExportResultPlan(CFile & file);
	bool ExportEchelle(CFile & file);

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPFrameQuickPlan_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_)
