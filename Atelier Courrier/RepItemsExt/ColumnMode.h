#pragma once
#include "linemode.h"

class AFX_EXT_CLASS CColumnMode :	public CLineMode
{
public:
					CColumnMode(CDlgItems *pDlgItems); 
	virtual			~CColumnMode(void);

public:
	// *************************************************************************************************************************
	// **************************** Abstract base class overrides **************************************************************
	// *************************************************************************************************************************
	/// StartMode the Mode
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
