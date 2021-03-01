#if !defined(AFX_ATPPERIODEACTION_H__927097FB_81C0_46A6_A239_86560CB4B1C0__INCLUDED_)
#define AFX_ATPPERIODEACTION_H__927097FB_81C0_46A6_A239_86560CB4B1C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "Resource.h"
#include "ATPPeriodeAction_A1.h"
#include "ATPPeriodeAction_A2.h"
#include "JATPDocCreuset.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>

/////////////////////////////////////////////////////////////////////////////
// CATPPeriodeAction

class CATPPeriodeAction : public JFCDialog
{
public:
	// le constructeur
	CATPPeriodeAction(UINT nIDTTLboite, UINT nIDTTL, CWnd* pParent = NULL);
	JDate m_DateD_Etude;
	JDate m_DateF_Etude;
	JDate m_DateD_Action;
	JDate m_DateF_Action;

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);
	JVoid SetHypo(JBool bHypo);

protected:
	//{{AFX_DATA(CATPPeriodeAction)
	enum { IDD = IDD_CR_ATPPERIODEACTION };
	CATPPeriodeAction_A1 m_A1;
	CATPPeriodeAction_A2 m_A2;
	JFCScroll m_HS;
	//}}AFX_DATA


protected:
	// la fonction pour mettre à jour la période d'action
	BOOL OnUpdatePeriodeAction();

protected:
	//{{AFX_VIRTUAL(CATPPeriodeAction)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPPeriodeAction)
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnT();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	// la classe amie
	friend class CATPPeriodeAction_A1;
	friend class CATPPeriodeAction_A2;

private:
	JATPDocCreuset* m_pATPDocCreuset;
	///Elements de la boite
	CLabel m_Backgnd;
	CLabel m_Ttl;
	CButtonST m_btnT;
	CButtonST m_btnOk;
	CButtonST m_btnCancel;

	UINT m_nIDTTLboite;
	UINT m_nIDTTL;

	///init de la boite
	void SetupControls();
	void SetColors();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPPERIODEACTION_H__927097FB_81C0_46A6_A239_86560CB4B1C0__INCLUDED_)
