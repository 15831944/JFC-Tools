
#pragma once

#include "IAbsolutVisitor.h"
#include "Terrain.h"

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
class CIsValidVisitor : public IAbsolutVisitor  
{
public:
	CIsValidVisitor();
	virtual ~CIsValidVisitor();

	virtual JBool Validate(CABuilder *pBuilder, CTerrain * pTerrain, CItemsConstruitsBase *pItemConst);

public:
	/// IAbsolutVisitor overloads
	virtual void VisitStart(CABuilder & Builder) {};
	virtual void VisitEnd(CABuilder & Builder) {};
	virtual void VisitStart(CABSegment & Segment) {};
	virtual void VisitEnd(CABSegment & Segment) {};
	virtual void VisitStart(CABQuestion & Question) {};
	virtual void VisitEnd(CABQuestion & Question) {};
	virtual void VisitStart(CABTerme & Terme) ;
	virtual void VisitEnd(CABTerme & Terme) {};
	virtual void VisitStart(CABItem & Item) ;
	virtual void VisitEnd(CABItem & Item) {};
	virtual void Visit(CABNumValue & NumValue) {};
	virtual void Visit(CABClass & Class)	{};
	virtual void Visit(CABScore & Score)	{};
	virtual void Visit(CABQuant & Quant)	{};
	virtual void Visit(CABPoint & Point)	{};

protected:

	JBool					m_Valid;
	CTerrain			*	m_pTerrain;
	CItemsConstruitsBase*	m_pItems;		
};