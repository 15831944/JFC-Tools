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

// Bo�te de dialogue CDlgPupitreEch
class CDlgPupitreEch : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgPupitreEch)

public:
	// constructeur standard
	CDlgPupitreEch(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL,CDlgEchelle *pDlg = NULL);   

	// destructeur
	virtual ~CDlgPupitreEch();

	virtual BOOL OnInitDialog();

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_CS_DLG_PUPITRE_ECH };

	// D�finition des couleurs interface
	void SetColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	DECLARE_MESSAGE_MAP()

protected:
	// la fonction pour mettre � jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

public :
	// Liste des grandeurs calcul � s�lectionner (public because Reprise en main ou Repet)
	// JFCListBox m_LstGrandeurs;
	JFCSelListBox m_LstGrandeurs;

private:
	// Set up des diff�rents contr�les
	void SetupControls();

	// Mise en place des contr�les
	void SetWindowRelations();

	// Cadre engloabnt les cont�les sp�cifiques pupitre �chelle
	CLabel m_CadrePupitreEch;

	// Label Liste des grandeurs � s�lectionner
	// CLabel m_LblGrandeurs;
	JFCTitle m_LblGrandeurs;

	// Nombre de grandeur de l'offre
	JUnt32 m_NbGrandeurs;

	// Objet pour le resize des objets
	CResizer m_Resizer;

	// Pointeur sur fen�tre dialogue
	CDlgEchelle *pDlgEchelle;

	// Pointeur sur s�lection choix support
	JATPDocChoixSupport *pDocChoixSupport;

	// Mise � jour des s�lections grandeurs
	void UpdateSelGrandeurs();

	// Mise � jour ordre liste des grandeurs dans panneau �chelle
	void AfficheListOrdoGrandeurs();

public:

	// Remplissage liste grandeurs (y compris grandeurs exog�nes)
	bool LoadGrandeurs();

	// Initialisation de la liste
	void InitLstGrandeurs();

	// Mise � jour de la liste
	void MajGrandeurs();

public:
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeCsListGrandeurs();
};
