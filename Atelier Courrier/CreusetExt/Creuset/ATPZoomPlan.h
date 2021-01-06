#if !defined(AFX_ATPZoomPlan_H__002AC5EE_9566_4A8C_A4DF_37FDA4581389__INCLUDED_)
#define AFX_ATPZoomPlan_H__002AC5EE_9566_4A8C_A4DF_37FDA4581389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "Resource.h"
#include "ATPZoomPlan_A1.h"
#include "ATPZoomPlan_A2.h"
#include "ATPZoomPlan_A3.h"
#include "JATPDocCreuset.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>

/////////////////////////////////////////////////////////////////////////////
// CATPZoomPlan

class CATPZoomPlan : public CDialog
{
public:
	// le constructeur
	CATPZoomPlan(CWnd* pParent = NULL);

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	//{{AFX_DATA(CATPZoomPlan)
	enum { IDD = IDD_CR_ATPZOOMPLAN };
	//}}AFX_DATA
protected:
	CATPZoomPlan_A1 m_A1;
	CATPZoomPlan_A2 m_A2;
	CATPZoomPlan_A3 m_A3;


protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();
	void CalculInfos();

protected:
	//{{AFX_VIRTUAL(CATPZoomPlan)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPZoomPlan)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBtnX();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

///Elements de la boite
	CLabel m_Backgnd;
	CLabel m_Ttl;
	CLabel m_Ttl2;
	CLabel m_Ttl3;
	CButtonST m_btnX;

	friend class CATPZoomPlan_A1;
	friend class CATPZoomPlan_A2;
	friend class CATPZoomPlan_A3;

	JDate m_dateD;
	JFlt64 m_partBubget;
	JFlt64 m_prix;
	JFlt64 m_partXGRP;
	JFlt64 m_pointsXGRP;
	JFlt64 m_ApportCouv;
	JFlt64 m_ApportRepet;
	JFlt64 m_ApportCouvMem;

	JFlt64 m_CtGRP;
	JFlt64 m_CtInd;
	JFlt64 m_CtCts;

	JFlt64 m_CoefAlpha;

	JUnt32 m_Numero;

	CString m_Title;

///init de la boite
	void SetupControls();
public:
	void SetColors();
	void SetNbFormats(int nbFormats);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPZoomPlan_H__002AC5EE_9566_4A8C_A4DF_37FDA4581389__INCLUDED_)
