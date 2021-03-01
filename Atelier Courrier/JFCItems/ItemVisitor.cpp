// ItemVisitor.cpp: implementation of the CItemVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemVisitor.h"
#include "IBSegment.h"
#include "IBItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemVisitor::CItemVisitor()
{
	m_pItemID = 0;
	m_pItem = 0;
	m_bItems = false;
	m_ID = 0;
}

CItemVisitor::~CItemVisitor()
{

}


JBool CItemVisitor::GetItemsID(CIBSegment *pSegment, JList<JInt32> &ItemID)
{
	m_pItemID = &ItemID;
	m_pItemID->Reset();
	pSegment->Accept(*this);
	return true;
}

CIBItem * CItemVisitor::FindItem(CIBSegment *pSegment, JInt32 Itid)
{
	m_pItemID = 0;
	m_pItem = 0;
	m_ID = Itid;
	if (pSegment)
		pSegment->Accept(*this);
	return m_pItem;
}

JBool CItemVisitor::HasItems(CIBSegment *pSegment)
{
	m_pItemID = 0;
	m_pItem = 0;
	m_bItems = false;
	if (pSegment)
		pSegment->Accept(*this);
	return m_bItems;
}

void CItemVisitor::VisitStart(CIBItem & Item)
{
	m_bItems = true;

	if(m_pItemID && Item.IsValid())
		m_pItemID->AddTail() = Item.GetID();

	if(m_ID == Item.GetID())
		m_pItem = &Item;

}
