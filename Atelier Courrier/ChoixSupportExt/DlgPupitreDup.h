#pragma once

#include "afxcmn.h"

#include <Resizer.h>
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"

#include "DlgDuplication.h"
#include "JATPDocChoixSupport.h"


// Bo�te de dialogue CDlgPupitreDup
class CDlgPupitreDup : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgPupitreDup)

public:
	// constructeur standard
	CDlgPupitreDup(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL,CDlgDuplication *pDlg = NULL);   

	// Destructeur
	virtual ~CDlgPupitreDup();


	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_CS_DLG_PUPITRE_DUP };

	// D�finition des couleurs interface
	void SetColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	DECLARE_MESSAGE_MAP()

protected:
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
	CDlgDuplication *pDlgDup;

	// Pointeur sur s�lection choix support
	JATPDocChoixSupport *pDocChoixSupport;

	// Cadre englobant les objets du pupitre duplication
	CLabel m_CadrePupitreDup;

	// Titre s�lection grandeurs duplication
	CLabel m_LblGrandDup;

	// Liste des grandeurs disponibles pour calcul duplication
	JFCSelListBox m_LstGrandDup;

	// Nombre de grandeur de l'offre
	JUnt32 m_NbGrandeurs;

	// Mise � jour des s�lections grandeurs
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
