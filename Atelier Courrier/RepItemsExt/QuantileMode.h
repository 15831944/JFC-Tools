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

class CQuantileMode : public CItemMode  
{
public:
						CQuantileMode(CDlgItems *pDlgItems);
	virtual				~CQuantileMode();

public: 
	// *************************************************************************************************************************
	// **************************** Abstract base class overrides **************************************************************
	// *************************************************************************************************************************
	void		StartMode();
	void		StopMode();
	void		OnSelchange_ListQuestions();
	void		OnContextMenu(CWnd* pWnd, CPoint point);
	/// Create the critere quantile
	bool		OnAccept(CString &Name);
	/// Button Value Ok pushed
	void		OnValuesQuantile(LONG Type);
	/// Button Raz Tout pushed
	void		OnTout_Raz();
	/// Delete the current critere quantile
	void		OnItem_Raz();
	/// Delete the current critere quantile
	void		OnSAS_Raz();
	/// Suppression segment actif
	void		OnSegment_Raz();
	/// List Items selection changed
	void		OnSelchange_ListItems();
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

	/// Update the Question list box
	void		UpdateQuestionList(long BlockId=0);
	/// Sets the active questions in the question list box
	void		SetActiveSegmentQuestions();
	/// Update the Modalite list box
	void		UpdateDimensionsList();
	/// Update the Modalite list box 1 uniquement
	void		UpdateFiltreDimension1List();
	/// Updates the Values from values dlg
	bool		UpdateItemValues(bool bValues);
	/// Update the Ibuilder selections
	void		UpdateIBuilder(bool Changed, bool bValues);

protected:
	/// Tercile selection
	void	Tercile();
	/// Quartile selection
	void	Quartile();
	/// Quintile selection
	void	Quintile();
	/// Decile selection
	void	Decile();
	/// Quantile selection
	void	CreateItems(CCritereQuantile *pQuantile, int Quantile);

protected:

	CCritereQuantile *	m_pCritereQuantile;
};
