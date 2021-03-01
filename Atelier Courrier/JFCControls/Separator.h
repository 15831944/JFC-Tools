
#pragma once


/**
 * \ingroup JFCControls
 * Written by Michael Dunn (mdunn at inreach dot com).  Copyright and all
 * that stuff.  Use however you like but give me credit where it's due.  I'll
 * know if you don't. bwa ha ha ha ha!
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-10-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CSeparator : public CStatic
{
// Construction
public:
	CSeparator();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeparator)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSeparator();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSeparator)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
