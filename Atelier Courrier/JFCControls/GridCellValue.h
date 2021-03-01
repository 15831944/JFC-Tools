// GridCellValue.h: interface for the CGridCellValue class.
//
// Written by Jeremy DEVILERS
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDVALUECELL_H__3479ED0D_B57D_4940_B83D_9E2296ED75B5__INCLUDED_)
#define AFX_GRIDVALUECELL_H__3479ED0D_B57D_4940_B83D_9E2296ED75B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCell.h"

class AFX_EXT_CLASS CGridCellValue : public CGridCell  
{
    DECLARE_DYNCREATE(CGridCellValue)
public:
    CGridCellValue();
	void SetValue(double value);
	double GetValue();

private:
	double m_value;
};

#endif // !defined(AFX_GRIDVALUECELL_H__3479ED0D_B57D_4940_B83D_9E2296ED75B5__INCLUDED_)
