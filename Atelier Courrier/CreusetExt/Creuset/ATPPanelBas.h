#pragma once

#include "BtnST.h"
#include "JATPDocCreuset.h"

#include "ETSLayout.h"

// Mode formulaire CATPPanelBas

class CATPPanelBas : public ETSLayoutFormView
{
	DECLARE_DYNCREATE(CATPPanelBas)
	DECLARE_LAYOUT();

protected:
	CATPPanelBas();           // constructeur protégé utilisé par la création dynamique
	virtual ~CATPPanelBas();

public:
	void SetColors();
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	static LONG GetCXMin();
	static LONG GetCYMin();


public:
	enum { IDD = IDD_CR_PANELBAS };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
//	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBtnEchelle();
	afx_msg void OnBtnResultats();
	afx_msg void OnBtnZoom();
	afx_msg void OnBtnFicheSupp();
	afx_msg void OnBtnCourbe();
	afx_msg void OnBtnFiltre();
	afx_msg void OnBtnSwitch();

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

	CButtonST m_Echelle;
	CButtonST m_Resultats;
	CButtonST m_Zoom;
	CButtonST m_FicheSupp;
	CButtonST m_Courbe;
	CButtonST m_Filtre;
	CButtonST m_SwitchPlan_Hypo;

#define YInter 2
#define XInter 2
#define XBorder 5
#define YBorder 5
//#define HBtn 22
#define HBtn 14

};

class CATPPanelBasQP : public CATPPanelBas
{
public:
	static LONG GetCYMin();
};

