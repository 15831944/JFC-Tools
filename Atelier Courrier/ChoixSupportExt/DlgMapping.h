#pragma once

#include <Resizer.h>
#include <BtnST.h>
#include "afxwin.h"
#include "JATPDocChoixSupport.h"
#include "afxcmn.h"
#include "label.h"
#include "BagMining.h"
#include "BagCurve.h"
#include "BagHierarchy.h"
#include "ListPoint.h"
#include "JFCMapping.h"
#include "MappingGridWnd.h"
#include "TextProgressCtrl.h"
#include "JFCHorzSplit.h"


// Boîte de dialogue CDlgMapping

class CDlgMapping : public JFCDialog
{
	friend class CLisPoint;
	DECLARE_DYNAMIC(CDlgMapping)

public:
	// constructeur standard
	CDlgMapping(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL);

	// destructeur
	virtual ~CDlgMapping();

	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_MAPPING };

	// Définition des couleurs interface
	void SetColors();

	// Récupération sélection Nb Insertions
	void SetNbInsertions(JUnt32 NbInsertions);

	// Renvoi sélection Nb Insertions
	JUnt32  GetNbInsertions();

	// Mise à jour sélections supports
	bool UpdateSelSupports();

	// Mise à jour des formats supports non définis
	JVoid MajFormat();


	// Mise à jour sélections format supports
	bool UpdateSelFormatSupports();

	// Update résultat mapping
	bool UpdateResultMapping(JBool CalculResult);

	// Update des résultats au niveau grille cellule (ne touche pas aux headers)
	bool UpdateResultat(JBool CalculResult);

	// Mise à jour du document Mapping
	void MajMapping(JBool InitPos);

	// la fonction pour mettre à jour la sélection
	void UpdateSelection();

	// Clipboard Resultat Mapping
	bool ClipboardResultat();

	// Print Resultat Mapping
	bool PrintExcel();

	// Open Resultat Mapping
	bool OpenExcel();

	// Clipboard Graphique Mapping
	void ClipboardGraphique();

	// Clipboard du bitmap mapping
	void ClipboardBitmap();

	// Récupèration du bitmap mapping
	CBitmap * GetBitmapMapping();

private:
	
	// SaveAsExcel Resultat Mapping
	bool SaveAsExcel(CString filePath);

	// Set up des différents contrôles
	void SetupControls();

	// Mise en place des contrôles
	void SetWindowRelations();

	// Initialisation des références colonnes
	void InitRefGrid();

	// Initialisation du grid
	void InitGrid();

	// Initialisation du résultat grid
	JBool InitGridResultat();

	// Affichage résultat mapping
	JBool AfficheResultat();

	// Affichage Moyenne des supports (visibles bien sur)
	JVoid AfficheResultFamille();

	// Mise à jour des formats supports dans mapping
	void UpdateFormatSupports();

public:

	// Affiche barre de calcul
	void AffichBarreCalcul();

	// Export des résultats Mapping
	void ExportResultat();

	// Export du graphique Mapping
	void ExportGraphique();

	// Récupération de l'entête pour export duplication
	void RecupEnteteExport(CString &Entete);

	// Récupération résultat cumulativité détaillée
	void RecupGrilleExport(CString &Entete);

	// Récupération bloc des infos terrains / cibles
	void RecupBlocTerrainCible(CString &Entete);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	afx_msg LRESULT OnSplitHorz(WPARAM WParam, LPARAM LPARAM);
	DECLARE_MESSAGE_MAP()

public:
	// Position colonne invisible (Id Support)
	JUnt32 m_NoColIdSupport;

	// Position des colonnes visibles prédéfinis (Sél support, Lib Support, Tarif, Lib Périodicité)
	JUnt32 m_NoColSelSupport;
	JUnt32 m_NoColSupport;
	JUnt32 m_NoColFormat;
	JUnt32 m_NoColTarif;
	JUnt32 m_NoColPeriodicite;

	// Récupére nombre de lignes header
	JUnt32 GetNbHeaderRow();

	// Récupére nombre de colonnes header
	JUnt32 GetNbHeaderCol();

	// Récupère n° colonne abscisse mapping
	JUnt32 RecupNoColAbscisseMap();

	// Récupère n° colonne ordonnée mapping	
	JUnt32 RecupNoColOrdonneeMap();

	// Récupération des résultats mode valeur
	/*
	double RecupResult(JUnt32 IdSupport, JUnt32 NoCol,  JFlt64 Tarif,  JFlt64 VolLecture, JFlt64 AudienceLDP, JFlt64 AudienceProba, JFlt64 Audience, JFlt64 Audience1Ins, 
													    JFlt64 Effectif,   JFlt64 NbCas,
													    JFlt64 VolLectAff, JFlt64 AudAffLDP, JFlt64 AudAffProba, JFlt64 AudAff,     JFlt64 EffAff);
	*/
	double RecupResult(JUnt32 IdSupport, JUnt32 NoCol,  JFlt64 Tarif,  CResultMap &ResultMap);

	// Récupération des résultats mode chaine formattée
	/*
	CString RecupResultTxt(JUnt32 IdSupport, JUnt32 NoCol,  JFlt64 Tarif,  JFlt64 VolLecture, JFlt64 AudienceLDP, JFlt64 AudienceProba, JFlt64 Audience, JFlt64 Audience1Ins, 
														    JFlt64 Effectif,   JFlt64 NbCas,
														    JFlt64 VolLectAff, JFlt64 AudAffLDP, JFlt64 AudAffProba, JFlt64 AudAff,     JFlt64 EffAff);
	*/
	CString RecupResultTxt(JUnt32 IdSupport, JUnt32 NoCol,  JFlt64 Tarif, CResultMap &ResultMap);

	// Récupère résultat formaté selon type et nb dec data exo
	CString GetResultDataExo(JUnt32 IdGrandeur, JFlt64 ResultCell);

	// Récupération texte header row
	CString RecupTexteHeaderRow(JUnt32 Lig, JUnt32 Col);

	// Update visualisation supports
	void UpdateVisuSupports();

	// Récupération Id Grandeur (Abscisse ou Ordonnée)
	JUnt32 RecupIdGrandeur(JUnt32 NoCol);

private:
	// Pointeur document choix supports
	JATPDocChoixSupport *pDocChoixSupport;

	// Objet pour le resize des objets
	CResizer m_Resizer;

	// Titre pour informations analyse Mapping
	CLabel m_TitreMapping;

	// Cadre englobant le mapping
	CLabel m_CadreMapping;

	// Objet Mapping
	JFCMapping m_Mapping;

	// Barre de progression résultat Mapping
	CTextProgressCtrl m_ProgressResultMap;

	// Cadre du grid relatif au mapping
	CLabel m_CadreGridMapping;

	// Nombre de lignes header 
	JUnt32 m_NbHeaderRow;

	// Nombre de lignes supports
	JUnt32 m_NbRowSupport;

	// Nombre de colonnes header 
	JUnt32 m_NbHeaderCol;

	// Nombre de colonnes
	JUnt32 m_NbCol;

	// Mise à jour colonnes grid avec nouvelle sélection
	bool UpdateCol();

	// Mise à jour lignes grid avec nouvelle sélection
	bool UpdateRow();

	// Init lignes supports
	void InitRowSupports();

	// Init size colonnes
	void InitSizeColonne();

	// Mise à jour sélection supports
	void UpdateEtatSelSupports();

	// Les diférentes couleurs pour headers ou cellules spécials
	void UpdateColorSpe();

	
protected:
	// Grid relatif au mapping
	CMappingGridWnd m_GridMapping;

	// Définit si la grille résultat est valide
	bool GrilleResultValid();

	// Les supports du mapping ont-ils été modifiés
	JBool SupportModif();

public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

// Implementation
protected:
	CBagMining	  m_BagMining;			// l'ensemble des éléments
	CBagCurve	  m_BagCurve;			// l'ensemble des courbes
	CBagHierarchy m_BagHierarchy;		// l'ensemble des hierarchie
	// CListPoint m_Liste;				// l'ensemble des éléments

	// Membre split horizontal
	JFCHorzSplit  m_SplitHorz;

protected:
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

	// Calcul des résultats selon grandeur
	CString ResultCellule(JUnt32 IdSupport, JUnt32 NoLig, JUnt32 NoCol, JFlt64 Tarif,
						  CResultMap &ResultMapping);
	
	// Update Entete Analyse (Nom de la source / terrain et date tarification)
	void UpdateTitreAnalyse();

	// Update Résultat des Coûts (suite à modif Format ou Tarif)
	JVoid UpdateResultCout();

	// Affichage ou pas certains supports
	JVoid UpdateAfficheSupports();

	// Visibilité des élmts support ou famille
	JBool VisibleSupportMode(JUnt32 IdSupport);

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	void PrintBmp(CBitmap *pBmp);
	CBitmap * CopyWindowToBitmap(JFControl* pWnd ,bool bWindowClient);
	CBitmap *CopyScreenToBitmap(LPRECT lpRect);
	bool CopyBitmapToClipBoard(CBitmap *pBitmap); 

};
