
#pragma once
#include "IBloc.h"
#include "IAbsolutVisitor.h"
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
class CCalcValuesVisitor : public IAbsolutVisitor  
{
public:
	CCalcValuesVisitor();
	virtual ~CCalcValuesVisitor();
	virtual JBool	Calculate(CABuilder *pBuilder, JFLT32VECTOR &Values, JInt32 &Divider, CTerrain * pTerrain);

public:
	/// IAbsolutVisitor overloads
	virtual void VisitStart(CABuilder & Builder){};
	virtual void VisitEnd(CABuilder & Builder){};
	virtual void VisitStart(CABSegment & Segment){};
	virtual void VisitEnd(CABSegment & Segment){};
	virtual void VisitStart(CABQuestion & Question);
	virtual void VisitEnd(CABQuestion & Question){};
	virtual void VisitStart(CABTerme & Terme){};
	virtual void VisitEnd(CABTerme & Terme);
	virtual void VisitStart(CABItem & Item){};
	virtual void VisitEnd(CABItem & Item){};
	virtual void Visit(CABClass & Class){};
	virtual void Visit(CABNumValue & NumValue){};
	virtual void Visit(CABQuant & Quant){};
	virtual void Visit(CABPoint & Point){};
	virtual void Visit(CABScore & Score){};

protected:

	virtual	void GetVarValues(JVector<JFlt32> & Values, JInt32 & Divider);

protected:
	JBool								m_Valid;
	CTerrain *							m_pTerrain;
	const CQuestion *					m_pQuestion;
	JMap< const JFLT32VECTOR *, JInt32>	m_ValuesMap;

	// Vecteur pour calcul Cumul Quanti
	CArray<long, long &>				VectCumulQuanti;

	// Map pour détection doublon eventuel des elmts
	JMap <JInt32, JInt32>				MapIdTerme;
};

