#pragma once

#include "crosstable.h"

class CProfileCrossTable :	public CCrossTable
{
public:
	CProfileCrossTable(void);
	virtual ~CProfileCrossTable(void);

public:
	// Gets the Formatted Cell Result String
	virtual void	GetFormattedResult(CString & ResultOut, double ResultIn, RESULTSTYLE Style, RESULTTYPE Type, int RowItem, int ColItem, int BaseRowItem, int BaseColItem, bool DivZero, double NbCas, bool MargeErreurDepasse, bool ItemQuanti, bool RowMoyennable, bool ColMoyennable);
};
