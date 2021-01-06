
#pragma once


#include "IAbsolutVisitor.h"
#include "IBuilder.h"
#include "IBSegment.h"
#include "IBQuestion.h"
#include "Terrain.h"
#include "Question.h"

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
class CToItemVisitor : public IAbsolutVisitor  
{
public:
	CToItemVisitor();
	virtual ~CToItemVisitor();
	/// Construct an Item Builder from an Absolut Builder
	virtual JBool ToItem(CABuilder *pABuilder, CIBuilder *pIBuilder, CTerrain * pTerrain);

public:
	/// IItemVisitor overloads

	virtual void VisitStart(CABuilder & Builder) ;
	virtual void VisitEnd(CABuilder & Builder) ;
	virtual void VisitStart(CABSegment & Segment) ;
	virtual void VisitEnd(CABSegment & Segment) ;
	virtual void VisitStart(CABQuestion & Question) ;
	virtual void VisitEnd(CABQuestion & Question) ;
	virtual void VisitStart(CABTerme & Terme) ;
	virtual void VisitEnd(CABTerme & Terme) ;
	virtual void VisitStart(CABItem & Item) ;
	virtual void VisitEnd(CABItem & Item) ;
	virtual void Visit(CABNumValue & NumValue) ;
	virtual void Visit(CABClass & Class)	;
	virtual void Visit(CABScore & Score)	;
	virtual void Visit(CABQuant & Quant)	;
	virtual void Visit(CABPoint & Point)	;

protected:

	CIBuilder	*					m_pIBuilder;
	CIBSegment	*					m_pCurSegment;
	CIBQuestion	*					m_pCurQuestion;
	CIBItem		*					m_pCurItem;
	JList<IBloc::OPERATOR>			m_Operators;
	JInt32							m_OperatorLevel;
	JList< JMap<JInt32,JBool> >		m_Elements;
	CTerrain	*					m_pTerrain;
	JInt32							m_QID;
	bool							m_bError;
};
