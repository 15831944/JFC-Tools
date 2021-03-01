#pragma once
#include "ATPColors.h"

// CBtnSTLabel

class CBtnSTLabel : public CButton
{
	DECLARE_DYNAMIC(CBtnSTLabel)

public:
	CBtnSTLabel();
	virtual ~CBtnSTLabel();

	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


	virtual CBtnSTLabel& SetFontBold(BOOL bBold);
	virtual CBtnSTLabel& SetFontName(const CString& strFont, BYTE byCharSet = ANSI_CHARSET);
	virtual CBtnSTLabel& SetFontUnderline(BOOL bSet);
	virtual CBtnSTLabel& SetFontItalic(BOOL bSet);
	virtual CBtnSTLabel& SetFontSize(int nSize);

protected:
	DECLARE_MESSAGE_MAP()
	void UpdateSurface();
	void ReconstructFont();

	CFont			m_font;
private:
	LOGFONT			m_lf;

};


