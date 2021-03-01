#include "StdAfx.h"
#include "profilecrosstable.h"

CProfileCrossTable::CProfileCrossTable(void)
: CCrossTable()
{
}

CProfileCrossTable::~CProfileCrossTable(void)
{
}

void CProfileCrossTable::GetFormattedResult(CString & ResultOut, double ResultIn, RESULTSTYLE Style, RESULTTYPE Type, int RowItem, int ColItem, int BaseRowItem, int BaseColItem, bool DivZero, double NbCas, bool MargeErreurDepasse, bool ItemQuanti, bool MoyQuantiRow, bool MoyQuantiCol)
{
	CCrossTable::GetFormattedResult(ResultOut, ResultIn, Style, Type, RowItem, ColItem, BaseRowItem, BaseColItem, DivZero, NbCas, MargeErreurDepasse, ItemQuanti,MoyQuantiRow, MoyQuantiCol);
}
