
#pragma once


class CABuilder;
class CABSegment;
class CABQuestion;
class CABTerme;
class CABNumValue;
class CABClass;
class CABScore;
class CABQuant;
class CABItem;
class CABPoint;

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
class IAbsolutVisitor  
{
public:
	IAbsolutVisitor();
	virtual ~IAbsolutVisitor();

	virtual void VisitStart(CABuilder & Builder) = 0;
	virtual void VisitEnd(CABuilder & Builder) = 0;
	virtual void VisitStart(CABSegment & Segment) = 0;
	virtual void VisitEnd(CABSegment & Segment) = 0;
	virtual void VisitStart(CABQuestion & Question) = 0;
	virtual void VisitEnd(CABQuestion & Question) = 0;
	virtual void VisitStart(CABTerme & Terme) = 0;
	virtual void VisitEnd(CABTerme & Terme) = 0;
	virtual void VisitStart(CABItem & Item) = 0;
	virtual void VisitEnd(CABItem & Item) = 0;
	virtual void Visit(CABNumValue & NumValue) = 0;
	virtual void Visit(CABClass & Class) = 0;
	virtual void Visit(CABScore & Score) = 0;
	virtual void Visit(CABQuant & Quant) = 0;
	virtual void Visit(CABPoint & Point) = 0;
};
