/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CFormatAttribLB
//
//	Descriptif:
//	ListBox spécailisé pour affiche l'etat attribué d'un format
//
//
//
//	Classe:
//	CDialogFormat
//
//	Descriptif:
//	Boite de dialogue de sélection des formats
*/


#pragma once

//JFCTable
#include <JSRCPresse.h>

//JFCControls
#include <JFC_MultiListBox.h>
#include <JFCListBox.h>
#include <ClrSelComboBox.h>
#include <Label.h>
#include <BtnST.h>
#include "resource.h"
#include "JIDList.h"
#include "ATPColors.h"
#include <afxwin.h>

// ListBox CFormatAttribLB
class CFormatAttribLB : public JFC_MultiListBox<IElem const *>
{
public:
	CFormatAttribLB(void);
	virtual ~CFormatAttribLB(void);

	JIDList* m_plstIDFormats;///Formats

private:
	virtual bool IsAttribue(int ID) const;/// redéfinition de la méthode
};

// Boîte de dialogue CDialogFormat
class AFX_EXT_CLASS CDialogFormat : public CDialog
{
	DECLARE_DYNAMIC(CDialogFormat)

public:
	CDialogFormat(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDialogFormat();

	bool IsModified() const { return m_bModif; };///Pour savoir s'il y a eu une modification

	JVoid SetMonoSel(bool bMonosel = true);

	JVoid SetData(JList<JUnt32> & lstIDSupport, JDate date);

	JVoid SetFormats(JList<JUnt32> & lstIDFormat);
	JVoid GetFormats(JList<JUnt32> & lstIDFormat) const;

	// Données de boîte de dialogue
	enum { IDD = IDD_OF_DLG_FORMAT };

	void SetColors(CATPColors::APPCOLOR app);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
	virtual void OnOK() {/*do nothing*/};
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeListHaut();
	afx_msg void OnLbnSelchangeListBas();

private:
	void EnableOK();
	void FillListFormats();
	void FillListSelFormats(JBool Select = false);

	bool m_bModif;
	JSRCPresse * m_srcPresse;
	CTarifs* m_trf;
	CRegroupements* m_regroup;

	JIDList m_lstIDFormats;///Formats
	
	JIDList m_lstIDSupport;///Support
	JDate m_date;

	CLabel m_bkgFormat;
	CLabel m_ttlFormat;
	JFC_MultiListBox<IElem const *> m_lstHaut;

	CFormatAttribLB m_lstBas;
	
	CButtonST m_btnOK;
	CButtonST m_btnCancel;

	void SetupControls();
	void SetColors();

	CATPColors::APPCOLOR m_appColor;

	bool m_bmonosel;

public:
	CButton m_Btn_Bidon;
};
