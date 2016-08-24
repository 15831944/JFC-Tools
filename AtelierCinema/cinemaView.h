// cinemaView.h : interface of the CCinemaView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CINEMAVIEW_H__33A5874E_0AED_11D4_989E_004095420987__INCLUDED_)
#define AFX_CINEMAVIEW_H__33A5874E_0AED_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "grille.h"
#include "print.h"


#include "dlgparamprint.h"
#include "Campagne.h"

class CCinemaView : public CFormView
{
protected: // create from serialization only
	CCinemaView();
	DECLARE_DYNCREATE(CCinemaView)
	CDlgParamPrint m_DlgParamPrint;

public:
	//{{AFX_DATA(CCinemaView)
	enum { IDD = IDD_CINEMA_FORM };
	CObj_Btn	m_BoutonTete;
	CScrollBar	m_ScrollV;
	CObj_Btn	m_BtnEffacer;
	CObj_Btn	m_BtnGraphe;
	CObj_Btn	m_BtnPerf;
	//CObj_Btn	m_BtnPanorama;
	//CObj_Gray	m_Cadre3;
	CObj_Gray	m_Cadre2;
	CObj_Btn	m_BtnD5;
	CObj_Btn	m_BtnD4;
	CObj_Btn	m_BtnD3;
	CObj_Btn	m_BtnD2;
	CObj_Btn	m_BtnD1;
	CObj_Gray	m_Cadre1;
	CObj_Btn	m_BtnH5;
	CObj_Btn m_BtnH4;
	CObj_Btn m_BtnH3;
	CObj_Btn m_BtnH2;
	CObj_Btn m_BtnH1;
	CObj_Gray	m_Cadre;
	CScrollBar	m_ScrollH;
	CGrille	m_NbPassages;
	CGrille	m_Grille;
	CGrille	m_Calendrier;
	CGrille	m_Binome;
	//}}AFX_DATA

// Attributes
public:
	CCinemaDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCinemaView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CPrint * m_Print;
	void SetStateBoutonHypothese();
	void SetStateBoutonDuree();
	void InitBoutonDuree();
	virtual ~CCinemaView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCinemaView)
	afx_msg void OnPaint();
	afx_msg void OnOutilsAjoutersupprimer();
	afx_msg void OnOutilsCopiercoller();
	afx_msg void OnOutilsCouper();
	afx_msg void OnOutilsDure();
	afx_msg void OnOutilsGlisser();
	afx_msg void OnOutilsGommer();
	afx_msg void OnOutilsTempo();
	afx_msg void OnUpdateOutilsAjoutersupprimer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOutilsCopiercoller(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOutilsCouper(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOutilsDure(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOutilsGlisser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOutilsGommer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOutilsTempo(CCmdUI* pCmdUI);
	afx_msg void OnParamtresExercice();
	afx_msg void OnGraphe();
	afx_msg void OnPerformances();
	afx_msg void OnEffacer();
	afx_msg void OnParamtresOptiondecalcul();
	afx_msg void OnParamtresBudgetnet();
	afx_msg void OnParamtresCible1();
	afx_msg void OnParamtresBta();
	afx_msg void OnDuree1();
	afx_msg void OnDuree2();
	afx_msg void OnDuree3();
	afx_msg void OnDuree4();
	afx_msg void OnDuree5();
	afx_msg void OnActionValider();
	afx_msg void OnHypothese1();
	afx_msg void OnHypothese2();
	afx_msg void OnHypothese3();
	afx_msg void OnHypothese4();
	afx_msg void OnHypothese5();
	afx_msg void OnAffichageTest();
	afx_msg void OnActionCopier();
	afx_msg void OnActionColler();
	afx_msg void OnUpdateActionColler(CCmdUI* pCmdUI);
	afx_msg void OnActionRecopierdanslhypotheseactiveLhypothse1();
	afx_msg void OnActionRecopierdanslhypotheseactiveLhypothse2();
	afx_msg void OnActionRecopierdanslhypotheseactiveLhypothse3();
	afx_msg void OnActionRecopierdanslhypotheseactiveLhypothse4();
	afx_msg void OnActionRecopierdanslhypotheseactiveLhypothse5();
	afx_msg void OnButton4();
	afx_msg void OnCampagneExporter();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnParamtresCible2();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnParamtresFormat();
	afx_msg void OnButtonchoix();
	afx_msg void OnPerfagglo();
	afx_msg void OnExportMediavision();
	afx_msg void OnUpdateExportMediavision(CCmdUI* pCmdUI);
	afx_msg void OnExportScreenvision();
	afx_msg void OnUpdateExportScreenvision(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool UndoPrepareCalculPrint();
	bool PrepareCalculPrint();
	bool OpenDialogPrint();

	// Export spécifique Médiavision
	void ExportMediavision(CCampagne *pCampagne, CString Titre, CString SsTitre, CArray <int,int&> &TabIdCible);
	
	// Export spécifique Screenvision
	void ExportScreenvision(CCampagne *pCampagne, CString Titre, CString SsTitre, CArray <int,int&> &TabIdCible, int InxCiblePerfSem);

	// Recup libelle du bac associé au libellé complexe
	CString GetLibelleBacCodeComplexe(CString LibComplexe);
};

#ifndef _DEBUG  // debug version in cinemaView.cpp
inline CCinemaDoc* CCinemaView::GetDocument()
   { return (CCinemaDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CINEMAVIEW_H__33A5874E_0AED_11D4_989E_004095420987__INCLUDED_)
