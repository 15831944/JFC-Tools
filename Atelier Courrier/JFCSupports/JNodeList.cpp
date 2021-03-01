#include "stdafx.h"
#include ".\jnodelist.h"
#include ".\Visiteurs\VisiteurDelete.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JNodeList::JNodeList(void)
{
}

JNodeList::~JNodeList(void)
{
}

//recupere le 1er item avec cet id
INode* JNodeList::GetNodeByID (JUnt32 ID) const
{
	MoveFirst();
	while ( IsValid() && GetItem()->GetID() != ID )
		MoveNext();

	if ( IsValid() )
		return GetItem();
	else
		return NULL;
}

//on peut dc appeller plusieurs fois cette méthode
//qd on a des id multiples
INode* JNodeList::GetNextNodeByID (JUnt32 ID)
{
	do
		MoveNext();
	while ( IsValid() && GetItem()->GetID() != ID );

	if ( IsValid() )
		return GetItem();
	else
		return NULL;
}

JVoid JNodeList::RemoveNode(INode* nd)
{
	MoveFirst();
	while ( IsValid() && GetItem() != nd )
		MoveNext();

	if ( IsValid() )
		 Remove(-1);
}

JVoid JNodeList::Empty()
{
	CVisiteurDelete vDelete;
	MoveLast();
	while ( IsValid() )
		GetItem()->Accept(&vDelete);
	//l 'item se retire tout seul de la liste
}
