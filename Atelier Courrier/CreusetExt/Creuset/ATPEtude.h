#if !defined(AFX_ATPETUDE_H__6A84DB6E_615B_4C8C_999F_2F983F26DF34__INCLUDED_)
#define AFX_ATPETUDE_H__6A84DB6E_615B_4C8C_999F_2F983F26DF34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "Resource.h"
#include "JATPDocCreuset.h"
#include "DialogChoixOffre.h"
#include "Offre2MainDialog.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>

/////////////////////////////////////////////////////////////////////////////
// CATPEtude

class CATPEtude : public CDialog
{
public:
	// le constructeur
	CATPEtude(JATPDocCreuset & ATPDocCreuset, CWnd* pParent = NULL);

protected:
	//{{AFX_DATA(CATPEtude)
	enum { IDD = IDD_CR_ATPETUDE };
	//}}AFX_DATA

protected:
	// la fonction pour mettre à jour l'étude
	BOOL OnUpdateEtude();

protected:
	// la fonction pour mettre à jour la boîte
	VOID OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPEtude)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPEtude)
	virtual BOOL OnInitDialog();
	afx_msg void OnPeriode();
	afx_msg void OnProduit();
	afx_msg void OnOffre();
	afx_msg void OnOffreImport();
	afx_msg void OnFormat();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset & m_ATPDocCreuset; // le document

private:
	JUnt32 m_IdProduit;// l'identifiant du produit

private:
	JDate m_DateD; // la date de début
	JDate m_DateF; // la date de fin

	int bCompleteAnnonc; //0 pas sel, 1 presel, 2 sel
	int bCompleteOffre;
	int bCompleteFormat;

	JString m_CodeGroupe;
	JString m_CodeAnnonceur;
	JString m_CodeProduit;

	///Elements de la boite
	CLabel m_Backgnd;
	CLabel m_Ttl;
	CButtonST m_btnOk;
	CButtonST m_btnCancel;
	CButtonST m_btnPeriode;
	CButtonST m_btnProduit;
	CButtonST m_btnOffre;
	CButtonST m_btnFormat;

	CDialogChoixOffre m_DlgChoixOffre;

///init de la boite
	void SetupControls();
	void SetColors();
	void SetIco();
public:
	afx_msg void OnOffre2Creer();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPETUDE_H__6A84DB6E_615B_4C8C_999F_2F983F26DF34__INCLUDED_)
