#pragma once

#define KEYWORDSIZE 6
const CString keywords[KEYWORDSIZE][2] = {
{"&", "&amp;"},
{"\"", "&quot;"},
{"<", "&lt;"},
{">", "&gt;"},
{"'", "&apos;"},
{"€", "&#x20AC;"},

};

class AFX_EXT_CLASS CStringToXML
{
private:
	CStringToXML(void) {};
	~CStringToXML(void) {};

public:
	static CString TextToXML(CString text)
	{
		for (int i = 0; i < KEYWORDSIZE; i++)
			text.Replace(keywords[i][0],keywords[i][1]);
		return text;
	}

	static CString TextToXML(LPCSTR ptext)
	{
		CString text = ptext;
		for (int i = 0; i < KEYWORDSIZE; i++)
			text.Replace(keywords[i][0],keywords[i][1]);
		return text;
	}

	static CString XMLToText(CString text)
	{
		for (int i = KEYWORDSIZE-1; i >= 0; i--)
			text.Replace(keywords[i][1],keywords[i][0]);
		return text;
	}

	static CString XMLToText(LPCSTR ptext)
	{
		CString text = ptext;
		for (int i = KEYWORDSIZE-1; i >= 0; i--)
			text.Replace(keywords[i][1],keywords[i][0]);
		return text;
	}
};
