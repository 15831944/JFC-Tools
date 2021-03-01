
#pragma once

#include "ABQuant.h"
#include "IAbsolutVisitor.h"

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
class CABScore : public CABQuant  
{
public:
	CABScore();
	virtual ~CABScore();

public:
	/// Overrides for the IBloc Interface
	/// Accepts a visiting Object
	virtual JVoid		Accept(IAbsolutVisitor &Visitor ) { Visitor.Visit(*this); } ;
	/// Converts this object to a string
	virtual JBool		AsString(std::string & str) const ;

private:
	CABScore(const CABScore & rhs);
	CABScore & operator=(const CABScore & rhs);

};

