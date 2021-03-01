/*
//	JFC Informatique et Média
//	2005
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JFCSplitterPane
//
//	Descriptif:
//	Classe abstraite pour la construction du Creuset
*/

#pragma once

// on inclut les définitions nécessaires
#include "JFCPane.h"

/////////////////////////////////////////////////////////////////////////////
// JFCSplitterPane
class JATPDocCreuset;

class AFX_EXT_CLASS JFCSplitterPane : public JFCPane
{
public:
	// le constructeur
	JFCSplitterPane();

	// le destructeur
	virtual ~JFCSplitterPane();

	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset) PURE;
	virtual JVoid Update() PURE;

	virtual void SetColors() PURE;

	// la fonction pour configurer la gestion des ascenseurs
	void RemoveScroll(CWnd * pWnd);

protected:
	//{{AFX_MSG(JFCSplitterPane)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	void PostNcDestroy( );
};

