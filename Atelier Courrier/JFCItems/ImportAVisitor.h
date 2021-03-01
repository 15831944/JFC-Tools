#pragma once
#include "IAbsolutVisitor.h"
#include "IItem.h"

class CImportAVisitor :
	public IAbsolutVisitor
{
public:
	CImportAVisitor(void);
	virtual ~CImportAVisitor(void);
	virtual JBool Import(CABuilder *pBuilder, JMap<JInt32, const IItem *> *pImpMap);

public:
	/// IItemVisitor overloads
	virtual void VisitStart(CABuilder & Builder) {};
	virtual void VisitEnd(CABuilder & Builder) {};
	virtual void VisitStart(CABSegment & Segment) {};
	virtual void VisitEnd(CABSegment & Segment) {};
	virtual void VisitStart(CABQuestion & Question) {};
	virtual void VisitEnd(CABQuestion & Question) {};
	virtual void VisitStart(CABTerme & Terme) {};
	virtual void VisitEnd(CABTerme & Terme) {};
	virtual void VisitStart(CABItem & Item) ;
	virtual void VisitEnd(CABItem & Item) {};
	virtual void Visit(CABNumValue & NumValue) {};
	virtual void Visit(CABClass & Class)	{};
	virtual void Visit(CABScore & Score)	{};
	virtual void Visit(CABQuant & Quant)	{};
	virtual void Visit(CABPoint & Point)	{};


protected:

	JMap<JInt32,const IItem *>	*	m_pImpMap;
};
