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
	search.Replace("�", "e");
	search.Replace("�", "e");
	search.Replace("�", "e");

	label.MakeLower();
	label.Replace("�", "e");
	label.Replace("�", "e");
	label.Replace("�", "e");

	if (label.Find(search) != -1)
		return true;

	return false;
}
