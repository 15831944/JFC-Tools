#pragma once

// on inclut les d�finitions n�cessaires
#include "JFCPane.h"

/////////////////////////////////////////////////////////////////////////////
// JFCFlatPane

class AFX_EXT_CLASS JFCFlatPane : public JFCPane
{
public:
	// le constructeur
	JFCFlatPane();

	// le destructeur
	virtual ~JFCFlatPane();

protected:
	//{{AFX_MSG(JFCFlatPane)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

