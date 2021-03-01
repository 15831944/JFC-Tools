#if !defined(AFX_ATPCreusetHypoQP_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_)
#define AFX_ATPCreusetHypoQP_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ATPCreusetHypo.h"
#include "ATPCreusetHypoQP_C2.h"
//////////////////////////////////////////////////////////////////////////
// CATPCreusetHypo

class CATPCreusetHypoQP : public CATPCreusetHypo
{
public:
	DECLARE_DYNCREATE(CATPCreusetHypoQP)
	// le constructeur
	CATPCreusetHypoQP();

	// le destructeur
	virtual ~CATPCreusetHypoQP();



protected:
	//{{AFX_MSG(CATPCreusetHypo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetHypo_H__1C819B8E_578F_4A32_9FD3_DF788C7DAA0C__INCLUDED_)
