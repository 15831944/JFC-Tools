#include "stdafx.h"
#include "convert.h"

Convert::Convert(void)
{
}

Convert::~Convert(void)
{
}

CString Convert::ToSearchString(LPCSTR Search)
{
	CString xSearch = Search;

	xSearch.MakeLower();
	xSearch.Replace('�', 'e');
	xSearch.Replace('�', 'e');
	xSearch.Replace('�', 'e');
	xSearch.Replace('�', 'a');
	xSearch.Replace('�', 'o');
	xSearch.Replace('�', 'i');
	xSearch.Replace('�', 'c');
	xSearch.Replace('�', 'u');

	return xSearch;
}

void Convert::ToSearchString(CString & Search)
{
	Search.MakeLower();
	Search.Replace('�', 'e');
	Search.Replace('�', 'e');
	Search.Replace('�', 'e');
	Search.Replace('�', 'a');
	Search.Replace('�', 'o');
	Search.Replace('�', 'i');
	Search.Replace('�', 'c');
	Search.Replace('�', 'u');
}
