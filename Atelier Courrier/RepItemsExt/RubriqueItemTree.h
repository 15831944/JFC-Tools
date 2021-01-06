/*
//	JFC Informatique et Média
//
//	Classe:
//	CRubriqueItemTree
//
//	Descriptif:
//	TreeCtrl spécifique pour affiche des rubriques items
*/

#pragma once
#include "ThemeTree.h"
#include "ItemsConstruitsBase.h"
#include "RubriqueItemTree.h"
#include "Terrain.h"
#include "JLib.h"

// #define WM_USER_ITEMLISTUPDATED (WM_USER + 102)

class CDlgItems;

class AFX_EXT_CLASS CRubriqueItemTree : public CThemeTree
{
public:
	// Constructeur
	CRubriqueItemTree();

	// Destructeur
	~CRubriqueItemTree();

	// Positionne le type de rubriques
	JVoid SetTypeRubrique(JUnt32 TypeRubrique);

	// Positionne mode item (si nécessaire)
	

	// Chargement des rubriques (items, palettes ou classes)
	// bool	LoadRubriquesItems();
	bool	LoadRubriquesItems(CItemsConstruitsBase *pItemsConstruits, CTerrain *pTerrain);
	bool	LoadRubriquesItems(CItemsConstruitsBase *pItemsConstruits, JList <CTerrain *> &LstTerrains);


	// Sauvegarde rubriques items en cours
	bool	SaveRubriquesItems();

	// Mise à jour des items via les rubriques sélectionnées
	void	UpdateItemList();

	// Visibilité ou pas d'un item
	bool	ShowItem(JInt32 IdItem);

	// Tous les nouveaux items seront d'office dans rubrique divers
	// void	ReactualiseItems();
	void ReactualiseItems(CItemsConstruitsBase *pItemsConstruits, CTerrain *pTerrain);
	void ReactualiseItems(CItemsConstruitsBase *pItemsConstruits, JList <CTerrain *> &LstTerrains);

	// Réordonne listes des rubriques par ordre chronologique
	JVoid	TriRubriques();

	// Classe dialogue définition du tree
	CItemsConstruitsBase	* m_pItemsConstruits;
	CTerrain				* m_pTerrain;	
	CDialog					* m_pDialog;

	// Récupère fentre items
	CDlgItems		* m_pDlgItems;
	
	// Les rubriques racine et divers
	HTREEITEM m_hRoot;
	HTREEITEM m_hRubriqueDivers;
	HTREEITEM m_hRubriqueDefaut;

	// Type de rubriques (items, palettes, classes, quantiles, questions induites...)
	const static int TYPE_RUBRIQUE_ITEM		= 0;
	const static int TYPE_RUBRIQUE_PALETTE	= 1;
	const static int TYPE_RUBRIQUE_CLASSE	= 2;
	const static int TYPE_RUBRIQUE_QUANTILE	= 3;
	const static int TYPE_RUBRIQUE_QINDUITE = 4;

	// expand de la totalité des rubriques
	void ExpandRubriqueDivers();

private:

	friend class CDlgClassementItems;

	// Variables gestion tree rubriques
	bool		m_bEditable;
	bool		m_bExpandAll;
	bool		m_bMultiSel;
	bool		m_bSelectionComplete;
	COLORREF	m_selColor;
	bool		m_Firstime;

	// Map des rubriques
	JMap <CString, JList<JInt32> > m_Map_RubriquesItems;

	// Map des items à afficher
	JMap <JInt32, bool> m_MapAffichItems;

	/// File Header
	CFileHeader				m_FileHeader;

	// Chargement fichier rubriques
	JBool LoadFileRubriques();

public:
	// Sauvegarde du fichier rubriques en cours
	JBool SaveFileRubrique();

private:
	// Chargement entete fichier rubrique
	JVoid LoadFileHeader(JArchive *pFile);

	// Sauve header file rubrique
	JVoid SaveFileHeader(JArchive *pFile);

	// Charge les rubriques
	JVoid LoadRubriques(JArchive *pFile);

	//Sauve les rubriques avec liste des items associés
	JVoid SaveRubriques(JArchive *pFile);
	
	// Init nouveau style
	void	SetNewStyle(long lStyleMask, BOOL bSetBits);

	// Désactive ou réactive affichage des enfants des branches fils
	void	SetEnfant(HTREEITEM hti, JUnt32 FlagEnfant);

	// Type de rubrique (0 pour items)
	JUnt32		 m_Type_Rubrique;	

	// Constante tree rubrique (on distingue racine, rubrique divers, et rubrique client)
	const static int ID_ROOT				= 0;
	const static int ID_RUBRIQUE_DIVERS		= 1;
	const static int ID_RUBRIQUE_CLIENT		= 2;

	// Compteur rubrique
	LONG m_CptRubrique;

	// Les modes édition rubrique
	JUnt32		 m_Mode_Edit;
	const static int MODE_EDIT_DIRECT		= 0;
	const static int MODE_ADD_RUBRIQUE		= 1;
	const static int MODE_RENAME_RUBRIQUE	= 2;

	// Fabrique texte pour nouvelle rubrique
	CString GetTxtNewRubrique();

	// Ajout d'une rubrique
	bool AddRubrique(HTREEITEM htiRubrique);

	// Suppression d'une rubrique
	bool SuppRubrique(HTREEITEM htiRubrique);

	// Suppression d'une rubrique et de ses items
	bool SuppRubriqueEtItems(HTREEITEM htiRubrique);

	// Renomme une rubrique
	bool RenameRubrique(HTREEITEM htiRubrique, CString OldLabel);

public:
	// Spécifique gestion dossiers rubriques items
	void	SetSelectionColor(COLORREF selColor);
	void	EnableEdit(bool bEnable); 
	void	EnableExpandAll(bool bEnable);
	void	EnableMultipleSel(bool bEnable);

	// Mise à jour des items via l'arborescence rubrique
	void	UpdateItemList(HTREEITEM hti);

	// Ajout d'un item dans la rubrique divers
	void	AddItemRubriqueDivers(JInt32 IdItem, JLabel LabelItem);

	// Ajout d'un item dans une rubrique
	void	AddItemRubrique(HTREEITEM htiRubrique, JInt32 IdItem, JLabel LabelItem);

public:
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);

protected:
	
	//{{AFX_MSG(CRubriqueItemTree)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	// Les événements menu contextuel rubriques items
	afx_msg void OnCtxRubriqueAdd();
	afx_msg void OnCtxRubriqueSupp();
	afx_msg void OnCtxRubriqueRename();
	afx_msg void OnCtxClasserItems();
	afx_msg void OnCtxRubriquePardefaut();
	afx_msg void OnUpdateCtxRubriquePardefaut(CCmdUI *pCmdUI);
	afx_msg void OnCtxRubriqueEtItemsSupp();

	afx_msg void OnBeginLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT *pLResult);
	afx_msg void OnBeginDrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnExpandingRubricItems(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()


};
