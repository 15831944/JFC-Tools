#pragma once

#include "resource.h"
#include "BtnST.h"
#include "afxwin.h"
#include "JATPDocCreuset.h"

#include "ETSLayout.h"

// Mode formulaire CResultBar

class CResultBar : public ETSLayoutFormView
{
	DECLARE_DYNCREATE(CResultBar)
	DECLARE_LAYOUT();

	void SetColors();
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	static LONG GetCXMin();
	static LONG GetCYMin();

	enum { IDD = IDD_CR_RESULTBAR };

protected:
	CResultBar();           // constructeur protégé utilisé par la création dynamique
	virtual ~CResultBar();

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnBtnRG();
	afx_msg void OnBtnDistriN();
	afx_msg void OnBtnDistriC();
	afx_msg void OnBtnVtl();
	afx_msg void OnItemVtl();
	afx_msg void OnBtnEval();

	afx_msg void OnContribCouv();
	afx_msg void OnContribGRP();
	afx_msg void OnContribPGRP();
	afx_msg void OnContribBudg();
	afx_msg void OnContribPBudg();
	afx_msg void OnContribCtCouv();
	afx_msg void OnContribModeSupports();
	afx_msg void OnContribModeGrandeurs();

	afx_msg void OnUpdContribCouv(CCmdUI *pCmdUI);
	afx_msg void OnUpdContribGRP(CCmdUI *pCmdUI);
	afx_msg void OnUpdContribPGRP(CCmdUI *pCmdUI);
	afx_msg void OnUpdContribBudg(CCmdUI *pCmdUI);
	afx_msg void OnUpdContribPBudg(CCmdUI *pCmdUI);
	afx_msg void OnUpdContribCtCouv(CCmdUI *pCmdUI);
	afx_msg void OnUpdContribModeSupports(CCmdUI *pCmdUI);
	afx_msg void OnUpdContribModeGrandeurs(CCmdUI *pCmdUI);

	afx_msg void OnVentilNbContacts();
	afx_msg void OnVentilAudience000();
	afx_msg void OnVentilGRP();
	afx_msg void OnVentilIndGRP();
	afx_msg void OnVentilCouv();
	afx_msg void OnVentilIndCouv();
	afx_msg void OnVentilRepet();

	afx_msg void OnUpdVentilNbContacts(CCmdUI *pCmdUI);
	afx_msg void OnUpdVentilAudience000(CCmdUI *pCmdUI);
	afx_msg void OnUpdVentilGRP(CCmdUI *pCmdUI);
	afx_msg void OnUpdVentilIndGRP(CCmdUI *pCmdUI);
	afx_msg void OnUpdVentilCouv(CCmdUI *pCmdUI);
	afx_msg void OnUpdVentilIndCouv(CCmdUI *pCmdUI);
	afx_msg void OnUpdVentilRepet(CCmdUI *pCmdUI);

	// Pour les graphes
	afx_msg void OnGraphGRPJour();
	afx_msg void OnGraphGRPCum();
	afx_msg void OnGraphCouvCum();
	afx_msg void OnGraphApportCouvCum();

	afx_msg void OnUpdGraphGRPJour(CCmdUI *pCmdUI);
	afx_msg void OnUpdGraphGRPCum(CCmdUI *pCmdUI);
	afx_msg void OnUpdGraphCouvCum(CCmdUI *pCmdUI);
	afx_msg void OnUpdGraphApportCouvCum(CCmdUI *pCmdUI);

private:
	afx_msg void OnContrib();

	JATPDocCreuset* m_pATPDocCreuset; // le document

	CButtonST m_BtnRG;
	CButtonST m_BtnDistribN;
	CButtonST m_BtnDistribC;
	CButtonST m_BtnVentil;
	CButtonST m_BtnMemo;
	CButtonST m_BtnEval;
	
	// Bouton menu popup graphes
	CButtonST m_BtnGraph;

#define XBorder 5
#define XInter 2
#define XLn 125
#define YBorder 5
#define YLn 22


};


