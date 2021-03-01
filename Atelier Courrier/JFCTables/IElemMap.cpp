// ITable.cpp: implementation of the ITable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IElemMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


IElemMap::IElemMap()
{
}

IElemMap::~IElemMap()
{
	Empty();
}

JVoid IElemMap::Empty()
{
	for (MoveFirst(); IsValid(); MoveNext())
		delete GetItem();
	Reset();
}


IElem* IElemMap::GetElemByID(const JUnt32 id) const
{
	MoveTo(id);
	if (IsValid())
		return GetItem();
	else
		return NULL;
}

JChar* IElemMap::GetLabelByID(const JUnt32 id) const
{
	JChar * ret;
	MoveTo(id);
	if (IsValid())
		ret = (JChar*)GetItem()->GetLabel().AsJCharPtr();
	else
		ret = "TBL IElemMap:GetLabelByID ID inconnu";
	return ret;
}

