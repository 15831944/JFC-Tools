#pragma once
#include "afxwin.h"
#include "BtnST.h"
#include "Label.h"
#include "JFCListBox.h"
// #include "JFCSelListCtrl.h"
#include "ItemsConstruitsBase.h"
#include "Terrain.h"
#include "JATPDocCreuset.h"
// #include "JFCSpinEditGrid.h"
#include "GridPonderation.h"

//////////////////////////////////////////////////////////////////
// Classe informations coefficient + noLigne
class CInfoCoeff
{

public:
	CInfoCoeff()
	{
		Coeff = 0.0;
		NoLigne = 0;
	};
	~CInfoCoeff() {}

public :
	JFlt64 Coeff;
	JInt32 NoLigne;
};


// Boîte de dialogue CDlgCiblePondere

class CDlgCiblePondere : public CDialog
{
	DECLARE_DYNAMIC(CDlgCiblePondere)

public:
	// Constructeur
	CDlgCiblePondere(CWnd* pParent = NULL);

	// Destructeur
	virtual ~CDlgCiblePondere();

	// Positionne l'offre en cours
	JVoid SetData(JATPDocCreuset * pDoc);

	// Positionne le mode d'entrée dans dialog
	JVoid SetMode(JUnt32 ModeCiblepond) {m_ModeCiblePond = ModeCiblepond;} ;


	// Données de boîte de dialogue
	enum { IDD = IDD_CR_DGL_CIBLEPOND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
public:

	// Backgroud fenêtre dialogue cible pondérée
	CLabel		m_Bk_CiblePond;

	// Titre au dessus de la liste des items
	CLabel		m_Titre_ListeItems;

	// Liste des items disponibles
	JFCListBox  m_Lst_Items;

	// Liste des cibles pondérées existantes
	JFCListBox	m_Lst_CiblesPonds;

	// Titre au dessus de la liste des composants à pondérer
	CLabel		m_Titre_Composants;

	// Liste des composants pour les pondérations
	JFCListBox  m_Lst_Composants;

	// Titre pour l'édition cible pondérée
	CLabel		m_Titre_CiblePonderee;

	// Boite dédition pour le nom de la cible pondérée
	CEdit		m_Edit_CiblePond;

	// Bouton pour créer une cible pondérée (par défaut)
	CButtonST	m_Btn_CreerCible;

	// Bouton pour modifier une cible pondérée (par défaut)
	CButtonST	m_Btn_ModifCible;

	// Titre au dessus du tableau des pondérations
	CLabel		m_Titre_Ponderation;

	// La grille permettant la saisie des pondérations
	CGridPonderation m_GridPonder;
	
	// Le complément
	CLabel		m_Titre_Complement;

	// Valeur % réel du complément via les composants
	CLabel		m_Prct_Compl_Reel;

	// Valeur % pondéré du complément via les composants
	CLabel		m_Prct_Compl_Pond;
	
	// Bouton pour valider la cible pondération en cours
	CButtonST   m_Btn_ValidCiblePond;

	// Bouton fermeture du dialogue cible pondérée
	CButtonST   m_Btn_FermerCiblePond;

	// Bouton suppression de la cible pondérée sélectionnée
	CButtonST	m_Btn_SuppCiblePond;

	// Les 2 types de fonctionalités dans dialog cibles pondérées
	enum ModeCiblePond {MODE_CREATION = 0, MODE_MODIFICATION};

public:
	// Update valeur d'une cellule envoyée par le grille pondération
	void UpdateValue(ROWCOL nRow, ROWCOL nCol);

private:
	// Initialisation des infos boite de dialogue
	void Init();

	// Chargement des items disponibles 
	JBool LoadItems();

	// Charge les dernières cibles pondérées
	JBool LoadCiblesPonds();

	/// Get the Items construits
	CItemsConstruitsBase * GetItems() { return CItemsConstruitsManager::GetInstance()->GetItemsInstance(); }

protected:

	// Initialisation fenêtre cible pondérée
	virtual BOOL OnInitDialog();

	// Gestion des controles
	void SetupControls();

	// Couleurs des contrôles
	void SetColors();

	// Passage mode création ou modification
	void UpdateMode();

	// Validité de la sélection composants
	JBool SelItemsComposOK();

	// Récupère vecteur de cible
	void GetVectItem(CItemConstruit * pItem, JInt32 IdTerrain, CCalcItemParams & Params);

	// Sauvegarde de l'item pondéré
	JBool SaveItemPondere();

	// Modification d'une cible pondérée existante
	JBool ModifItemPondere();

	// Suppression d'une cible pondérée existante
	JBool SupprimeItemPondere();

	// Activation de la cible pondérée sélectionnée
	JVoid ActiveCiblePond();

	// Désélection de tous les composants
	JVoid DeselectAllCompos();

	// Ecriture d'une ligne du grid
	void UpdateGridLigne(JUnt32 IndexLigne, JLabel Libelle, JFlt64 CoeffReel, JFlt64 CoeffPond);

	// Update des composants dans la grille via le map courant des items composants
	void UpdateGrilleComposants(const JMap <JInt32, JFlt64> & MapCoeff);

	// Update de la ligne complément
	void UpdateInfoComplement(JFlt64 CumulCoeff, JFlt64 CumulPonder);

	// Total pondération en cours avec la ligne modifiée en cours
	JFlt64 GetTotPonderation(const ROWCOL nRow, const JFlt64 NewValue);

	// Calcul coeff reel item composant via l'item de base
	JFlt64 CalcCoeffReel(JUnt32 IdItemBase, JUnt32 IdCompo);

	// Vérification de la cohérence de la configuration des composants pondérés
	JBool CoeffCompoValide();

private:
	// Le document
	JATPDocCreuset *	m_pDoc;

	// Liste des terrains
	JList<CTerrain*> m_LstTerrains;

	// Item de base sélectionné
	JInt32	m_ItemBase;

	// Ident cible pondéré sélectionné
	JInt32  m_ItemPondere;

	// Map des composants et de leurs coefficients
	JMap <JInt32, CInfoCoeff>	m_MapNoLigCompoCoeff;

	// Mode fonctionnel en cours
	JUnt32 m_ModeCiblePond; 

	// Terrain par défaut (le 1er de la liste des sélectionnés)
	JUnt32 m_IdTerrainDef; 

private:
	//////////////////////// Paramètrage de la grille pondération ////////////
	//
	// Les 2 ères colonnes fixes
	static const int Const_NbColFixe = 2;

	// 1 seule décimale pour les %
	static const int Const_NbDecimale = 1;

private:
	//////////////////////////////////////////////////////////////////////////
	//						Les opérations de calcul
	//////////////////////////////////////////////////////////////////////////

	// Effectif d'un item via un filtre item source
	JFlt64 PoidsItemComposant(const JUnt32 IdSource, const CTerrain * pTerrain, const JUnt32 IDItemBase, JFlt64 & PoidsItemBase, const JUnt32 IDItem);

public:
	afx_msg void OnLbnSelchangeCrLstComposants();
	afx_msg void OnLbnSelchangeCrLstItems();
	afx_msg void OnBnClickedCrValider();
	afx_msg void OnBnClickedCrCreerCiblepond();
	afx_msg void OnBnClickedCrModifCiblepond();
	afx_msg void OnBnClickedCrSupprimer();
	afx_msg void OnLbnSelchangeCrLstCiblepond();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnBnClickedCrFermer();
};
