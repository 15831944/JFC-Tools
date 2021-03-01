// IElem.cpp: implementation of the IElem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IElem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IElem::IElem()
{
}

IElem::IElem(JUnt32 id, const JChar* lbl)
{
	m_ID = id;
	m_Label = lbl;
}

IElem::~IElem()
{
}

//JVoid IElem::SetID(JUnt32 id)
//{
//	m_ID = id;
//}

JUnt32 IElem::GetID() const
{
	return m_ID;
}

const JLabel & IElem::GetLabel() const
{
	return m_Label;
}

//JVoid IElem::SetLabel(const JChar* lbl)
//{
//	m_Label = lbl;
//}