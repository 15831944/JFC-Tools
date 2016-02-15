// NomenclatureView.h : interface of the CNomenclatureView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOMENCLATUREVIEW_H__9271C150_507B_4242_B3A0_FE5D7197C0B3__INCLUDED_)
#define AFX_NOMENCLATUREVIEW_H__9271C150_507B_4242_B3A0_FE5D7197C0B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Formulaire.h"
#include "Noeud.h"
#include "Printer.h"	// Added by ClassView


class CNomenclatureView : public CFormView
{
protected: // create from serialization only
	CNomenclatureView();
	DECLARE_DYNCREATE(CNomenclatureView)

public:
	//{{AFX_DATA(CNomenclatureView)
	enum { IDD = IDD_NOMENCLATURE_FORM };
	CStatic	m_Trait;
	CStatic	m_TitreGestNomencl;
	CObj_Gray	m_Cadre1;
	CTreeCtrlX	m_TreeNomenclature;
	//}}AFX_DATA

// Attributes
public:
	CNomenclatureDoc* GetDocument();

	int m_InxMenuCur;
	CStringArray m_TypeElmt;
	HTREEITEM m_BufHItemCopie;
	HTREEITEM m_Marque ;
	HTREEITEM m_Produit;
	CStringArray m_TMenu;
	TV_ITEM m_ItemCur;

	// CNomenclDlg(CWnd* pParent = NULL);	// standard constructor
	
	enum {Popup_Ajouter = 0,Popup_AjouterAnnSsGrp,Popup_AjouterAnnSsGrpSsProd,Popup_Renommer,Popup_Supprimer,Popup_Sep1,Popup_Couper,Popup_Coller,Popup_Info,Popup_Sep2,Popup_TauxFO};
	CFont m_FontTime8Bold;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNomenclatureView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	HTREEITEM m_Secteur;
	CStringArray m_TabTxtNomencl;
	CString m_TitreNomenclature;
	CPrinter *m_Print;
	HTREEITEM m_HItemCur;
	virtual ~CNomenclatureView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public: 
	bool m_AnnonceurIsole;

	// Gestion utilisateur Taux
	CString m_MPUtilTauxCur;

	CString m_DateCur;
	CStringArray m_TabAnneeNomencl;
	// Chargement des marques existantes
	bool ChargeTabMarque(CStringArray &TabMarque);
	bool ChargeTabFamille(CStringArray &TabFamille);
	bool ChargeTabSecteur(CStringArray &TabSecteur,CString Famille);

protected:

	// Instance des images associées à l'arbre
	CMoveManager m_MM;
	CImageList m_ImageList;
	CNoeud 	m_ArbreEnMemoire;

	// A replacer dans un composant
	bool CopyDirectory(CString DirSrc,CString DirDst);
	bool DeleteDirectory(CString DirSup);

	// Creer et charger un nouvel arbre nomenclature
	bool CreerEtChargerNouvelArbreNomencl(CString AnneeNomenclature);

	// Creer nouvel arbre nomenclature
	bool CreerNouvelArbreNomencl(CString AnneeNomenclature);

	// Test si nomenclature vierge
	bool NomenclatureVide(CString AnneeNomenclature);

	// Duplication Nomencalture Annee courante sur autre année
	bool DupliqueAnneeNomenclature(CString AnneeSrc,CString AnneeDst);

	// Chargement des années nomenclatures existantes
	bool ChargeTabAnneeNomencl(bool ViaFichierAnnee);

	// Chargement des années possibles à créer (via fichier Année txt)
	bool ChargeTabAnneePossible(CStringArray &TabAnneePossible);
	void AjouteAnneeNomenclature(CString AnneeNomenclature);

	// Chargement des années existantes
	void ChargeTabAnneeExistante(CStringArray &TabAnneePossible);

	// Création du menu popup des années disponibles
	void CreerMenuPopupAnnee();

	// Copie d'une nomenclature année vers une nomenclature année nvlle ou existante
	bool CopierAnneeNomencl(CString AnneeSrc,CString AnneeDst);

	// Chargement de l'arbre nomenclature Marque/Produit
	bool ChargeFicProduitMarqueSecteur(CString AnneeNomenclature); 
	bool ChargeFicMarque(CNoeud *Arbre); 
	bool ChargeFicSecteur(CNoeud *Arbre); 
	bool ChargeFicProduit(CNoeud *Arbre,CNoeud *m_ArbreMarque); 

	// Libération des noeuds er arbre nomenclature en cours
	bool DegageNomenclatureCourante(); 

	// Fonction Test pour les évenements sur les élmts ou branche
	bool CopieItemPossible(HTREEITEM &HItemCopie,HTREEITEM &HItemParent,bool AvecMessage);

	// Synchronisation Arborescence et fichiers Arb
	bool SynchroArbre_FicArb();
	bool AjoutRepNvlleBranche(CString RepACreer);
	bool RenommerRepBranche(CString NvRepBranche,CString ExRepBranche);
	bool SuppressionFichier(HTREEITEM HItem,CString ExtensionFic);

	// Suppression des répertoires et ss-répertoire d'un noeud
	bool SuppressionRepertoire(CNoeud *pNoeud);

	bool CopieRepItemPossible(HTREEITEM HItemCopie,HTREEITEM HItemParent);
	bool CopieFicItemPossible(HTREEITEM HItemCopie,HTREEITEM HItemParent,CString FicCopie);
	bool SsRepExist(CNoeud *pNoeudParent,CString NomSsRep);
	void MajPathNoeudNiveauInf(CNoeud *pNoeud);

	// Creation du tableau texte ligne associé à la nomenclature (pour module impression)
	void CreerTabTxtNomencl();
	void MajTabTxtNomencl(HTREEITEM hItem,BOOL AvecCampagne);

	// Tri d'un tableau de String Value (Mode = 1 Décroissant, Mode = 2 Croissant)
	void TriTableauStringValue(CStringArray &TabArray,int Mode);

	// Verif au niveau famille et secteur sur les autres nomenclatures annuelles	
	bool FamilleAuMoins1ProduitAutresAnnees(CString TxtFamille);
	bool SecteurAuMoins1ProduitAutresAnnees(CString TxtFamille,CString TxtItem);

	// Gestion des utilisateurs taux fin d'ordre
	bool UtilisateurTauxOk();
	bool UtilisateurDejaPresent(CString Nom,CString &ExMotDePasse,int &NoUtilisateur);

	// Routine partucilère récupération données Nomenclature et mise au format EDI
	bool NomenclatureFormatEDI(CString &FicAnnonceur);

protected:
	//{{AFX_MSG(CNomenclatureView)
	afx_msg void OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenunomenclAjouternvllebranche();
	afx_msg void OnMenunomenclCollerbranche();
	afx_msg void OnMenunomenclCouperbranche();
	afx_msg void OnMenunomenclInfoproduit();
	afx_msg void OnMenunomenclRenommerbranche();
	afx_msg void OnMenunomenclSupprimerbranche();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrintbis();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenunomenclAjouterannonceursansgroupe();
	afx_msg void OnMenunomenclAjouterannonceursansgroupesansproduit();
	afx_msg void OnMenunomenclTaux();
	afx_msg void OnUtiltaux();
	afx_msg void OnUpdateUtiltaux(CCmdUI* pCmdUI);
	afx_msg void OnCentralesdachat();
	afx_msg void OnRegies();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in NomenclatureView.cpp
inline CNomenclatureDoc* CNomenclatureView::GetDocument()
   { return (CNomenclatureDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOMENCLATUREVIEW_H__9271C150_507B_4242_B3A0_FE5D7197C0B3__INCLUDED_)
