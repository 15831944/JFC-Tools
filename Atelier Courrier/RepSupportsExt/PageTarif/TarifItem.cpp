// TarifItem.cpp: implementation of the TarifItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TarifItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TarifItem::TarifItem()
{
}

TarifItem::TarifItem(JUnt32 id, const JChar* lbl, CTarifManuel* trf)
{
	m_ID = id;
	m_Label = lbl;
	m_pTarif = trf;
}

TarifItem::~TarifItem()
{
}

//JVoid TarifItem::SetID(JUnt32 id)
//{
//	m_ID = id;
//}

JUnt32 TarifItem::GetID() const
{
	return m_ID;
}

const JLabel & TarifItem::GetLabel() const
{
	return m_Label;
}

//JVoid TarifItem::SetLabel(const JChar* lbl)
//{
//	m_Label = lbl;
//}

CTarifManuel* TarifItem::GetTarifManuel() const
{
	return m_pTarif;
}