#pragma once
#include "iitemvisitor.h"
#include "IItem.h"

class CImportIVisitor :
	public IItemVisitor
{
public:
	// Constructeur
	CImportIVisitor(void);

	// Destructeur
	virtual ~CImportIVisitor(void);

	// Import avec StdioFile
	virtual JBool Import(CIBuilder *pBuilder, JMap<JInt32, const IItem *> *pImpMap);

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

	JMap<JInt32,const IItem *>	*	m_pImpMap;
};
