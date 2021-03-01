/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDlgCible
//
//	Descriptif:
//	Boite fille de DlgSource
//	Permet la sélection de la source (source d'audience = source de ciblage)
*/

#pragma once

#include "DialogChoixOffre.h"
#include "JFCSelListBox.h"

// Boîte de dialogue CDlgSrcUnique

class CDlgSrcUnique : public ETSLayoutDialog
{
	DECLARE_DYNAMIC(CDlgSrcUnique)

public:
	CDlgSrcUnique(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgSrcUnique();

	// Positionne l'offre
	void SetData (CDialogChoixOffre* data);
	
	// Récup l'offre
	CDialogChoixOffre * GetData();

	void Init();
	bool IsComplete();

// Données de boîte de dialogue
	enum { IDD = IDD_FD_DLG_SRC_UNIQUE };

protected:
	CDialogChoixOffre* m_pData;
	JSRCTables* m_srcTables;

	void FillComboSource();
	void FillListTerrain(int idSource);
	void FillListSegment();
	void FillListPoids();

	DECLARE_MESSAGE_MAP()
	virtual void OnOK() {/*do nothing*/};
	virtual void OnCancel() {/*do nothing*/};
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboSource();
	afx_msg void OnLbnSelchangeListTerrain();
	afx_msg void OnLbnSelchangeListSegment();
	afx_msg void OnLbnSelchangeListPoids();
	afx_msg void OnPaint();


private:
	void ResetSize();
	CLabel m_ttlSource;
	CClrSelComboBox m_comboSource;

	// Liste des terrains (sélection multiple)
	CLabel m_ttlTerrain;
	// JFCListBox m_lstTerrain;
	JFCSelListBox m_lstTerrain;

	// Liste des segments (sélection multiple)
	CLabel m_ttlSegment;
	// JFCListBox m_lstSegment;
	JFCSelListBox m_lstSegment;

	// Liste des poids (sélection simple)
	CLabel m_ttlPoids;
	JFCListBox m_lstPoids;

	void SetColors();
	void SetupControls();

	// Warning enquête
	JBool m_OkWarningEnqueteAEPM2006;
	JBool m_OkWarningEnqueteFCA2007;

};
