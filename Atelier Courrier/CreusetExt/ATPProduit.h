#pragma once
#include "afxwin.h"

//JFCControls
#include <ClrSelComboBox.h>
#include <Label.h>
#include <BtnST.h>

// Boîte de dialogue CATPProduit

class CATPProduit : public CDialog
{
	DECLARE_DYNAMIC(CATPProduit)

public:
	CATPProduit(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CATPProduit();

	// les fonctions pour manipuler les codes groupe/annonceur/produit
	JString GetCodeGroupe() const;
	JString GetCodeAnnonceur() const;
	JString GetCodeProduit() const;

	// Données de boîte de dialogue
	enum { IDD = IDD_CR_ATPPRODUIT };

private:
	// Les codes
	JString m_CodeGroupe;
	JString m_CodeAnnonceur;
	JString m_CodeProduit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()

private:
	CClrSelComboBox m_ComboGroupe;
	CClrSelComboBox m_ComboAnnonceur;
	CClrSelComboBox m_ComboProduit;
	CLabel m_bkg;
	CLabel m_ttl0;
	CLabel m_ttl1;
	CLabel m_ttl2;
	CButtonST m_btnOK;
	CButtonST m_btnCancel;

	void SetColors();
	void SetupControls();

};
