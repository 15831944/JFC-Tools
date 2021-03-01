// UnRegisterItemsVisitor.cpp: implementation of the CUnRegisterItemsVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnRegisterItemsVisitor.h"
#include "ItemsConstruitsBase.h"

#include "IBItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnRegisterItemsVisitor::CUnRegisterItemsVisitor()
{
	m_OwnerID	=	0;
	m_pItems	=	0;
	m_Valid		=	true;
}

CUnRegisterItemsVisitor::~CUnRegisterItemsVisitor()
{

}

JBool CUnRegisterItemsVisitor::UnRegisterItems(CIBuilder *pBuilder, CItemsConstruitsBase *pItemConst,JInt32 OwnerID)
{
	m_OwnerID	=	OwnerID;
	m_pItems	=	pItemConst;
	m_Valid		=	true;
	pBuilder->Accept(*this);
	return m_Valid ;
}

void CUnRegisterItemsVisitor::VisitStart(CIBItem & Item)
{
	JList<JInt32> Values;
	Item.GetValues(Values);

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		m_pItems->UnRegisterItemConstruit(Values.GetItem(),m_OwnerID);
	}
}
