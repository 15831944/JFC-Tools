// QuestionVisitor.cpp: implementation of the CQuestionVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestionVisitor.h"
#include "IBSegment.h"
#include "IBQuestion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestionVisitor::CQuestionVisitor()
{
	m_pItemID = 0;
	m_pQuestion = 0;
	m_bQuestions = false;
	m_ID = 0;
	m_pModalites = 0;
}

CQuestionVisitor::~CQuestionVisitor()
{

}

JBool CQuestionVisitor::GetQuestionsID(CIBSegment *pSegment, JList<JInt32> &ItemID)
{
	m_pItemID = &ItemID;
	m_pItemID->Reset();
	pSegment->Accept(*this);
	return true;
}

CIBQuestion * CQuestionVisitor::FindQuestion(CIBSegment *pSegment, JInt32 Quid)
{
	m_pItemID = 0;
	m_pQuestion = 0;
	m_ID = Quid;
	pSegment->Accept(*this);
	return m_pQuestion;
}

JBool CQuestionVisitor::HasQuestions(CIBSegment *pSegment)
{
	m_bQuestions = false;
	pSegment->Accept(*this);
	return m_bQuestions;
}

void CQuestionVisitor::VisitStart(CIBQuestion & Question)
{
	m_bQuestions = true;
	if(m_pItemID && Question.IsValid())
		m_pItemID->AddTail() = Question.GetID();

	if(m_ID && m_ID == Question.GetID())
	{
		m_pQuestion = &Question;
	}
}

void CQuestionVisitor::GetModalites(CIBQuestion *pQuestion, JList<CIBModalite *> &Modalites)
{
	m_pModalites = &Modalites;
	m_pModalites->Reset();
	pQuestion->Accept(*this);
}

void CQuestionVisitor::Visit(CIBModalite & Modalite)
{
	if(m_pModalites)
		m_pModalites->AddTail() = &Modalite;
}

