// Composite.cpp: implementation of the IComposite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Composite.h"
#include "..\Iterateurs\IterateurFils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IComposite::IComposite()
{
}

IComposite::IComposite(JUnt32 ID, const JChar* lbl, JUnt32 parentID) : INode(ID, lbl, parentID)
{
}


IComposite::~IComposite()
{
}

IIterateur* IComposite::CreateIterateur()
{
	return new CIterateurFils( &m_LstNode );
}

JInt32 IComposite::GetNbFils() const
{
	return m_LstNode.GetCount();
}





