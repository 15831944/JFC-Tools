#pragma once

#include "afxcmn.h"

#include <Resizer.h>
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"

#include "DlgEchelle.h"
#include "JATPDocChoixSupport.h"
#include "JFCSelListBox.h"
#include "JFCTitle.h"

// Boîte de dialogue CDlgPupitreEch
class CDlgPupitreEch : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgPupitreEch)

public:
	// constructeur standard
	CDlgPupitreEch(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL,CDlgEchelle *pDlg = NULL);   

	// destructeur
	virtual ~CDlgPupitreEch();

	virtual BOOL OnInitDialog();

	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_PUPITRE_ECH };

	// Définition des couleurs interface
	void SetColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	DECLARE_MESSAGE_MAP()

protected:
	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

public :
	// Liste des grandeurs calcul à sélectionner (public because Reprise en main ou Repet)
	// JFCListBox m_LstGrandeurs;
	JFCSelListBox m_LstGrandeurs;

private:
	// Set up des différents contrôles
	void SetupControls();

	// Mise en place des contrôles
	void SetWindowRelations();

	// Cadre engloabnt les contôles spécifiques pupitre échelle
	CLabel m_CadrePupitreEch;

	// Label Liste des grandeurs à sélectionner
	// CLabel m_LblGrandeurs;
	JFCTitle m_LblGrandeurs;

	// Nombre de grandeur de l'offre
	JUnt32 m_NbGrandeurs;

	// Objet pour le resize des objets
	CResizer m_Resizer;

	// Pointeur sur fenêtre dialogue
	CDlgEchelle *pDlgEchelle;

	// Pointeur sur sélection choix support
	JATPDocChoixSupport *pDocChoixSupport;

	// Mise à jour des sélections grandeurs
	void UpdateSelGrandeurs();

	// Mise à jour ordre liste des grandeurs dans panneau échelle
	void AfficheListOrdoGrandeurs();

public:

	// Remplissage liste grandeurs (y compris grandeurs exogènes)
	bool LoadGrandeurs();

	// Initialisation de la liste
	void InitLstGrandeurs();

	// Mise à jour de la liste
	void MajGrandeurs();

public:
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeCsListGrandeurs();
};
