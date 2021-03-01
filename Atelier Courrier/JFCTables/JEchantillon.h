/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JEchantillon
//
//	Descriptif:
//	Element echantillon de la table des echantillons
*/

#if !defined(AFX_JECHANTILLON_H__INCLUDED_)
#define AFX_JECHANTILLON_H__INCLUDED_

#pragma once
#include "IElem.h"


class AFX_EXT_CLASS JEchantillon : public IElem  
{

public:
	JEchantillon(JUnt32 id, const JChar* lbl);
	virtual ~JEchantillon();
};


#endif // !defined(AFX_JECHANTILLON_H__INCLUDED_)
