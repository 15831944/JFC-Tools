#pragma once

#include <memory>
#include "resource.h"
#include <Terrain.h>
#include <ItemsConstruits.h>
#include <ItemsConstruitsAuto.h>
#include "ItemsConstruitsManager.h"
#include "Defines.h"
#include "Resizer.h"
#include "JFC_MultiListBox.h"
#include "ThemeTree.h"
#include "Separator.h"
#include "label.h"
#include "BtnST.h"
#include "BitArray.h"
#include "JFCTabCtrl.h"
#include "ListBoxQuestions.h"
#include "ListBoxDimensions.h"
#include "ListBoxSas.h"
#include "BlocQuestions.h"
#include "DlgVisu.h"
#include "ListBoxItems.h"
#include "JFCListCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "IBloc.h"
#include "JFCVertSplit.h"
#include "JFCSelListBox.h"
#include "JFCSelListCtrl.h"
#include "RubriqueItemTree.h"

#include "ItemWrapper.h"
#include <afxwin.h>


#include "..\Config\BuildConfig.h"

class CAbstractMode;
typedef std::auto_ptr<CAbstractMode> PTR_ABSTRACTMODE;
/// The RepItems main Dialog
/**
* \ingroup RepItems
*
*
* \par requirements
* win98 or later\n
* win2k or later\n
* MFC\n
*
* \version 1.0
* first version
*
* \date 07-09-2003
*
* \author eddie
*
* \par license
* This code is the property of JFC Informatique & Media\n
* Unauthorized copying or use is strictly forbiden.
* 
* \todo 
*
* \bug 
*
*/


#if defined(BUILD_FOR_ATELIER_INTERNET)
#include "DlgItemsMessage.h"

#define LOADSTRING(cstr, id)	cstr = DlgItemsMessage::GetString(DlgItemsMessage::MSG_##id)
#define GETSTRING(id)			DlgItemsMessage::GetString(DlgItemsMessage::MSG_##id)
#define SETDLGITEMTEXT(id)		SetDlgItemText(id, DlgItemsMessage::GetString(DlgItemsMessage::MSG_##id));
#define LOADMENUITEMSTRING(menu, id)	menu.ModifyMenu(id, MF_STRING | MF_BYCOMMAND, id, DlgItemsMessage::GetString(DlgItemsMessage::MSG_##id))


#else

#define LOADSTRING(cstr, id)	cstr.LoadString(id)
#define GETSTRING(id)			id
#define LOADMENUITEMSTRING(menu, id)

#endif


// Les modes de construction utilisés dans le répertoire des items
typedef	enum MODE_REPITEM
{
	Mode_Items			=	1,
	Mode_Classes		=	2,
	Mode_Quantiles		=	3,
	Mode_Palettes		=   4,
	Mode_Consultation	=	5,
	Mode_QuestInduites	=   6
};


class AFX_EXT_CLASS CDlgItems : public CDialog
{
	/// Construction
public:

	CDlgItems(CWnd* pParent = NULL);   /// standard constructor

#if defined(BUILD_FOR_ATELIER_INTERNET)
	CDlgItems(int lang, CWnd* pParent = NULL);
#endif

#if defined(BUILD_FOR_ATELIER_INTERNET)
	void Setup(int lang, CWnd* pParent);
#else
	void Setup(CWnd* pParent);
#endif

	virtual ~CDlgItems();

	/// Set the Show Mode parameters
	void SetShowMode(int ShowMode) { m_ShowMode = ShowMode; } ;

	/// Get Show Mode parameters
	int GetShowMode() { return m_ShowMode; } ;

	// Get Type de boite
	JBool GetTypeRepItem() {return m_TypeRepItem;} ;

	// Type affichage opérateurs
	JBool GetAffichageIcones() {return m_FIcones;} ;

	/// Sets the ShowResults parameters
	void SetShowResults(int ShowResults) { m_ShowResults = ShowResults; } ;

	/// Get Show Results parameters
	int GetShowResults() { return m_ShowResults; } ;

	/// Sets the ShowSite parameters
	void SetShowSite(int ShowSite) {m_ShowSite = ShowSite; } ;

	/// Get Show Site parameters
	int GetShowSite() { return m_ShowSite; } ;

	// returns true if visualisation mode is active
	bool IsVisualModeActive(void);

	// returns true if effective should be visible
	bool IsEffectiveVisible(void);

	// returns Value Cur Mode
	JInt32 ValCurMode(void);

	// return Titre Appli (Rep Items ou Pupitre)
	CString GetTitleAppli(void);

	/// Gets the Selected Contingent Mask
	JInt32	GetContingentMask() { return m_ContingentMask; } ;

	/// Gets the Selected Population Segment
	CBitArray & GetPopulationSegment() { return m_PopSegmentFilter; } ;

	/// Gets the selected Type Poids Id
	JInt32	GetTypePoids() { return m_Id_TypePoids; } ;

	/// Get the selected segment ids
	const JVector<JInt32> & GetSegments() { return m_Id_PopSegments; } ;

	/// Get the Terrain
	CTerrain * GetTerrain() { return m_pTerrain; } ;

	/// Get the Items construits
	// CItemsConstruits * GetItems() { return CItemsConstruits::GetInstance(); }
	CItemsConstruitsBase * GetItems() { return CItemsConstruitsManager::GetInstance()->GetItemsInstance(); }

	/// Get the Items construits automatisés 
	// CItemsConstruitsAuto * GetItemsAuto() { return CItemsConstruitsAuto::GetInstanceAuto(); }

	/// Get the item Label
	CString GetItemLabel();

	/// Get the title
	const CString & GetTitle() const { return m_Title; }

	// Get CurMode
	CAbstractMode	* GetCurMode();

	// Positionnement source/terrain/poids
	void SetPreloadTerrain(int idSource, int idTerrain, int idPoids);

	// Vérification item
	void VerifierItem();

	// Afficchage item
	void AfficherItem();

	// Remplacement chaines caractères sur élmt modalités
	void RemplacerStrItem(JList <JInt32> &ListIDs);

	// Efface le panneau visualisation
	void EffacePanelVisu();

	// Informe si fenêtre visualisation visible
	bool IsWindowVisuVisible();

	// Rend visible ou pas selon la question la boite dimension 2
	void SetDimension2Visible(bool Visible);

	// Initialise la liste des élmts boite questionnaire
	void InitListQuestion();

	virtual void GetSelectedItems(JList<CItemWrapper *> & ItemsPalette) {m_ItemsPalette.Swap(ItemsPalette); };

	// Mise à jour de la liste compo palette via les sélections mode palette
	virtual void UpdateListSelCompoPalette(bool Raz = true);

	// Mise à jour nb cellules tableau croisé
	void UpdateNbCells();

	// Activation opération SAS
	virtual void ActiveOpeSAS(IBloc::OPERATOR Operator);

	// Genese possible ou pas
	bool OnGenesePossible(); 

	// Test si item quantitatif
	JBool ItemQuantitatif(const CItemConstruit * pItem, JInt32 IdSource, JInt32 IdTerrain);

	// Map sélection en cours sur la dimension1
	JMap<JInt32,JBool>  m_MapSelModaDimension1;

	void Selchange_ListDimension_1();


private:
	void SetCheckItem(HTREEITEM hti);

public:
	// pour quitter l'application
	BOOL CanClose();
	void Close();

	// Reinit de l'appli
	void InitRepItem();

	// Pour visualiser structure item
	virtual void SetData (JList<CVisuItems> &Items);

	void SetCloseAfterOpen(bool setCloseAfterOpen);

private:
	
		bool m_SetCloseAfterOpen;

protected:
	virtual BOOL OnInitDialog();

	// virtual void GetFocus();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	/// Sets up control fonts etc...
	virtual void SetupControls();

	// Visibilité Boite Theme
	void SetVisibleBoiteTreeTheme(bool Visible);

	// Visibilité Boite Items Validation
	virtual void SetVisibleBoiteValidItem(bool Visible);

	// Visibilité Boite Résultat
	void SetVisibleBoiteResultat(bool Visible);

	// Visibilité Boite Visu Palettes Rep Items
	void SetVisibleResultPalette(bool Visible);

	// Visibilité Mode Pupitre
	void SetVisibleModePupitre(bool Visible);

	// Visibilité Boite Pupitre
	void SetVisibleBoitePupitre(bool Visible);

	// Visibilité de la fenêtre segment
	void SetVisibleFenSegment(bool Visible);

	// Rend visible pour la sélection des opérateurs
	void SetCtrlDimension2(bool Visible);

	// Rend visible ou non les opérateurs sur dimension 1
	void SetOpeDimension1(bool Visible, bool Actif = true);

	// Rend visible ou non les opérateurs sur dimension 2
	void SetOpeDimension2(bool Visible,bool Actif = true);

	// Impression possible ou pas des résultats
	bool ImpResultPossible();

	// Export Construction Item dans un fichier, impression ou pas
	void OnExportItem(JList<JInt32> &Values, bool Impression);

	// Export Item en cours de construction !!!!!
	void OnExportItemEnConstruction(JList<JInt32> &Values, bool Impression);

	// Export Construction Palettes
	void OnExportPalette();

	// Export de l'entete création d'un item
	void ExportEntete(JList<JInt32> &Values, CString &TxtMacro, bool ItemEnConstruction = false);

	// Export des composants de  l'item
	void ExportItem(CString &TxtMacro);

	// Export des composants de  la palette
	void ExportPalette(CString &TxtMacro);

	// Passe le descriptif item dans le clipboard
	bool Clipboard(CString DescriptifItem);

	// Mise à jour sélection des opérateurs sur dimension 1, 2, 3 etc...
	void UpdateSelectionOpe();


protected:
	/// Loads the given terrain
	bool LoadTerrain(JInt32 SourceId, JInt32 TerrainId);

	/// Sets the window size & positions for resizing
	void SetWindowRelations();

	/// Sets up the current mode
	virtual void UpdateMode(int ID);

	/// Displays the selected theme, support window
	virtual void UpdateTheme(int ID);

	// Dessine l'onglet actif
	void UpdateOngletConstruction(int ID);

	/// Sets up the theme data
	virtual void SetThemeData();

	// Sets up rubriques items
	virtual void SetRubriqueItems();

	// Sets up rubriques palettes
	virtual void SetRubriquePalettes();

	// Sets up colors for all controls
	void SetColors();

	// Sets up icones et apparence for all controls logiques
	void SetUpdateOpeLogiques();

	// Update Titre terrains segments
	void UpdateTitreTerrainSeg();

	// Test si au moins 1 menu déroulant encore actif
	bool OpeActif();

	// Rend visible pour la sélection des opérateurs sur les boites dimension 1 et 2
	void SetVisibleOpeDim(JUnt32 NoDim, bool Visible);

	// Rend visible pour la sélection des opérateurs du sas questions
	void SetVisibleOpeSelQu(bool Visible);

	// Rend visible pour la sélection des opérateurs segments
	void SetVisibleOpeSegment(bool Visible);

	// Interface graphique boite de dialogue Rep Items
	void ResizeInterfaceItem();

	// Interface graphique boite de dialogue Pupitre (avec positionnement des boutons sélections)
	void ResizeInterfacePupitre(JBool PosSelectionBas);

protected:
	// Titre de l'appli
	CString m_Title;

	// Type de boite dialogue (répertoire item ou pupitre)
	JBool m_TypeRepItem;

	// List visualisation items
	JList<CVisuItems>		m_Items;

	// Ident Item sélectioné en mode genèse
	JInt32					m_IdItemGenese;

	// List des items palette en cours
	JList<CItemWrapper *>	m_ItemsPalette;

	// Activation ou page des modes ajouts sélections (via dimension 1 et dimension 2)
	bool				m_AjoutSelCompoPalette_Dim1;
	bool				m_AjoutSelCompoPalette_Dim2;

	// Map de sauvegarde pour le mode sélection choisi pour les items sélectionnés, sur dimension1 et Dimension2
	JMap<JInt32,JBool>  m_MapModeSelQuestionDim1;
	JMap<JInt32,JBool>  m_MapModeSelQuestionDim2;

	// Liste sélection en cours sur tree theme (index thématique pour répertoire items)
	JMap<DWORD,JUnt32>	m_MapSelTreeTheme;

	// Liste sélection en cours sur rubrique items
	JMap<DWORD,JUnt32>	m_MapSelRubriqueItem;

	// Liste sélection en cours sur tree quanti des elmts à moyenner
	JMap<DWORD,JUnt32>	m_MapSelTreeQuanti;

public:
	// Renvoi si la question est en mode activation (pour palette essentiellement)
	bool				QuestionSelActive(JInt32 IdItem);

	// Positionnement de tous les opérateurs par défaut
	void				AllOperateurParDefaut();

	// Visualisation ou pas d'un item
	bool				ShowItem(JInt32 IdItem);

	// Visualisation ou pas d'une palette
	bool				ShowPalette(JInt32 IdItem);

protected:
	// Sauvegarde des modes des sélections pour dimension 1 et dimension 2
	virtual void SaveModeSelectionDim1Dim2(JInt32 IdItem);

	// Récupére modes des sélections pour dimension 1 et dimension 2
	virtual void LoadModeSelectionDim1Dim2(JInt32 IdItem, JUnt32 NbDimension);

	// Selon les modes on positionne ou pas les boutons sélections
	virtual void SetBoutonSelection();

	// Active ou Désacte du mode sélection courante (lignes ou colonnes ou pages)
	virtual void ActiveModeSel(JUnt32 CurModeSel, JUnt32 Dimension, bool Actif);

	// Change mode activation de la dimension 1 pour constrcution palette
	void ActiveModeAjoutDim1(bool RazCompoPalette);

public:
	// Renvoi état sélection dimension 1 ou 2
	virtual bool GetSelDimActive(JUnt32 Dim);

	// Renvoi état sélection dimension 1 ou 2 dans pupitre
	virtual bool GetSelDimPupitreActive(JUnt32 Dim);

	// Validation du mode (surtout pour mode classe)
	virtual void ValideMode();

	// Positionne la fenêtre des valeurs
	virtual void PosFenValeursQuantis();

public:
	// Recup Id Terrain
	JUnt32 GetIdTerrain(){return m_Id_Terrain;};

	// Recup Id Source
	JUnt32 GetIdSource(){return m_Id_Source;};

protected:
	/// Resize class
	CResizer						m_Resizer;
	/// Mode Classes
	JMap< JUnt32, PTR_ABSTRACTMODE>	m_Modes;
	JInt32							m_Id_CurMode;

	/// Current Mode Pointer
	CAbstractMode	*				m_pCurMode;

	/// Current Mode Pointer mode ligne, et mode colonne, et mode page
	CAbstractMode	*				m_pCurModeLigne;
	CAbstractMode	*				m_pCurModeColonne;
	CAbstractMode	*				m_pCurModePage;

	/// Pointer to the terrain
	CTerrain *						m_pTerrain;
	/// Source ID
	int								m_Id_Source;
	/// Terrain ID
	int								m_Id_Terrain;
	/// Population Segment ID vector
	JVector<JInt32>					m_Id_PopSegments;
	/// ID Type Poids
	JInt32							m_Id_TypePoids;
	/// Contingent bit mask for filtering questions
	JUnt32							m_ContingentMask;
	/// Population Segment Filter
	CBitArray						m_PopSegmentFilter;

	BOOL							m_bFirstTime;
	static const CRect				s_rectDefault;
	/// The Dialogs Icon handle
	HICON							m_hIcon;

	// Mode résultats dans répertoire des items
	int								m_ShowResults;

	// Mode visualisation (standard ou simplifié)
	int								m_ShowMode;

	// Mode sélection ou pas des sites
	int								m_ShowSite;

	// Mode visualisation bouton transfert lignes, colonnes, pages
	JBool							m_SelectionEnBas;

	// Membre split vertical
	// @@@ REPITEMS : ATELIER INTERNET
	#if defined(BUILD_FOR_ATELIER_INTERNET)
		JFCListBox					m_InutileCtrl;
	#else
		JFCVertSplit				m_SplitVert;
	#endif

protected :
	// Configuration du split vertical
	void ConfigSplit();

	// Sauve dernière position fenêtre
	void SavePosFenDlg();

	// Déplacement du split vertical
	virtual void DeplaceSplitVert(LONG Disp);

	// Repositionne le split via dernière sauvegarde
	virtual void PosLastSplit();

	// Voir si sauvegarde d'un élmt item ou palette ou autre en construction
	JBool Sauvegarde_ElmtEnConstruction();

	// Change Couleur
	void ChangeColor(int NoColor);

public:
	//@{
	/// Subclasses for certain dialog controls

	/// Color Scheme
	CATPColors::APPCOLOR						m_CATPColorsApp;

	/// Icones displayed flag
	bool										m_FIcones;

	/// Constructed Target visualisation window
	CThemeTree									m_Tree_Theme;

	// Nomenclature classement des items
	CRubriqueItemTree							m_Tree_Rubrique_Items;

	// Nomenclature classement des palettes
	CRubriqueItemTree							m_Tree_Rubrique_Palettes;

	CListBoxItems								m_List_Items;
	CListBoxQuestions							m_List_Questions;
	CListBoxDimensions							m_List_Dimension1;

	// Liste des elmts en cours de filtrage
	JMap<JUnt32, JBool>							m_MapFiltrage;

	CListBoxDimensions							m_List_Dimension2;
	CListBoxSas									m_List_SAS;
	CEdit										m_Edit_Search;
	CEdit										m_Edit_QFilter;
	CEdit										m_Edit_Name;
	CLabel										m_Mode_Title;
	CLabel										m_Box_Title;
	CLabel										m_Label_Source;
	CLabel										m_Box_Source;
	CLabel										m_Label_Item;
	CLabel										m_Label_NbCells;

	CLabel										m_Label_Theme;
	CLabel										m_Label_DefItem;

	// Boite tree items
	CLabel										m_Box_Theme;

	// Boite résultat
	CLabel										m_Box_Result;

	// Boite pupitre pour sélection des lignes, colonnes, pages
	CLabel										m_Box_Pupitre;
	CLabel										m_Label_Questions;
	CLabel										m_Box_Dimension1;
	CLabel										m_Header_Dimension2;
	CLabel										m_Box_Dimension2;
	CLabel										m_Header_ItemLibelle;
	CLabel										m_Label_TypePoids;
	CLabel										m_Label_Effectif;
	CLabel										m_Static_Effectif;
	CLabel										m_Label_Percent;
	CLabel										m_Static_Percent;
	CLabel										m_Label_NbCas;
	CLabel										m_Static_NbCas;

	// Case à cocher pour indiquer si on met les nouveaux items dans rubrique par défaut
	// CButtonST								m_Chk_Rubrique_Defaut;
	CButton										m_Chk_Rubrique_Defaut;

	// Cadre englobant les résultats (effectif, nb cas, % couv) item en cours
	CStatic										m_Cadre_Result;

	CLabel										m_Box_Question;
	CLabel										m_Header_Dimension1;
	CLabel										m_Box_Items;
	CLabel										m_Label_SAS;
	CLabel										m_Box_SAS;
	CLabel										m_Box_Segment;
	CButtonST									m_Button_Genese;
	CButtonST									m_Button_Questions_Tous;
	CButtonST									m_Button_RAZ_search;
	CButtonST									m_Button_QFilter;
	CButtonST									m_Button_QFilterRaz;
	CButtonST									m_Button_TreeQuestions;
	CButtonST									m_Button_List1;
	CButtonST									m_Button_List2;
	CButtonST									m_Button_Title;			// IDC_RI_RAD_TITLE

	// Opérateurs sur panel dimension 1
	CButtonST									m_Button_Dimension1;
	CButtonST									m_Button_Dim1_OK;
	CButtonST									m_Button_Dim1_ET;
	CButtonST									m_Button_Dim1_OU;
	CButtonST									m_Button_Dim1_NI;
	CButtonST									m_Button_Dim1_XOR;

	// Menu pour les opérateurs sur panel dimension 1
	CButtonST									m_Button_Menu_Ope_Dim1;	
	CButtonST									m_Button_Dim1_RAZ;
	CButtonST									m_Button_Filtre;
	CButtonST									m_Button_Dim1_UP;
	CButtonST									m_Button_Dim1_DOWN;

	// Opérateurs sur panel dimension 2
	CButtonST									m_Button_Dimension2;
	CButtonST									m_Button_Dim2_ET;
	CButtonST									m_Button_Dim2_OU;
	CButtonST									m_Button_Dim2_NI;
	CButtonST									m_Button_Dim2_XOR;
	CButtonST									m_Button_Menu_Ope_Dim2;
	CButtonST									m_Button_Dim2_RAZ;

	// Validation et mode de calcul des effectifs 
	CButtonST									m_Button_CalcSegment;
	CButtonST									m_Button_Accept;

	// Les boutons SAS
	CButtonST									m_Button_SAS_RAZ;

	// Menu pour les opérateurs sur panel SAS Sélection Questions
	CButtonST									m_Button_Menu_Ope_QuSel;
	CButtonST									m_Button_SAS_ET;
	CButtonST									m_Button_SAS_OU;

	// Menu pour les opérateurs sur panel Segments
	CButtonST									m_Button_Menu_Ope_Segment;
	CButtonST									m_Button_Segment_ET;
	CButtonST									m_Button_Segment_OU;
	CButtonST									m_Button_Segment_RAZ;
	CButtonST									m_Button_Suppression;

	// Boutons Validation / Annulation
	CButtonST									m_Button_OK;
	CButtonST									m_Button_Cancel;

	// Bouton sélection des modes pupitre
	CButtonST									m_Button_Mode_Ligne;
	CButtonST									m_Button_Mode_Colonne;
	CButtonST									m_Button_Mode_Page;

	JFCTabCtrl									m_Tab_Segment;
	CFont										m_Font_Title;
	CFont										m_Font_Labels;
	CBlocQuestions								m_BlocQuestion;

	CQuickList									m_List_Result;

	// Bouton commande down classement item
	CButtonST									m_But_Down_Item;

	// Bouton commande up classement item
	CButtonST									m_But_Up_Item;

	// Bouton commande impression composition items
	CButtonST									m_But_Imprimer_Result;

	// Bouton commande pour export composition item
	CButtonST									m_But_Export_Item;

	// Bouton commande pour supprimer composition item
	CButtonST									m_But_Supp_Item;

	// Boite contenant les commandes du pupitre
	CLabel										m_Box_Onglet_Pupitre;

	int											m_iModality;
	CString										m_QFilter;

	// Liste des composants palette (ssi en mode palette)
	JFCListCtrl									m_List_Compo_Palette;
	// CQuickList								m_List_Compo_Palette;

	// Les élmts pour la sélection ligne, colonne, pages du tableau tri croisé
	//
	// Liste des lignes sélectionnées pour le tableau tri croisé
	JFCSelListCtrl								m_List_Row;

	// Liste sélection des élmts en colonne pour la tableau tri croisé
	JFCSelListCtrl								m_List_Col;

	// Liste sélection des élmts en page pour la tableau tri croisé
	JFCSelListCtrl								m_List_Page;

	// Liste temporaire pour tri
	JFCSelListCtrl								m_List_Tri;

	// Informations lignes, colonnes, pages
	CLabel										m_Label_Lignes;
	CLabel										m_Label_Colonnes;
	CLabel										m_Label_Pages;

	// Sélecteur des items à moyenner en ligne
	CButtonST									m_Button_Item_Moy_Lig;

	// Sélecteur des items à moyenner en colonne
	CButtonST									m_Button_Item_Moy_Col;

	// Tri ascendant pour sélection lignes ou colonnes ou pages
	CButtonST									m_Button_Ligne_UP;
	CButtonST									m_Button_Colonne_UP;
	CButtonST									m_Button_Page_UP;

	// Tri desccendant pour sélection lignes ou colonnes ou pages
	CButtonST									m_Button_Ligne_DOWN;
	CButtonST									m_Button_Colonne_DOWN;
	CButtonST									m_Button_Page_DOWN;

	// Bouton pour trier les lignes composants sélectionnées
	CButtonST									m_Btn_All_Lignes;
	// Bouton pour trier les colonnes composants sélectionnées
	CButtonST									m_Btn_All_Colonnes;
	// Bouton pour trier les pages composants sélectionnées
	CButtonST									m_Btn_All_Pages;

	// Bouton pour ajouter des sites en ligne
	CButtonST									m_Btn_SitesEnLigne;
	CButtonST									m_Btn_SitesEnColonne;
	CButtonST									m_Btn_SitesEnPage;

	// Combo box avec toutes les rubriques dispos
	CComboBox m_ComboRubriques;

	// Selon mode on viualise en entier ou pas la liste mode items (sinon tree rubrique apparent)
	JVoid ResizeListItems(bool RubriqueVisible);

	// Renvoi la rubrique item par défaut, sinon rien
	HTREEITEM GetRubriqueItemDefaut();

	// Renvoi la rubrique palette par défaut, sinon rien
	HTREEITEM GetRubriquePaletteDefaut();

	// Ajout d'un item dans une rubrique via sélection dans boite de dialogue rubriques en cours
	void	AddItemRubrique(JInt32 IdItem, JLabel LabelItem);

private:
	// Les brushs utiles
	CBrush m_BrushColorLight;
	CBrush m_BrushColorMedium;
	CBrush m_BrushColorDark;

	//@}
public:

	/// Dialog Data
	//{{AFX_DATA(CDlgItems)
	enum { IDD = IDD_RI_DLG_ITEMS };
	BOOL		m_bSegment;
	//}}AFX_DATA


	/// Overrides
	/// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgItems)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    /// DDX/DDV support
	//}}AFX_VIRTUAL

	/// Implementation
protected:
#define IdTimer_Search WM_USER+101
	/// The message sent by the support tree
#define WM_TREENEEEDUPD WM_USER + 200

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnAccept();
	afx_msg void OnDim_2_Et();
	afx_msg void OnDim_2_Xor();
	afx_msg void OnDim_2_Ni();
	afx_msg void OnDim_2_Ou();
	afx_msg virtual void OnDim_2_Raz();
	afx_msg void OnGenese();
	afx_msg void OnDim_1_Ok();
	afx_msg void OnDim_1_Down();
	afx_msg void OnDim_1_Et();
	afx_msg void OnDim_1_Xor();
	afx_msg void OnDim_1_Ni();
	afx_msg void OnDim_1_Ou();
	afx_msg virtual void OnDim_1_Raz();
	afx_msg void OnDim_Filtre();
	afx_msg void OnDim_1_Up();
	afx_msg void OnSAS_Et();
	afx_msg void OnSAS_Ou();
	afx_msg void OnSAS_Raz();
	afx_msg void OnSegment_Raz();
	afx_msg void OnSegment_Et();
	afx_msg void OnSegment_Ou();
	afx_msg void OnTout_Raz();
	afx_msg void OnItem_Raz();
	afx_msg void OnSelchange_TabSegment(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg virtual void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchange_ListQuestions();
	afx_msg void OnSelchange_ListDimension_1();
	afx_msg void OnSelchange_ListItems();
	afx_msg void OnSelchange_ListDimension_2();
	afx_msg void OnSelchange_ListSas();
	afx_msg virtual void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnChange_PopSegment();
	afx_msg void OnChange_TypePoids();
	afx_msg void OnChange_Source();
	afx_msg void OnChange_SegmentCalc();
	afx_msg void OnUpdate_ModePalette(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_ModeQuantile(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_ModeClasse(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_ModeItem(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_ModeConsulter(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_ModeInduite(CCmdUI *pCmdUI);
	afx_msg void OnChange_ModePalette();
	afx_msg void OnChange_ModeQuantile();
	afx_msg void OnChange_ModeClasse();
	afx_msg void OnChange_ModeItem();
	afx_msg void OnChange_ModeConsulter();
	afx_msg void OnChange_ModeInduite();
	afx_msg void OnTree_Questions();
	afx_msg void OnList_1();
	afx_msg void OnList_2();
	afx_msg virtual void OnChange_Dim_2_PaletteSel();
	afx_msg virtual void OnChange_Dim_1_PaletteSel();
	/// Message sent from modeless Modality and Element dialogs
	afx_msg LRESULT OnChange_Dim_x_PaletteSel(WPARAM wParam, LPARAM lParam);//WM_USER_DIMENSION_X_DLG_DIMSEL
	/// Message sent from modeless Modality and Element dialogs
	afx_msg virtual LRESULT OnSelChange_ListDimension_x(WPARAM wParam, LPARAM lParam);
	/// Message sent from theme tree control
	afx_msg LRESULT OnUpdated_QuestionList(WPARAM wParam, LPARAM lParam);
	/// Message sent from tree rubrique control
	afx_msg LRESULT OnUpdated_ItemList(WPARAM wParam, LPARAM lParam);
	/// Message sent from values dialog
	afx_msg virtual LRESULT OnChange_Dim_ValueSel(WPARAM wParam, LPARAM lParam);
	/// Message sent from values dialog
	afx_msg virtual LRESULT OnSelChange_ListValue(WPARAM wParam, LPARAM lParam);
	/// Message sent from values dialog
	afx_msg LRESULT OnValuesQuantile(WPARAM wParam, LPARAM lParam);
	/// Message sent from visu dialog
	afx_msg LRESULT OnVisuClose(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRazSearch();
	afx_msg void OnChanged_EditSearch();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdate_Palette1(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette2(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette3(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette4(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette5(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette6(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette7(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette8(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette9(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette10(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette11(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette12(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette13(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette14(CCmdUI *pCmdUI);
	afx_msg void OnUpdate_Palette15(CCmdUI *pCmdUI);
	afx_msg void OnChange_Palette1();
	afx_msg void OnChange_Palette2();
	afx_msg void OnChange_Palette3();
	afx_msg void OnChange_Palette4();
	afx_msg void OnChange_Palette5();
	afx_msg void OnChange_Palette6();
	afx_msg void OnChange_Palette7();
	afx_msg void OnChange_Palette8();
	afx_msg void OnChange_Palette9();
	afx_msg void OnChange_Palette10();
	afx_msg void OnChange_Palette11();
	afx_msg void OnChange_Palette12();
	afx_msg void OnChange_Palette13();
	afx_msg void OnChange_Palette14();
	afx_msg void OnChange_Palette15();
	afx_msg void OnAboutbox();
	afx_msg void OnSysColorChange();
	afx_msg void OnIcones();
	afx_msg void OnUpdateIcones(CCmdUI *pCmdUI);
	afx_msg void OnPatrimoine_Consult();
	afx_msg void OnPatrimoine_Import();
	afx_msg void OnPatrimoine_Export();
	afx_msg virtual void OnQuestions_Tous();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMove(int, int);
	afx_msg void OnKeyUp(int,int,int);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnQFilter_Tous();
	afx_msg void OnQFilter_Questions();
	afx_msg void OnQFilter_Modalites();
	afx_msg void OnQFilter_RAZ();
	afx_msg void OnQFilter_Palette();

	// Gestion des modes présentoir tris croisés
	afx_msg virtual void OnBnClickedRiRadModeLignes();
	afx_msg virtual void OnBnClickedRiRadModeColonnes();
	afx_msg virtual void OnBnClickedRiRadModePages();

	// Message split vertical
	afx_msg virtual LRESULT OnSplitVert(WPARAM WParam, LPARAM LPARAM);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnQuitter();
	afx_msg void OnTvnSelchangedRiTreeTheme(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRiBtnDownItem();
	afx_msg void OnBnClickedRiBtnUpItem();
	afx_msg void OnBnClickedRiBtnImpitem();
	afx_msg void OnBnClickedRiBtnSuppItem();
	afx_msg void OnBnClickedRiBtnExportItem();
	afx_msg LRESULT OnGetListItem(WPARAM wParam, LPARAM lParam);

	afx_msg void OnRiEffacerSegment();
	// afx_msg void OnRiEffacerSas();
	afx_msg void OnRiEffacerTout();
	afx_msg void OnRiEffacerItem();
	afx_msg void OnRiOpeDim1Ou();
	afx_msg void OnRiOpeDim1Et();
	afx_msg void OnRiOpeDim1Ex();
	afx_msg void OnRiOpeDim1Ni();
	
	afx_msg void OnRiOpeDim2Ou();
	afx_msg void OnRiOpeDim2Et();
	afx_msg void OnRiOpeDim2Xor();
	afx_msg void OnRiOpeDim2Ni();
	afx_msg void OnBnClickedCancel();
	
	// afx_msg void OnBnClickedRiButMenuOpeDim1();
	afx_msg void OnBnClickedRiButMenuOpeDim1();
	afx_msg void OnBnClickedRiButMenuOpeDim2();
	afx_msg virtual void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnBnClickedRiButMenuOpeQusel();
	afx_msg void OnBnClickedRiButMenuOpeSegment();
	
	afx_msg void OnRiMenuSuppLignes();
	afx_msg void OnRiMenuSuppColonnes();
	afx_msg void OnRiMenuSuppPages();
	afx_msg void OnRiMenuSuppTout();
	afx_msg void OnBnClickedOk();

	// Les messages déplacement pour réordonnacement
	afx_msg virtual void OnBnClickedRiItemMoyenLig();
	afx_msg virtual void OnBnClickedRiButLignesDown();
	afx_msg virtual void OnBnClickedRiButLignesUp();
	afx_msg virtual void OnBnClickedRiItemMoyenCol();
	afx_msg virtual void OnBnClickedRiButColonnesUp();
	afx_msg virtual void OnBnClickedRiButColonnesDown();
	afx_msg virtual void OnBnClickedRiButPagesUp();
	afx_msg virtual void OnBnClickedRiButPagesDown();

	afx_msg void OnStnClickedRiLabelDimension2();

	afx_msg virtual void OnLvnKeydownRiListLignes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnKeydownRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnKeydownRiListPages(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg virtual void OnLvnBegindragRiListLignes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnBegindragRiListPages(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnBegindragRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void		 OnBnClickedRiButSasRaz();
	afx_msg virtual void OnNMClickRiListLignes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnNMClickRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnNMClickRiListPages(NMHDR *pNMHDR, LRESULT *pResult);
	
	// Suppression globale en lignes, colonnes, pages
	afx_msg virtual void OnSupprimertoutesleslignes();
	afx_msg virtual void OnSupprimertouteslescolonnes();
	afx_msg virtual void OnSupprimertouteslespages();

	// Changement des items lignes, colonnes, pages
	afx_msg virtual void OnLvnItemchangedRiListLignes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnItemchangedRiListPages(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnItemchangedRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult);

	// Gestion ajout des sites en lignes, colonnes ou pages
	afx_msg virtual void OnBnClickedRiBtnSitesLig();
	afx_msg virtual void OnBnClickedRiBtnSitesCol();
	afx_msg virtual void OnBnClickedRiBtnSitesPage();
	afx_msg virtual void OnBnClickedRiButAllLignes();
	afx_msg virtual void OnBnClickedRiButAllColonnes();
	afx_msg virtual void OnBnClickedRiButAllPages();
	afx_msg void OnRiEffacerToutPalette();
	afx_msg void OnRiEffacerPalette();
	afx_msg virtual void OnRiMenuSuppAllLcp();
	
	// Gestion couleur spécifique contrôles
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

//{{AFX_INSERT_LOCATION}}
/// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
