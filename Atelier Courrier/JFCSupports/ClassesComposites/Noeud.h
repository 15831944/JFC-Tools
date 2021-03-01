/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	INode
//
//	Descriptif:
//	Noeud dans l'arborescence(permet de creer des iterateurs)
*/


#if !defined(AFX_NOEUD_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
#define AFX_NOEUD_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_

#pragma once

#include "..\..\JFCTables\IElem.h"


class IComposite;
class IIterateur;
class IVisiteur;


class AFX_EXT_CLASS INode : public IElem 
{

public:
	virtual ~INode();

	virtual JVoid SetLabel(const JChar* lbl);

	virtual JUnt32 GetParentID() const;
	virtual JVoid SetParentID(JUnt32 parentID);

	virtual INode* GetParentNode() const;
	virtual JVoid SetParentNode(INode* nd);

	virtual IIterateur* CreateIterateur();
	virtual JInt32 GetNbFils() const;


	virtual JVoid Accept (IVisiteur *v) {};

	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;
	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);

protected:
	//Constructeur protected pour ne pas creer d'instance de INode
	INode();
	INode(JUnt32 ID, const JChar* lbl, JUnt32 parentID);

	JUnt32 m_ParentID;

	INode* m_hParent;

};


#endif // !defined(AFX_NOEUD_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
