#pragma once
#include "dlganalyseur.h"

class CDlgProfiling : public CDlgAnalyseur
{
	DECLARE_DYNAMIC(CDlgProfiling)

public:

	CDlgProfiling(CWnd* pParent = NULL);
	virtual ~CDlgProfiling(void);

protected:
	virtual void SetupGrandeursBase();
	virtual void UpdateResultStyles();
	virtual void SetupCrossTable();

	// Suppression de certains menu bouton grandeur
	virtual void SuppOptionButGlobal(CMenu *pMenu);

	// Aller dans pupitre
	virtual void GoPupitre();

	
public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileNew();
	afx_msg void OnBnClickedButVectorielle();
	afx_msg void OnTableauTransposer();
	afx_msg void OnTableauAppliquerCanevas() {};
	afx_msg void OnTableauEnregistrerCanevas(){};
	virtual BOOL OnInitDialog();
};
