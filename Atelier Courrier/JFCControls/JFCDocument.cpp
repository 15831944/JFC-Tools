// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JFCDocument.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// le constructeur
//////////////////////////////////////////////////////////////////////

JFCDocument::JFCDocument()
{
	// on initialise l'indicateur
	m_Modified = FALSE;
}

//////////////////////////////////////////////////////////////////////
// la fonction pour initialiser le frame
//////////////////////////////////////////////////////////////////////
VOID JFCDocument::AddFrame(JFCFrame* pFrame)
{
	// on initialise le pointeur sur le frame
	m_lstFrame.AddTail() = pFrame;
}

//////////////////////////////////////////////////////////////////////
// la fonction pour initialiser les dialogues
//////////////////////////////////////////////////////////////////////
VOID JFCDocument::AddDialog(JFCDialog* pDialog)
{
	// on initialise le pointeur sur le dialogue
	m_lstDialog.AddTail() = pDialog;
}

// la fonction pour initialiser les dialogues
VOID JFCDocument::AddDialog(JFCETSLayoutDialog* pDialog)
{
	// on initialise le pointeur sur le dialogue
	m_lstETSDialog.AddTail() = pDialog;
}

//////////////////////////////////////////////////////////////////////
// la fonction pour pr�parer le document
//////////////////////////////////////////////////////////////////////

BOOL JFCDocument::PrepareContents()
{
	// on pr�pare le document
	return (this->OnPrepareContents());
}

//////////////////////////////////////////////////////////////////////
// la fonction pour mettre � jour les vues du frame
//////////////////////////////////////////////////////////////////////

VOID JFCDocument::UpdateAllViews(const JFCUpdate & Message)
{
	// on met � jour le document
	this->OnUpdate(Message);

	// on met � jour les vues du frame
	for(m_lstFrame.MoveFirst(); m_lstFrame.IsValid(); m_lstFrame.MoveNext())
		m_lstFrame.GetItem()->UpdateAllViews(Message);

	JUnt32 NbDlg = m_lstDialog.GetCount();
	// on met � jour les vues du dialogue
	for(m_lstDialog.MoveFirst(); m_lstDialog.IsValid(); m_lstDialog.MoveNext())
	{
		// R�cup�re pointeur sur chaque �lmt fen dialogue
		JFCDialog *pDlg = m_lstDialog.GetItem();
	}

	// on met � jour les vues du dialogue
	// OPTIM 2.7
	for(m_lstDialog.MoveFirst(); m_lstDialog.IsValid(); m_lstDialog.MoveNext())
	{
		// R�cup�re pointeur sur chaque �lmt fen dialogue
		JFCDialog *pDlg = m_lstDialog.GetItem();
		if (pDlg != NULL)
			// on fait la mise � jour ad�quate
			pDlg->UpdateAllViews(Message); 
	}

	// on met � jour les vues du dialogue
	for(m_lstETSDialog.MoveFirst(); m_lstETSDialog.IsValid(); m_lstETSDialog.MoveNext())
	{
		// R�cup�re pointeur sur chaque �lmt fen dialogue
		JFCETSLayoutDialog *pDlg = m_lstETSDialog.GetItem();
		if (pDlg != NULL)
			// on fait la mise � jour ad�quate
			pDlg->UpdateAllViews(Message); 
	}
}

//////////////////////////////////////////////////////////////////////
// la fonction pour pr�parer le contenu du document
//////////////////////////////////////////////////////////////////////

BOOL JFCDocument::OnPrepareContents()
{
	// on continue l'initialisation du document
	return (TRUE);
}

//////////////////////////////////////////////////////////////////////
// la fonction pour mettre � jour le document
//////////////////////////////////////////////////////////////////////

VOID JFCDocument::OnUpdate(const JFCUpdate & Message)
{
	// on ne fait rien
}

//////////////////////////////////////////////////////////////////////
// la fonction pour mettre � jour l'indicateur de modification
//////////////////////////////////////////////////////////////////////

VOID JFCDocument::SetModifiedFlag(BOOL Modified)
{
	// on met � jour l'indicateur
	m_Modified = Modified;
}

//////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer l'indicateur de modification
//////////////////////////////////////////////////////////////////////

BOOL JFCDocument::IsModified() const
{
	// on renvoie l'indicateur
	return (m_Modified);
}

//////////////////////////////////////////////////////////////////////
// le destructeur
//////////////////////////////////////////////////////////////////////

JFCDocument::~JFCDocument()
{
	// on ne fait rien
}
