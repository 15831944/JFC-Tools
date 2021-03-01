#pragma once

#include "afxcmn.h"

#include <Resizer.h>
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"

#include "DlgDuplication.h"
#include "JATPDocChoixSupport.h"


// Boîte de dialogue CDlgPupitreDup
class CDlgPupitreDup : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgPupitreDup)

public:
	// constructeur standard
	CDlgPupitreDup(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL,CDlgDuplication *pDlg = NULL);   

	// Destructeur
	virtual ~CDlgPupitreDup();


	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_PUPITRE_DUP };

	// Définition des couleurs interface
	void SetColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	DECLARE_MESSAGE_MAP()

protected:
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
	CDlgDuplication *pDlgDup;

	// Pointeur sur sélection choix support
	JATPDocChoixSupport *pDocChoixSupport;

	// Cadre englobant les objets du pupitre duplication
	CLabel m_CadrePupitreDup;

	// Titre sélection grandeurs duplication
	CLabel m_LblGrandDup;

	// Liste des grandeurs disponibles pour calcul duplication
	JFCSelListBox m_LstGrandDup;

	// Nombre de grandeur de l'offre
	JUnt32 m_NbGrandeurs;

	// Mise à jour des sélections grandeurs
	void UpdateSelGrandeurs();

public:

	// Remplissage liste grandeurs
	bool LoadGrandeurs();

	// Initialisation de la liste
	void InitLstGrandeurs();


public:
	afx_msg void OnLbnSelchangeCsListGranddup();
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
};
