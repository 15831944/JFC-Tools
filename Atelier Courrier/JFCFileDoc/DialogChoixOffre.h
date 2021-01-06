/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CDialogChoixOffre
//
//	Descriptif:
//	Boite de dialogue du choix de l'offre, accueille les boites filles suivantes
//	CDlgSource CDlgSrcUnique CDlgSrcMultiple CDlgSupport CDlgCible
//	Utilisé dans le creuset et dans choix-support
*/

#pragma once


//JFCSupport
#include <AllTypeOfNode.h>
#include <IDLimits.h>

//JFCTable
#include <JSRCPresse.h>
#include <JSRCTables.h>

//JFCSource
#include <Terrain.h>

//JFCItems
#include <ItemsConstruits.h>

//JFCControls
#include <JFC_MultiListBox.h>
#include <JFCListBox.h>
#include <ClrSelComboBox.h>
#include <Label.h>
#include <BtnST.h>

#include "JATPOffre.h"
#include "JIDList.h"
#include "KeyTerrain.h"
#include "resource.h"
#include "ATPColors.h"

#include "OffreTerrainManager.h"

#include "ETSLayout.h"

#include <list>

// Boîte de dialogue CDialogChoixOffre
class AFX_EXT_CLASS CDialogChoixOffre : public ETSLayoutDialog
{
	//declarer en friend les fenetres filles
	friend class CDlgSrcUnique;
	friend class CDlgSupport;
	friend class CDlgCible;
	friend class CRedListBox;
	friend class CRedTree;

	DECLARE_DYNAMIC(CDialogChoixOffre)

public:
	CDialogChoixOffre(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDialogChoixOffre();

	enum STARTUPMODE {SOURCE, SUPPORT, CIBLE, IMPORT};///le mode de démarrage
	void SetStartupMode(STARTUPMODE start, CATPColors::APPCOLOR app);///Pour choisir le mode de démarrage

	///Pour savoir s'il y a eu une modification
	bool IsModified();

	void SetOffre(JATPOffre& offre);

	void GetOffre(JATPOffre& offre);

	// Récupère offre restreinte aux terrains (sélection - Red Sélection)
	void GetOffreRestreinte(JATPOffre& offre);

	// Données de boîte de dialogue
	enum idd{ IDD = IDD_FD_DLG_OFFRE };


protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnOK() {/*do nothing*/};
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSource();
	afx_msg void OnBnClickedBtnSupport();
	afx_msg void OnBnClickedBtnCible();
	afx_msg void OnBnClickedBtnSeuils();
	afx_msg void OnBnClickedBtnInfoSeuils();
	afx_msg void OnBnClickedBtnImport();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	void EnableOK();
	void RazSeuils();
	void CheckItems();
	bool TestItem(JUnt32 IdItem, CString &txt);

	// idTerrainSel : dernier terrain selectionnée, si 0 : aucun terrain selectionné dans le dernier click 
	void CheckSupports(std::list<JUnt32> & lAddedTerrains) const;
	
	// test d'un support sur un terrain particulier et un seul
	bool TestSupport(JUnt32 IdSupport, const std::list<JUnt32> & lAddedTerrains, CString &txt) const ;

	// Validité support via les terrains sélectionnés
	JBool SupportValid(IElem* pelem);

	// Pour détection des doublons dans tableau supports
	JBool NewSupportTblSupp(JList<JUnt32> &TblSupports, JUnt32 IdSupp);

	//les fenetres
	CDialog* m_pActivePage; /// la fenêtre active

	// Fenêtre sélection des terrains
	CDlgSrcUnique*	m_pdlgSource;

	// Fenêtre sélection des supports
	CDlgSupport*	m_pdlgSupport;

	// Fenêtre sélection des cibles
	CDlgCible*		m_pdlgCible;

	bool m_bModif;
	STARTUPMODE m_startMode;
	
	///L'offre
	// JATPOffre m_offre;
	//JMap< CKeyTerrain , CTerrain * > m_mapLoadedTerrain;

	COffreTerrainManager * m_OffreTerrainManager;
	
	///Pour memoriser la selection initiale
	JUnt32 idSrcAudienceduDoc;
	JList<JUnt32> m_lstidTerAudienceduDoc;
	JUnt32 idSrcCiblageduDoc;
	JUnt32 idTerrCiblageduDoc;

/////Les données pour les supports
	JIDList m_lstIDLot;
	JIDList m_lstIDTitre;
	JIDList m_lstIDCouplage;


///les elements de la boite
	CButtonST m_btnOK;
	CButtonST m_btnCancel;

	CButtonST m_btnImport;
	CButtonST m_btnSeuils;
	
	// Bouton permettant d'afficher les infos seuils des enquêtes sélectionnées
	CButtonST m_btnInfoSeuils;

	CButtonST m_btnSource;
	CButtonST m_btnSupport;
	CButtonST m_btnCible;
	bool m_preselcible;

///init de la boite
	void SetupControls();
	void SetIco();
	void SetColor();
	void UpdateDlgPos(BOOL b_show = FALSE);

	// Affichage informations Source et Terrain(s) en cours
	void AfficheTitreFenetre();

	CATPColors::APPCOLOR m_appColor;

friend class CDlgCible;
	JATPOffre m_offre;
	
};
