#if _MSC_VER > 1000
#pragma once
#endif 

#include "ItemMode.h"

/**
 * \ingroup RepItems
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
 * \date 07-09-2003
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

class CConsultationMode : public CItemMode
{
public:
						CConsultationMode(CDlgItems *pDlgItems);
	virtual				~CConsultationMode(void);

public:
	// *************************************************************************************************************************
	// **************************** Abstract base class overrides **************************************************************
	// *************************************************************************************************************************
	void		StartMode();
	void		StopMode();
	void		OnSelchange_ListQuestions();
	void		OnContextMenu(CWnd* pWnd, CPoint point);
	/// List Items selection changed
	void		OnSelchange_ListItems();
	/// Sets the active questions in the question list box
	void		SetActiveSegmentQuestions();
	/// Button Accept pushed
	bool		OnAccept(CString &Name);
	/// Update the collection list box with existing classes
	void		UpdateListItems();
	/// Displays the selected theme, support window
	void		UpdateTheme(int ID);

protected:
	// *************************************************************************************************************************
	// **************************** ItemMode base class overrides **************************************************************
	// *************************************************************************************************************************
	/// Updates the number of segments
	// void		UpdateSegmentTabs(int Sel);

	/// Update the Ibuilder selections
	void		UpdateIBuilder(bool Changed, bool bValues);
};
