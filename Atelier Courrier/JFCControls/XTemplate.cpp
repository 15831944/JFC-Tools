
#include "stdafx.h"

#include "XTemplate.h"

HXTemplateHandle CXTemplateManager::m_hxSystemTemplate = -1;

CXTemplateManager::CXTemplateManager()
{
}

CXTemplateManager::~CXTemplateManager()
{
}

// initialisation du singleton
CXTemplateManager * CXTemplateManager::pInstance = 0;

CXTemplateManager * CXTemplateManager::GetInstance()
{
	// on retourne et/ou crée le singleton
	return (pInstance ? pInstance : pInstance = new CXTemplateManager);
}

void CXTemplateManager::Destroy()
{
	// on detruit le singleton
	if (pInstance) delete pInstance;
	pInstance = 0;
}

HXTemplateHandle CXTemplateManager::CreateTemplate(const CXTemplate Template)
{	
	// on ajoute un nouveau template
	m_Templates.push_back(Template);

	// on retourne son index
	return static_cast<HXTemplateHandle>(m_Templates.size());
}

HXTemplateHandle CXTemplateManager::CreateTemplate(COLORREF textcolor, COLORREF backcolor, COLORREF seltextcolor, COLORREF selbackcolor, bool bBold /*= false*/, bool bItalic /*= false*/, bool bUnderline /*= false*/ )
{
	CXTemplate Template;

	// on assigne les couleurs
	Template.textcolor_		= textcolor;
	Template.backcolor_		= backcolor;
	Template.seltextcolor_	= seltextcolor;
	Template.selbackcolor_	= selbackcolor;

	// on crée le masque de formattage de texte
	Template.textformat_	= 0;
	if (bBold)		Template.textformat_ |= CXTemplate::Bold;
	if (bItalic)	Template.textformat_ |= CXTemplate::Italic;
	if (bUnderline) Template.textformat_ |= CXTemplate::Underline;

	return CreateTemplate(Template);
}

const CXTemplate CXTemplateManager::GetTemplate(const HXTemplateHandle hTemplate) const
{
	// on récupère l'index correspondant au handle du template
	const size_t IdxTemplate = static_cast<size_t>(hTemplate - 1);

	// on verifie la validité de l'index
	ASSERT(IdxTemplate < m_Templates.size());

	// on retourne le template
	return m_Templates[IdxTemplate];	
}

const HXTemplateHandle CXTemplateManager::GetSystemTemplate()
{
	if (m_hxSystemTemplate == -1)
	{
		// on récupère les couleurs system
		COLORREF ColorWindow        = ::GetSysColor(COLOR_WINDOW);
		COLORREF ColorHighlight     = ::GetSysColor(COLOR_HIGHLIGHT);
		COLORREF ColorWindowText    = ::GetSysColor(COLOR_WINDOWTEXT);
		COLORREF ColorHighlightText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);

		// création du template system
		m_hxSystemTemplate = CXTemplateManager::GetInstance()->CreateTemplate(ColorWindowText, ColorWindow, ColorHighlightText, ColorHighlight);
	}

	return m_hxSystemTemplate;
}