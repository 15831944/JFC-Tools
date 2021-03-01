#pragma once

#include <Resizer.h>
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"
#include "JATPDocChoixSupport.h"
#include "JLib.h"
#include "CumGridWnd.h"
#include "afxcmn.h"
#include "ATPGraphCrbCum.h"
#include "TextProgressCtrl.h"
#include "JFCCourbe.h"
#include "JFCHorzSplit.h"

// Boîte de dialogue CDlgCumulativite
class CDlgCumulativite : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgCumulativite)

public:
    // constructeur standard
	CDlgCumulativite(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL);

	// Destructeur
	virtual ~CDlgCumulativite();

	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_CUMULATIVITE };

	// Définition des couleurs interface
	void SetColors();

	// Mise à jour sélections supports
	bool UpdateSelSupports();

	// Mise à jour sélections format supports
	bool UpdateSelFormatSupports();

	// Update grille résultat cumulativité via new sélection
	bool UpdateResultCum(JBool CalculResult);

	// Update des résultats au niveau grille cellule (ne touche pas aux headers)
	bool UpdateResultat(JBool CalculResult);

	// Récupére indice couleur pour courbe cumulativité
	JUnt32 GetIdColorCourbe(JUnt32 NoRow, JUnt32 NoCol);

	// Récupère le nombre de courbe cumulation en cours
	JUnt32 GetNbCourbeCum();

	// Nb lignes ou colonnes pour couleur courbe cum
	JUnt32 GetNbRowColorCum();
	JUnt32 GetNbColColorCum();
	
	// Renvoi sélection Nb Insertions
	JUnt32 GetNbInsertions();

	// Clipboard Cumulativite
	bool Clipboard();

	// Clipboard Graphique Cumulativite
	bool ClipboardGraphique();

	// Print Cumulativite
	bool PrintExcel();

	// Open (Excel) Cumulativite
	bool OpenExcel();

	// Récupération sélection Nb Insertions
	void SetNbInsertions(JUnt32 NbInsertions);

private:
	// SaveAsExcel Cumulativite
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
	JBool InitGridResultat_OPTIM();
	JBool InitGridResultat_OPTIM_OLD();

	// Vérifie si tous les calculs effectuées
	JBool AuMoins1ResultatNonCalcule(CKeyResultCum KeyResultCum, JUnt32 MaxInsSupport);

	// Affichage ou pas certains supports
	JVoid UpdateAfficheSupports();

	// Affichage résultat cumulativité
	JBool AfficheResultat();

	// Mise à jour des formats supports dans cumulativité
	void UpdateFormatSupports();

	// Maximum insertions sur les supports traités
	JUnt32 MaxInsertionsAllSupports();

	// Mise à jour du tri courant
	JVoid MajTriCourant();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	afx_msg LRESULT OnSplitHorz(WPARAM WParam, LPARAM LPARAM);
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

	// Barre de progression pour les calculs résultats cumulativité
	CTextProgressCtrl m_ProgressResultCum;

	// Cadre englobant les objets cumulativité
	CLabel m_CadreCumulativite;

	// Cadre du graphe cumulativité
	CLabel m_CadreGrapheCum;

	// Titre pour informations analyse cumulativité
	CLabel m_TitreCumulativite;

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

	// Update couleur courante pour graphe cumulativité
	void UpdateVisuColorGraphe();

	// Update couleur graphe associé aux supports
	void UpdateColorGraphSupports();

	// Couleurs spéciales de certaines cellules
	void UpdateColorSpe();

	// Nombre de couleur courbe en ligne
	JUnt32 m_NbLigColor;

	// Nombre de couleur courbe en colonne
	JUnt32 m_NbColColor;

	// Graphe cumulativité
	JFCCourbe		m_GraphCum;

	// Interface graphe cumulativité
	CATPGraphCrbCum m_IGraphCourbeCum;

	// Scroll horizontal du graphe cumulativité
	JFCScroll		m_ScrollGraph;


public:

	// Position colonne invisible (Id Support)
	JUnt32 m_NoColIdSupport;

	// Position des colonnes prédéfinis (Sél support, Libellé support, Libellé périodicité)
	JUnt32 m_NoColSelSupport;
	JUnt32 m_NoColSupport;
	// JUnt32 m_NoColFormat;
	JUnt32 m_NoColColorGraphe;
	JUnt32 m_NoColPeriodicite;

	// Affiche barre de calcul
	void AffichBarreCalcul();

	// Export des résultats Cumulation d'audience
	void ExportResultat();

	// Récupération de l'entête pour export cumulativité
	void RecupEnteteExport(CString &Entete);

	// Récupération résultat cumulativité détaillée
	void RecupGrilleExport(CString &Entete);

	// Récupération bloc des infos terrains / cibles
	void RecupBlocTerrainCible(CString &Entete);

	// Récupération des infos groupe
	void RecupContenuGroupe(CString &Entete);

	// Update visualisation supports
	void UpdateVisuSupports();

	// Export du graphique cumulativité
	void ExportGraphique();

	// Export des items
	BOOL ExportItems(long VSize, const TCHAR* pFilename); 

protected:
	// Membre split horizontal
	JFCHorzSplit  m_SplitHorz;

	// Grille résultat cumulativité
	CCumGridWnd m_GridCumulativite;

	// Maximum de colonnes insertions (selon périodicité supports sélmectionnés)
	JUnt32		m_MaxColIns;

	/////////////////////// UPDATE SPECIFIQUE FENETRE CUMULATIVITE ///////////////////
	// Update Entete Analyse (Nom de la source / terrain et date tarification)
	void UpdateTitreAnalyse();

	// Update Résultat des Coûts (suite à modif Format ou Tarif)
	void UpdateResultCout();

	// Résultat cellule selon grandeur analyse demandée
	/*
	CString ResultCellule(JUnt32 NoLig, JUnt32 NoCol, JFlt64 Tarif, JFlt64 VolLecture, JFlt64 Audience, JFlt64 AudienceProb1Ins, 
													  JFlt64 Audience1Ins, JFlt64 Effectif, JFlt64 NbCas, JInt32 NbInsertions);
	*/
	CString ResultCellule(JUnt32 NoLig, JUnt32 NoCol, JFlt64 Tarif, JInt32 NbInsertions, CResultCum &ResultCum);

public :
	// Récupére nombre de lignes header
	JUnt32 GetNbHeaderRow();

	// Récupére nombre de colonnes header
	JUnt32 GetNbHeaderCol();

	// Update du graphe
	void UpdateGraphe();

private  :
	// Limite maximum insertions selon périodicité
	JUnt32 MaxInsGeneral;
	JUnt32 MaxInsMensuel;
	JUnt32 MaxInsBiMensuel;
	JUnt32 MaxInsHebdo;
	JUnt32 MaxInsBiHebdo;
	JUnt32 MaxInsQuotidien;

	// Gestion arrêt des calcums
	JBool m_StopCalcul;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
