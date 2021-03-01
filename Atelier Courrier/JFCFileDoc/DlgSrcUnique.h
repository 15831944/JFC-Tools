/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CDlgCible
//
//	Descriptif:
//	Boite fille de DlgSource
//	Permet la s�lection de la source (source d'audience = source de ciblage)
*/

#pragma once

#include "DialogChoixOffre.h"
#include "JFCSelListBox.h"

// Bo�te de dialogue CDlgSrcUnique

class CDlgSrcUnique : public ETSLayoutDialog
{
	DECLARE_DYNAMIC(CDlgSrcUnique)

public:
	CDlgSrcUnique(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgSrcUnique();

	// Positionne l'offre
	void SetData (CDialogChoixOffre* data);
	
	// R�cup l'offre
	CDialogChoixOffre * GetData();

	void Init();
	bool IsComplete();

// Donn�es de bo�te de dialogue
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

	// Liste des terrains (s�lection multiple)
	CLabel m_ttlTerrain;
	// JFCListBox m_lstTerrain;
	JFCSelListBox m_lstTerrain;

	// Liste des segments (s�lection multiple)
	CLabel m_ttlSegment;
	// JFCListBox m_lstSegment;
	JFCSelListBox m_lstSegment;

	// Liste des poids (s�lection simple)
	CLabel m_ttlPoids;
	JFCListBox m_lstPoids;

	void SetColors();
	void SetupControls();

	// Warning enqu�te
	JBool m_OkWarningEnqueteAEPM2006;
	JBool m_OkWarningEnqueteFCA2007;

};
