/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JNodeList
//
//	Descriptif:
//	Liste de INode*
*/

#pragma once

#include ".\ClassesComposites\Noeud.h"

class AFX_EXT_CLASS JNodeList : public JList<INode*>
{
public:
	JNodeList(void);
	virtual ~JNodeList(void);

	INode* GetNodeByID(JUnt32 ID) const;
	INode* GetNextNodeByID (JUnt32 ID);

	JVoid RemoveNode(INode* nd);
	JVoid Empty();

};
