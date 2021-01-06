/*
//	JFC Informatique et Média
//	2005
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CPrintAnyCWnd
//
//	Descriptif:
//	Imprime une CWnd
*/

#pragma once

class AFX_EXT_CLASS CPrintAnyCWnd
{
	CPrintAnyCWnd(void);
	~CPrintAnyCWnd(void);

public:
	static bool CopyToClipBoard(CWnd* pWnd);
	static void Print(CWnd* pWnd, CString title);

};
