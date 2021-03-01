#pragma once


#include "BtnST.h"
#include "Label.h"
#include "JATPDocCreuset.h"
#include <GridCtrl.h>
#include <GridCellNumClick.h>
#include "Courbes.h"

// Boîte de dialogue CDlgEditCourbe

class CDlgEditCourbe : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditCourbe)

public:
	CDlgEditCourbe(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgEditCourbe();

// Données de boîte de dialogue
	enum { IDD = IDD_CR_NVLCRB };

	void SetIdCourbe(int id);
	int GetIdCourbe() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	virtual BOOL OnInitDialog();
//	virtual void OnCancel();
	virtual void OnOK();
	afx_msg LRESULT OnNcHitTest(CPoint point);

	DECLARE_MESSAGE_MAP()

private:
	
	void SetupControls();
	void SetColors();
	void InitGrid();

	CLabel m_bkg;
	CLabel m_ttl;
	CLabel m_Txt;

	CButtonST m_OK;
	CButtonST m_Cancel;

	CCourbes* m_courbes;
	int m_idCourbe;
	
	CGridCtrl m_gridValues;

	CString m_lbl;
};
