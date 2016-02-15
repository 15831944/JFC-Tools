#if !defined(AFX_PREVIEWBAR_H__19958A5C_0A02_46DC_8A21_4C5AA4ED7206__INCLUDED_)
#define AFX_PREVIEWBAR_H__19958A5C_0A02_46DC_8A21_4C5AA4ED7206__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreviewBar window

#include "PageInfo.h"
#include "ViewportWindow.h"

class CPreviewBar : public CToolBar
{
// Construction
public:
	CPreviewBar();

	// la fonction pour récupérer la valeur du zoom
	long GetZoomValue();
	void SetZoomValue(long zoomvalue);

	// Recuperation de l'indice de nuancier Gris
	long GetValeurGris();
	void SetValeurGris(long valeurGris);

// Attributes
public:
	CComboBox			m_ComboPage;
	CSliderCtrl			m_SliderNuancier;

	CPageInfo *			m_pPageInfo;
	CDocument *			m_pDoc;

public:
	virtual ~CPreviewBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPreviewBar)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelChangeNuanceGris();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWBAR_H__19958A5C_0A02_46DC_8A21_4C5AA4ED7206__INCLUDED_)
