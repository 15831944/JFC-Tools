#if !defined(AFX_ATPSplitterPlan_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_)
#define AFX_ATPSplitterPlan_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

// on inclut les définitions nécessaires
#include "ATPCreusetPlan.h"
#include "ATPEchelle.h"
#include "ResultBar.h"
#include "ATPResultPlan.h"

#include "VertSplitterWndEx.h"
#include "SplitterWndEx.h"
/////////////////////////////////////////////////////////////////////////////
// CATPSplitterPlan

class CATPSplitterPlan : public JFCFrame
{
public:
	//DECLARE_DYNCREATE(CATPSplitterPlan)
	// le constructeur
	CATPSplitterPlan();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual VOID UpdateAllViews(const JFCUpdate & Message);
	
	void SetColors();

	// le destructeur
	virtual ~CATPSplitterPlan();


public:
	// les fonctions pour récupérer les dimensions minimales du panneau
	static LONG GetCXMin();
	static LONG GetCYMin();
	void Invalidate(BOOL bErase = TRUE );

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPSplitterPlan)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPSplitterPlan)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
friend class CATPFramePlan;

	CHideSplitterWndEx m_wndCreusetResEch;
	CHideSplitterWndEx m_wndBarSplitter;

	CHideSplitterWndEx m_wndVertSplitter;
	CSplitterWndEx m_wndPanelSplitter;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPSplitterPlan_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_)
