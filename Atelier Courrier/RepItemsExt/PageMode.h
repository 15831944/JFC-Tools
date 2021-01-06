#pragma once
#include "linemode.h"

class AFX_EXT_CLASS CPageMode : public CLineMode
{
public:
					CPageMode(CDlgItems *pDlgItems);
	virtual			~CPageMode(void);

public:
	// *************************************************************************************************************************
	// **************************** Abstract base class overrides **************************************************************
	// *************************************************************************************************************************
	// StartMode Mode
	void	StartMode();
	// returns the current modes label
	void	SetModeTitle(void);

protected:
	// *************************************************************************************************************************
	// **************************** LineMode base class overrides **************************************************************
	// *************************************************************************************************************************
	/// Returns a string to the current dimension mode
	CString GetDimensionModeString(int Mode);
};
