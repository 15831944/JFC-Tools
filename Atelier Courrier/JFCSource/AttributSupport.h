#pragma once
#include <memory>
#include "VarAgrSupport.h"

/**
 * \ingroup JFCSOURCE EXTERNAL
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
class AFX_EXT_CLASS CAttributSupport
{
public:
	CAttributSupport(void);
	virtual ~CAttributSupport(void);
	JArchive &				Recv( JArchive& lhs, JInt32 Version, JInt32 NbDimensions);
	const JINT32VECTOR &	GetAttributs() { return m_vAttributs; } ;
	/**
	 * Gets the Dimension Support ID
	 * \return the Dimension Support ID 
	 */
	const JINT32VECTOR & GetIDs() const { return m_Ids; }

private:
	JInt32				m_NbAttributs;
	/// Support IDs
	JINT32VECTOR		m_Ids; 
	/// Attribts
	JINT32VECTOR		m_vAttributs; 

private:

	/// assignment operator
	CAttributSupport & operator =(const CAttributSupport & rhs);
	/// copy constructor
	CAttributSupport(const CAttributSupport & rhs);
};

typedef std::auto_ptr<CAttributSupport> PTR_ATTRIBUTSUPPORT;
typedef JList<PTR_ATTRIBUTSUPPORT> ATTRIBUTSUPPORTLIST;
// Map of CVarAgrSupport pointers
typedef JMap<CDimSupportKey,const CAttributSupport *> ATTRIBUTSUPPORTMAP;
