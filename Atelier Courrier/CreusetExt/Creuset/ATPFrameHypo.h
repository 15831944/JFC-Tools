#if !defined(AFX_ATPFrameHypo_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_)
#define AFX_ATPFrameHypo_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "ATPBaseFrame.h"

// on inclut les définitions nécessaires
#include "ATPSplitterHypo.h"

#include "DialogContrib.h"

/////////////////////////////////////////////////////////////////////////////
// CATPFrameHypo

class CATPFrameHypo : public CATPBaseFrame
{
public:
	// le constructeur
	CATPFrameHypo(JATPDocCreuset *pDoc);

	// le destructeur
	virtual ~CATPFrameHypo();


	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

protected:
	//{{AFX_VIRTUAL(CATPFrameHypo)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPFrameHypo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	
	CATPSplitterHypo* m_pSplitterHypo;
    BOOL m_bFirstTimeHypo;
    static const CRect s_rectDefault;

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPFrameHypo_H__69DBE77F_27CB_4150_A428_A4CDBB286F46__INCLUDED_)
