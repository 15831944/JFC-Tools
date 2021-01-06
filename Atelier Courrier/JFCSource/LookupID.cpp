#include "stdafx.h"
#include ".\lookupid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LookupID::LookupID(void)
{
}

LookupID::~LookupID(void)
{
}

bool LookupID::operator < (const LookupID &rhs) const
{
	if(m_ID1 < rhs.m_ID1)
		return true;
	if(m_ID1 == rhs.m_ID1 && m_ID2 < rhs.m_ID2)
		return true;
	return false;
}

bool LookupID::operator > (const LookupID &rhs) const
{
	if(m_ID1 > rhs.m_ID1)
		return true;
	if(m_ID1 == rhs.m_ID1 && m_ID2 > rhs.m_ID2)
		return true;
	return false;
}
