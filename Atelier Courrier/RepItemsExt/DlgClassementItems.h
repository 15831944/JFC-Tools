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

// Bo�te de dialogue CDlgClassementItems
class CDlgClassementItems : public CDialog
{
	DECLARE_DYNCREATE(CDlgClassementItems)

public:
	// Constructeur
	CDlgClassementItems(CWnd* pParent = NULL);   // constructeur standard

	// Destructeur
	virtual ~CDlgClassementItems();

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_RI_DLG_CLASSE_ITEMS};

	// D�finition couleurs des contr�les
	JVoid SetColors();

	// Classe dialogue d�finition du tree
	// CDlgItems * m_pDlgItems;
	CItemsConstruitsBase	* m_pItemsConstruits;
	CTerrain				* m_pTerrain;

	// Nomenclature rubriques concern�es (selon type items, palettes ou autres...)
	CRubriqueItemTree	* m_pTreeRubrique;

private:

	// Tableau des items en cours
	JMap <JUnt32,CString> m_MapItems;	

	// Liste des rubriques existantes
	JFCListBox m_Lst_Rubriques;

	// Liste des items de la rubrique s�lectionn�e
	JFCListBox m_Lst_Items;

	// Liste des rubriques destinataires
	JFCListBox m_Lst_Rubrique_Dest;

	// Bouton permettant le transfert d'items d'une rubrique � une autre
	CButtonST m_Btn_Transfert_Items;

	// Cadre englobant les contr�les
	CLabel m_Cadre;

	// Label ent�tes
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

	// Initialisation des contr�les
	JVoid SetupControls();

	// Relations et positionnements des contr�les
	JVoid SetWindowRelations();

	DECLARE_MESSAGE_MAP()

private:
	
	// Init tableau des items en cours
	JVoid LoadItems();

	// Remplir liste des rubriques sources
	JVoid RecupListRubriqueSrc();

	// Remplir liste rubrique destination
	JVoid RecupListRubriqueDst();

	// Affiche items des rubriques sources s�lectionn�es
	JVoid AffichItemRubriqueSrc();

	// Permuter item d'une rubrique � l'autre
	JBool PermutItemRubrique(CString TxtRubriqueSrc, CString TxtRubriqueDst, JUnt32 IdItem);

	// Mise � jour liste items de transfert en cours
	JVoid UpdateItemTransfert();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeRiListRubriques();
	afx_msg void OnBnClickedRiButTransfertItems();
};
