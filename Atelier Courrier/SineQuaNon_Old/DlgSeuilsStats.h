/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CDlgSeuilsStats
//
//	Descriptif:
//	Boide de dialogue permettant de saisir les seuils statistiques
*/

#pragma once

#include <Label.h>
#include <BtnST.h>
#include "ATPColors.h"
#include "afxwin.h"

#define MargeErreurMax		75
// Bo�te de dialogue CDlgSeuilsStats

class CDlgSeuilsStats : public CDialog
{
	DECLARE_DYNAMIC(CDlgSeuilsStats)

public:
	CDlgSeuilsStats(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgSeuilsStats();

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_ATP_SEUILS };

	void SetColors(CATPColors::APPCOLOR app);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

private:

	int m_Avertissement;
	int m_Limite;

	// Variable indiquant la limite pour afficher les r�sultats effectif en rouge
	int m_SeuilMargeErreur_Util;

	// Limite pour la marge d'erreur dans l'appli / indiqu�e par des *** sur les effectifs
	int m_SeuilMargeErreur_Appli;

	CLabel m_EditSeuilA;
	CLabel m_EditMargeErreur;

	CLabel m_bkg;
	CLabel m_ttl;
	CLabel m_Lbl1;
	CLabel m_Lbl2;

	CLabel m_Lbl_MargeErrUtil;
	CLabel m_Lbl_MargeErrAppli;
	CLabel m_ttlMargeErreur;
	CLabel m_bkgMargeErreur;

	CLabel m_PrctMargeUtil;
	CLabel m_PrctMargeAppli;

	CButtonST m_btnOK;
	CButtonST m_btnCancel;

	void SetupControls();
	void SetColors();
	CATPColors::APPCOLOR m_appColor;

	BOOL m_bShow;

};
