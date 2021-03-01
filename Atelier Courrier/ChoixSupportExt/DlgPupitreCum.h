#pragma once

#include "afxcmn.h"

#include <Resizer.h>
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"

#include "DlgCumulativite.h"
#include "JATPDocChoixSupport.h"
#include "GridColorCum.h"

// Boîte de dialogue CDlgPupitreCum
class CDlgPupitreCum : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgPupitreCum)

public:
	// constructeur standard
	CDlgPupitreCum(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL,CDlgCumulativite *pDlg = NULL);   

	// Destructeur
	virtual ~CDlgPupitreCum();

	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_PUPITRE_CUM };

	// Définition des couleurs interface
	void SetColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	DECLARE_MESSAGE_MAP()

	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);
	
private:
	// Set up des différents contrôles
	void SetupControls();

	// Mise en place des contrôles
	void SetWindowRelations();

	// Objet pour le resize des objets
	CResizer m_Resizer;

	// Pointeur sur fenêtre dialogue
	CDlgCumulativite *pDlgCum;
	
	// Pointeur sur sélection choix support
	JATPDocChoixSupport *pDocChoixSupport;

	// Titre pour liste sélection des drandeurs analyse cumulativité
	CLabel m_LblGrandCum;

	// Liste des grandeurs d'analyse pour cumulativité
	// JFCSelListBox m_LstGrandCum;
	JFCListBox m_LstGrandCum;

	// Nombre de grandeur de l'offre
	JUnt32 m_NbGrandeurs;

	// Titre sélection des couleurs courbe cumulativité
	CLabel m_TitreCourbe;

	// Cadre sélection pupitre cumulativité
	CLabel m_CadrePupitreCum;
	
	// Bouton sélection mode calcul graphe couleurs séparées
	CButtonST m_BtnSeparColor;

	// Bouton sélection mode calcul graphe couleurs identiques groupées (cumul)
	CButtonST m_BtnGroupColor;

	// Mise à jour des sélections grandeurs
	void UpdateSelGrandeurs();

	// Initialisation grille sélection couleur
	void InitGridCouleur();

	// Mise à jour mode sélection couleur
	void UpdateModeSelColor();

	// Mise à jour du mode courbe
	void MajModeCourbe();

	// Activation ou désactivation bouton groupement courbe (seul GRP ou Nb Contact cumulable)
	void UpdateBtnGroup(JUnt32 IdGrandeurSel);

protected :
	// Grille couleur pour graphe cumulativité
	CGridColorCum m_GridColorCum;

public:

	// Remplissage liste grandeurs
	bool LoadGrandeurs();

	// Initialisation de la liste
	void InitLstGrandeurs();

	// Mise à jour de la liste
	void MajGrandeurs();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeCsListGrandcum();



	afx_msg void OnBnClickedCsLabelSepcolor();
	afx_msg void OnBnClickedCsLabelGroupcolor();
};
