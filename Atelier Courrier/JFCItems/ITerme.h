
#pragma once


#include "IBloc.h"

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
class ITerme : public IBloc  
{
public:
	ITerme();
	virtual ~ITerme();

public:
	virtual	JVoid AddTerme(ITerme *pObject) = 0;
	virtual JBool SetID(JInt32) = 0;
	virtual JInt32 GetID() = 0;

};
