// cinemaDoc.h : interface of the CCinemaDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CINEMADOC_H__33A5874C_0AED_11D4_989E_004095420987__INCLUDED_)
#define AFX_CINEMADOC_H__33A5874C_0AED_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dlgchangerperiode.h"
#include "dlgfonctioncible.h"
#include "dlgperformances.h"
#include "dlgperfagglo.h"
#include "dlggraphe.h"
#include "campagne.h"
#include "campagneagglo.h"

class CCinemaDoc : public CDocument
{
protected: // create from serialization only
	CCinemaDoc();
	DECLARE_DYNCREATE(CCinemaDoc)

// Attributes
public:
	//CCampagne m_Campagne;
	CCampagneAgglo m_Campagne;

	CDlgPerformances m_DlgPerf;
	CDlgPerfAgglo m_DlgPerfAgglo;
	CDlgGraphe m_DlgGraphe;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCinemaDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCinemaDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCinemaDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CINEMADOC_H__33A5874C_0AED_11D4_989E_004095420987__INCLUDED_)
