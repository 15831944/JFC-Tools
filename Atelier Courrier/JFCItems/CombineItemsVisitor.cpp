// CombineItemsVisitor.cpp: implementation of the CCombineItemsVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CombineItemsVisitor.h"
#include "IBQuestion.h"
#include "IBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCombineItemsVisitor::CCombineItemsVisitor()
{
	m_bGood = true;
	m_Quid = 0;
	m_ListModalites.Reset();
}

CCombineItemsVisitor::~CCombineItemsVisitor()
{

}

// Combines an Ibuilder with an existing configuration
JBool CCombineItemsVisitor::Combine(CIBuilder &IBuilder)
{
	m_bGood = true;
	IBuilder.Accept(*this);
	return m_bGood;
}

// returns true if the IBuilder has multiple modalites or elements
JBool CCombineItemsVisitor::IsComposed(CIBuilder &IBuilder)
{
	IBuilder.Accept(*this);

	if(m_ListModalites.GetCount() > 1 /*|| m_ListElements.GetCount() > 1*/)
		return true;
	return false;
}

void CCombineItemsVisitor::GetItemBuilder(CIBuilder & IBuilder)
{
	IBuilder.Reset();
	IBuilder.SetOperator(IBloc::Or);

	// New segment
	CIBSegment * pSegment = new CIBSegment;
	pSegment->SetOperator(IBloc::And);

	// New question
	CIBQuestion * pQuestion = new CIBQuestion;
	pQuestion->SetID(m_Quid);

	// Get new modalites (if present)
	if(m_ListModalites.GetCount())
	{
		CIBModalite * pModalite = new CIBModalite;
		pModalite->SetOperator(IBloc::Or);
		pModalite->SetValues(m_ListModalites); 
		pQuestion->AddModalite(pModalite); 
	}

	pSegment->Add(pQuestion);
	IBuilder.Add(pSegment);
}

void CCombineItemsVisitor::VisitStart(CIBQuestion & Question)
{
	if(m_Quid && m_Quid != Question.GetID())
		m_bGood = false;
	m_Quid = Question.GetID();
}

void CCombineItemsVisitor::Visit(CIBModalite & Modalite)
{
	if(m_bGood)
	{
		JList<JInt32> Values;
		Modalite.GetValues(Values);

		for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
		{
			m_ListModalites.AddTail() = Values.GetItem();
		}
	}
}
