// ATPrintPDFDoc.h : interface of the CATPrintPDFDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATPRINTPDFDOC_H__F2E691E4_78D5_4EAC_A376_FB2A0A94ED0B__INCLUDED_)
#define AFX_ATPRINTPDFDOC_H__F2E691E4_78D5_4EAC_A376_FB2A0A94ED0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les fichiers nécessaires
#include "./Composite/JFCDrawDocument.h"

class CATPrintPDFDoc : public CDocument
{
protected: // create from serialization only
	CATPrintPDFDoc();
	DECLARE_DYNCREATE(CATPrintPDFDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATPrintPDFDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CATPrintPDFDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	// un pointeur sur le document à dessiner
	DrawDocument * m_pDrawDoc;

// Generated message map functions
protected:
	//{{AFX_MSG(CATPrintPDFDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPRINTPDFDOC_H__F2E691E4_78D5_4EAC_A376_FB2A0A94ED0B__INCLUDED_)
