
#pragma once


#include "IItemVisitor.h"
class CItemsConstruitsBase;

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
class CRegisterItemsVisitor : public IItemVisitor  
{
public:
	CRegisterItemsVisitor();
	virtual ~CRegisterItemsVisitor();
	virtual JBool RegisterItems(CIBuilder *pBuilder, CItemsConstruitsBase *pItemConst,JInt32 OwnerID, JInt32 ItemID);

public:
	/// IItemVisitor overloads
	virtual void VisitStart(CIBuilder & Builder) {};
	virtual void VisitEnd(CIBuilder & Builder) {};
	virtual void VisitStart(CIBSegment & Segment) {};
	virtual void VisitEnd(CIBSegment & Segment) {};
	virtual void VisitStart(CIBQuestion & Question) {};
	virtual void VisitEnd(CIBQuestion & Question) {};
	virtual void VisitStart(CIBItem & Item) ;
	virtual void VisitEnd(CIBItem & Item) {};
	virtual void Visit(CIBModalite & Modalite)  {};
	virtual void Visit(CIBNumValue & NumValue) {};
	virtual void Visit(CIBClass & Class)	{};
	virtual void Visit(CIBScore & Score)	{};
	virtual void Visit(CIBQuant & Quant)	{};
	virtual void Visit(CIBPoint & Point)	{};


protected:

	JInt32					m_OwnerID;
	JInt32					m_ItemID;
	JBool					m_Valid;
	CItemsConstruitsBase*	m_pItems;		

};
