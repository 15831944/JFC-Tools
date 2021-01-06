#pragma once

#include "JATPDocCreuset.h"
#include "ATPContrib_A1.h"
#include "ATPContrib_A2.h"
#include "ATPContrib_TXT.h"
#include "ATPContrib_TXTA1.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>

// Boîte de dialogue CDialogContrib

class CDialogContrib : public CDialog
{
	DECLARE_DYNAMIC(CDialogContrib)

public:
	CDialogContrib(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDialogContrib();

	virtual void SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual void Update();

	// Données de boîte de dialogue
	enum { IDD = IDD_CR_DLG_CONTRIBUTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	void OnUpdate();

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBtnX();


private:

	JATPDocCreuset* m_pATPDocCreuset; // le document

	//elements de la boite
	CLabel m_bkg;
	CLabel m_ttl;
	CButtonST m_btnX;
	//CATPContrib_A1 m_A1;
	//CATPContrib_TXTA1 m_TXTA1;
	CATPContrib_A2 m_A2;
	CATPContrib_TXT m_Txt;

	void SetupControls();
public:
	void SetColors();
};
