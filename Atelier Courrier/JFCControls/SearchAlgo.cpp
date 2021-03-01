#include "StdAfx.h"
#include ".\searchalgo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSearchAlgo::CSearchAlgo(void)
{
}

CSearchAlgo::~CSearchAlgo(void)
{
}

bool CSearchAlgo::Contains(const char* str, CString searchstr)
{
	if(searchstr.IsEmpty())
		return true;

	CString label = str;
	CString search = searchstr;

	search.MakeLower();
	search.Replace("é", "e");
	search.Replace("è", "e");
	search.Replace("ê", "e");

	label.MakeLower();
	label.Replace("é", "e");
	label.Replace("è", "e");
	label.Replace("ê", "e");

	if (label.Find(search) != -1)
		return true;

	return false;
}
