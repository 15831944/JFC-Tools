#pragma once

#include "BtnST.h"
#include "Label.h"
#include "KeyTerrain.h"
#include "JFC_MultiListBox.h"
#include "JATPDocCreuset.h"
#include "QstTree.h"
#include "ETSLayout.h"
// Boîte de dialogue CDialogVentilation

class CDialogVentilation : public ETSLayoutDialog
{
	DECLARE_DYNAMIC(CDialogVentilation)

public:
	CDialogVentilation(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDialogVentilation();

	void SetTerrain(CKeyTerrain keyterrain);

	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);


	void SetColors();

// Données de boîte de dialogue
	enum { IDD = IDD_CR_VENTIL };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	afx_msg void OnBnClickedCrBtn1question();
	afx_msg void OnBnClickedCrBtn2item();
	afx_msg void OnBnClickedCrBtn3palette();
	afx_msg void OnSelChangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelChangeList();
	afx_msg void OnSelChangeResultList();
	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	virtual void OnCancel();

private:

	void SetupControls();
	void ResetSize();

	void FillListItem();
	void FillListPalette();
	void FillResultList();

	JATPDocCreuset* m_pATPDocCreuset; // le document

	CLabel m_bkg;
	CLabel m_ttl;
	
	JFC_MultiListBox<IItem*> m_list;
	JFC_MultiListBox<IItem*> m_listHaut;
	CQstTree m_tree;

	CButtonST m_btnQuestion;
	CButtonST m_btnItem;
	CButtonST m_btnPalette;

	CButtonST m_btnOk;
	CButtonST m_btnCancel;

	CButtonST m_btnCheckGRP;
	CButtonST m_btnCheckIndGRP;
	CButtonST m_btnCheckCouv;
	CButtonST m_btnCheckIndCouv;
	CButtonST m_btnCheckRepet;

	CItemsConstruitsBase * m_pItems;

	CTerrain* m_terrainC;

	JList<IItem*> m_listItemQuestion;
	JList<IItem*> m_listAffItemQuestion;
	JList<IItem*> m_listItem;
	JList<IItem*> m_listPalettes;

	JList<JInt32> m_listIDItem;
	JList<JInt32> m_listIDPal;

};
