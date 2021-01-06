// JCouplageFixe.cpp: implementation of the JCouplageFixe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JCouplageFixe.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JCouplageFixe::JCouplageFixe(JUnt32 id, const JChar* lbl) : IElem(id, lbl)
{
}

JCouplageFixe::~JCouplageFixe()
{
	Empty();
}

JVoid JCouplageFixe::Empty()
{
	for ( m_mapIElem.MoveFirst(); m_mapIElem.IsValid(); m_mapIElem.MoveNext() )
		delete m_mapIElem.GetItem();
	m_mapIElem.Reset();
}

JInt32 JCouplageFixe::GetNbElem() const
{
	return m_mapIElem.GetCount();
}

//IELEMMAP * JCouplageFixe::GetTitres()
//{
//	return &m_mapIElem;
//}

IElem* JCouplageFixe::GetFilsByID(JUnt32 id) const
{
	m_mapIElem.MoveTo(id);
	if (m_mapIElem.IsValid())
		return m_mapIElem.GetItem();
	return NULL;
}

JVoid JCouplageFixe::GetFils(JList<JUnt32> & lst) const
{
	for(m_mapIElem.MoveFirst(); m_mapIElem.IsValid(); m_mapIElem.MoveNext() )
		lst.AddTail() = m_mapIElem.GetItem()->GetID(); 
}
JVoid JCouplageFixe::GetFils(JList<IElem*> & lst) const
{
	for(m_mapIElem.MoveFirst(); m_mapIElem.IsValid(); m_mapIElem.MoveNext() )
		lst.AddTail() = m_mapIElem.GetItem(); 
}
