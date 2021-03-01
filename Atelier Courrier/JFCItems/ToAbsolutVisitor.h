
#pragma once


#include "IItemVisitor.h"
#include "ABuilder.h"
#include "ABSegment.h"
#include "ABQuestion.h"
#include "ABTerme.h"
#include "ABNumValue.h"
#include "Terrain.h"

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
class CToAbsolutVisitor : public IItemVisitor  
{
public:
	CToAbsolutVisitor();
	virtual ~CToAbsolutVisitor();

	/// Construct an Absolut Builder from an Item Builder
	virtual JBool ToAbsolut(CIBuilder *pIBuilder, CABuilder *pABuilder, CTerrain * pTerrain);

public:
	/// IItemVisitor overloads
	virtual void VisitStart(CIBuilder & Builder);
	virtual void VisitEnd(CIBuilder & Builder);
	virtual void VisitStart(CIBSegment & Segment);
	virtual void VisitEnd(CIBSegment & Segment);
	virtual void VisitStart(CIBQuestion & Question);
	virtual void VisitEnd(CIBQuestion & Question);
	virtual void VisitStart(CIBItem & Item);
	virtual void VisitEnd(CIBItem & Item);
	virtual void Visit(CIBModalite & Modalite);
	virtual void Visit(CIBNumValue & NumValue);
	virtual void Visit(CIBClass & Class);
	virtual void Visit(CIBScore & Score);
	virtual void Visit(CIBQuant & Quant);
	virtual void Visit(CIBPoint & Point);

protected:
	virtual JBool		SetTermes(ITerme *pTerme,JVector<JInt32> &Values ,JInt32 Index);

	CABuilder	*				m_pABuilder;
	CABSegment	*				m_pCurSegment;
	CABQuestion	*				m_pCurQuestion;
	CABItem		*				m_pCurItem;
	JInt32						m_QID;
	JList<IBloc::OPERATOR>		m_Operators;
	JList< JList<JInt32> >		m_Elements;
	CABTerme	*				m_pCurTerme;
	CTerrain	*				m_pTerrain;
	JBool						m_Valid;
};
