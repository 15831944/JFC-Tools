// ATPrintPDFView.h : interface of the CATPrintPDFView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATPRINTPDFVIEW_H__23DCB370_C752_4DF2_AFC2_78DB74F309C9__INCLUDED_)
#define AFX_ATPRINTPDFVIEW_H__23DCB370_C752_4DF2_AFC2_78DB74F309C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// on inclut les fichiers nécessaires
#include "PageInfo.h"
#include "ViewportWindow.h"
#include "ATCloseWnd.h"
#include "PreviewBar.h"

class CATPrintPDFView : public CView
{
protected: // create from serialization only
	CATPrintPDFView();
	DECLARE_DYNCREATE(CATPrintPDFView)

// Attributes
public:
	CATPrintPDFDoc* GetDocument();

	// la fonction pour zoommer
	void Zoom(long zoom);

protected:
	// pour empêcher l'annulation de l'impression au milieu d'une page
	static BOOL CALLBACK OnAbortProc(HDC hdc, int nCode);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATPrintPDFView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

	// la fonction pour mettre à jour l'affichage de la fenêtre enfant
	void UpdateChild();

	// l'operateur d'egalité
	CATPrintPDFView & operator = (const CATPrintPDFView & operande);

	// le destructeur:
	virtual ~CATPrintPDFView();

	// les fonction de contrôle lors du debug
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// les scrollbars
	CScrollBar m_HorzScrollBar;
	CScrollBar m_VertScrollBar;

	// la fenêtre enfant
	CViewportWindow * m_pWindow;

	// les informations de la page
	CPageInfo * m_PageInfo;
	
	// la barre d'outils
	CPreviewBar * m_PreviewBar;

	// le point de départ du grab
	long m_StartGrabX;
	long m_StartGrabY;

	// l'origine au moment du grab
	long m_OrgGrabX;
	long m_OrgGrabY;

	// la rotation de la molette
	short m_zDelta;
	short m_xWheel;
	short m_yWheel;

	// Flag de la page courrante
	bool m_IsCurrentPage;
	
// Generated message map functions
protected:
	//{{AFX_MSG(CATPrintPDFView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnParentNotify( UINT message, LPARAM lParam );	
	afx_msg void OnPrintcurrentpage();
	afx_msg void OnPrintallpage();	
	afx_msg void OnViewFirstpage();		
	afx_msg void OnViewPrevpage();	
	afx_msg void OnViewNextpage();
	afx_msg void OnViewLastpage();	
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnFileExportpdf();	
	afx_msg void OnFileExportExcel();
	afx_msg void OnFileExportExcel03();
	afx_msg void OnSelChangePage();		
	afx_msg void OnUpdateViewFirstpage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPrevpage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewNextpage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewLastpage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileExportpdf(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileExportExcel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomout(CCmdUI* pCmdUI);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ATPrintPDFView.cpp
inline CATPrintPDFDoc* CATPrintPDFView::GetDocument()
   { return (CATPrintPDFDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPRINTPDFVIEW_H__23DCB370_C752_4DF2_AFC2_78DB74F309C9__INCLUDED_)
