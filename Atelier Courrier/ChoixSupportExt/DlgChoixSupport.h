#pragma once

#include <Resizer.h>
#include <BtnST.h>
#include "Resource.h"
#include "label.h"
#include "JLib.h"
#include "afxwin.h"

#include "DlgEchelle.h"
#include "DlgMapping.h"
#include "DlgCumulativite.h"
#include "DlgDuplication.h"
#include "DlgPupitreEch.h"
#include "DlgPupitreMap.h"
#include "DlgPupitreCum.h"
#include "DlgPupitreDup.h"
#include "DialogFiltres.h"
#include "DlgChoixMoteur.h"
#include "JATPDocChoixSupport.h"
#include "afxcmn.h"

#include "JFCSelListBox.h"

// CDlgChoixSupport dialog
class CDlgChoixSupport : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgChoixSupport)

public:
	// constructeur standard
	CDlgChoixSupport(CWnd* pParent = NULL);   

	// destructeur standard
	virtual ~CDlgChoixSupport();

	// les fonctions pour charger/sauver les documents
	virtual BOOL OnLoadPlateau(CString path, bool bDialog = false);

	// Fermeture dialogue
	virtual JBool CanClose();

    // Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_CHOIXSUPPORT };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);	// Prise en charge DDX/DDV

	// Fonctions générées de la table des messages
	virtual BOOL OnInitDialog();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// DECLARE_MESSAGE_MAP()

public:
	// Le document Choix Support
	JATPDocChoixSupport m_DocChoixSupport;

	// Color Scheme fenêtre dialogue ChoixSupport
	CATPColors::APPCOLOR								m_CATPColorsApp;
	
private:
	// Bouton visualisation échelle / mapping / cumulativité / duplication
	CButtonST m_BtnEchelle;
	CButtonST m_BtnMapping;
	CButtonST m_BtnCumulativite;
	CButtonST m_BtnDuplication;

	// Controles sélection terrains
	CLabel m_LblTerrains;
	// JFCListBox m_LstTerrains;
	JFCSelListBox m_LstTerrains;
	
	// Controles sélection cibles
	CLabel		m_LblCibles;
	// JFCListBox	m_LstCibles;
	JFCSelListBox	m_LstCibles;

	// Controles sélection Segment Population
	CLabel		m_LblSegPop;
    // JFCListBox	m_LstSegPop;
	JFCSelListBox	m_LstSegPop;

	// Controles sélection Ref Affinité
	CLabel		m_LblRefAff;
	JFCListBox	m_LstRefAff;

	// Controles sélection formats
	CLabel		m_LblFormats;
	JFCListBox	m_LstFormats;

	// Contrôles sélection mode affichage supports (par familles ou par supports ou les 2 à la fois)
	CLabel		m_LblNiveau;

	// Bouton sélection mode famille pour affichage supports
	CButtonST	m_BtnFamilles;

	// Bouton passage au mode affichage par supports
	CButtonST	m_BtnSupports;

	// Bouton pour boite de dialogue filtre d'audience
	CButtonST	m_BtnFiltreAudience;

	// Bouton affichage fiche détaillée support
	CButtonST	m_BtnFicheSupport;

	// Bouton passerelle Creuset
	CButtonST	m_BtnCreuset;

	// Controle cadres
	CLabel m_CadreSelection;
	CLabel m_CadreResult;
	CLabel m_CadreSelSpe;

	// Cadre de la barre horizontale onglet sélection fenêtres Choix Supports
	CLabel m_CadreOnglet;

	// Zone Cadre Pupitre Fixe
	CLabel m_CadreBtnPupitre;

public:
	// La page choix support active
	JFCDialog				*m_pActivePage;

private:
	// La page pupitre spécifique à chaque page
	JFCDialog				*m_pActivePupitre;

	// Les pages principales choix support
	CDlgEchelle				*m_pPageEchelle;
	CDlgMapping				*m_pPageMapping;
	CDlgCumulativite		*m_pPageCumulativite;
	CDlgDuplication			*m_pPageDuplication;

	// Les zones sélections pupitres spécifiques à chaque page
	CDlgPupitreEch			*m_pPupitreEch;
	CDlgPupitreMap			*m_pPupitreMap;
	CDlgPupitreCum			*m_pPupitreCum;
	CDlgPupitreDup			*m_pPupitreDup;

	
private:

	// Définit les relations entre objet de la fenêtre
	void SetWindowRelations();
	
	// Association boutons
	void SetupControls();

	// Définition des couleurs interface
	void SetColors();

	// Mise en place des différentes pages (viendront se plaquer sur la fenêtre de base)
	void UpdateDlgPos(BOOL b_show = FALSE);

	// Redéfinition des couleurs interface Choix Support
	void InitCouleurCs(JUnt32 IndiceCouleur);

	// Mise à jour Nb Insertions et ss-Menu (pour module Echelle)
	void MajNbInsertions(JUnt32 NbInsertions);

	// Sauvegarde du plateau courant si bon vouloir utilisateur
	void SavePlateauViaQuestion();

	// Mise à jour des sélecteurs (Filtre audience, Contexte lecture etc ...)
	void UpdateSelecteur();

	// Mise à jour des formats supports dans chacun des modules
	void UpdateFormatSupports(JUnt32 IdFormat, bool ReplacePrecFormat);

	// Mise à jour des formats supports dans chacun des modules
	void UpdateModuleFormatSupports(JUnt32 IdFormat, JInt32 TypeModule);

	// Mise à jour boutons sélections "Familles/Supports"
	void UpdateBtnModeSelSupports();

	// Avertissement si cible non représentative (Nb cas non atteint via seuil rédhibitoire)
	void AvertissementCible(JMap <JUnt32, JUnt32> *pMapCibleSel);

private :

	////////////////////////// Gestion des menus et ss menu ///////////////////////
	// Validation ss-menu insertion
	void OnValideSsMenuNbInsertion(JUnt32 NbMaxInsertion);

	// Dévalidation ss-menu insertion
	void OnInvalideSsMenuNbInsertion();

	// Création d'un nouveau plateau
	void CreerNvPlateau();

private:
	// Objet pour mise en page
	CResizer m_Resize;

protected:
	// les menus dialogue Choix Support
	enum {POS_MENU_PLATEAU = 0, POS_MENU_OFFRES, POS_MENU_OPTIONS, POS_MENU_AFFICH, POS_MENU_FEN, POS_MENU_HELP};


	// Sauvegarde document
	virtual BOOL OnSaveDocument(JATPDocChoixSupport & ATPDocChoixSupport, JBool bRename);

	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

	// Mise à jour des sélections terrains / cibles / Seg. Pop / réf. affinité /formats
	void UpdateSelTerrains();
	void UpdateSelCibles();
	void UpdateSelSegPop();
	void UpdateSelRefAff();
	void UpdateSelFormats();

	// Requête pour savoir si cible valide sur tous les terrains sélectionnés
	JBool AuMoins1TerrainNonAutorise(JUnt32 IdCible, CString & TxtAvertissement);

	// Mise sélection supports dans toutes les fenêtres associés
	bool UpdateSelSupports();

	// Mise à jour des indications grandeurs (pour les reprises en main)
	void UpdateTxtGrandeur();

	// Mise à jour des indications grandeurs (pour les reprises en main)
	void UpdateTxtHeader();

	// Mise à jour des fenêtres associés
	bool UpdateFenetreFille();

	// Mise à jour des résultats
	bool UpdateResultat(JBool CalculResult);

	// Affichage barre calcul pour l'application en cours
	void AfficheBarreCalcul();

	// Nombre de segments maxi pour la source sélectionné
	JUnt32 GetNbTotSegmentTerrains();

protected:
	#define IdTimer WM_USER + 101

	// Gestion Menu
	afx_msg void OnCsPlateauNouveau();
	afx_msg void OnCsPlateauOuvrir();
	afx_msg void OnCsPlateauEnregistrer();
	afx_msg void OnCsPlateauEnregistrersous();
	afx_msg void OnCsPlateauFermer();
	afx_msg void OnCsPlateauQuitter();
	afx_msg void OnCsSources();
	afx_msg void OnCsSupports();
	afx_msg void OnCsCibles();
	afx_msg void OnCsFormats();
	afx_msg void OnCsPeriodeTarif();
	afx_msg void OnUpdateCsPlateauOuvrir(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPlateauEnregistrer(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPlateauEnregistrersous(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPlateauFermer(CCmdUI *pCmdUI);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	

	// Gestion des couleurs
	afx_msg void OnCsPalette1();
	afx_msg void OnCsPalette2();
	afx_msg void OnCsPalette3();
	afx_msg void OnCsPalette4();
	afx_msg void OnCsPalette5();
	afx_msg void OnCsPalette6();
	afx_msg void OnCsPalette7();
	afx_msg void OnCsPalette8();
	afx_msg void OnCsPalette9();
	afx_msg void OnCsPalette10();
	afx_msg void OnCsPalette11();
	afx_msg void OnCsPalette12();
	afx_msg void OnCsPalette13();
	afx_msg void OnCsPalette14();
	afx_msg void OnCsPalette15();

	// Gestion Nb insrtions
	afx_msg void OnCs1Insertion();
	afx_msg void OnCs2Insertions();
	afx_msg void OnCs3Insertions();
	afx_msg void OnCs4Insertions();
	afx_msg void OnCs5Insertions();
	afx_msg void OnCs6Insertions();
	afx_msg void OnCs7Insertions();
	afx_msg void OnCs8Insertions();
	afx_msg void OnCs9Insertions();
	afx_msg void OnCs10Insertions();
	afx_msg void OnCs12Insertions();
	afx_msg void OnCs24Insertions();
	afx_msg void OnCs53Insertions();
	afx_msg void OnCs105Insertions();
	afx_msg void OnCs365Insertions();

	afx_msg void OnCsMaxInsertions();
	afx_msg void OnBnClickedCsEchelles();
	afx_msg void OnBnClickedCsMapping();
	afx_msg void OnBnClickedCsCumulativite();
	afx_msg void OnBnClickedCsDuplication();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLbnSelchangeCsListTerrains();
	afx_msg void OnLbnSelchangeCsListCibles();
	afx_msg void OnLbnSelchangeCsListFormats();
	afx_msg void OnCsPlateauImportEtude();
	afx_msg void OnCsVersion();
	afx_msg void OnCsOptRepriseenmain();
	afx_msg void OnUpdateCsOptRepriseenmain(CCmdUI *pCmdUI);
	afx_msg void OnCsOptLnmmagazine();
	afx_msg void OnUpdateCsOptLnmmagazine(CCmdUI *pCmdUI);
	afx_msg void OnCsOptLdpquotidien();
	afx_msg void OnUpdateCsOptLdpquotidien(CCmdUI *pCmdUI);
	afx_msg void OnCsModel();
	afx_msg void OnUpdateCsModel(CCmdUI *pCmdUI);
	afx_msg void OnCsModeletal();
	afx_msg void OnUpdateCsModeletal(CCmdUI *pCmdUI);
	afx_msg void OnCsProbabilit();
	afx_msg void OnUpdateCsProbabilit(CCmdUI *pCmdUI);
	afx_msg void OnCsBasetarifplaq();
	afx_msg void OnUpdateCsBasetarifplaq(CCmdUI *pCmdUI);
	afx_msg void OnCsBasetarifnego();
	afx_msg void OnUpdateCsBasetarifnego(CCmdUI *pCmdUI);
	// afx_msg void OnCsFiltreAudience();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);

	afx_msg void OnCsOptionMoteur();
	afx_msg void OnUpdateCsOptionMoteur(CCmdUI *pCmdUI);

protected:
	// Boite de dialogue Filtre d'audience
	CDialogFiltres m_DlgFiltresAudience;

	// Boite de dialogue choix moteur
	CDlgChoixMoteur m_DlgChoixMoteur;
	
public:
	afx_msg void OnUpdateCsFiltreAudience(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsSources(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsSupports(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsCibles(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsFormats(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPeriodetarif(CCmdUI *pCmdUI);

private :
	// Mise à jour entete fenêtre active
	void MajTitreFenetreCS();
	
	// Mise à jour des sélections fenêtre active
	void MajSelFenetreCS();

	// Positionne flags modif calcul pour tous les modules
	void ParamCalculModif();

	// Chargement des dernieres options moteurs
	void LoadOptionMoteur(JATPDocChoixSupport &ATPDocChoixSupport);

public:
	afx_msg void OnCsOrdreEntetes();
	afx_msg void OnUpdateCsOrdreEntetes(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsMaxInsertions(CCmdUI *pCmdUI);

private:
	BOOL m_bFirstTime;
    static const CRect s_rectDefault;

	// Flag Resize
	JBool FlagResize;


public:
	afx_msg void OnBnClickedCsFiltreAudience();

	afx_msg void OnBnClickedCsCreuset();
	afx_msg void OnOptionsLDP30j();
	afx_msg void OnUpdateOptionsLDP30j(CCmdUI *pCmdUI);
	afx_msg void OnSysColorChange();

	// Messages d'impression résultats par module
	afx_msg void OnCsPrintEchelle();
	afx_msg void OnUpdateCsPrintEchelle(CCmdUI *pCmdUI);
	afx_msg void OnCsPrintMapping();
	afx_msg void OnUpdateCsPrintMapping(CCmdUI *pCmdUI);
	afx_msg void OnCsPrintCumulation();
	afx_msg void OnUpdateCsPrintCumulation(CCmdUI *pCmdUI);
	afx_msg void OnCsPrintDuplication();
	afx_msg void OnUpdateCsPrintDuplication(CCmdUI *pCmdUI);
	afx_msg void OnCsPrintEchParcible();
	afx_msg void OnUpdateCsPrintEchParcible(CCmdUI *pCmdUI);

	afx_msg void OnCsOrdreGrandech();
	afx_msg void OnUpdateCsOrdreGrandech(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsNInsertion(CCmdUI *pCmdUI);

	afx_msg void OnBnClickedCsLabelFamilles();
	afx_msg void OnBnClickedCsLabelSupports();

	// Messages pour précision résultat en décimales
	afx_msg void OnNbdAucunedecimale();
	afx_msg void OnNbd1decimale();
	afx_msg void OnNbd2decimale();
	afx_msg void OnCsAucunedecimale();
	afx_msg void OnCs1decimale();
	afx_msg void OnCs2decimale();
	afx_msg void OnUpdateNbddecimale(CCmdUI *pCmdUI);

	// Messages gestion des rangs
	afx_msg void OnCsRangColVisible();
	afx_msg void OnUpdateCsRangColVisible(CCmdUI *pCmdUI);
	afx_msg void OnCsAffichageRangResult();
	afx_msg void OnUpdateAffichageRangResult(CCmdUI *pCmdUI);

	// Message gestion des moyennes
	afx_msg void OnCsMoyenneVisible();
	afx_msg void OnUpdateCsMoyenneVisible(CCmdUI *pCmdUI);
	
	// Message d'exports presse papier pour chaque module
	afx_msg void OnPlateauExporterechelledanspressepapier();
	afx_msg void OnUpdatePlateauExporterechelledanspressepapier(CCmdUI *pCmdUI);
	afx_msg void OnExporterdanslepressepapierEchelles();
	afx_msg void OnUpdateExporterdanslepressepapierEchelles(CCmdUI *pCmdUI);
	afx_msg void OnExporterdanslepressepapierMapping();
	afx_msg void OnUpdateExporterdanslepressepapierMapping(CCmdUI *pCmdUI);
	afx_msg void OnExporterdanslepressepapierCumulativit();
	afx_msg void OnUpdateExporterdanslepressepapierCumulativit(CCmdUI *pCmdUI);
	afx_msg void OnExporterdanslepressepapierDuplication();
	afx_msg void OnUpdateExporterdanslepressepapierDuplication(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterPressepapierEch();
	afx_msg void OnUpdateCsExporterPressepapierEch(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterPressepapierEchParcible();
	afx_msg void OnUpdateCsExporterPressepapierEchParcible(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterPressepapierMap();
	afx_msg void OnUpdateCsExporterPressepapierMap(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterPressepapierCum();
	afx_msg void OnUpdateCsExporterPressepapierCum(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterPressepapierDup();
	afx_msg void OnUpdateCsExporterPressepapierDup(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterPressepapierGraphmapping();
	afx_msg void OnUpdateCsExporterPressepapierGraphmapping(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterPressepapierGraphcumul();
	afx_msg void OnUpdateCsExporterPressepapierGraphcumul(CCmdUI *pCmdUI);

	// Message d'exports fichier pour chaque module
	afx_msg void OnCsExporterFichierEch();
	afx_msg void OnUpdateCsExporterFichierEch(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterFichierEchParcible();
	afx_msg void OnUpdateCsExporterFichierEchParcible(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterFichierMap();
	afx_msg void OnUpdateCsExporterFichierMap(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterFichierCum();
	afx_msg void OnUpdateCsExporterFichierCum(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterFichierDup();
	afx_msg void OnUpdateCsExporterFichierDup(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterFichierGraphmapping();
	afx_msg void OnUpdateCsExporterFichierGraphmapping(CCmdUI *pCmdUI);
	
	// export excel (ouverture dans Excel)
	afx_msg void OnCsExporterExcelEch();
	afx_msg void OnUpdateCsExporterExcelEch(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterExcelMap();
	afx_msg void OnUpdateCsExporterExcelMap(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterExcelCum();
	afx_msg void OnUpdateCsExporterExcelCum(CCmdUI *pCmdUI);
	afx_msg void OnCsExporterExcelDup();
	afx_msg void OnUpdateCsExporterExcelDup(CCmdUI *pCmdUI);

	// Divers
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnLbnSelchangeCsListSegPop();
	afx_msg void OnCsExporterFichierGraphcumul();
	afx_msg void OnUpdateCsExporterFichierGraphcumul(CCmdUI *pCmdUI);
	// afx_msg void OnCsOptCumulFamille();
	// afx_msg void OnUpdateCsOptCumulFamille(CCmdUI *pCmdUI);
	afx_msg void OnCsOptFamCumul();
	afx_msg void OnCsOptFamMoyenne();
	afx_msg void OnUpdateCsOptFamCumul(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsOptFamMoyenne(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette7(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette8(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette9(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette10(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette11(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette12(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette13(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette14(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCsPalette15(CCmdUI *pCmdUI);
};
