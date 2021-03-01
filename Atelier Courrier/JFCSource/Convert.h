#pragma once

class AFX_EXT_CLASS Convert
{
public:
	static CString ToSearchString(LPCSTR Search);
	static void ToSearchString(CString & Search);

private:
	Convert(void);
	~Convert(void);
};

