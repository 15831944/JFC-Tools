#pragma once
#include "DlgItems.h"
#include "ItemWrapper.h"
#include "PaletteMode.h"

class CAnalyseurDoc;

// CDlgTableau dialog


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classe clef élmt item + terrain (servira aux états sélécteur dimension 1,2,3 par terrains)
class CKeyItemTerrain
{
public:

	// test d'infériorité clef item + terrain 
	bool operator<(const CKeyItemTerrain & Rhs) const
	{
		if(m_IdItem < Rhs.m_IdItem)
			return true;
			
		if(m_IdItem == Rhs.m_IdItem && m_IdTerrain < Rhs.m_IdTerrain)
			return true;
			
		return false;
	};

	// test de supériorité clef item + terrain 
	bool operator>(const CKeyItemTerrain & Rhs) const
	{
		if(m_IdItem > Rhs.m_IdItem)
			return true;
			
		if(m_IdItem == Rhs.m_IdItem && m_IdTerrain > Rhs.m_IdTerrain)
			return true;
			
		return false;
	};

	// test d'égalité clef item + terrain 
	bool operator==(const CKeyItemTerrain & Rhs) const
	{
		return (m_IdItem == Rhs.m_IdItem && m_IdTerrain == Rhs.m_IdTerrain);
	};

	// affectation via clef item + terrain 
	CKeyItemTerrain & operator=(const CKeyItemTerrain & Rhs)
	{
		m_IdItem		= Rhs.m_IdItem;
		m_IdTerrain		= Rhs.m_IdTerrain;
		return *this;
	};

	// sérialisation clef item + terrain 
	void Send(JStream & Stream) const
	{
		// on sérialise l'élément
		Stream.Send(m_IdItem);
		Stream.Send(m_IdTerrain);
	};

	// sérialisation clef item + terrain 
	void Recv(JStream & Stream)
	{
		// Récup élmt id item et id terrain
		Stream.Recv(m_IdItem);
		Stream.Recv(m_IdTerrain);
	};

	// test d'infériorité clef item + terrain 
	int CKeySelItem() 
	{ 
		m_IdItem	= 0; 
		m_IdTerrain = 0; 
	};
	
	// attributs clef item + terrain
	JInt32	m_IdItem;
	JInt32	m_IdTerrain;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CDlgTableau : public CDlgItems
{
public:

	// CDlgTableau(CTerrain * pTerrain, CWnd* pParent = NULL);   // standard constructor
	CDlgTableau(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgTableau();

	// Init source et terrain utilisés
	void SetTerrainTemoin(CTerrain * pTerrain);

	// Mode expoité
	void SetMode(CPaletteMode::MODE_TABLEAU CurMode) { m_Id_CurMode = CurMode; } ;

    void SetParams(UINT TypePoidsId, const JList<UINT> & SegmentsId);
	void GetSelectedItems(JList<CItemWrapper *> & Items) { m_Items.Swap(Items); };

	// Récupére les choix déjà effectués
	bool GetData(CAnalyseurDoc *pDoc);

	// Positionne les choix déjà effectués
	bool SetData(CAnalyseurDoc *pDoc);

	// Récupération des modes avant appel à pupitre
	void RecupExModePalette();

	// Réinitialisation des élmts sélection
	void InitSelGeneral();

	// Sauvegarde des palettes ligne, colonne, page
	// bool SavePalettes(CString PathName);
	bool SavePalettes(CStdioFile & file);

	// Récupération des palettes ligne, colonne, page
	// bool LoadPalettes(CString PathName);
	bool LoadPalettes(CStdioFile & file);

	// Transposition des palettes
	void Transpose(int Mode1, int Mode2);

	// Récupère document associé
	CAnalyseurDoc * GetDoc(){return m_pDoc;};

	void Cancel();
	void Hide();

protected:
	virtual BOOL OnInitDialog();
	virtual void SetupControls();

	/// Displays the selected theme, support window
	virtual void UpdateTheme(int ID);

	/// Sets up the theme data
	virtual void SetThemeData();

	// Récupération des items sélectionnés
	void RecupItemSelect();

	// Validation des sélections tris croisés (en lignes/ colonnes/ pages)
	bool ValideSelectionTriCroise();

	// Mise à jour du nombre de sélection (en ligne, en colonne, en page)
	void UpdateCount();

	/// Displays the selected theme, support window
	void UpdatePupitre(int ID);

	/// Sets up the current mode
	void UpdateMode(int ID, bool MajSAS = true);

	// Suppression de toute la sélection (lignes + colonnes + pages)
	void EffacerAllSel();

	// Mise à jour de la liste sélection en mode ajout présentoir ligne, colonne ou page 
	void UpdateListSelect(bool ModeSelItemQuanti = false);

	// Mise à jour nb cellules correspondant à la sélection
	void UpdateNbCells();

	// Mise à jour de la liste sélection courante via les sélections ligne, colonne ou page (avec ou pas raz)
	void UpdateListSelect_Synchro(bool Raz);

	// Update de toutes les listes sélections
	void UpdateAllListSelect(bool ModeAjout = true);

	// Mise à jour validation des boutons tri sélection lignes/colonnes/pages
	void UpdateBtnTriSel();

	// Active ou Désacte du mode sélection courante (lignes ou colonnes ou pages)
	void ActiveModeSel(JUnt32 CurModeSel, JUnt32 Dimension, bool Actif);

	// Affichage de la palette du mode en cours
	void AffichePaletteModeCur();

	// Valide le 1er elmt du Sas - Attention si quelque chose dans le sas et non sélectionné
	void ValideElmtSas(bool ChangeSas = true);

	// Affichage choix menu opérateur SAS pour ET
	void AfficheChoixMenuSasEt();

	// Affichage choix menu opérateur SAS pour OU
	void AfficheChoixMenuSasOu();

	// Change mode activation de la dimension 1 pour construction palette
	void ActiveModeAjoutDim1(bool RazSelection);

	// Visibilité Boite Items Validation
	void SetVisibleBoiteValidItem(bool Visible);

	// Déplacement item de liste en liste
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList);

	// Questions en cours de sélection non disposé en ligne ou colonne ou page
	JBool QuestionsSelEnCours();

protected:
	JList<CItemWrapper *>	m_Items;

	JBool MajNbCells;

protected:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnGenese();

	// Gestion des modes présentoir tris croisés
	afx_msg void OnBnClickedRiRadModeLignes();
	afx_msg void OnBnClickedRiRadModeColonnes();
	afx_msg void OnBnClickedRiRadModePages();

	// Gestion des items à moyenner en ligne et colonne
	afx_msg virtual void OnBnClickedRiItemMoyenLig();
	afx_msg virtual void OnBnClickedRiItemMoyenCol();

	// Gestion suppression lignes ou colonnes ou pages sélectionnées
	afx_msg void OnRiMenuSuppLignes();
	afx_msg void OnRiMenuSuppColonnes();
	afx_msg void OnRiMenuSuppPages();
	afx_msg void OnRiMenuSuppTout();

	afx_msg void OnSelchange_ListItems();
	afx_msg void OnChange_Dim_1_PaletteSel();
	afx_msg void OnChange_Dim_2_PaletteSel();
	afx_msg void OnSelchange_ListDimension_1();
	afx_msg void OnSelchange_ListDimension_2();
	afx_msg void OnSelchange_ListQuestions();
	afx_msg void OnSelchange_ListSas();
	afx_msg void OnDim_1_Raz();
	afx_msg void OnDim_2_Raz();

	/// Message sent from theme tree control
	afx_msg LRESULT OnUpdated_QuestionList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdated_ItemQuantiList(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSAS_Et();
	afx_msg void OnSAS_Ou();
	afx_msg void OnAccept();

	afx_msg virtual void OnQuestions_Tous();

	// Les messages déplacement pour réordonnacement
	afx_msg virtual void OnBnClickedRiButLignesDown();
	afx_msg virtual void OnBnClickedRiButLignesUp();
	afx_msg virtual void OnBnClickedRiButColonnesUp();
	afx_msg virtual void OnBnClickedRiButColonnesDown();
	afx_msg virtual void OnBnClickedRiButPagesUp();
	afx_msg virtual void OnBnClickedRiButPagesDown();

	// Sélection ou déselection de tous les élmts
	afx_msg virtual void OnBnClickedRiButAllLignes();
	afx_msg virtual void OnBnClickedRiButAllColonnes();
	afx_msg virtual void OnBnClickedRiButAllPages();

	/// Message sent from modeless Modality and Element dialogs
	afx_msg virtual LRESULT OnSelChange_ListDimension_x(WPARAM wParam, LPARAM lParam);
	/// Message sent from values dialog
	afx_msg virtual LRESULT OnChange_Dim_ValueSel(WPARAM wParam, LPARAM lParam);
	/// Message sent from values dialog
	afx_msg virtual LRESULT OnSelChange_ListValue(WPARAM wParam, LPARAM lParam);

	// Gestion Keydown sur liste sélections lignes
	afx_msg virtual void OnLvnKeydownRiListLignes(NMHDR *pNMHDR, LRESULT *pResult);

	// Gestion Keydown sur liste sélections colonnes
	afx_msg virtual void OnLvnKeydownRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult);

	// Gestion Keydown sur liste sélections pages
	afx_msg virtual void OnLvnKeydownRiListPages(NMHDR *pNMHDR, LRESULT *pResult);

	// Gestion Drag Drop
	afx_msg virtual void OnLvnBegindragRiListLignes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnBegindragRiListPages(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnBegindragRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg virtual void OnMouseMove(UINT nFlags, CPoint point);

	// Resize fenetre dialog
	afx_msg virtual void OnSize(UINT nType, int cx, int cy);

	// Traitement menu contextuel
	afx_msg virtual void OnContextMenu(CWnd* pWnd, CPoint point);

	// Message split vertical
	afx_msg virtual LRESULT OnSplitVert(WPARAM WParam, LPARAM LPARAM);

	// Message sélection ou déselection elmt listes
	afx_msg virtual void OnNMClickRiListLignes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnNMClickRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnNMClickRiListPages(NMHDR *pNMHDR, LRESULT *pResult);

	// Message suppression de toutes les sélections par mode (ligne, colonne,page)
	afx_msg virtual void OnSupprimertoutesleslignes();
	afx_msg virtual void OnSupprimertouteslescolonnes();
	afx_msg virtual void OnSupprimertouteslespages();

	// état liste items row change
	afx_msg virtual void OnLvnItemchangedRiListLignes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnItemchangedRiListPages(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnItemchangedRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult);

	// Gestion ajout des sites en lignes, colonnes ou pages
	afx_msg virtual void OnBnClickedRiBtnSitesLig();
	afx_msg virtual void OnBnClickedRiBtnSitesCol();
	afx_msg virtual void OnBnClickedRiBtnSitesPage();

	// Effaecement de toute la sélection (lignes, colonnes, pages)
	afx_msg virtual void OnRiMenuSuppAllLcp();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	DECLARE_MESSAGE_MAP()

protected:

	/// Drag and drop variables
	int							m_nDragIndex;
	int							m_nDropIndex;
	CWnd *						m_pDragWnd;
	CWnd *						m_pDropWnd;
	bool						m_bDragging;
	CImageList *				m_pDragImage;

	// Flags changement sélection
	bool						m_bItemsChanged;
	bool						m_bRowsChanged;
	bool						m_bColsChanged;
	bool						m_bPagesChanged;

	// Flags positions tris
	bool						m_bRowsSort;
	bool						m_bColsSort;
	bool						m_bPagesSort;

	// Les sélections items
	int							m_DType_Row;
	CONST_LIST_ITEM_PUPITRE		m_Items_Row;
	int							m_DType_Col;
	CONST_LIST_ITEM_PUPITRE		m_Items_Col;
	int							m_DType_Page;
	CONST_LIST_ITEM_PUPITRE		m_Items_Page;
	LIST_ITEM_PUPITRE			m_Items_Original;
	LIST_ITEM_PUPITRE			m_Items_New;
	LIST_ITEM_PUPITRE			m_Items_Del;

	// Les items à moyenner
	LIST_ITEM_PUPITRE			m_AllItemsAutos;
	LIST_ITEM_PUPITRE			m_AllItemsUtils;

	// Les mementos temporaires des modes ligne, colonne, page
	JMemento	*pMementoLigne;
	JMemento	*pMementoColonne;
	JMemento	*pMementoPage;

	// Activation ou pas des modes ajouts sélections (via dimension 1 et dimension 2)
	bool				m_AjoutSelLigActif_Dim1;
	bool				m_AjoutSelLigActif_Dim2;
	bool				m_AjoutSelLigActif_Dim3;
	bool				m_AjoutSelColActif_Dim1;
	bool				m_AjoutSelColActif_Dim2;
	bool				m_AjoutSelColActif_Dim3;
	bool				m_AjoutSelPageActif_Dim1;
	bool				m_AjoutSelPageActif_Dim2;
	bool				m_AjoutSelPageActif_Dim3;
	
	// Mise à jour des sélecteurs dimesnion 1, dimension 2, éventuellement dimension 3
	void UpdateSelecteurDimension(bool Dim1Active, bool Dim2Active, bool Dim3Active);

	// Le mode onglet en cours, ligne ou colonne ou page
	JUnt32				m_Id_CurModePrec;

	// Map de sauvegarde pour le mode sélection choisi pour les items sélectionnés, sur dimension1 et Dimension2
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionLigDim1;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionLigDim2;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionLigDim3;

	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionColDim1;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionColDim2;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionColDim3;

	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionPageDim1;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionPageDim2;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionPageDim3;

	// Sauvegarde des modes des sélections pour dimension 1 et dimension 2
	void SaveModeSelectionDim1Dim2(JInt32 IdItem);

	// Récupére modes des sélections pour dimension 1 et dimension 2
	void LoadModeSelectionDim1Dim2(JInt32 IdItem, JUnt32 NbDimension);

	// Selon les modes on positionne ou pas les boutons sélections
	void SetBoutonSelection();

	// Disposition listes résultas (lignes, colonnes, pages)
	void SetListeResultat();

	// Mise en place des fenêtres résultats
	void SetFenResultat(JInt32 NbFenVisible);

	// Mise à jour des listes sélections
	void MiseAJourListeSelection(JList<CItemWrapper *> &List);

	void MajViaNewElementAvecTri(JInt32 Mode, JFCSelListCtrl &ListCtrlCourante, JList<CItemWrapper *> &List, int IndiceDepIdemHeader);
	void MajViaNewElementSansTri(JInt32 Mode, JFCSelListCtrl &ListCtrlCourante, JList<CItemWrapper *> &List);
	
	// Retaille largeur colonnes des liste de  sélection
	void ResizeColListSelect(JUnt32 IdMode);

	// Renvoi état sélection dimension 1 ou 2 dans pupitre
	virtual bool GetSelDimPupitreActive(JUnt32 Dim);

	// Suppression des élmts sélections (en ligne, colonne ou page)
	void SuppressionSelection(JFCListCtrl &Liste);

	// Suppression des élmts en doublon (en ligne, colonne ou page)
	void SuppressionDoublon(JFCListCtrl &Liste, JBool MessageSiPasDeDoublon);

	// Suppression des élmts en doublon via le repère absolu (en ligne, colonne ou page)
	void SuppressionDoublonRepAbsolu(JFCListCtrl &Liste, JBool MessageSiPasDeDoublon);

	// Tri des elmts sélectionnés en lignes ou colonnes ou pages
	void TriAllItemsOrdreAlpha(JFCListCtrl &Liste);

	// Copie Items sélection sur la liste cible (avec suppression éventuellement) 
	void CopierItemOnList(JFCListCtrl *pListSrc, JFCListCtrl *pListDst, JBool Remove = false);

	// Décalage bloc items vers le bas sur les listes sélections
	void DecalItemDown(JFCListCtrl &Liste);

	// Décalage bloc items vers le haut sur les listes sélections
	void DecalItemUp(JFCListCtrl &Liste);

	// Mise à jour validite bouton recalage en bas, en haut
	void SetValidUpDownListe(JFCListCtrl &Liste);

	// Tri ascendant ou descendant sur les sélections lignes, ou colonnes, ou pages
	void TriSelElmt(JFCSelListCtrl &ListElmt);

	// Déplacement du split vertical
	void DeplaceSplitVert(LONG Disp);

	// Repositionne le split via dernière sauvegarde
	void PosLastSplit();

	// Vire les sites sélectionnés en ligne, ou en colonne, ou en page
	void VireSites(JFCSelListCtrl &Liste);

	// Test si dans sélection active, sites présents
	bool SelectAvecSite(JFCListCtrl *pListSrc);

	// Récupération liste sélection sites
	void RecupListeSelSite(JFCSelListCtrl &Liste, JList <JLabel> &LstSiteSel);

	// Remet d'aplomb les flags sites (lignes, colonnes, pages)
	void UpdateModeSites();

	// Ajout des items quantitatifs sélectionnés et positionnement en ligne
	void AddItemsQuantis(JList <JUnt32> *pLstItemMoyAuto, JList <JUnt32> *pLstItemMoyUtil);

	// Validation des items à moyenner
	void ValideItemsQuantis(JList <JUnt32> *pLstItemMoyAuto, JList <JUnt32> *pLstItemMoyUtil);

protected:
	// Type poids Id
	UINT				m_TypePoidsId;

	// Segment Ids
	JList<UINT>			m_SegmentsId;

	// le document
	CAnalyseurDoc		*m_pDoc;

};
