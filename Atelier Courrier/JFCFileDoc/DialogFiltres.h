/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDialogFiltres
//
//	Descriptif:
//	Boite de dialogue permettant la sélection des filtres d'audience utilisée dans le creuset et dans choix-support
*/

#pragma once

//#include "JATPDocCreuset.h"
#include "resource.h"
//JFCControls
#include <JFC_MultiListBox.h>
#include <Label.h>
#include <BtnST.h>
#include "IElem.h"
#include "JSRCPresse.h"
#include "ATPColors.h"

#include "JATPDocument.h"
// Boîte de dialogue CDialogFiltres

class AFX_EXT_CLASS CDialogFiltres : public CDialog
{
	DECLARE_DYNAMIC(CDialogFiltres)

public:
	CDialogFiltres(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDialogFiltres();

	virtual void SetDocument(JATPDocument* pATPDocument);

	// la fonction pour mettre à jour le composant
	virtual void Update();

	// Données de boîte de dialogue
	enum { IDD = IDD_FD_DLG_FILTREAUDIENCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	void OnUpdate();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLbnSelchangeListBas();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBtnX();


	void FillListFiltres();

private:

	JATPDocument* m_pATPDocument; // le document
	JSRCPresse* m_srcPresse;

	//elements de la boite
	CLabel m_bkgFiltre;
	CLabel m_ttlFiltre;
	CLabel m_ttlMag;
	CLabel m_ttlQtd;
	CLabel m_FilterSel;
	CButtonST m_btnX;

	JFC_MultiListBox<IElem const *> m_lstBas;

	void SetupControls();
public:
	void SetColors(CATPColors::APPCOLOR app);
};
