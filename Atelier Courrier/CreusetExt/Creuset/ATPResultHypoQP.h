#if !defined(AFX_ATPResultHypoQP_H__D92F785C_31AE_43E0_8174_FCE190CBD0BF__INCLUDED_)
#define AFX_ATPResultHypoQP_H__D92F785C_31AE_43E0_8174_FCE190CBD0BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "ATPResultHypo.h"

/////////////////////////////////////////////////////////////////////////////
// CATPResultHypo

class CATPResultHypoQP : public CATPResultHypo
{
public:
	DECLARE_DYNCREATE(CATPResultHypoQP)
	// le constructeur
	CATPResultHypoQP();


	// le destructeur
	virtual ~CATPResultHypoQP();


protected:
	//{{AFX_MSG(CATPResultHypo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPResultHypo_H__D92F785C_31AE_43E0_8174_FCE190CBD0BF__INCLUDED_)
