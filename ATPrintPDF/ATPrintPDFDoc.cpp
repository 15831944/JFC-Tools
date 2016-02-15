// ATPrintPDFDoc.cpp : implementation of the CATPrintPDFDoc class
//

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "ATPrintPDF.h"
#include "ATPrintPDFDoc.h"
#include "./ARML/ARMLContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CATPrintPDFApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CATPrintPDFDoc

IMPLEMENT_DYNCREATE(CATPrintPDFDoc, CDocument)

BEGIN_MESSAGE_MAP(CATPrintPDFDoc, CDocument)
	//{{AFX_MSG_MAP(CATPrintPDFDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//============================================================================
// le constructeur:
//============================================================================
CATPrintPDFDoc::CATPrintPDFDoc()
{
	// on initialise les paramètres
	m_pDrawDoc = 0;
}

//============================================================================
// la fonction pour gérer la creation d'un nouveau document:
//============================================================================
BOOL CATPrintPDFDoc::OnNewDocument()
{
	// on teste le traitement par défaut
	if (!CDocument::OnNewDocument()) return FALSE;

	// on libère le document a dessiner
	delete (m_pDrawDoc); m_pDrawDoc = 0;

	// on renvoie le code de succés
	return TRUE;
}

//============================================================================
// la fonction pour gérer l'ouverture d'un nouveau document:
//============================================================================
BOOL CATPrintPDFDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	// on récupère le répertoire courant
	char * CurrentDir = new char[MAX_PATH]; (::GetCurrentDirectory(MAX_PATH, CurrentDir));

	if (!CDocument::OnOpenDocument(lpszPathName))
	{
		delete CurrentDir;
		return FALSE;
	}

	// on remet le répertoire courant
	(::SetCurrentDirectory(CurrentDir));

	// on libère le texte
	delete CurrentDir;

	// on libère le document courant
	delete m_pDrawDoc;
	
	// on crée un document à dessiner
	m_pDrawDoc = new DrawDocument();

	// on crée un contexte pour le fichier sélectionné
	Context contexte(lpszPathName, m_pDrawDoc);

	// on fixe les paramètres du contexte
	if (theApp.m_ModePaysage == FALSE)
	{
		contexte.SetPageHeight(2816);
		contexte.SetPageWidth(1900);
		contexte.SetLineHeight(32);
		contexte.SetNbLinePage(88);
	}
	else
	{
		contexte.SetPageHeight(1900);
		contexte.SetPageWidth(2816);
		contexte.SetLineHeight(32);
		contexte.SetNbLinePage(60);
	}
	// on lance le traitement sur le fichier
	if (!contexte.Manage()) {delete (m_pDrawDoc); m_pDrawDoc = 0;}
	this->UpdateAllViews(NULL, UPDATE_DOC_CHANGED, NULL);
	return TRUE;
}

//============================================================================
// la fonction pour la sérialisation:
//============================================================================
void CATPrintPDFDoc::Serialize(CArchive& ar)
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

//============================================================================
// les fonction pour le mode debug:
//============================================================================
#ifdef _DEBUG
void CATPrintPDFDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CATPrintPDFDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


//============================================================================
// le destructeur:
//============================================================================
CATPrintPDFDoc::~CATPrintPDFDoc()
{
	// on libère le document
	delete (m_pDrawDoc);
}

