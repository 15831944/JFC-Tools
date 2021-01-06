/*
//	JFC Informatique et Média
//	2005
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CMemPositionWnd
//
//	Descriptif:
//	Ensemble de méthodes statique pour mémoriser dans le registre la position d'une fenêtre
*/

#pragma once


// Classe CMemPositionWnd
//Méthodes statiques pour enregistrer et restaurer la position d'une fenêtre

class AFX_EXT_CLASS CMemPositionWnd
{

private:
	CMemPositionWnd();   // constructeur standard
	virtual ~CMemPositionWnd();

	static void SetPos(CWnd * wnd, CString strRect);

public:
	//dans OnInitDialog ou OnInitialUpdate ou OnCreate
	static void LoadPos(CWnd * wnd);
	//on peut mettre un rect par défaut
	static void LoadPos(CWnd * wnd, CString strRectDefault);


	//dans OnDestroy
	static void SavePos(CWnd * wnd);

};
