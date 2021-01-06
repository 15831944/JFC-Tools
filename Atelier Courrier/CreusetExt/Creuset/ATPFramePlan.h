#if !defined(AFX_ATPFramePlan_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_)
#define AFX_ATPFramePlan_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "ATPBaseFrame.h"

// on inclut les définitions nécessaires

#include "ATPSplitterPlan.h"

// on inclut les définitions nécessaires
#include "ATPZoomPlan.h"

/////////////////////////////////////////////////////////////////////////////
// CATPFramePlan

class AFX_EXT_CLASS CATPFramePlan : public CATPBaseFrame
{
public:
	// le constructeur
	CATPFramePlan(JATPDocCreuset *pDoc);

	// le destructeur
	virtual ~CATPFramePlan();


	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

protected:
	//{{AFX_VIRTUAL(CATPFramePlan)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPFramePlan)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnClipboardGraph();
	afx_msg void OnUpdateClipboardGraph(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CATPZoomPlan       m_Z2; // le ZoomPlan

	CATPSplitterPlan* m_pSplitterPlan;

    BOOL m_bFirstTimePlan;
    static const CRect s_rectDefault;

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPFramePlan_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_)
