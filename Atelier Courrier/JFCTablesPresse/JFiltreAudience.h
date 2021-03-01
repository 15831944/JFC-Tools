/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JFiltreAudience
//
//	Descriptif:
//	Repr�sentation d'un �l�ment filtre d'audience
*/



#if !defined(AFX_JFILTREAUDIENCE_H__INCLUDED_)
#define AFX_JFILTREAUDIENCE_H__INCLUDED_

#pragma once
#include "IElem.h"


class AFX_EXT_CLASS JFiltreAudience : public IElem  
{

public:
	JFiltreAudience(JUnt32 id, const JChar* lbl);
	virtual ~JFiltreAudience();
};


#endif // !defined(AFX_JFILTREAUDIENCE_H__INCLUDED_)
