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


// Bo�te de dialogue CDlgCiblePondere

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

	// Positionne le mode d'entr�e dans dialog
	JVoid SetMode(JUnt32 ModeCiblepond) {m_ModeCiblePond = ModeCiblepond;} ;


	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_CR_DGL_CIBLEPOND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
public:

	// Backgroud fen�tre dialogue cible pond�r�e
	CLabel		m_Bk_CiblePond;

	// Titre au dessus de la liste des items
	CLabel		m_Titre_ListeItems;

	// Liste des items disponibles
	JFCListBox  m_Lst_Items;

	// Liste des cibles pond�r�es existantes
	JFCListBox	m_Lst_CiblesPonds;

	// Titre au dessus de la liste des composants � pond�rer
	CLabel		m_Titre_Composants;

	// Liste des composants pour les pond�rations
	JFCListBox  m_Lst_Composants;

	// Titre pour l'�dition cible pond�r�e
	CLabel		m_Titre_CiblePonderee;

	// Boite d�dition pour le nom de la cible pond�r�e
	CEdit		m_Edit_CiblePond;

	// Bouton pour cr�er une cible pond�r�e (par d�faut)
	CButtonST	m_Btn_CreerCible;

	// Bouton pour modifier une cible pond�r�e (par d�faut)
	CButtonST	m_Btn_ModifCible;

	// Titre au dessus du tableau des pond�rations
	CLabel		m_Titre_Ponderation;

	// La grille permettant la saisie des pond�rations
	CGridPonderation m_GridPonder;
	
	// Le compl�ment
	CLabel		m_Titre_Complement;

	// Valeur % r�el du compl�ment via les composants
	CLabel		m_Prct_Compl_Reel;

	// Valeur % pond�r� du compl�ment via les composants
	CLabel		m_Prct_Compl_Pond;
	
	// Bouton pour valider la cible pond�ration en cours
	CButtonST   m_Btn_ValidCiblePond;

	// Bouton fermeture du dialogue cible pond�r�e
	CButtonST   m_Btn_FermerCiblePond;

	// Bouton suppression de la cible pond�r�e s�lectionn�e
	CButtonST	m_Btn_SuppCiblePond;

	// Les 2 types de fonctionalit�s dans dialog cibles pond�r�es
	enum ModeCiblePond {MODE_CREATION = 0, MODE_MODIFICATION};

public:
	// Update valeur d'une cellule envoy�e par le grille pond�ration
	void UpdateValue(ROWCOL nRow, ROWCOL nCol);

private:
	// Initialisation des infos boite de dialogue
	void Init();

	// Chargement des items disponibles 
	JBool LoadItems();

	// Charge les derni�res cibles pond�r�es
	JBool LoadCiblesPonds();

	/// Get the Items construits
	CItemsConstruitsBase * GetItems() { return CItemsConstruitsManager::GetInstance()->GetItemsInstance(); }

protected:

	// Initialisation fen�tre cible pond�r�e
	virtual BOOL OnInitDialog();

	// Gestion des controles
	void SetupControls();

	// Couleurs des contr�les
	void SetColors();

	// Passage mode cr�ation ou modification
	void UpdateMode();

	// Validit� de la s�lection composants
	JBool SelItemsComposOK();

	// R�cup�re vecteur de cible
	void GetVectItem(CItemConstruit * pItem, JInt32 IdTerrain, CCalcItemParams & Params);

	// Sauvegarde de l'item pond�r�
	JBool SaveItemPondere();

	// Modification d'une cible pond�r�e existante
	JBool ModifItemPondere();

	// Suppression d'une cible pond�r�e existante
	JBool SupprimeItemPondere();

	// Activation de la cible pond�r�e s�lectionn�e
	JVoid ActiveCiblePond();

	// D�s�lection de tous les composants
	JVoid DeselectAllCompos();

	// Ecriture d'une ligne du grid
	void UpdateGridLigne(JUnt32 IndexLigne, JLabel Libelle, JFlt64 CoeffReel, JFlt64 CoeffPond);

	// Update des composants dans la grille via le map courant des items composants
	void UpdateGrilleComposants(const JMap <JInt32, JFlt64> & MapCoeff);

	// Update de la ligne compl�ment
	void UpdateInfoComplement(JFlt64 CumulCoeff, JFlt64 CumulPonder);

	// Total pond�ration en cours avec la ligne modifi�e en cours
	JFlt64 GetTotPonderation(const ROWCOL nRow, const JFlt64 NewValue);

	// Calcul coeff reel item composant via l'item de base
	JFlt64 CalcCoeffReel(JUnt32 IdItemBase, JUnt32 IdCompo);

	// V�rification de la coh�rence de la configuration des composants pond�r�s
	JBool CoeffCompoValide();

private:
	// Le document
	JATPDocCreuset *	m_pDoc;

	// Liste des terrains
	JList<CTerrain*> m_LstTerrains;

	// Item de base s�lectionn�
	JInt32	m_ItemBase;

	// Ident cible pond�r� s�lectionn�
	JInt32  m_ItemPondere;

	// Map des composants et de leurs coefficients
	JMap <JInt32, CInfoCoeff>	m_MapNoLigCompoCoeff;

	// Mode fonctionnel en cours
	JUnt32 m_ModeCiblePond; 

	// Terrain par d�faut (le 1er de la liste des s�lectionn�s)
	JUnt32 m_IdTerrainDef; 

private:
	//////////////////////// Param�trage de la grille pond�ration ////////////
	//
	// Les 2 �res colonnes fixes
	static const int Const_NbColFixe = 2;

	// 1 seule d�cimale pour les %
	static const int Const_NbDecimale = 1;

private:
	//////////////////////////////////////////////////////////////////////////
	//						Les op�rations de calcul
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
