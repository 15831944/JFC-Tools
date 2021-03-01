
#pragma once

#include "IBloc.h"
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
class AFX_EXT_CLASS CIBSegment : public IBloc  
{
public:
	CIBSegment();
	virtual ~CIBSegment();

	JVoid			Add(IBloc *pBloc) { m_lBloc.AddTail() = pBloc; } ;
	JVoid			GetQuestionsID(JList<JInt32> & Items);
	JVoid			GetItemsID(JList<JInt32> & Items);
	CIBQuestion *	FindQuestion(JInt32 Quid);
	CIBItem *		FindItem(JInt32 Itid=0);
	JBool			RemoveQuestion(CIBQuestion *pQuestion);
	JBool			RemoveItem(CIBItem *pItem);
	JBool			DeleteQuestion(CIBQuestion *pQuestion);
	JBool			DeleteItem(CIBItem *pItem);
	JBool			HasQuestions();
	JBool			HasItems();
public:
	/// Overrides for the IBloc Interface
	/// Accepts a visiting Object
	virtual JVoid		Accept(IItemVisitor &Visitor );
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

protected:
	JList<IBloc *>		m_lBloc;

private:
	CIBSegment(const CIBSegment & rhs);
	CIBSegment & operator=(const CIBSegment & rhs);

};
