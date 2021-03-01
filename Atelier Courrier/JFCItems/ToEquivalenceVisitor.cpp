// ToEquivalenceVisitor.cpp: implementation of the CToEquivalenceVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToEquivalenceVisitor.h"
#include "ABuilder.h"
#include "ABQuestion.h"
#include "ABItem.h"
#include "ABNumValue.h"
#include "ItemsConstruitsBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToEquivalenceVisitor::CToEquivalenceVisitor()
{

}

CToEquivalenceVisitor::~CToEquivalenceVisitor()
{

}

// Convert the Reperes to equivalence reperes
JBool CToEquivalenceVisitor::Convert(CABuilder *pBuilder, CTerrain * pTerrain, CItemsConstruitsBase *pItemConst)
{
	m_pTerrain	=	pTerrain ;
	m_pItems	=	pItemConst ;
	m_Valid		=	true;
	pBuilder->Accept(*this);
	return m_Valid ;
}

void CToEquivalenceVisitor::VisitStart(CABTerme & Terme)
{
	if(Terme.HasOperator())
	{
		// Does this repere exist for this terrain
		if(!m_pTerrain->HasRepere(Terme.GetID()))
		{
			// Does it have an equivalence
			if(m_pTerrain->HasEquivalence(Terme.GetID()))
			{
				// Get equivalence
				const JChar * pStr = m_pTerrain->GetEquivalence(Terme.GetID());
				pStr++;
				Terme.FromString(&pStr); 
			}
			else
				// No equivalence
				m_Valid = false;
		}
	}
}

