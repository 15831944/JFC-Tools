
#pragma once


#include "IItemVisitor.h"

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
class CQuestionVisitor : public IItemVisitor  
{
public:
	CQuestionVisitor();
	virtual ~CQuestionVisitor();
	virtual JBool GetQuestionsID(CIBSegment *pSegment, JList<JInt32> &ItemID);
	virtual CIBQuestion * FindQuestion(CIBSegment *pSegment, JInt32 Quid);
	virtual void GetModalites(CIBQuestion *pQuestion, JList<CIBModalite *> &Modalites);
	virtual JBool HasQuestions(CIBSegment *pSegment);

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

	JInt32					m_ID;
	JList<JInt32>	*		m_pItemID;
	CIBQuestion *			m_pQuestion;
	JList<CIBModalite *> *	m_pModalites;
	JBool					m_bQuestions;
};
