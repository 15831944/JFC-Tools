/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CDlgChoixMoteur
//
//	Descriptif:
//	Boite de dialogue permettant de choisir le moteur � utiliser et de d�finir certains de ses param�tres.
*/

#pragma once

#include <Label.h>
#include <BtnST.h>

#include "JATPDocument.h"
#include "ATPColors.h"
#include "resource.h"

// Bo�te de dialogue CDlgChoixMoteur

class AFX_EXT_CLASS CDlgChoixMoteur : public CDialog
{
	DECLARE_DYNAMIC(CDlgChoixMoteur)

public:
	CDlgChoixMoteur(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgChoixMoteur();

	virtual void SetDocument(JATPDocument* pATPDoc);

	// la fonction pour mettre � jour le composant
	virtual void Update();

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_FD_DLG_CHOIXMOTEUR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	void OnUpdate();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBtnX();
	afx_msg void OnBnClickedMoteur();
	afx_msg void OnBtnReprise();
	afx_msg void OnBtnLDP();
	afx_msg void OnBtnLNM();
	afx_msg void OnBtnLDP30J();


private:

	JATPDocument* m_pATPDoc; // le document

	//elements de la boite
	CLabel m_bkg;
	CLabel m_ttl;
	CLabel m_lblMoteur;
	CButtonST m_btnX;
	CButtonST m_btnMoteur;
	CButtonST m_ChkReprise;
	CButtonST m_ChkLDP;
	CButtonST m_ChkLNM;
	CButtonST m_ChkLDP30J;


	void SetupControls();
public:
	void SetColors(CATPColors::APPCOLOR app);
	
};
