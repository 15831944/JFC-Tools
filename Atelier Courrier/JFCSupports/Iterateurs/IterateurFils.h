/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CIterateurFils
//
//	Descriptif:
//	Iterateur pour parcourir les fils d'un composite
//	DesignPattern Iterateur
*/

#if !defined(AFX_ITERATEURFILS_H__7FC1B42C_482A_4F73_B11E_C518AF427478__INCLUDED_)
#define AFX_ITERATEURFILS_H__7FC1B42C_482A_4F73_B11E_C518AF427478__INCLUDED_

#pragma once

#include "Iterateur.h"
#include "..\JNodeList.h"


class AFX_EXT_CLASS CIterateurFils : public IIterateur  
{
public:
	CIterateurFils(JNodeList* lstNode);
	virtual ~CIterateurFils();

	virtual INode* getNodeByID(JUnt32 id);
	virtual INode* getHead();
	virtual INode* getNext();
	virtual JInt32 nbElem();

protected:
	JNodeList* m_plstNode;
};

#endif // !defined(AFX_ITERATEURFILS_H__7FC1B42C_482A_4F73_B11E_C518AF427478__INCLUDED_)
