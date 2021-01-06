#pragma once

#include "ItemConstruit.h"
#include <Label.h>
#include "BtnST.h"
#include "JFC_MultiListBox.h"
#include "ItemWrapper.h"
#include "afxwin.h"

// CSiteSelectionDlg dialog

class CSiteSelectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSiteSelectionDlg)

public:
	CSiteSelectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSiteSelectionDlg();

	// Initialisation de la fenêtre sélection sites
	virtual BOOL OnInitDialog();

	// Récupération des sites sélectionnés
	virtual void GetSelectedItems(JList<CItemWrapper *> & Items);

	// Positionnement des sites sélectionnés
	virtual void SetSiteSel(JList <JLabel> & LstSiteSel);

	// Positionne le terrain témoin pour sélection des sites
	virtual void SetTerrainTemoin(CTerrain *pTerrain);

// Dialog Data
	enum { IDD = IDD_AN_DLG_SITE };

protected:
	virtual void SetupControls();
	virtual void SetColors();

protected:
	CLabel										m_bkgSite;
	CLabel										m_ttlSite;
	CButtonST									m_But_Ok;
	CButtonST									m_But_Cancel;

	// Bouton permettant au retour affichage de tous les sites
	CButtonST									m_AllSites;
	CButtonST									m_NoSite;

	// Liste des sites
	JFC_MultiListBox<const CItemConstruit *>	m_List_Sites;

	// Map des sites
	JMap<JInt32, CItemConstruit *>				m_ImpMap;
	JList<JInt32>								m_SelectedItems;

	// Map des sites en cours de sélection
	JMap<JInt32,JBool>							m_MapSiteSel;

	// Map des labels sites déjà précédemment sélectionnés
	JList<JLabel>								m_LstSiteSel; 

	// Sauve et récupère les sélections en cours
	void SaveSelCur();
	void RecupSelCur();

	// Mise à jour des sites sélectionnés via précédente sélection
	void MarqueSelPrevious();
	
	// Affichage des informations sur la sélection des sites
	void AffichInfoSite();

	// Mise à jour info site
	void MajInfoSite();

	// Récupération nombre total de sélection
	JUnt32 GetNbTotSel();

	// Controle boite de recherche texte
	CEdit										m_EditSearch;

	// Terrain témoin
	CTerrain									*m_pTerrainTemoin;

protected:
	#define IdTimer_Search WM_USER+101

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnChangeAnEditSearch();

public:
	afx_msg void OnBnClickedAnAllSites();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedAnNoSelSite();
	afx_msg void OnLbnSelchangeAnListSites();
};
