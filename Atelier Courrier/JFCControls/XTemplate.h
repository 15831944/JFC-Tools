#pragma once


#include <vector>

// XListbox template management

struct CXTemplate
{
	COLORREF textcolor_;
	COLORREF backcolor_;
	COLORREF seltextcolor_;
	COLORREF selbackcolor_;

	// faire un mask avec les valeurs Bold, Italic et Undelrine 
	BYTE	textformat_;

	enum
	{
		Bold		=	0x2,
		Italic		=	0x4,
		Underline	=	0x8
	};
};

typedef unsigned int HXTemplateHandle;

class AFX_EXT_CLASS CXTemplateManager
{
public:

	static CXTemplateManager * GetInstance();		
	static void Destroy();

	HXTemplateHandle CreateTemplate(const CXTemplate Template);

	HXTemplateHandle CreateTemplate(
		COLORREF textcolor,	COLORREF backcolor,
		COLORREF seltextcolor, COLORREF selbackcolor,
		bool bBold = false,
		bool bItalic = false,
		bool bUnderline = false);

	const CXTemplate GetTemplate(const HXTemplateHandle) const;

	static const HXTemplateHandle GetSystemTemplate();
	static const HXTemplateHandle InvalidHandle = -1;

private:

	CXTemplateManager();
	~CXTemplateManager();

	static CXTemplateManager * pInstance;

	typedef std::vector<CXTemplate> TemplateTable;
	typedef TemplateTable::iterator Iter;
	TemplateTable m_Templates;

	static HXTemplateHandle	m_hxSystemTemplate;
};
