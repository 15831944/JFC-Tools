/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	IComposite
//
//	Descriptif:
//	Classe composite (DesignPattern)
*/


#if !defined(AFX_COMPOSITE_H__7224560C_6EBF_488C_A615_542D91D92F3D__INCLUDED_)
#define AFX_COMPOSITE_H__7224560C_6EBF_488C_A615_542D91D92F3D__INCLUDED_

#pragma once

#include "Noeud.h"
#include "..\JNodeList.h"
#include "..\Visiteurs\Visiteur.h"



class AFX_EXT_CLASS IComposite : public INode  
{

//pour permettre aux visiteurs d'acceder protected
	friend class CVisiteurInsert;
	friend class CVisiteurRemove;
	friend class CVisiteurDelete;
	friend class CVisiteurDeleteTA;

public:
	virtual ~IComposite();

	virtual JInt32 GetNbFils() const;
	virtual IIterateur* CreateIterateur();


protected:
	IComposite();
	IComposite(JUnt32 ID, const JChar* lbl, JUnt32 parentID);

	JNodeList m_LstNode;

};

#endif // !defined(AFX_COMPOSITE_H__7224560C_6EBF_488C_A615_542D91D92F3D__INCLUDED_)
