///////////////////////////////////////////////////////////////////////////////////////////
// Classe boite de dialogue pour classement des items dans rubriques
//
// Novembre 2006

#pragma once

#include "Resizer.h"
#include "afxwin.h"
#include "BtnST.h"
#include "JFCListBox.h"
#include "label.h"
#include "RubriqueItemTree.h"

class CDlgItems;

// Boîte de dialogue CDlgClassementItems
class CDlgClassementItems : public CDialog
{
	DECLARE_DYNCREATE(CDlgClassementItems)

public:
	// Constructeur
	CDlgClassementItems(CWnd* pParent = NULL);   // constructeur standard

	// Destructeur
	virtual ~CDlgClassementItems();

	// Données de boîte de dialogue
	enum { IDD = IDD_RI_DLG_CLASSE_ITEMS};

	// Définition couleurs des contrôles
	JVoid SetColors();

	// Classe dialogue définition du tree
	// CDlgItems * m_pDlgItems;
	CItemsConstruitsBase	* m_pItemsConstruits;
	CTerrain				* m_pTerrain;

	// Nomenclature rubriques concernées (selon type items, palettes ou autres...)
	CRubriqueItemTree	* m_pTreeRubrique;

private:

	// Tableau des items en cours
	JMap <JUnt32,CString> m_MapItems;	

	// Liste des rubriques existantes
	JFCListBox m_Lst_Rubriques;

	// Liste des items de la rubrique sélectionnée
	JFCListBox m_Lst_Items;

	// Liste des rubriques destinataires
	JFCListBox m_Lst_Rubrique_Dest;

	// Bouton permettant le transfert d'items d'une rubrique à une autre
	CButtonST m_Btn_Transfert_Items;

	// Cadre englobant les contrôles
	CLabel m_Cadre;

	// Label entêtes
	CLabel m_Label_Rubr_Src;
	CLabel m_Label_Items;
	CLabel m_Label_Rubr_Dst;

	// Sortie transfert
	CButtonST m_BtnOK;

protected:
	/// Resize class
	CResizer				m_Resizer;

	virtual JVoid DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	// Initialisation boite dialogue
	virtual BOOL OnInitDialog();

	// Initialisation des contrôles
	JVoid SetupControls();

	// Relations et positionnements des contrôles
	JVoid SetWindowRelations();

	DECLARE_MESSAGE_MAP()

private:
	
	// Init tableau des items en cours
	JVoid LoadItems();

	// Remplir liste des rubriques sources
	JVoid RecupListRubriqueSrc();

	// Remplir liste rubrique destination
	JVoid RecupListRubriqueDst();

	// Affiche items des rubriques sources sélectionnées
	JVoid AffichItemRubriqueSrc();

	// Permuter item d'une rubrique à l'autre
	JBool PermutItemRubrique(CString TxtRubriqueSrc, CString TxtRubriqueDst, JUnt32 IdItem);

	// Mise à jour liste items de transfert en cours
	JVoid UpdateItemTransfert();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeRiListRubriques();
	afx_msg void OnBnClickedRiButTransfertItems();
};
