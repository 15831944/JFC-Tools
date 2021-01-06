// GridCellValue.cpp: implementation of the CGridCellValue class.
//
// Written by Jérémy DEVILERS
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCellValue.h"
#include "GridCtrl.h"

IMPLEMENT_DYNCREATE(CGridCellValue, CGridCell)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
CGridCellValue::CGridCellValue()
{
	m_value = 0.0;
}
 
void CGridCellValue::SetValue(double value)
{
	m_value = value;
}

double CGridCellValue::GetValue()
{
	return m_value;
}

