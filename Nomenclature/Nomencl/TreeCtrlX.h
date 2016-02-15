#if !defined(AFX_TREECTRLX_H__0ABB415C_1933_4F11_A86D_82C86008DD2F__INCLUDED_)
#define AFX_TREECTRLX_H__0ABB415C_1933_4F11_A86D_82C86008DD2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeCtrlX.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlX window

class CTreeCtrlX : public CTreeCtrl
{
// Construction
public:
	CTreeCtrlX();

// Attributes
public:
	CImageList ImageNormal;
	enum EnumImage{NoImage=0,ImageOpen,ImageClose,ImageMarque,ImageProduit,ImageAnnonceur,ImageGroupe,ImageCampagne,ImageFamille,ImageSecteur};


// Operations
public:


	// Chargement des images
	bool LoadBitmapNormal(UINT NormalId,int SizeInPixel);

	// Presentation des differentes branches de l'arbre
	bool PresenteArbre(CNoeud *ArbreProduitMem,CNoeud *Arbre,HTREEITEM &m_Tree,CString &NomBranche1,CString &NomBranche2,CString &NomBranche3,HTREEITEM &m_TreeMarque);
	void TrierArbre();
	void TrierBranche(HTREEITEM hBranch);

	// Operations sur les branches
	HTREEITEM CopierBranche(HTREEITEM hBranch,HTREEITEM hNvParent,HTREEITEM hApres=TVI_LAST);
	HTREEITEM NewCopierBranche(HTREEITEM hBranch,HTREEITEM hNvParent,HTREEITEM hApres=TVI_LAST);

	HTREEITEM DeplacerBranche(HTREEITEM hBranchSrc,HTREEITEM hNvParent);
	HTREEITEM AjouterBranche(HTREEITEM hBrancheRacine,CString Alias,CString Libelle,CString Code,CString Marque,CString Famille,CString Secteur,CString DateDeb,bool AnnonceurIsole);

	// Ajout Annonceur (classique)
	HTREEITEM AjouterBrancheAnnonceur(HTREEITEM hBrancheRacine,CString Alias,CString Libelle,CString Code,
		                              CString Marque,CString Famille,CString Secteur,CString DateDeb,
							          CString m_TypeTiers,CString m_CodeTiers,CString m_TypeCodeTiers,
							          CString m_NomTiers,CString m_AdresseTiers1,CString m_AdresseTiers2,
							          CString m_CodePostalTiers,CString m_VilleTiers,CString m_CodePaysTiers,
							          CString m_NoTelTiers,CString m_NoFaxTiers,
						  	          CString m_NomContactTiers,CString m_PrenomContactTiers,CString m_AdrMailContactTiers,
	 						          bool AnnonceurIsole);

	// Ajout specif annonceur direct / ne suit pas l'arborecence groupe/annonceur/produit
	HTREEITEM AjouterBrancheAnnonceurDirect(HTREEITEM hBrancheRacine,CString Alias,CString Libelle,CString Code,CString Marque,CString Famille,CString Secteur,CString DateDeb);

	// Ajout annonceur sans branche mère groupe
	HTREEITEM AjouterBrancheAnnonceurSsGroupe(HTREEITEM hBrancheRacine,CString Alias,CString Libelle,CString Code,CString DateDeb);

	bool SupprimerBranche(HTREEITEM hBranch,HTREEITEM hBranchOrg,bool VirerNoeudMem,bool VirerNvPathFichier,HTREEITEM hBranchMarque = NULL,HTREEITEM hBranchFamille = NULL);

	// Operations sur les elmts
	HTREEITEM CopierElmt(HTREEITEM hElmt, HTREEITEM hNvParent,HTREEITEM hApres=TVI_LAST);
	HTREEITEM DeplacerElmt(HTREEITEM hElmtSrc,HTREEITEM hNvParent,HTREEITEM hApres);
	bool SupprimerElmt(HTREEITEM hBranch);


	// Operations sur les produits et campagne
	void MajMarqueProduit(HTREEITEM HItemProduit,CString ExMarque,CString NvMarque);
	void RenommerProduitMarque(HTREEITEM hBrancheMarque,CString Marque,CString ExNomProduit ,CString NvNomProduit);
	void VirerProduitDansMarque(HTREEITEM hBrancheMarque,CString AliasMarque,CString AliasProduit);
	void AjouterProduitDansMarque(HTREEITEM hBrancheMarque,CString AliasMarque,CString AliasProduit,CString LibelleProduit,CString CodeProduit);
	void DeplacerCampagneProduitDansMarque(HTREEITEM hBrancheMarque,CString MarqueExProduit,CString AliasExProduit,CString MarqueNvProduit,CString AliasNvProduit,CString NomCampagne);
	void InsertionCampagneProduitMarque(HTREEITEM hBrancheMarque,CString MarqueNvProduit,CString AliasNvProduit,CString NomCampagne);
	void SuppressionCampagneProduitMarque(HTREEITEM hBrancheMarque,CString MarqueExProduit,CString AliasExProduit,CString NomCampagne);

	// Operations sur Familles et Secteurs
	void MajFamilleProduit(HTREEITEM HItemProduit,CString ExFamille,CString NvFamille);
	void MajSecteurProduit(HTREEITEM HItemProduit,CString ExSecteur,CString NvSecteur);
	void RenommerProduitFamille(HTREEITEM hBrancheFamille,CString Famille,CString Secteur,CString ExNomProduit ,CString NvNomProduit);
	void AjouterProduitDansFamilleSecteur(HTREEITEM hBrancheFamille,CString Famille,CString Secteur,CString AliasProduit,CString LibelleProduit,CString CodeProduit);
	void VirerProduitDansFamilleSecteur(HTREEITEM hBrancheFamille,CString Famille,CString Secteur,CString AliasProduit);
	void DeplacerCampagneProduitDansSecteur(HTREEITEM HBranchFamille,CString FamilleExProduit,CString SecteurExProduit,CString AliasExProduit,
		                                                             CString FamilleNvProduit,CString SecteurNvProduit,CString AliasNvProduit,CString NomCampagne);
	void InsertionCampagneProduitFamilleSecteur(HTREEITEM hBrancheFamille,CString FamilleNvProduit,CString SecteurNvProduit,CString AliasNvProduit,CString NomCampagne);
	void SuppressionCampagneProduitFamilleSecteur(HTREEITEM hBrancheFamille,CString FamilleExProduit,CString SecteurExProduit,CString AliasExProduit,CString NomCampagne);

	// Pour les annonceurs directs
	void AjouterAnnonceurDansFamilleSecteur(HTREEITEM hBrancheFamille,CString Famille,CString Secteur,CString AliasAnnonceur,CString LibelleAnnonceur,CString CodeAnnonceur,CString DateAnnonceur);
	void RenommerAnnonceurFamille(HTREEITEM hBrancheFamille,CString Famille,CString Secteur,CString ExNomAnnonceur ,CString NvNomAnnonceur);
	void VirerAnnonceurDansFamilleSecteur(HTREEITEM hBrancheFamille,CString Famille,CString Secteur,CString AliasAnnonceur);
	void SuppressionCampagneAnnonceurFamilleSecteur(HTREEITEM hBrancheFamille,CString FamilleExAnnonceur,CString SecteurExAnnonceur,CString AliasExAnnonceur,CString NomCampagne);
	void InsertionCampagneAnnonceurFamilleSecteur(HTREEITEM hBrancheFamille,CString FamilleNvAnnonceur,CString SecteurNvAnnonceur,CString AliasNvAnnonceur,CString NomCampagne);
	void DeplacerCampagneAnnonceurDansSecteur(HTREEITEM HBranchFamille,CString FamilleExAnnonceur,CString SecteurExAnnonceur,CString AliasExAnnonceur,
		                                                               CString FamilleNvAnnonceur,CString SecteurNvAnnonceur,CString AliasNvAnnonceur,CString NomCampagne);

	// Opérations sur répertoire ou fichier associés aux branches
	bool FichiersCampagnePresent(HTREEITEM hBranche); 
	bool SupprimerRepBranche(HTREEITEM hBranche,bool SuppNvPathNoeud);
	bool AjoutRepBranche(CString RepACreer);
	//bool DeplacerRepBranche(HTREEITEM hBranchSrc,HTREEITEM hBranchDst);

	bool ReconstituerRepBranche(HTREEITEM hBranche);
	void SupprimerLienNoeud(CNoeud *pNoeud);

	// Recherche ss-branche juste apres racine
	HTREEITEM TypeSsBranche(HTREEITEM hItem);

	// Récupération de la date d'exercice à tous les niveaux
	CString DateExerciceBranche(HTREEITEM hItem);


protected:
    virtual void OnItemCopied( HTREEITEM hElmt, HTREEITEM hNvElmt);


// Implementation
public:
	virtual ~CTreeCtrlX();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlX)
	afx_msg void OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREECTRLX_H__0ABB415C_1933_4F11_A86D_82C86008DD2F__INCLUDED_)
