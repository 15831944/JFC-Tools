/*
//	JFC Informatique et M�dia
//	2005
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CMemPositionWnd
//
//	Descriptif:
//	Ensemble de m�thodes statique pour m�moriser dans le registre la position d'une fen�tre
*/

#pragma once


// Classe CMemPositionWnd
//M�thodes statiques pour enregistrer et restaurer la position d'une fen�tre

class AFX_EXT_CLASS CMemPositionWnd
{

private:
	CMemPositionWnd();   // constructeur standard
	virtual ~CMemPositionWnd();

	static void SetPos(CWnd * wnd, CString strRect);

public:
	//dans OnInitDialog ou OnInitialUpdate ou OnCreate
	static void LoadPos(CWnd * wnd);
	//on peut mettre un rect par d�faut
	static void LoadPos(CWnd * wnd, CString strRectDefault);


	//dans OnDestroy
	static void SavePos(CWnd * wnd);

};
