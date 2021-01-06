#pragma once

#include "JFControl.h"

class AFX_EXT_CLASS JFCTruc : public JFControl
{
	//une classe ki fait des trucs !
public:
	JFCTruc(void);
	~JFCTruc(void);

protected:
	//{{AFX_MSG(JFCButton)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
