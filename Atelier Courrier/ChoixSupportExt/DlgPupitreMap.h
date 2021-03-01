#pragma once

#include "afxcmn.h"

#include <Resizer.h>
#include <Label.h>
#include <BtnST.h>
#include "afxwin.h"

#include "DlgMapping.h"
#include "JATPDocChoixSupport.h"


// Boîte de dialogue CDlgPupitreMap
class CDlgPupitreMap : public JFCDialog
{
	DECLARE_DYNAMIC(CDlgPupitreMap)

public:
	// constructeur standard
	CDlgPupitreMap(CWnd* pParent = NULL,JATPDocChoixSupport *pDoc = NULL,CDlgMapping *pDlg = NULL);   

	// Destructeur
	virtual ~CDlgPupitreMap();

	virtual BOOL OnInitDialog();

	// Données de boîte de dialogue
	enum { IDD = IDD_CS_DLG_PUPITRE_MAP };

	// Définition des couleurs interface
	void SetColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	DECLARE_MESSAGE_MAP()

public:
	// Listes des grandeurs disponibles en abscisse et ordonnée du mapping
	JFCListBox m_LstGrandAbs;
	JFCListBox m_LstGrandOrd;

private:
	// Set up des différents contrôles
	void SetupControls();

	// Mise en place des contrôles
	void SetWindowRelations();

	// Bouton choix familles
	CButtonST m_BtnMapFamilles;

	// Bouton sélection centres mapping
	CButtonST m_BtnMapCentres;

	// Bouton sélection supports mapping
	CButtonST m_BtnMapSupports;

	// Cadre du pupitre spécipifique Mapping
	CLabel m_CadrePupitreMap;

	// Labels abscisse et ordonnée
	CLabel m_LblAbscisse;
	CLabel m_LblOrdonnee;

	// Nombre de grandeur en abscisse
	JUnt32 m_NbGrandAbs;

	// Nombre de grandeur en ordonnée
	JUnt32 m_NbGrandOrd;

	// Objet pour le resize des objets
	CResizer m_Resizer;

	// Pointeur sur fenêtre dialogue
	CDlgMapping *pDlgMapping;

	// Pointeur sur sélection choix support
	JATPDocChoixSupport *pDocChoixSupport;

	// Mise à jour des sélections grandeurs
	void UpdateSelGrandeurs();

public:

	// Remplissage liste grandeurs
	bool LoadGrandeurs();
	bool LoadGrandeursAbs();
	bool LoadGrandeursOrd();

	// Initialisation de la liste
	void InitLstGrandeurs();

	// Mise à jour de la liste
	void MajGrandeurs();
	void MajGrandeurAbs();
	void MajGrandeurOrd();

protected :
	// la fonction pour mettre à jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLbnSelChangeListGrandOrd();
	afx_msg void OnLbnSelChangeListGrandAbs();
	
};
