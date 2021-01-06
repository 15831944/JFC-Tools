
#pragma once

#pragma warning( disable : 4786 )

#include <memory>

#include "BlocQuestions.h"
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
class CTheme : public ITheme
{
public:

	CTheme();
	virtual ~CTheme();

	JArchive&		Recv( JArchive& lhs, JInt32 Version);
	JBool			FillTree	( CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM hParent, JBool AllLeaves = false, JBool OnlyQuanti = false);

public:
	/// ITheme interface overrides

	/// Gets the label
	virtual const JLabel &	GetLabel		() const { return m_Label; };
	/// Gets the id
	virtual JInt32			GetID			() const { return m_ID; };
	/// Return true if the object is a question bloc
	virtual JBool			IsTheme			() const { return true; } ;

private:

	JInt32					m_ID;
	JLabel					m_Label;
	JMap<JInt32, JInt32>	m_mBlocID;
	BLOCVECTOR				m_vBlocs;

private:
	/// assignment operator
	CTheme & operator =(const CTheme & rhs);
	/// copy constructor
	CTheme(const CTheme & rhs);

};

typedef std::auto_ptr<CTheme> PTR_THEME;
typedef JVector<PTR_THEME> THEMEVECTOR;
