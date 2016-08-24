#if !defined(AFX_FILMVIEW_H__4CE3FCB0_14DA_431B_8D3A_C306784963BE__INCLUDED_)
#define AFX_FILMVIEW_H__4CE3FCB0_14DA_431B_8D3A_C306784963BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "grille.h"
#include "printfilm.h"
#include "ProduitFilm.h"
#include "GridVisuFilm.h"
#include "CalculProduitFilm.h"
#include "dlgparamprint.h"

// FilmView.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CFilmView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

// Couleur spécifique grille films
#define RGB_Tarif		   RGB(216,243,255)		//RGB(255,255,192)
#define RGB_CNC            RGB(216,243,255)
#define RGB_Totaux         RGB(185,175,205)     // RGB(192,255,255)


class CFilmView : public CFormView
{
protected:
	CFilmView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFilmView)
	CDlgParamPrint m_DlgParamPrint;

// Form Data
public:

	//{{AFX_DATA(CFilmView)
	enum { IDD = IDD_ANALYSE_FILM };
	CEdit	m_BtnPeriode;
	CEdit	m_BtnCibleBase;
	CObj_Gray m_Cadre1;
	CObj_Gray m_Cadre0;
	CButton	  m_MajFilms;
	CButton	  m_BtnPerf;
	CButton	  m_BtnGraphe;
	CString	  m_LibCampagneFilm;
	CString	  m_CibleBase;
	CString	  m_Periode;
	//}}AFX_DATA


// Attributes
public:
	CFilmDoc* GetDocument();

	// Grid affichage infos films
	CGridVisuFilm m_GridVisuFilm;

	// Campagne produit films en cours
	CCampagneFilm *m_pCampagneFilm;

	// Période en cours
	COleDateTime m_DebutCampagneFilm;
	COleDateTime m_FinCampagneFilm;

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilmView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public :

	// Pour impression résultat analyse films
	CPrintFilm * m_PrintFilm;

private:

	// Update des résultats performances et mémo-démémo
	void UpdateResultat();

	// Update Affichage des résultats semaines 
	void UpdateAffichageResult(CCalculProduitFilm::CResultatNbEntree * pResult);

	// Initialisation des infos générales
	void InitInfoGene();

	// Initialisation Grid des produits films
	void InitGridFilm();

	// Chargement des infos grilles films
	bool LoadGridFilm(); 

	// Infos colonnes
	enum ColonneGridFilm{ColSelectFilm= 0,ColLibFilm=1,ColTarif=2,ColCNC=3};
	enum ColonneGridModa{ColLibModa=0,ColFreq=1,ColPluriFreq=2,};

	// Affichage Resultat semaines
	void AfficheResultSemaine(int InxProduitFilm,CProduitFilm ProduitFilm,CCalculProduitFilm::CResultatNbEntree * resultat);
	
	// Affichage des totaux
	void AfficheTotalColonne(CCalculProduitFilm::CResultatNbEntree * Result);
	void AfficheTotalLigne(CCalculProduitFilm::CResultatNbEntree * Result);


// Variables privées pour l'interface analyse film
private :

	bool UndoPrepareCalculPrint();
	//bool PrepareCalculPrint();
	bool OpenDialogPrint();


protected:
	virtual ~CFilmView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFilmView)
	afx_msg void OnGraphe();
	afx_msg void OnPerf();
	afx_msg void OnMajfilm();
	afx_msg void OnPaint();
	afx_msg void OnFilePrint2();
	afx_msg void OnFilePrintPreview();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnParamtresOptiondecalcul();
	afx_msg void OnParamtresBta();
	afx_msg void OnParamtresExercice();
	afx_msg void OnParamtresCible1();
	afx_msg void OnParamtresBudgetnet();
	afx_msg void OnParamtresOptiondecalculmemo();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in cinemaView.cpp
inline CFilmDoc* CFilmView::GetDocument()
   { return (CFilmDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILMVIEW_H__4CE3FCB0_14DA_431B_8D3A_C306784963BE__INCLUDED_)
