// RegisterItemsVisitor.cpp: implementation of the CRegisterItemsVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegisterItemsVisitor.h"
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

CRegisterItemsVisitor::CRegisterItemsVisitor()
{
	m_OwnerID	=	0;
	m_ItemID	=	0;
	m_pItems	=	0;
	m_Valid		=	true;
}

CRegisterItemsVisitor::~CRegisterItemsVisitor()
{

}

JBool CRegisterItemsVisitor::RegisterItems(CIBuilder *pBuilder, CItemsConstruitsBase *pItemConst,JInt32 OwnerID, JInt32 ItemID)
{
	m_OwnerID	=	OwnerID;
	m_ItemID	=	ItemID;
	m_pItems	=	pItemConst;
	m_Valid		=	true;
	pBuilder->Accept(*this);
	return m_Valid ;
}

void CRegisterItemsVisitor::VisitStart(CIBItem & Item)
{
	JList<JInt32> Values;
	Item.GetValues(Values);

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		m_pItems->RegisterItemConstruit(m_pItems->GetItemConstruit(Values.GetItem()),m_OwnerID);
	}
}
