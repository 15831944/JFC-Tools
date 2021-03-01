#pragma once

#include <Label.h>
#include <BtnST.h>
#include "AnalyseurDoc.h"
#include "afxwin.h"


// Bo�te de dialogue CDlgParamsTri

class CDlgParamsTri : public CDialog
{
	DECLARE_DYNAMIC(CDlgParamsTri)

public:
	CDlgParamsTri(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDlgParamsTri();

	// R�cup du document courant
	void SetParams(CAnalyseurDoc *pDoc, CPoint Pt);

	// Liste

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_AN_DLG_PARAMS_TRI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	// Fonctions design controles
	void SetupControls();
	void SetColors();
	void SetControls();

	// R�cup�rartion des �lmts tri en cours
	void SetParamsTri();

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();


protected:

	// Le document courant
	CAnalyseurDoc *		m_pDoc;

	// Position de la fenn�tre
	CPoint		m_PosFen;

	// Les variables de s�lection temporaire
	BYTE		m_TypeTri;
	BYTE		m_ModeTri;

	// Les boites de s�lection
	CLabel		m_Box_OrdreTri;			// Boite de s�lection ordre de tri
	CLabel		m_Box_GrandeursTri;		// Boite de s�lection grandeur du tri
	CLabel		m_Box_ResultsTri;		// Boite de s�lection r�sultat cellule pour le tri	
	CLabel		m_Box_NatureTri;		// Boite de s�lection nature du tri

	// Les titres des boites de s�lection
	CLabel		m_Ttl_OrdreTri;			// Titre panneau s�lection ordre de tri
	CLabel		m_Ttl_GrandeurTri;		// Titre s�lection grandeur de tri
	CLabel		m_Ttl_ResultTri;		// Titre s�lection resultat cellule affichage tri
	CLabel		m_Ttl_NatureTri;		// Titre s�lectionnature du tri

	// Les boutons radio s�lection ordre de tri
	CButtonST	m_Rad_TriCroissant;
	CButtonST	m_Rad_TriDecroissant;
	CButtonST	m_Rad_AucunTri;

	// Liste des grandeurs disponibles
	CListBox	m_Lst_Grandeurs;

	// La liste s�lection r�sultat cellule
	CListBox	m_Lst_Results;	

	// Les boutons radio s�lection nature du tri
	CButtonST	m_Rad_Global;
	CButtonST	m_Rad_ParBloc;

	// Les boutons de sortie
	CButtonST	m_Btn_OK;
	CButtonST	m_Btn_CANCEL;
	
public:	
	afx_msg void OnBnClickedAnRadCroissant();
	afx_msg void OnBnClickedAnRadDecroissant();
	afx_msg void OnBnClickedAnRadAucuntri();
	afx_msg void OnBnClickedAnRadGlobal();
	afx_msg void OnBnClickedAnRadParbloc();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
