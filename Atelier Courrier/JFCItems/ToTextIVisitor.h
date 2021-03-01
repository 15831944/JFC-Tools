//////////////////////////////////////////////////////////////////////////////////////////
// Création du descriptif construction d'un item
//
#pragma once
#include "iitemvisitor.h"
#include "Terrain.h"
#include "ItemsConstruits.h"

class CToTextIVisitor : public IItemVisitor
{
public:
	CToTextIVisitor(void);
	~CToTextIVisitor(void);
	virtual JBool ToDescription(CIBuilder *pBuilder, CItemsConstruitsBase *pItemConst, CTerrain * pTerrain, CString * pstrDescript);

	virtual void VisitStart(CIBuilder & Builder);
	virtual void VisitEnd(CIBuilder & Builder);
	virtual void VisitStart(CIBSegment & Segment);
	virtual void VisitEnd(CIBSegment & Segment);
	virtual void VisitStart(CIBQuestion & Question);
	virtual void VisitEnd(CIBQuestion & Question);
	virtual void VisitStart(CIBItem & Item) ;
	virtual void VisitEnd(CIBItem & Item) ;
	virtual void Visit(CIBModalite & Modalite);
	virtual void Visit(CIBNumValue & NumValue);
	virtual void Visit(CIBClass & Class);
	virtual void Visit(CIBScore & Score){};
	virtual void Visit(CIBQuant & Quant);
	virtual void Visit(CIBPoint & Point);

protected:

	CString				   m_OpStrate;
	CString				   m_OpQuestion;
	CString				   m_OpModalite;

	CTerrain			 * m_pTerrain;
	CItemsConstruitsBase * m_pIConst;
	CIBuilder			 * m_pBuilder;
	CString				 * m_pstrDescript;

	const CQuestion		 * m_pQuestion;
	int					   m_ModaDim;

	int					   m_NoSegment;	
	int					   m_NoQuestion;
	int					   m_NoModalite;

};
