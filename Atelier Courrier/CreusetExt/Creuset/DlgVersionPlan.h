#pragma once
#include "afxwin.h"
#include "JATPDocCreuset.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>
#include <JFCListBox.h>

// Bo�te de dialogue CDlgVersionPlan

class CDlgVersionPlan : public CDialog
{
	DECLARE_DYNAMIC(CDlgVersionPlan)

public:
	// Constructeur (sauvegarde auto ou non auto pour r�cup�ration des plans)
	CDlgVersionPlan(JBool SaveAuto, CWnd* pParent = NULL);

	// Destructeur
	virtual ~CDlgVersionPlan();

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_CR_DLG_VERSIONPLANMAITRE };

	// pointeur sur document associ�
	JATPDocCreuset* m_pATPDocCreuset;

	// Plan insertion r�cup�r�
	CPlanInsertions m_PlanRefArchive;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

private:

	// Chargement de la liste des plans pr�c�dents
	void LoadListPlanPrec();

	// Cette liste contient tous les anciens plans de r�f�rence
	JFCListBox m_ListPlan;

	// Mode s�lection plan archiv�s (false : plan avec nom
	//								(true  : plan avec date sauvegarde automatique)
	JBool m_SaveAuto;

	CLabel m_bkg;
	CLabel m_ttl;
	CButtonST m_btnOK;
	CButtonST m_btnCancel;

	void SetColors();
	void SetupControls();

public:
	afx_msg void OnLbnDblclkCrListplan();
};
