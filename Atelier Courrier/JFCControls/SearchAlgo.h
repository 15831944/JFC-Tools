/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CSearchAlgo
//
//	Descriptif:
//	Teste si une chaine contient la chaine de recherche 
*/

#pragma once

class AFX_EXT_CLASS CSearchAlgo
{
	CSearchAlgo(void);
	~CSearchAlgo(void);

public:
	static bool Contains(const char* str, CString searchstr);

};
