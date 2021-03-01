
#pragma once

class CIBuilder;
class CIBSegment;
class CIBQuestion;
class CIBModalite;
class CIBNumValue;
class CIBClass;
class CIBScore;
class CIBQuant;
class CIBItem;
class CIBPoint;


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
class IItemVisitor  
{
public:
	IItemVisitor();
	virtual ~IItemVisitor();

	virtual void VisitStart(CIBuilder & Builder) = 0;
	virtual void VisitEnd(CIBuilder & Builder) = 0;
	virtual void VisitStart(CIBSegment & Segment) = 0;
	virtual void VisitEnd(CIBSegment & Segment) = 0;
	virtual void VisitStart(CIBQuestion & Question) = 0;
	virtual void VisitEnd(CIBQuestion & Question) = 0;
	virtual void VisitStart(CIBItem & Item) = 0;
	virtual void VisitEnd(CIBItem & Item) = 0;
	virtual void Visit(CIBModalite & Modalite) = 0;
	virtual void Visit(CIBNumValue & NumValue) = 0;
	virtual void Visit(CIBClass & Class) = 0;
	virtual void Visit(CIBScore & Score) = 0;
	virtual void Visit(CIBQuant & Quant) = 0;
	virtual void Visit(CIBPoint & Point) = 0;
};
