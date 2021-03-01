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

	// Initialisation de la fen�tre s�lection sites
	virtual BOOL OnInitDialog();

	// R�cup�ration des sites s�lectionn�s
	virtual void GetSelectedItems(JList<CItemWrapper *> & Items);

	// Positionnement des sites s�lectionn�s
	virtual void SetSiteSel(JList <JLabel> & LstSiteSel);

	// Positionne le terrain t�moin pour s�lection des sites
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

	// Map des sites en cours de s�lection
	JMap<JInt32,JBool>							m_MapSiteSel;

	// Map des labels sites d�j� pr�c�demment s�lectionn�s
	JList<JLabel>								m_LstSiteSel; 

	// Sauve et r�cup�re les s�lections en cours
	void SaveSelCur();
	void RecupSelCur();

	// Mise � jour des sites s�lectionn�s via pr�c�dente s�lection
	void MarqueSelPrevious();
	
	// Affichage des informations sur la s�lection des sites
	void AffichInfoSite();

	// Mise � jour info site
	void MajInfoSite();

	// R�cup�ration nombre total de s�lection
	JUnt32 GetNbTotSel();

	// Controle boite de recherche texte
	CEdit										m_EditSearch;

	// Terrain t�moin
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
