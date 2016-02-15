// NomenclatureDoc.h : interface of the CNomenclatureDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOMENCLATUREDOC_H__4ACC99B3_AE5E_41CD_8C19_9AF2090655B5__INCLUDED_)
#define AFX_NOMENCLATUREDOC_H__4ACC99B3_AE5E_41CD_8C19_9AF2090655B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNomenclatureDoc : public CDocument
{
protected: // create from serialization only
	CNomenclatureDoc();
	DECLARE_DYNCREATE(CNomenclatureDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNomenclatureDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNomenclatureDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNomenclatureDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOMENCLATUREDOC_H__4ACC99B3_AE5E_41CD_8C19_9AF2090655B5__INCLUDED_)
