
#pragma once

#include "IItemVisitor.h"

/// This class should only be used with Critere Modifies class items
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
class CCombineItemsVisitor : public IItemVisitor  
{
public:
	CCombineItemsVisitor();
	virtual ~CCombineItemsVisitor();
	/// Combines an Ibuilder with an existing configuration
	virtual JBool Combine(CIBuilder &IBuilder);
	/// returns true if the IBuilder has multiple modalites or elements
	virtual JBool IsComposed(CIBuilder &IBuilder);
	/// Fills the Item builder with the combines items
	virtual void GetItemBuilder(CIBuilder & IBuilder);

public:
	/// IItemVisitor overloads
	virtual void VisitStart(CIBuilder & Builder) {};
	virtual void VisitEnd(CIBuilder & Builder) {};
	virtual void VisitStart(CIBSegment & Segment) {};
	virtual void VisitEnd(CIBSegment & Segment) {};
	virtual void VisitStart(CIBQuestion & Question) ;
	virtual void VisitEnd(CIBQuestion & Question) {};
	virtual void VisitStart(CIBItem & Item) {};
	virtual void VisitEnd(CIBItem & Item) {};
	virtual void Visit(CIBModalite & Modalite);
	virtual void Visit(CIBNumValue & NumValue) {};
	virtual void Visit(CIBClass & Class)	{};
	virtual void Visit(CIBScore & Score)	{};
	virtual void Visit(CIBQuant & Quant)	{};
	virtual void Visit(CIBPoint & Point)	{};

protected:

	/// Flag if visit was good
	JBool	m_bGood;
	/// Question ID
	JInt32	m_Quid;
	/// List of modalité IDs
	JList<JInt32>	m_ListModalites;
	/// List of Element IDs
	JList<JInt32>	m_ListElements;

};

