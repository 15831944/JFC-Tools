#pragma once
// on inclut les définitions nécessaires
#include "JFCFrame.h"
#include "JFCUpdate.h"
#include "JFCDialog.h"
#include "JFCETSLayoutDialog.h"


class AFX_EXT_CLASS JFCDocument  
{
public:
	// le constructeur
	JFCDocument();

	// la fonction pour initialiser le frame
	virtual VOID AddFrame(JFCFrame* pFrame);

	// la fonction pour initialiser les dialogues
	virtual VOID AddDialog(JFCDialog* pDialog);

	// la fonction pour initialiser les dialogues
	virtual VOID AddDialog(JFCETSLayoutDialog* pDialog);

	// la fonction pour préparer le document
	virtual BOOL PrepareContents();

	// la fonction pour mettre à jour les vues du frame
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

	// la fonction pour mettre à jour l'indicateur de modification
	virtual VOID SetModifiedFlag(BOOL Modified = TRUE);

	// la fonction pour récupérer l'indicateur de modification
	virtual BOOL IsModified() const;

	// le destructeur
	virtual ~JFCDocument();

protected:
	// la fonction pour préparer le contenu du document
	virtual BOOL OnPrepareContents();

protected:
	// la fonction pour mettre à jour le document
	virtual VOID OnUpdate(const JFCUpdate & Message);

private:
	JList<JFCFrame*>  m_lstFrame;				// le pointeur sur les frames

	JList<JFCDialog*> m_lstDialog;				// le pointeur sur les dialogues

	JList<JFCETSLayoutDialog*> m_lstETSDialog;	// le pointeur sur les dialogues

private:
	BOOL m_Modified; // l'indicateur de modification
};

