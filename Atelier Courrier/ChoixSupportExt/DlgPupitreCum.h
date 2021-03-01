#pragma once

#include "afxcmn.h"

#include <Resizer.h>
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"

#include "DlgCumulativite.h"
#include "JATPDocChoixSupport.h"
#include "GridColorCum.h"

// Bo�te de dialogue CDlgPupitreCum
class CDlgPupitreCum : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgPupitreCum)

public:
	// constructeur standard
	CDlgPupitreCum(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL,CDlgCumulativite *pDlg = NULL);   

	// Destructeur
	virtual ~CDlgPupitreCum();

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_CS_DLG_PUPITRE_CUM };

	// D�finition des couleurs interface
	void SetColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	DECLARE_MESSAGE_MAP()

	// la fonction pour mettre � jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);
	
private:
	// Set up des diff�rents contr�les
	void SetupControls();

	// Mise en place des contr�les
	void SetWindowRelations();

	// Objet pour le resize des objets
	CResizer m_Resizer;

	// Pointeur sur fen�tre dialogue
	CDlgCumulativite *pDlgCum;
	
	// Pointeur sur s�lection choix support
	JATPDocChoixSupport *pDocChoixSupport;

	// Titre pour liste s�lection des drandeurs analyse cumulativit�
	CLabel m_LblGrandCum;

	// Liste des grandeurs d'analyse pour cumulativit�
	// JFCSelListBox m_LstGrandCum;
	JFCListBox m_LstGrandCum;

	// Nombre de grandeur de l'offre
	JUnt32 m_NbGrandeurs;

	// Titre s�lection des couleurs courbe cumulativit�
	CLabel m_TitreCourbe;

	// Cadre s�lection pupitre cumulativit�
	CLabel m_CadrePupitreCum;
	
	// Bouton s�lection mode calcul graphe couleurs s�par�es
	CButtonST m_BtnSeparColor;

	// Bouton s�lection mode calcul graphe couleurs identiques group�es (cumul)
	CButtonST m_BtnGroupColor;

	// Mise � jour des s�lections grandeurs
	void UpdateSelGrandeurs();

	// Initialisation grille s�lection couleur
	void InitGridCouleur();

	// Mise � jour mode s�lection couleur
	void UpdateModeSelColor();

	// Mise � jour du mode courbe
	void MajModeCourbe();

	// Activation ou d�sactivation bouton groupement courbe (seul GRP ou Nb Contact cumulable)
	void UpdateBtnGroup(JUnt32 IdGrandeurSel);

protected :
	// Grille couleur pour graphe cumulativit�
	CGridColorCum m_GridColorCum;

public:

	// Remplissage liste grandeurs
	bool LoadGrandeurs();

	// Initialisation de la liste
	void InitLstGrandeurs();

	// Mise � jour de la liste
	void MajGrandeurs();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeCsListGrandcum();



	afx_msg void OnBnClickedCsLabelSepcolor();
	afx_msg void OnBnClickedCsLabelGroupcolor();
};
