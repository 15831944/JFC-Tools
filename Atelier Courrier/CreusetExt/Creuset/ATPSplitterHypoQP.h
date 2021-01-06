#if !defined(AFX_ATPSplitterHypoQP_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_)
#define AFX_ATPSplitterHypoQP_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "ATPSplitterHypo.h"
#include "ATPCreusetHypoQP.h"
#include "ATPResultHypoQP.h"

/////////////////////////////////////////////////////////////////////////////
// CATPSplitterHypo

class CATPSplitterHypoQP : public CATPSplitterHypo
{
public:
	//DECLARE_DYNCREATE(CATPSplitterHypoQP)
	// le constructeur
	CATPSplitterHypoQP();

	

	// le destructeur
	virtual ~CATPSplitterHypoQP();



public:
	// les fonctions pour récupérer les dimensions minimales du panneau
	//static LONG GetCXMin();
	//static LONG GetCYMin();


protected:
	virtual JVoid OnUpdate();

	//{{AFX_MSG(CATPSplitterHypoQP)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPSplitterHypo_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_)
