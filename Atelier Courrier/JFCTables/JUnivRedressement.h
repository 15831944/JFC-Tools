/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JUnivRedressement
//
//	Descriptif:
//	Element de la table des univers de redressements
*/


#if !defined(AFX_JUNIVREDRESSEMENT_H__INCLUDED_)
#define AFX_JUNIVREDRESSEMENT_H__INCLUDED_

#pragma once
#include "IElem.h"


class AFX_EXT_CLASS JUnivRedressement : public IElem  
{

public:
	JUnivRedressement(JUnt32 id, const JChar* lbl);
	virtual ~JUnivRedressement();
};


#endif // !defined(AFX_JUNIVREDRESSEMENT_H__INCLUDED_)
