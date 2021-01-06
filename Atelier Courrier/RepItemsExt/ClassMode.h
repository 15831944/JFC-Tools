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

class CClassMode : public CItemMode  
{
public:
						CClassMode(CDlgItems *pDlgItems);
	virtual				~CClassMode();

public: 
	// *************************************************************************************************************************
	// **************************** Abstract base class overrides **************************************************************
	// *************************************************************************************************************************
	void		StartMode();
	void		StopMode();
	void		OnSelchange_ListQuestions();
	void		OnContextMenu(CWnd* pWnd, CPoint point);
	/// Create the critere modifie
	bool		OnAccept(CString &Name);
	/// Button Modalite Ok pushed
	void		OnDim_1_Ok();
	/// Button Raz Tout pushed
	void		OnTout_Raz();
	/// Delete the current critere modifie
	void		OnItem_Raz();
	/// Delete the current critere modifie
	void		OnSAS_Raz();
	/// Suppression segment actif
	void		OnSegment_Raz();
	/// List Items selection changed
	void		OnSelchange_ListItems();
	// Modifiaction sélection dans la liste des classes
	void		SelChangeListClasse();
	/// Update the collection list box with existing classes
	void		UpdateListItems();
	/// Displays the selected theme, support window
	void		UpdateTheme(int ID);

	// Ajoute nvlle classe
	void		AddNewClasse(JInt32 IdNewClasse);

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
	// Update the Modalite list box
	void		UpdateDimensionsList();
	/// Update the Modalite list box 1 uniquement
	void		UpdateFiltreDimension1List();
	/// Update the Ibuilder selections
	void		UpdateIBuilder(bool Changed, bool bValues);

protected:
	/// Group modalites
	void GroupDimension1();
	/// Creates an item from with the Question and modalité selected with the default operators
	CItemConstruit * CreateNewItem(IQuestion const * pQuestion,IModalite const *pModalite);

protected:

	CCritereModifie *	m_pCritereModifie;
	JInt32				m_IdCritereModif;
};
