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
	xSearch.Replace('é', 'e');
	xSearch.Replace('è', 'e');
	xSearch.Replace('ê', 'e');
	xSearch.Replace('â', 'a');
	xSearch.Replace('ô', 'o');
	xSearch.Replace('ï', 'i');
	xSearch.Replace('ç', 'c');
	xSearch.Replace('û', 'u');

	return xSearch;
}

void Convert::ToSearchString(CString & Search)
{
	Search.MakeLower();
	Search.Replace('é', 'e');
	Search.Replace('è', 'e');
	Search.Replace('ê', 'e');
	Search.Replace('â', 'a');
	Search.Replace('ô', 'o');
	Search.Replace('ï', 'i');
	Search.Replace('ç', 'c');
	Search.Replace('û', 'u');
}
