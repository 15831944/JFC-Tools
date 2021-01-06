
#pragma once


#pragma warning( disable : 4786 )

#include <memory>

#include "Theme.h"
#include "ITheme.h"

/**
 * \ingroup JFCSOURCE
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-09-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class CGrandTheme  : public ITheme
{
public:

	CGrandTheme();
	virtual ~CGrandTheme();
	/// Loads the object
	JArchive &		Recv		( JArchive& lhs, JInt32 Version);
	/// Fills a CTreeCtrl with the index thematique
	JBool			FillTree	( CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM hParent, JBool AllLeaves = false, JBool OnlyQuanti = false);

public:
	/// ITheme interface overrides

	/// Gets the label
	virtual const JLabel &	GetLabel		() const { return m_Label; };
	/// Gets the id
	virtual JInt32			GetID			() const { return m_ID; };
	/// Return true if the object is a question bloc
	virtual JBool			IsGrandTheme	() const { return true; } ;

private:

	JInt32						m_ID;
	JLabel						m_Label;
	THEMEVECTOR					m_vThemes;
	JMap<JInt32, JInt32>		m_mThemeID;
	
private:
	/// assignment operator
	CGrandTheme & operator =(const CGrandTheme & rhs);
	/// copy constructor
	CGrandTheme(const CGrandTheme & rhs);

};

typedef std::auto_ptr<CGrandTheme> PTR_GRANDTHEME;
typedef JVector<PTR_GRANDTHEME> GRANDTHEMEVECTOR;
