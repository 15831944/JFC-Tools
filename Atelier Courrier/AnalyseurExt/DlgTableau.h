#pragma once
#include "DlgItems.h"
#include "ItemWrapper.h"
#include "PaletteMode.h"

class CAnalyseurDoc;

// CDlgTableau dialog


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classe clef �lmt item + terrain (servira aux �tats s�l�cteur dimension 1,2,3 par terrains)
class CKeyItemTerrain
{
public:

	// test d'inf�riorit� clef item + terrain 
	bool operator<(const CKeyItemTerrain & Rhs) const
	{
		if(m_IdItem < Rhs.m_IdItem)
			return true;
			
		if(m_IdItem == Rhs.m_IdItem && m_IdTerrain < Rhs.m_IdTerrain)
			return true;
			
		return false;
	};

	// test de sup�riorit� clef item + terrain 
	bool operator>(const CKeyItemTerrain & Rhs) const
	{
		if(m_IdItem > Rhs.m_IdItem)
			return true;
			
		if(m_IdItem == Rhs.m_IdItem && m_IdTerrain > Rhs.m_IdTerrain)
			return true;
			
		return false;
	};

	// test d'�galit� clef item + terrain 
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

	// s�rialisation clef item + terrain 
	void Send(JStream & Stream) const
	{
		// on s�rialise l'�l�ment
		Stream.Send(m_IdItem);
		Stream.Send(m_IdTerrain);
	};

	// s�rialisation clef item + terrain 
	void Recv(JStream & Stream)
	{
		// R�cup �lmt id item et id terrain
		Stream.Recv(m_IdItem);
		Stream.Recv(m_IdTerrain);
	};

	// test d'inf�riorit� clef item + terrain 
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

	// Init source et terrain utilis�s
	void SetTerrainTemoin(CTerrain * pTerrain);

	// Mode expoit�
	void SetMode(CPaletteMode::MODE_TABLEAU CurMode) { m_Id_CurMode = CurMode; } ;

    void SetParams(UINT TypePoidsId, const JList<UINT> & SegmentsId);
	void GetSelectedItems(JList<CItemWrapper *> & Items) { m_Items.Swap(Items); };

	// R�cup�re les choix d�j� effectu�s
	bool GetData(CAnalyseurDoc *pDoc);

	// Positionne les choix d�j� effectu�s
	bool SetData(CAnalyseurDoc *pDoc);

	// R�cup�ration des modes avant appel � pupitre
	void RecupExModePalette();

	// R�initialisation des �lmts s�lection
	void InitSelGeneral();

	// Sauvegarde des palettes ligne, colonne, page
	// bool SavePalettes(CString PathName);
	bool SavePalettes(CStdioFile & file);

	// R�cup�ration des palettes ligne, colonne, page
	// bool LoadPalettes(CString PathName);
	bool LoadPalettes(CStdioFile & file);

	// Transposition des palettes
	void Transpose(int Mode1, int Mode2);

	// R�cup�re document associ�
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

	// R�cup�ration des items s�lectionn�s
	void RecupItemSelect();

	// Validation des s�lections tris crois�s (en lignes/ colonnes/ pages)
	bool ValideSelectionTriCroise();

	// Mise � jour du nombre de s�lection (en ligne, en colonne, en page)
	void UpdateCount();

	/// Displays the selected theme, support window
	void UpdatePupitre(int ID);

	/// Sets up the current mode
	void UpdateMode(int ID, bool MajSAS = true);

	// Suppression de toute la s�lection (lignes + colonnes + pages)
	void EffacerAllSel();

	// Mise � jour de la liste s�lection en mode ajout pr�sentoir ligne, colonne ou page 
	void UpdateListSelect(bool ModeSelItemQuanti = false);

	// Mise � jour nb cellules correspondant � la s�lection
	void UpdateNbCells();

	// Mise � jour de la liste s�lection courante via les s�lections ligne, colonne ou page (avec ou pas raz)
	void UpdateListSelect_Synchro(bool Raz);

	// Update de toutes les listes s�lections
	void UpdateAllListSelect(bool ModeAjout = true);

	// Mise � jour validation des boutons tri s�lection lignes/colonnes/pages
	void UpdateBtnTriSel();

	// Active ou D�sacte du mode s�lection courante (lignes ou colonnes ou pages)
	void ActiveModeSel(JUnt32 CurModeSel, JUnt32 Dimension, bool Actif);

	// Affichage de la palette du mode en cours
	void AffichePaletteModeCur();

	// Valide le 1er elmt du Sas - Attention si quelque chose dans le sas et non s�lectionn�
	void ValideElmtSas(bool ChangeSas = true);

	// Affichage choix menu op�rateur SAS pour ET
	void AfficheChoixMenuSasEt();

	// Affichage choix menu op�rateur SAS pour OU
	void AfficheChoixMenuSasOu();

	// Change mode activation de la dimension 1 pour construction palette
	void ActiveModeAjoutDim1(bool RazSelection);

	// Visibilit� Boite Items Validation
	void SetVisibleBoiteValidItem(bool Visible);

	// D�placement item de liste en liste
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList);

	// Questions en cours de s�lection non dispos� en ligne ou colonne ou page
	JBool QuestionsSelEnCours();

protected:
	JList<CItemWrapper *>	m_Items;

	JBool MajNbCells;

protected:
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnGenese();

	// Gestion des modes pr�sentoir tris crois�s
	afx_msg void OnBnClickedRiRadModeLignes();
	afx_msg void OnBnClickedRiRadModeColonnes();
	afx_msg void OnBnClickedRiRadModePages();

	// Gestion des items � moyenner en ligne et colonne
	afx_msg virtual void OnBnClickedRiItemMoyenLig();
	afx_msg virtual void OnBnClickedRiItemMoyenCol();

	// Gestion suppression lignes ou colonnes ou pages s�lectionn�es
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

	// Les messages d�placement pour r�ordonnacement
	afx_msg virtual void OnBnClickedRiButLignesDown();
	afx_msg virtual void OnBnClickedRiButLignesUp();
	afx_msg virtual void OnBnClickedRiButColonnesUp();
	afx_msg virtual void OnBnClickedRiButColonnesDown();
	afx_msg virtual void OnBnClickedRiButPagesUp();
	afx_msg virtual void OnBnClickedRiButPagesDown();

	// S�lection ou d�selection de tous les �lmts
	afx_msg virtual void OnBnClickedRiButAllLignes();
	afx_msg virtual void OnBnClickedRiButAllColonnes();
	afx_msg virtual void OnBnClickedRiButAllPages();

	/// Message sent from modeless Modality and Element dialogs
	afx_msg virtual LRESULT OnSelChange_ListDimension_x(WPARAM wParam, LPARAM lParam);
	/// Message sent from values dialog
	afx_msg virtual LRESULT OnChange_Dim_ValueSel(WPARAM wParam, LPARAM lParam);
	/// Message sent from values dialog
	afx_msg virtual LRESULT OnSelChange_ListValue(WPARAM wParam, LPARAM lParam);

	// Gestion Keydown sur liste s�lections lignes
	afx_msg virtual void OnLvnKeydownRiListLignes(NMHDR *pNMHDR, LRESULT *pResult);

	// Gestion Keydown sur liste s�lections colonnes
	afx_msg virtual void OnLvnKeydownRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult);

	// Gestion Keydown sur liste s�lections pages
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

	// Message s�lection ou d�selection elmt listes
	afx_msg virtual void OnNMClickRiListLignes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnNMClickRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnNMClickRiListPages(NMHDR *pNMHDR, LRESULT *pResult);

	// Message suppression de toutes les s�lections par mode (ligne, colonne,page)
	afx_msg virtual void OnSupprimertoutesleslignes();
	afx_msg virtual void OnSupprimertouteslescolonnes();
	afx_msg virtual void OnSupprimertouteslespages();

	// �tat liste items row change
	afx_msg virtual void OnLvnItemchangedRiListLignes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnItemchangedRiListPages(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg virtual void OnLvnItemchangedRiListColonnes(NMHDR *pNMHDR, LRESULT *pResult);

	// Gestion ajout des sites en lignes, colonnes ou pages
	afx_msg virtual void OnBnClickedRiBtnSitesLig();
	afx_msg virtual void OnBnClickedRiBtnSitesCol();
	afx_msg virtual void OnBnClickedRiBtnSitesPage();

	// Effaecement de toute la s�lection (lignes, colonnes, pages)
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

	// Flags changement s�lection
	bool						m_bItemsChanged;
	bool						m_bRowsChanged;
	bool						m_bColsChanged;
	bool						m_bPagesChanged;

	// Flags positions tris
	bool						m_bRowsSort;
	bool						m_bColsSort;
	bool						m_bPagesSort;

	// Les s�lections items
	int							m_DType_Row;
	CONST_LIST_ITEM_PUPITRE		m_Items_Row;
	int							m_DType_Col;
	CONST_LIST_ITEM_PUPITRE		m_Items_Col;
	int							m_DType_Page;
	CONST_LIST_ITEM_PUPITRE		m_Items_Page;
	LIST_ITEM_PUPITRE			m_Items_Original;
	LIST_ITEM_PUPITRE			m_Items_New;
	LIST_ITEM_PUPITRE			m_Items_Del;

	// Les items � moyenner
	LIST_ITEM_PUPITRE			m_AllItemsAutos;
	LIST_ITEM_PUPITRE			m_AllItemsUtils;

	// Les mementos temporaires des modes ligne, colonne, page
	JMemento	*pMementoLigne;
	JMemento	*pMementoColonne;
	JMemento	*pMementoPage;

	// Activation ou pas des modes ajouts s�lections (via dimension 1 et dimension 2)
	bool				m_AjoutSelLigActif_Dim1;
	bool				m_AjoutSelLigActif_Dim2;
	bool				m_AjoutSelLigActif_Dim3;
	bool				m_AjoutSelColActif_Dim1;
	bool				m_AjoutSelColActif_Dim2;
	bool				m_AjoutSelColActif_Dim3;
	bool				m_AjoutSelPageActif_Dim1;
	bool				m_AjoutSelPageActif_Dim2;
	bool				m_AjoutSelPageActif_Dim3;
	
	// Mise � jour des s�lecteurs dimesnion 1, dimension 2, �ventuellement dimension 3
	void UpdateSelecteurDimension(bool Dim1Active, bool Dim2Active, bool Dim3Active);

	// Le mode onglet en cours, ligne ou colonne ou page
	JUnt32				m_Id_CurModePrec;

	// Map de sauvegarde pour le mode s�lection choisi pour les items s�lectionn�s, sur dimension1 et Dimension2
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionLigDim1;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionLigDim2;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionLigDim3;

	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionColDim1;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionColDim2;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionColDim3;

	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionPageDim1;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionPageDim2;
	JMap<CKeyItemTerrain, JBool>  m_MapModeSelQuestionPageDim3;

	// Sauvegarde des modes des s�lections pour dimension 1 et dimension 2
	void SaveModeSelectionDim1Dim2(JInt32 IdItem);

	// R�cup�re modes des s�lections pour dimension 1 et dimension 2
	void LoadModeSelectionDim1Dim2(JInt32 IdItem, JUnt32 NbDimension);

	// Selon les modes on positionne ou pas les boutons s�lections
	void SetBoutonSelection();

	// Disposition listes r�sultas (lignes, colonnes, pages)
	void SetListeResultat();

	// Mise en place des fen�tres r�sultats
	void SetFenResultat(JInt32 NbFenVisible);

	// Mise � jour des listes s�lections
	void MiseAJourListeSelection(JList<CItemWrapper *> &List);

	void MajViaNewElementAvecTri(JInt32 Mode, JFCSelListCtrl &ListCtrlCourante, JList<CItemWrapper *> &List, int IndiceDepIdemHeader);
	void MajViaNewElementSansTri(JInt32 Mode, JFCSelListCtrl &ListCtrlCourante, JList<CItemWrapper *> &List);
	
	// Retaille largeur colonnes des liste de  s�lection
	void ResizeColListSelect(JUnt32 IdMode);

	// Renvoi �tat s�lection dimension 1 ou 2 dans pupitre
	virtual bool GetSelDimPupitreActive(JUnt32 Dim);

	// Suppression des �lmts s�lections (en ligne, colonne ou page)
	void SuppressionSelection(JFCListCtrl &Liste);

	// Suppression des �lmts en doublon (en ligne, colonne ou page)
	void SuppressionDoublon(JFCListCtrl &Liste, JBool MessageSiPasDeDoublon);

	// Suppression des �lmts en doublon via le rep�re absolu (en ligne, colonne ou page)
	void SuppressionDoublonRepAbsolu(JFCListCtrl &Liste, JBool MessageSiPasDeDoublon);

	// Tri des elmts s�lectionn�s en lignes ou colonnes ou pages
	void TriAllItemsOrdreAlpha(JFCListCtrl &Liste);

	// Copie Items s�lection sur la liste cible (avec suppression �ventuellement) 
	void CopierItemOnList(JFCListCtrl *pListSrc, JFCListCtrl *pListDst, JBool Remove = false);

	// D�calage bloc items vers le bas sur les listes s�lections
	void DecalItemDown(JFCListCtrl &Liste);

	// D�calage bloc items vers le haut sur les listes s�lections
	void DecalItemUp(JFCListCtrl &Liste);

	// Mise � jour validite bouton recalage en bas, en haut
	void SetValidUpDownListe(JFCListCtrl &Liste);

	// Tri ascendant ou descendant sur les s�lections lignes, ou colonnes, ou pages
	void TriSelElmt(JFCSelListCtrl &ListElmt);

	// D�placement du split vertical
	void DeplaceSplitVert(LONG Disp);

	// Repositionne le split via derni�re sauvegarde
	void PosLastSplit();

	// Vire les sites s�lectionn�s en ligne, ou en colonne, ou en page
	void VireSites(JFCSelListCtrl &Liste);

	// Test si dans s�lection active, sites pr�sents
	bool SelectAvecSite(JFCListCtrl *pListSrc);

	// R�cup�ration liste s�lection sites
	void RecupListeSelSite(JFCSelListCtrl &Liste, JList <JLabel> &LstSiteSel);

	// Remet d'aplomb les flags sites (lignes, colonnes, pages)
	void UpdateModeSites();

	// Ajout des items quantitatifs s�lectionn�s et positionnement en ligne
	void AddItemsQuantis(JList <JUnt32> *pLstItemMoyAuto, JList <JUnt32> *pLstItemMoyUtil);

	// Validation des items � moyenner
	void ValideItemsQuantis(JList <JUnt32> *pLstItemMoyAuto, JList <JUnt32> *pLstItemMoyUtil);

protected:
	// Type poids Id
	UINT				m_TypePoidsId;

	// Segment Ids
	JList<UINT>			m_SegmentsId;

	// le document
	CAnalyseurDoc		*m_pDoc;

};
