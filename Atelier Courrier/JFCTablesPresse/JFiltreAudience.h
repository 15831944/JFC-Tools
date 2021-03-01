/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JFiltreAudience
//
//	Descriptif:
//	Représentation d'un élément filtre d'audience
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
