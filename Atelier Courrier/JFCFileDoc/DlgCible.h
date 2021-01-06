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
//	Boite fille de DialogChoixOffre
//	Permet la sélection des cibles, palette et de la référence affinité dans l'offre
*/

#pragma once

#include "DialogChoixOffre.h"
#include "RedItemListBox.h"
#include "CalcItemParams.h"
#include "afxcmn.h"
#include "TextProgressCtrl.h"
#include "RubriqueItemTree.h"

// Boîte de dialogue CDlgCible

class CDlgCible : public ETSLayoutDialog
{
	DECLARE_DYNAMIC(CDlgCible)

public:
	// Type d'interdiction
	enum TYPE_INTERDIT
	{
		Item_Interdit_Aucun		=	0,		// Aucune Interdiction
		Item_Interdit_Partiel	=	1,		// Interdiction partielle (uniquement sur certains terrains)
		Item_Interdit_Total		=	2,		// Interdiction totale (sur tous les terrains)
	};

	CDlgCible(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgCible();

	// Positionne l'offre en cours
	void SetData (CDialogChoixOffre* data);

	// Récupère l'offre en cours
	CDialogChoixOffre * GetData ();

	bool IsComplete();
// Données de boîte de dialogue
	enum { IDD = IDD_FD_DLG_CIBLE };

	void Init();

	// Fonction vérification et identification des items en dessous des seuils
	void CheckSeuils();

	// Fonction affichage des seuils des enquêtes sélectionnées
	void AffichSeuils();

	void ResetMapParams();

public:
	// Infos sélection source et terrain
	JUnt32							m_IdSrcAudience;
	JList <JUnt32>					m_LstIdTerrains;

	// Map informations des terrains interdits par items
	JMap <JUnt32, JMap<JUnt32, JBool> > m_MapTerrainsInterdits;

protected:
	enum INTERNALMODE {ITEM, PALETTE, FLORILEGE, AFFINITE};
	INTERNALMODE m_internalMode;

	CDialogChoixOffre		* m_pData;
	CItemsConstruitsBase    * m_pItems;
	CString m_searchStr;

	// Liste sélection en cours sur rubrique items
    JMap<DWORD,JUnt32>	m_MapSelRubriqueItem;
	JMap<DWORD,JUnt32>	m_MapSelRubriquePalette;
	JMap<DWORD,JUnt32>	m_MapSelRubriqueRefAff;


	DECLARE_MESSAGE_MAP()
	virtual void OnOK() {/*do nothing*/};
	virtual void OnCancel() {/*do nothing*/};
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnItems();
	afx_msg void OnBnClickedBtnPalettes();
	afx_msg void OnBnClickedBtnAffinite();
	afx_msg void OnLbnSelchangeListHaut();
	afx_msg void OnLbnSelchangeListBas();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnChangeSearch();
	afx_msg void OnBnClickedRazSearch();
	afx_msg void OnBnClickedBtnAll();
	afx_msg void OnPaint();

	/// Message sent from tree rubrique control
	afx_msg LRESULT OnUpdated_ItemList(WPARAM wParam, LPARAM lParam);

	void FillListItem();
	void FillListPalette();
	void FillListRefAff();

	void FillResultListItem();
	void FillResultListPalette();
	void FillResultListAffinite();

	// Sets up rubriques items
	void SetRubriqueItems();
	void SetRubriquePalettes();
	void SetRubriqueRefAffs();

	// Visualisation ou pas d'un item
	bool ShowItem(JInt32 IdItem);

	// Visualisation ou pas d'une palette
	bool ShowPalette(JInt32 IdItem);

	// Visualisation ou pas d'une réf affinité
	bool ShowRefAff(JInt32 IdItem);

private:

	CTerrain * m_pTerrainC;
	JList<CTerrain*> m_Terrains;

	bool IDExist( JUnt32 id,	JList<JUnt32>& lst );
	void RemoveID( JUnt32 id ,	JList<JUnt32>& lst );

	CLabel				m_ttlCibles;

	CRedItemListBox		m_lstHaut;

	// Nomenclature classement des items
	CRubriqueItemTree	m_Tree_Rubrique_Items;
	CRubriqueItemTree	m_Tree_Rubrique_Palettes;
	CRubriqueItemTree	m_Tree_Rubrique_RefAffs;

	CRedItemListBox		m_lstBas;
	CButtonST			m_btnItem;
	CButtonST			m_btnPalette;
	CButtonST			m_btnAffinite;
	CButtonST			m_btnRAZSearch;
	CButtonST			m_btnAfficheAll;

	void SetColors();
	void SetupControls();
	void ResetSize();
	void AddItemParam(const CItemConstruit *item);

	// Mise en place des contrôles selon l'élément en cours (soit ITEMS, ou PALETTES, ou REF AFF)
	void SetLayoutElmt(BOOL bShow, UINT nStatus);

	// Item interdit ou pas
	JBool ItemSimpleInterdit(JInt32 ID_Item, JUnt32 InxLst, JBool & MessageAffich);

	// Renvoi si avertissement nb cas cible en dessous seuil avertissement
	JVoid AvertissementItemSimple(JInt32 ID_Item, JUnt32 IndexLst);

	// Nombre d'éléments sélectionnables
	JUnt32 m_NbElmtLstBas;

	// Map informations des terrains interdits par items
	// JMap <JUnt32, JMap<JUnt32, JUnt32> > m_MapTerrainsInterdits;

friend class CRedItemListBox;

	// Les infos items (via tous les terrains)
	JMap  <JInt32, JMap <JInt32, CCalcItemParams*> > m_mapItemParams;

	// Avec en + retour des terrains interdits
	JUnt32 DegreInterdit(int ID);

	// Degré d'interdiction des items ou palettes
	JUnt32 DegrePaletteInterdit(int ID);
	JUnt32 DegreItemInterdit(int ID);

#define IdTimer_Search WM_USER+101

	CTextProgressCtrl m_progress2;

};
