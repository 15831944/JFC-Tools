
#pragma once

#pragma warning( disable : 4786 )

#include <memory>

#include "IModalite.h"

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
class CElement : public IModalite 
{
public:
	CElement();
	virtual ~CElement();

public:
	/// IModalite interface overrides
	/// Return a pointer to this const object
	virtual const CElement *		AsElement() const { return this; } ;
	/// Return a pointer to this object
	virtual CElement *				AsElement() { return this; } ;

	virtual JBool			IsElement()	const { return true; } ;
	virtual JBool			IsSupport()	const { return (m_Type != 0); } ;
	virtual const JLabel &	GetLabel()	const { return m_Label; } ;
	virtual JInt32			GetID()		const { return m_ID; } ;
	virtual JUnt32			GetContingentMask()	const { return m_Mask; } ;
	virtual JBool			ToItemString(JStringEx &String) const ;
	/// Check valid strate
	virtual JBool			IsInContingent(JUnt32 ContingentMask) const { if(ContingentMask&m_Mask) return true; else return false; } ;

public:
	JArchive& Recv(JArchive& lhs, JInt32 Version);

private:

	JInt32			m_ID;
	JUnt32			m_Mask;
	JUnt32			m_Type;
	JLabel			m_Label;
	
private:

	/// assignment operator
	CElement & operator =(const CElement & rhs);
	/// copy constructor
	CElement(const CElement & rhs);
};

typedef std::auto_ptr<CElement> PTR_ELEMENT;
