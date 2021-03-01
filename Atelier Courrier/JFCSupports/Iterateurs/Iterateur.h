/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	IIterateur
//
//	Descriptif:
//	Interface d'un iterateur
//	DesignPattern Iterateur
*/

#if !defined(AFX_ITERATEUR_H__FB2EE77C_F4EE_4BD5_9101_CDD469FF3C79__INCLUDED_)
#define AFX_ITERATEUR_H__FB2EE77C_F4EE_4BD5_9101_CDD469FF3C79__INCLUDED_

#pragma once

#include "..\ClassesComposites\Noeud.h"


class AFX_EXT_CLASS IIterateur  
{
public:
	IIterateur(){};
	virtual ~IIterateur(){};

	virtual INode* getNodeByID(JUnt32 id) PURE;
	virtual INode* getHead() PURE;
	virtual INode* getNext() PURE;
	virtual JInt32 nbElem() PURE;

};

#endif // !defined(AFX_ITERATEUR_H__FB2EE77C_F4EE_4BD5_9101_CDD469FF3C79__INCLUDED_)
