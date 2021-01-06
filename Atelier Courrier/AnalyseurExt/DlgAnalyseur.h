#pragma once

#include "ICellResult.h"
#include "resource.h"
#include "Resizer.h"
#include "label.h"
#include "BtnST.h"
#include "JFCSelListBox.h"
#include "XListBox.h"
#include "AnCrossTable.h"
#include "AnalyseurDoc.h"
#include "DlgSurlignage.h"
#include "IItemTable.h"
#include "DlgTableau.h"
#include "afxwin.h"

class COpeSurlignage;

// CDlgAnalyseur dialog

class CDlgAnalyseur : public CDialog
{
	DECLARE_DYNAMIC(CDlgAnalyseur)

public:
	CDlgAnalyseur(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAnalyseur();
	void OpenFile(CString path);
	// Dialog Data
	enum { IDD = IDD_AN_DLG_ANALYSEUR };

	enum Option
	{
		Quantities	=	1,
		Cibles		=	2,
	};

public:
	// pour quitter l'application
	BOOL CanClose();
	void Close();

public :
	// Couleur d'affichage résultat

public:
	// Par défaut les paramètres d'affichage
	COpeSurlignage					m_Default_OpeSurlignage;	

	CDlgTableau	 *				    m_pDlgTableau;

public:
	// Mise à jour titre tris croisés
	virtual void SetTitre();

	// Set cursor grille
	void SetGridCursor();

	// Positionne curseur tri
	void SetCursorTri(JBool Tri);

	// Recup info si au moins 1 grandeur moyennable de sélectionner
	void UpdateElmtMoyennable();

	// Test si item quantitatif 
	JBool ItemQuantitatif(const CItemConstruit * pItem, CTerrain * pTerrain);

	// Get selected cible
	int GetIndexSelCible();

	const CItemConstruit * GetItemCible(int Sel);


public:
	// A remettre en protected
	CAnalyseurDoc *					m_pDoc;

	// Liste des variables moyennables en mode tableau
	CXListBox						m_List_VarMoy;

protected:
	/// The Dialogs Icon handle
	HICON							m_hIcon;
	/// Resize class
	CResizer						m_Resizer;

	CLabel							m_Box_Album;

	CLabel							m_Box_Grid;
	CLabel							m_Box_Strates;
	CLabel							m_Grid_Title;
	CProgressCtrl					m_ProgCtrl;

	CLabel							m_Box_Config;

	// Sélection des terrains
	CLabel							m_Label_Terrains;
	JFCSelListBox					m_List_Terrains;
	JFCSelListBox					m_List_TmpTerrains;

	// Bouton Up/Down Terrains
	CButtonST						m_But_Up_Terrain;
	CButtonST						m_But_Down_Terrain;

	// Sélection des cibles
	CLabel							m_Label_Cibles;
	// JFCListBox						m_List_Cibles;
	CXListBox						m_List_Cibles;

	// Sélection des grandeurs
	CLabel							m_Label_Grandeurs;
	JFCSelListBox					m_List_Grandeurs;

	CLabel							m_Label_Tableau;
	CButtonST						m_But_Vectorielle;
	CButtonST						m_But_Matricielle;

	CLabel							m_Label_Marquage;

	// CButtonST						m_But_Base100;

	// Bouton permettant de revenir en arrière pour le surlignage
	CButtonST						m_Btn_PrecSurlign;

	// Bouton appel fenêtre paramétrage surlignage
	CButtonST						m_But_Surlignage;

	// Bouton supprimant toutes les opérations surlignage sur le tableau
	CButtonST						m_Btn_Raz_AllSurlign;

	// Bouton appel fenêtre affichage ordonnancement ou Ranking
	CButtonST						m_But_Ordonner;

	// Bouton pour les variables moyennables
	CLabel							m_Label_VarMoy;

	// Bouton appel sélection boite items à moyenner
	CButtonST						m_Btn_Moyenne_Tableau;

	// Bouton suppression des variables moyennables
	CButtonST						m_Btn_Raz_VarMoy;

	/*
	//Enumeration des différents modes de tri
	enum ModeTri {MODE_LIGNE = 0, MODE_BLOC};

	// Enumération des différents type de tri
	enum TypeTri {TYPE_BASE = 0, TYPE_DECROISSANT, TYPE_CROISSANT};
	*/

	// Sélection du tri courant
	CLabel							m_Label_Tri;
	CComboBox						m_Combo_TriGrandeurs;		// Sélection de la grandeur pour le tri
	CComboBox						m_Combo_TriResultats;		// Sélection du type de résultat
	CComboBox						m_Combo_TriType;			// Sélection du type de tri (croissant, décroissant, retour origine)
	CComboBox						m_Combo_TriMode;			// Sélection mode tri (sur toutes les lignes, par blocs de critères)

	CLabel							m_Label_Result;

	// Boutons sélection grandeur disposition grandeurs mode 2 * 2
	CButtonST						m_But_Grandeur;
	CButtonST						m_But_Horiz;
	CButtonST						m_But_Vert;
	CButtonST						m_But_Global;				// Bouton spécial (avec click droit menu contextuel)

	// Boutons sélection grandeur disposition grandeurs mode llignes
	CButtonST						m_But_Grandeur2;
	CButtonST						m_But_Horiz2;
	CButtonST						m_But_Vert2;
	CButtonST						m_But_Global2;				// Bouton spécial (avec click droit menu contextuel)

	// Boutons sélection grandeur disposition grandeurs mode colonnes
	CButtonST						m_But_Grandeur3;
	CButtonST						m_But_Horiz3;
	CButtonST						m_But_Vert3;
	CButtonST						m_But_Global3;				// Bouton spécial (avec click droit menu contextuel)

	// Boutons exclusifs et duplicants (maintenant inutilisé et reportés dans menu) 
	CButtonST						m_But_Exclusifs;
	CButtonST						m_But_Dupliquants;

	CAnCrossTable					m_CrossTable;
	IItemTable *					m_pCrossTable;

	// CAnalyseurDoc *					m_pDoc;

	CDlgSurlignage					m_DlgSurlignage;

	// CDlgTableau	 *				    m_pDlgTableau;

	// Objet Tooltip - Visu en live
	CToolTipCtrl					m_ToolTip;

	// Edition cellle tableur
	CEdit							m_EditInfoCell;

	// Mode base 100, actif / non actif
	BOOL							m_ModeBase100Actif;

	// Mode Exclusif
	BOOL							m_ModeExclusif;

	// Mode Duplicant
	BOOL							m_ModeDuplicant;

	// Mode Tri
	BOOL m_ModeCurseurTri;

	// Template paramétrage pour les items standards
	HXTemplateHandle				m_TemplateStandard; 

	// Template paramétrage liste des items quanntis
	HXTemplateHandle				m_TemplateQuanti; 

private:
	BOOL m_bFirstTime;
    static const CRect s_rectDefault;

protected:
	virtual void SetupControls();
	virtual void SetupGrandeursBase();
	
	// Définition des sélections pour le tri
	virtual void SetupSelTri();

	virtual void SetColors();
	virtual void SetWindowRelations();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void GetGrandeurs();
	virtual void SetGrandeurs();
	virtual bool LoadTerrains(const JUnt32 SourceId, const JList<UINT> & TerrainIds, const JList<UINT> & Segments, const JUnt32 TypePoidsId);
	virtual void CheckItems();
	virtual void SetSelectedUniverses(bool bChanged);
	
	virtual void UpdateListCibles();
	virtual void SetupGrandeurs(JBool Reset = true);
	virtual void LoadProfile();
	virtual void SaveProfile();
	virtual void SetResultControls();
	virtual void SetupCrossTable();
	virtual void SuppOptionButGlobal(CMenu *pMenu);
	virtual void ClickedAnButHoriz();

	// Gestion Bouton Global (Click gauche -> résultat globaux visible ou pas
	virtual void LClickedAnButGlobal();

	virtual void ClickedAnButGlobal();

	// Gestion Bouton Global (Click droit gauche -> sélection type résultat globaux
	virtual void RClickedAnButGlobal();

	// Aller dans pupitre
	virtual void GoPupitre();

	// Suppression sélection
	virtual	void SupprimeSelection(); 

	// Mise en place élmt du Grid suivi du Calcul
	virtual void MiseEnPlaceGridTC(JBool NewCalcul = true);

	// Retaillage de la fenêtre ordonnancement
	virtual void RetailleFenOrdonnancement();

	virtual JBool OkFileClose();

	// Redispositin des boutons sélection grandeur (pour respecter la disposition sur la grille)
	void DispoBtnGrandeurs();

	// Update couleur bouton surlignage précédent
	void UpdateColorBtnPrecSurlign();

	// Mise à jour du surlignage via les opérations de surlignage
	void UpdateSurlignage();

	// Update des éléments concernant les variables moyennables en mode tableau
	void UpdateElmtVarMoy();

	// Test si item quantitatif 
	// JBool ItemQuantitatif(const CItemConstruit * pItem, CTerrain * pTerrain);

	// Repositionnement des ascenseurs horizontals et verticals
	void GoToInitialPosCroll(ROWCOL & TopRow, ROWCOL & LeftCol, UINT StyleStructure, JBool ChkBtn);

	// Repositionnement des ascenseurs horizontals et verticals après changement mode resultat cellule
	void GoToInitialPosCroll2(ROWCOL & TopRow, ROWCOL & LeftCol, int ExModeAffich, int NewModeAffich);

	// Repositionnement des ascenseurs horizontals et verticals après changement dans liste des grandeurs
	void GoToInitialPosCroll3(ROWCOL & TopRow, ROWCOL & LeftCol, int ExNbGrandeurs, int NewNbGrandeurs);

public:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnAboutbox();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysColorChange();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnUpdatePalette1(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette2(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette3(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette4(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette5(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette6(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette7(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette8(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette9(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette10(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette11(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette12(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette13(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette14(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePalette15(CCmdUI *pCmdUI);
	afx_msg void OnPalette1();
	afx_msg void OnPalette2();
	afx_msg void OnPalette3();
	afx_msg void OnPalette4();
	afx_msg void OnPalette5();
	afx_msg void OnPalette6();
	afx_msg void OnPalette7();
	afx_msg void OnPalette8();
	afx_msg void OnPalette9();
	afx_msg void OnPalette10();
	afx_msg void OnPalette11();
	afx_msg void OnPalette12();
	afx_msg void OnPalette13();
	afx_msg void OnPalette14();
	afx_msg void OnPalette15();
	afx_msg void OnBnClickedButVectorielle();
	afx_msg void OnLbnSelchangeAnListGrandeurs();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnChangerTerrains();
	afx_msg void OnLbnSelchangeAnListTerrains();
	afx_msg void OnTableauTransposer();
	afx_msg void OnLbnSelchangeAnListCibles();
	afx_msg void OnDefinirStructureTableau();

	afx_msg void OnLbnSelchangeAnListVarMoy();

	// afx_msg void OnBnClickedAnButBase100();

	afx_msg void OnBnClickedAnButUpTerrain();
	afx_msg void OnBnClickedAnButDownTerrain();

	afx_msg void OnBnClickedAnButOrdonner();

	afx_msg void OnBnClickedAnButMoyenneTableau();
	afx_msg void OnBnClickedAnButRazVarMoy();

	afx_msg void OnBnClickedAnButParamSurlignage();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnAnOptionsQuantity();
	afx_msg void OnUpdateAnOptionsQuantity(CCmdUI *pCmdUI);
	afx_msg void OnInterpreterUniversCommeCible();
	afx_msg void OnUpdateInterpreterUniversCommeCible(CCmdUI *pCmdUI);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileClose();
	afx_msg void OnTableauQuitter();
	afx_msg void OnTableauAppliquerCanevas();
	afx_msg void OnTableauEnregistrerCanevas();

	afx_msg void OnBnClickedAnButGrandeur();
	afx_msg void OnBnClickedAnButVert();
	afx_msg void OnBnClickedAnButHoriz();
	afx_msg void OnBnClickedAnButGlobal();
	afx_msg void OnBnClickedAnButGrandeur2();
	afx_msg void OnBnClickedAnButVert2();
	afx_msg void OnBnClickedAnButHoriz2();
	afx_msg void OnBnClickedAnButGlobal2();
	afx_msg void OnBnClickedAnButGrandeur3();
	afx_msg void OnBnClickedAnButVert3();
	afx_msg void OnBnClickedAnButHoriz3();
	afx_msg void OnBnClickedAnButGlobal3();

	afx_msg void OnBnClickedAnButExclu();
	afx_msg void OnBnClickedAnButDupli();
	afx_msg LRESULT OnSeuilsChanged(WPARAM wParam, LPARAM lParam);

	// Gestion menu contextuel en cliquant avec bouton droit
	afx_msg LRESULT OnBnLClickedAnButGlobal(WPARAM wParam, LPARAM lParam);

	afx_msg void OnTableauExporterPressepapier();
	afx_msg void OnUpdateTableauExporterPressepapier(CCmdUI *pCmdUI);
	afx_msg void OnTableauExporterFichier();
	afx_msg void OnUpdateTableauExporterFichier(CCmdUI *pCmdUI);
	// afx_msg void OnTableauExporterExcel();
	// afx_msg void OnUpdateTableauExporterExcel(CCmdUI *pCmdUI);
	afx_msg void OnTableauExporterExcel_AvecDecoup();
	afx_msg void OnUpdateTableauExporterExcel_AvecDecoup(CCmdUI *pCmdUI);
	afx_msg void OnTableauExporterExcel_SansDecoup();
	afx_msg void OnUpdateTableauExporterExcel_SansDecoup(CCmdUI *pCmdUI);
	afx_msg void OnTableauImprimer();
	afx_msg void OnChoisirUnivers();
	afx_msg void OnUpdateChoisirUnivers(CCmdUI *pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnDoubleclickedAnButGlobal();
	afx_msg void OnAnParametresPriseEnCompteMargerreur();
	afx_msg void OnUpdateAnParametresPriseEnCompteMargerreur(CCmdUI *pCmdUI);

	afx_msg void OnAnParametresForceQuanti();
	afx_msg void OnUpdateAnParametresForceQuanti(CCmdUI *pCmdUI);
	

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnAnParamExclusifs();
	afx_msg void OnUpdateAnParamExclusifs(CCmdUI *pCmdUI);
	afx_msg void OnAnParamDuplicants();
	afx_msg void OnUpdateAnParamDuplicants(CCmdUI *pCmdUI);
	afx_msg void OnAnParamBase100();
	afx_msg void OnUpdateAnParamBase100(CCmdUI *pCmdUI);
	afx_msg void OnCbnSelchangeComboTriGrandeurs();
	afx_msg void OnCbnSelchangeComboTriResultats();
	afx_msg void OnCbnSelchangeComboTypetri();
	afx_msg void OnCbnSelchangeComboModetri();
	afx_msg void OnAnDispoLignes();
	afx_msg void OnUpdateAnDispoLignes(CCmdUI *pCmdUI);
	afx_msg void OnAnDispo2par2();
	afx_msg void OnUpdateAnDispo2par2(CCmdUI *pCmdUI);
	afx_msg void OnAnDispoColonnes();
	afx_msg void OnUpdateAnDispoColonnes(CCmdUI *pCmdUI);

	afx_msg void OnAnParamMoyenneTableau();
	afx_msg void OnUpdateAnParamMoyenneTableau(CCmdUI *pCmdUI);
	afx_msg void OnAnParamMoyenneLigne();
	afx_msg void OnAnParamMoyenneColonne();
	afx_msg void OnUpdateAnParamMoyenneLigne(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnParamMoyenneColonne(CCmdUI *pCmdUI);

	afx_msg void OnAnParamMoyenne0Inclus();
	afx_msg void OnUpdateAnParamMoyenne0Inclus(CCmdUI *pCmdUI);
	afx_msg void OnAnParamMoyenne0Exclus();
	afx_msg void OnUpdateAnParamMoyenne0Exclus(CCmdUI *pCmdUI);

	afx_msg void OnAnParamIndiceAffinite();
	afx_msg void OnUpdateAnParamIndiceAffinite(CCmdUI *pCmdUI);
	afx_msg void OnAnParamIndiceStructure();
	afx_msg void OnUpdateAnParamIndiceStructure(CCmdUI *pCmdUI);
	afx_msg void OnAnParamIndiceProximite();
	afx_msg void OnUpdateAnParamIndiceProximite(CCmdUI *pCmdUI);

	afx_msg void OnAnParamAffichEffMilliers();
	afx_msg void OnUpdateAnParamAffichEffMilliers(CCmdUI *pCmdUI);
	afx_msg void OnAnParamAffichEffGlobal();
	afx_msg void OnUpdateAnParamAffichEffGlobal(CCmdUI *pCmdUI);

	afx_msg void OnAnParamAffichMoy2Dec();
	afx_msg void OnUpdateAnParamAffichMoy2Dec(CCmdUI *pCmdUI);
	afx_msg void OnAnParamAffichMoySansDec();
	afx_msg void OnUpdateAnParamAffichMoySansDec(CCmdUI *pCmdUI);

	afx_msg void OnBnClickedAnButPrecSurlign();
	afx_msg void OnAnOutilsLinearisation();
	afx_msg void OnAnOutilsSurlignage();
	afx_msg void OnUpdateAnOutilsLinearisation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAnOutilsSurlignage(CCmdUI *pCmdUI);
	
	afx_msg void OnBnClickedAnButRazSurlign();
};
