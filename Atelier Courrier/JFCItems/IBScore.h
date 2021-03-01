
#pragma once


#include "IBQuant.h"
#include "IItemVisitor.h"

/**
 * \ingroup JFCITEMS
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
class AFX_EXT_CLASS CIBScore : public CIBQuant  
{
public:
	CIBScore();
	virtual ~CIBScore();

public:
	/// Overrides for the CIBQuant Interface
	/// Accepts a visiting Object
	virtual JVoid		Accept(IItemVisitor &Visitor ) { Visitor.Visit(*this); } ;

public:
	/// Converts this object to a string
	virtual JBool		AsString(std::string & str) const ;

private:
	CIBScore(const CIBScore & rhs);
	CIBScore & operator=(const CIBScore & rhs);

};
