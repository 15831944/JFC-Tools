
#pragma once

#include "ITerme.h"
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
class CABTerme : public ITerme  
{
public:
	CABTerme();
	virtual ~CABTerme();

public:
	/// Overrides ITerme Interface
	/// Adds a terme to the terme list
	virtual	JVoid AddTerme(ITerme *pObject) { m_Terme.AddTail() = (CABTerme *)pObject; } ;
	/// Gets the objects ID
	virtual JInt32 GetID() { return m_ID; } ;
	/// Sets the object with the given ID
	virtual JBool SetID(JInt32 ID) { m_ID = ID; return ID!=0; } ;

	/// Overrides for the IBloc Interface
	/// Accepts a visiting Object
	virtual JVoid		Accept(IAbsolutVisitor &Visitor );
	/// Resets to zero this object
	virtual JBool		Reset();
	/// Is this object Empty
	virtual JBool		IsEmpty();
	/// Validate this object removing all empty sub objects
	virtual	JBool		Validate();
	/// Validate this object
	virtual	JBool		IsValid() ;
	/// Converts this object to a string
	virtual JBool		AsString(std::string & str) const;
	/// Builds this object from a string
	virtual JBool		FromString(const JChar ** pstr);

public:

protected:
	JInt32					m_ID;
	JList<CABTerme *>		m_Terme;

private:
	CABTerme (const CABTerme & rhs);
	CABTerme & operator=(const CABTerme & rhs);
};

