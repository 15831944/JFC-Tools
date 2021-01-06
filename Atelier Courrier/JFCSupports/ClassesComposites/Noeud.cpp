// Noeud.cpp: implementation of the INode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Noeud.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

INode::INode()
{
	m_ParentID = 0;
	m_hParent = NULL;
}

INode::INode(JUnt32 ID, const JChar* lbl, JUnt32 parentID)
{
	m_ID = ID;
	m_Label = lbl;
	m_ParentID = parentID;
	m_hParent = NULL;
}

INode::~INode()
{
}

JVoid INode::SetLabel(const JChar* lbl)
{
	m_Label = lbl;
}

JUnt32 INode::GetParentID() const
{
	return m_ParentID;
}

JVoid INode::SetParentID(JUnt32 parentID)
{
	m_ParentID = parentID;
}

INode* INode::GetParentNode() const
{
	return m_hParent;
}

JVoid INode::SetParentNode(INode* nd)
{
	m_hParent = nd;
}

IIterateur* INode::CreateIterateur()
{
	return NULL;
}

JInt32 INode::GetNbFils() const
{
	return 0;
}

JArchive& INode::Send(JArchive & lhs, JInt32 Version) const
{
	switch(Version)
	{
	case 1:
		lhs.Send(m_ID);
		m_Label.Send(lhs);
		break;

	default:
//		TRACE("INode   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}

JArchive& INode::Recv(JArchive & lhs, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		lhs.Recv(m_ID);
		m_Label.Recv(lhs);
		break;

	default:
//		TRACE("INode   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}