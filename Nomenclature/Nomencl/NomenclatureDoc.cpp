// NomenclatureDoc.cpp : implementation of the CNomenclatureDoc class
//

#include "stdafx.h"
#include "Nomenclature.h"

#include "NomenclatureDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNomenclatureDoc

IMPLEMENT_DYNCREATE(CNomenclatureDoc, CDocument)

BEGIN_MESSAGE_MAP(CNomenclatureDoc, CDocument)
	//{{AFX_MSG_MAP(CNomenclatureDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNomenclatureDoc construction/destruction

CNomenclatureDoc::CNomenclatureDoc()
{
	// TODO: add one-time construction code here

}

CNomenclatureDoc::~CNomenclatureDoc()
{
}

BOOL CNomenclatureDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNomenclatureDoc serialization

void CNomenclatureDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNomenclatureDoc diagnostics

#ifdef _DEBUG
void CNomenclatureDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNomenclatureDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNomenclatureDoc commands
