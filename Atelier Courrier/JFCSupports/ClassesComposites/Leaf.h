/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	ILeaf
//
//	Descriptif:
//	Classe feuille du DesignPattern Classe composite
*/


#if !defined(AFX_LEAF_H__D04100A0_FD7B_4ED8_9A7A_584E8EFFE787__INCLUDED_)
#define AFX_LEAF_H__D04100A0_FD7B_4ED8_9A7A_584E8EFFE787__INCLUDED_

#pragma once

#include "Noeud.h"


class AFX_EXT_CLASS ILeaf : public INode  
{
public:
	virtual ~ILeaf() {};
protected:
	ILeaf() {};
	ILeaf(JUnt32 ID, const JChar* lbl, JUnt32 parentID) : INode(ID, lbl, parentID) {};
};

#endif // !defined(AFX_LEAF_H__D04100A0_FD7B_4ED8_9A7A_584E8EFFE787__INCLUDED_)
