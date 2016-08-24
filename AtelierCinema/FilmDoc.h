#if !defined(AFX_FILMDOC_H__4351F3F3_4DB9_4016_AADE_45089D67EA45__INCLUDED_)
#define AFX_FILMDOC_H__4351F3F3_4DB9_4016_AADE_45089D67EA45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilmDoc.h : header file
//

#include "DlgChangerPeriode.h"
#include "DlgFonctionCible.h"
#include "DlgSelectionProduit.h"
#include "dlgperformancefilm.h"
#include "dlggraphefilm.h"
#include "campagnefilm.h"


extern CCinemaApp theApp; 

/////////////////////////////////////////////////////////////////////////////
// CFilmDoc document

class CFilmDoc : public CDocument
{
protected:
	CFilmDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFilmDoc)

// Attributes
public:

	// Campagne films
	CCampagneFilm m_CampagneFilm;

	// Boite de dialogue résultat campagne film
	CDlgPerformanceFilm m_DlgPerfFilm;

	// Boite de dialogue pour visu mémo-démémo campagne film
	CDlgGrapheFilm m_DlgGrapheFilm;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilmDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

public :
	bool m_UpdateView; 

// Implementation
public:
	virtual ~CFilmDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFilmDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILMDOC_H__4351F3F3_4DB9_4016_AADE_45089D67EA45__INCLUDED_)
