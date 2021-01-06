#pragma once

#include <Resizer.h>
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"
#include "JATPDocChoixSupport.h"
#include "JLib.h"
#include "DupGridWnd.h"
#include "afxcmn.h"
#include "TextProgressCtrl.h"

// Boîte de dialogue CDlgDuplication
class CDlgDuplication : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgDuplication)

public:
    // Constructeur standard
	CDlgDuplication(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL);

	// Constructeur
	virtual ~CDlgDuplication();

	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_DUPLICATION };

	// Définition des couleurs interface
	void SetColors();

	// Mise à jour sélections supports
	bool UpdateSelSupports();

	// Mise à jour sélections format supports
	bool UpdateSelFormatSupports();

	// Update résultat duplication
	bool UpdateResultDup(JBool CalculResult);

	// Update des résultats au niveau grille cellule (ne touche pas aux headers)
	bool UpdateResultat(JBool CalculResult);

	// Clipboard Duplication
	bool Clipboard();

	// Print Duplication
	bool PrintExcel();

	// Open (Excel) Duplication
	bool OpenExcel();

private:

	// SaveAsExcel Duplication
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

	// Affichage ou pas certains supports
	JVoid UpdateAfficheSupports();

	// Affichage résultat duplication
	JBool AfficheResultat();

	// Mise à jour des formats supports dans duplication
	void UpdateFormatSupports();

	// Mise à jour du tri courant
	JVoid MajTriCourant();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

private:
	// Pointeur document choix supports
	JATPDocChoixSupport *pDocChoixSupport;

	// Objet pour le resize des objets
	CResizer m_Resizer;

	// Barre de progression calcul duplication
	CTextProgressCtrl m_ProgressResultDup;

	// Cadre englobant les objets duplication
	CLabel m_CadreDuplication;

	// Titre informations analyse duplication
	CLabel m_TitreDuplication;

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

	// Définit si la grille résultat est valide
	bool GrilleResultValid();

	// Mise à jour sélection supports
	void UpdateEtatSelSupports();

	// Couleurs spéciales de certaines cellules
	void UpdateColorSpe();

public :
	// Récupére nombre de lignes header
	JUnt32 GetNbHeaderRow();

	// Récupére nombre de colonnes header
	JUnt32 GetNbHeaderCol();

	// Affiche barre de calcul
	void AffichBarreCalcul();

	// Export des résultats Duplication
	void ExportResultat();

	// Récupération de l'entête pour export mapping
	void RecupEnteteExport(CString &Entete);

	// Récupération résultats duplication détaillée
	void RecupGrilleExport(CString &Entete);

	// Récupération bloc des infos terrains / cibles
	void RecupBlocTerrainCible(CString &Entete);

	// Récupération Id Grandeur
	JUnt32 RecupIdGrandeur(JUnt32 &NoCol, JUnt32 NivDim);

	// Update visualisation supports
	void UpdateVisuSupports();

	// Position colonne invisible (Id Support)
	JUnt32 m_NoColIdSupport;

	// Position des colonnes prédéfinis (Sél support, Libellé support, Format Support, Libellé périodicité)
	JUnt32 m_NoColSelSupport;
	JUnt32 m_NoColSupport;
	JUnt32 m_NoColFormat;
	JUnt32 m_NoColPeriodicite;

	// Calcul Centrage entre 2 titres en horizontal ou en vertical
	JFlt64 CalculCentrage(JFlt64 &AudienceDupliquee, JFlt64 &AudienceCible);

	// Calcul Coefficient de Yule
	JFlt64 CalculCoeffYule(JFlt64 &AudienceDupliquee, JFlt64 &AudienceCibleRow, JFlt64 &AudienceCibleCol, JFlt64 &EffectifGlobal);

protected:
	// Grille résultat duplication
	CDupGridWnd m_GridDuplication;

	// Update Entete Analyse (Nom de la source / terrain et date tarification)
	void UpdateTitreAnalyse();

	// Update Résultat des Coûts (suite à modif Format ou Tarif)
	JVoid UpdateResultCout();

	// Résultat cellule pour toutes les grandeurs duplication
	CString ResultCellule(JUnt32 NoLig, JUnt32 NoCol, CKeyResultDup KeyResultDup, CResultDup ResultDup);
};
