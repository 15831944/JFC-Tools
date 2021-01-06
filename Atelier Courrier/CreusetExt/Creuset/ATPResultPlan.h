#pragma once

#include "resource.h"
#include "BtnST.h"
#include "Label.h"
#include "JATPDocCreuset.h"

#include "ATPGraphCouv.h"
#include "ATPGraphGRP.h"
#include "ATPGraphGRPCumul.h"
#include "ATPGraphCouvJour.h"

#include "ETSLayout.h"


#include "ATPResultPlanTbl.h"

// Mode formulaire CATPResultPlan

class CATPResultPlan : public ETSLayoutFormView
{
	DECLARE_DYNCREATE(CATPResultPlan)
	DECLARE_LAYOUT();

protected:
	CATPResultPlan();           // constructeur protégé utilisé par la création dynamique
	virtual ~CATPResultPlan();

public:
	void SetColors();
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();
	void Init();

	static LONG GetCXMin();
	static LONG GetCYMin();


public:
	enum { IDD = IDD_CR_RESULTPLAN };
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnBtn1GRPJour();
	afx_msg void OnBtn2GRPCumul();
	afx_msg void OnBtn3CouvCumul();
	afx_msg void OnBtn4CouvJour();
	afx_msg void OnBtnPlus();
	afx_msg void OnBtnMoins();
	afx_msg void OnBtnAuto();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()

private:
friend class CATPSplitterPlan;
friend class CATPFramePlan;

	JATPDocCreuset* m_pATPDocCreuset; // le document

	CString GetLabel();
	CLabel m_lblRes;
	CLabel m_Ttl;
	CATPResultPlanTbl m_ResultTbl;
	JFCScroll m_VSRes;

	CATPGraphGRP m_grphGRPJour;
	CATPGraphCouv m_grphCouvCumul;

	CATPGraphGRPCumul m_grphGRPCumul;
	CATPGraphCouvJour m_grphCouvJour;

	/*
	CButtonST m_BtnGRPJour;
	CButtonST m_BtnGRPCumul;
	CButtonST m_BtnCouvCumul;
	CButtonST m_BtnCouvJour;
	*/

	CButtonST m_BtnPlus;
	CButtonST m_BtnMoins;
	CButtonST m_BtnAuto;

	bool b_ini;
};


