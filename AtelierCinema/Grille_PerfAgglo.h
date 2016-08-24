#if !defined(AFX_GRILLE_PERFAGGLO_H__90164540_2D12_48C2_82A3_5D6889DC6B8B__INCLUDED_)
#define AFX_GRILLE_PERFAGGLO_H__90164540_2D12_48C2_82A3_5D6889DC6B8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Grille_PerfAgglo.h : header file
//
#include "campagneagglo.h"


/////////////////////////////////////////////////////////////////////////////
// CGrille_PerfAgglo window

class CGrille_PerfAgglo : public CStatic
{
// Construction
public:
	CGrille_PerfAgglo();

// Attributes
public:
	CCampagneAgglo * m_pCampagne;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrille_PerfAgglo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGrille_PerfAgglo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGrille_PerfAgglo)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRILLE_PERFAGGLO_H__90164540_2D12_48C2_82A3_5D6889DC6B8B__INCLUDED_)
