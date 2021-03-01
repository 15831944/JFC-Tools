/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CRepSupports
//
//	Descriptif:
//	Point d'entré du répertoire des supports pour le lanceur.
*/

#pragma once

#include "RepSupportsDlg.h"

class AFX_EXT_CLASS CRepSupports
{
public:
	CRepSupports(void);
	virtual ~CRepSupports(void);
	void Show(CWnd *pWnd = NULL);
	void Hide();

	BOOL GetState();

	BOOL ForwardMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);

private:

	CRepSupportsDlg	m_DlgSupports;
};
