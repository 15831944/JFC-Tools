// VarAgregationManager.cpp: implementation of the CVarAgregationManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VarAgregationManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVarAgregationManager::CVarAgregationManager(JStringEx &Name)
:m_sName(Name)
{
}

CVarAgregationManager::~CVarAgregationManager()
{
}

JBool CVarAgregationManager::HasVarBinValues(JInt32 VarId)
{
	if(GetVariable(VarId))
		return m_mVariables.GetItem()->HasBinValues();
	else
		return 0;
}

JBool CVarAgregationManager::GetVariable(JInt32 VarId)
{
	m_mVariables.MoveTo(VarId);

	if(!m_mVariables.IsValid())
	{
		PTR_VARAGREGATION pItem(new CVarAgregation);

		// Load the variable file
		if(!pItem->Load(m_sName, VarId))
			return false;

		// Insert file into variable set
		m_mVariables.Add(VarId) = pItem;
		m_mVariables.MoveTo(VarId);
	}
	return true;
}

JInt32	CVarAgregationManager::GetVarAgregatDivider(JInt32 VarId)
{
	if(GetVariable(VarId))
		return m_mVariables.GetItem()->GetDivider();
	else
		return 0;

}

const CBitArray *	CVarAgregationManager::GetVarBinValuesByIdx(JInt32 VarId, JInt32 Index)
{
	if(GetVariable(VarId))
		return m_mVariables.GetItem()->GetBinValuesByIdx(Index);
	else
		return 0;
}

const JFLT32VECTOR * CVarAgregationManager::GetVarNumValuesByIdx(JInt32 VarId, JInt32 Index)
{
	if(GetVariable(VarId))
		return m_mVariables.GetItem()->GetNumValuesByIdx(Index);
	else
		return 0;
}

const CBitArray * CVarAgregationManager::GetVarBinValuesByIds(JInt32 VarId, const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds)
{
	if(GetVariable(VarId))
		return m_mVariables.GetItem()->GetBinValuesByIds(DimensionIds,SupportIds);
	else
		return 0;
}

const JFLT32VECTOR * CVarAgregationManager::GetVarNumValuesByIds(JInt32 VarId, const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds)
{
	if(GetVariable(VarId))
		return m_mVariables.GetItem()->GetNumValuesByIds(DimensionIds,SupportIds);
	else
		return 0;
}
