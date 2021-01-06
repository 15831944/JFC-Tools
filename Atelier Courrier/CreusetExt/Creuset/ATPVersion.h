#if !defined(AFX_ATPVERSION_H__027E2E0C_681E_4725_851A_032447348C78__INCLUDED_)
#define AFX_ATPVERSION_H__027E2E0C_681E_4725_851A_032447348C78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "Resource.h"
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CATPVersion

class CATPVersion : public CDialog
{
public:
	// le constructeur
	CATPVersion(CWnd* pParent = NULL);

public:
	//{{AFX_DATA(CATPVersion)
	enum { IDD = IDD_CR_ATPVERSION };
	//}}AFX_DATA
	virtual BOOL OnInitDialog();

protected:
	//{{AFX_VIRTUAL(CATPVersion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPVersion)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CButtonST m_OK;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPVERSION_H__027E2E0C_681E_4725_851A_032447348C78__INCLUDED_)
