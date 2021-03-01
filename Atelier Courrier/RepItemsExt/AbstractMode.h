
#pragma once

#include "DlgItems.h"
#include "ItemWrapper.h"
#include "JLib.h"

/// Abstract Mode interface for CDlgItems 
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
class CAbstractMode  
{
public:
						CAbstractMode(CDlgItems *pDlgItems);
	virtual				~CAbstractMode();
	/// Generate Items based on the current valid configuration
	virtual bool		BuildSelectedItems(bool RecupSel = false)					= 0;
	virtual bool		BuildSelectedItemsPalette()									= 0;
	/// Get the generated items
	virtual void		GetSelectedItems(JList< CItemWrapper *> &Items)				= 0;
	/// Set the generated items
	virtual void		SetSelectedItems(JList< CItemWrapper *> &Items)				= 0;
	/// StartMode the Mode
	virtual void		StartMode()													= 0;
	/// StopMode the Mode
	virtual void		StopMode()													= 0;
	/// RecupMode the Mode
	virtual void		RecupMode()													= 0;
	// Updates the current display
	virtual void		UpdateDisplay()												= 0;
	/// Button Accept pushed
	virtual bool		OnAccept(CString &Name)										= 0;
	/// Button Element Et pushed
	virtual void		OnDim_2_Et()												= 0;
	/// Button Element XOr pushed
	virtual void		OnDim_2_Xor()												= 0;
	/// Button Element Ni pushed
	virtual void		OnDim_2_Ni()												= 0;
	/// Button Element Ou pushed
	virtual void		OnDim_2_Ou()												= 0;
	/// Button Element Raz pushed
	virtual void		OnDim_2_Raz()												= 0;
	/// Button Genese pushed
	virtual bool		OnGenese()													= 0;
	/// Button Modalite Down pushed
	virtual void		OnDim_1_Down()												= 0;
	/// Button Modalite Ok pushed
	virtual void		OnDim_1_Ok()												= 0;
	/// Button Modalite Et pushed
	virtual void		OnDim_1_Et()												= 0;
	/// Button Modalite XOr pushed
	virtual void		OnDim_1_Xor()												= 0;
	/// Button Modalite Ni pushed
	virtual void		OnDim_1_Ni()												= 0;
	/// Button Modalite Ou pushed
	virtual void		OnDim_1_Ou()												= 0;
	/// Button Modalite Raz pushed
	virtual void		OnDim_1_Raz()												= 0;
	/// Button Modalite Filtrage Modalité
	virtual void		OnDim_Filtre()												= 0;
	/// Button Modalite Down pushed
	virtual void		OnDim_1_Up()												= 0;
	/// Button Item Raz pushed
	virtual void		OnItem_Raz()												= 0;
	/// Button Question Et pushed
	virtual void		OnSAS_Et()													= 0;
	/// Button Question Ou pushed
	virtual void		OnSAS_Ou()													= 0;
	/// Button SAS Raz pushed
	virtual void		OnSAS_Raz()													= 0;
	/// Segment actif supprimer
	virtual void		OnSegment_Raz()												= 0;
	/// Button Segment Et pushed
	virtual void		OnSegment_Et()												= 0;
	/// Button Segment Ou pushed
	virtual void		OnSegment_Ou()												= 0;

	// Update the Modalite list box
	virtual void		UpdateDimensionsList()										= 0;

	// Update the Modalite list box
	virtual void		UpdateFiltreDimension1List()								= 0;


	/// Button Dimension2 pushed
	virtual void		OnChange_Dim_2_PaletteSel()									= 0;
	virtual void		OnSetDimSelection2()										= 0;

	/// Button Dimension1 pushed
	virtual void		OnChange_Dim_1_PaletteSel()									= 0;
	virtual void		OnSetDimSelection1()										= 0;

	/// Button Raz Tout pushed
	virtual void		OnTout_Raz()												= 0;

	// Clearing all the current item && palettes sauf tree questions
	virtual void		OnTout_Raz_ItemPalette()									= 0;
	
	// Clear seulement sélection item
	virtual void		OnDeselect_Items()											= 0;

	/// Tab Segment Selection Changed
	virtual void		OnSelchange_TabSegment(NMHDR* pNMHDR, LRESULT* pResult)		= 0;
	/// List Question Selection changed
	virtual void		OnSelchange_ListQuestions()									= 0;
	/// List modalites selection changed
	virtual void		OnSelchange_ListDimension_1()								= 0;
	/// List Items selection changed
	virtual void		OnSelchange_ListItems()										= 0;

	/// Mise en place des pseudos items sélectionnés
	virtual void		UpdatePseudoItem(JList<JInt32> &Values)						= 0;
	virtual void		UpdatePseudoItemLineMode(JList<JInt32> &Values)				= 0;

	/// List Dimension2 selection changed
	virtual void		OnSelchange_ListDimension_2()								= 0;
	/// List Sas selection changed
	virtual void		OnSelchange_ListSas()										= 0;
	/// Context menu selected
	virtual void		OnContextMenu(CWnd* pWnd, CPoint point)						= 0;
	/// Segment calculation button Clicked
	virtual void		OnChange_SegmentCalc()										= 0;
	/// Message sent from modeless Modality and Element dialogs
	virtual LRESULT		OnChange_Dim_x_PaletteSel(WPARAM wParam, LPARAM lParam)		= 0;
	/// Message sent from modeless Modality and Element dialogs
	virtual LRESULT		OnSelChange_ListDimension_x(WPARAM wParam, LPARAM lParam)	= 0;
	/// Message sent from theme tree control
	virtual LRESULT		OnUpdated_QuestionList(WPARAM wParam, LPARAM lParam)		= 0;
	/// Message sent from rubrique tree control
	virtual LRESULT		OnUpdated_ItemList(WPARAM wParam, LPARAM lParam)			= 0;
	/// Message sent from values dialog
	virtual void		OnSelchange_ListValue()										= 0;
	/// Message sent from values dialog
	virtual void		OnChange_Value_PaletteSel()									= 0;
	/// Message sent from values dialog
	virtual void		OnValuesQuantile(LONG Type)									= 0;
	/// Population Segment has Changed
	virtual void		OnChange_PopSegment()										= 0;
	/// Type Poids has Changed
	virtual void		OnChange_TypePoids()										= 0;
	/// Reset the colors
	virtual void		SetColors()													= 0;
	/// Sets up icones for all controls
	virtual void		SetIcones()													= 0;
	/// Displays the selected theme, support window
	virtual void		UpdateTheme(int ID)											= 0;
	/// Update the items construits list
	virtual void		UpdateListItems()											= 0;
	/// Questions to palette
	virtual void		OnQuestions_Tous()											= 0;
	/// Returns a string to the current dimension mode
	virtual CString		GetDimensionModeString(int Mode)							= 0;
	/// Do visualisation Mode
	virtual void		DoVisualisation(void)										= 0;
	/// Get visualisation Mode
	virtual CString		GetVisualisation()											= 0;
	/// Returns nb élmts sélectionnés
	virtual int			GetNbSelectUserItem()										= 0;

	// Build Map all current questions in all segment 
	virtual void		Find_IBListQuestionSAS(JList <JInt32> &ListQuestionSAS)		= 0;

	/// Update the Question list box
	virtual void		UpdateQuestionList(long BlockId=0)							= 0;

	// Quel mode pour le Sas
	virtual bool		ModeCroisementPalette()										= 0;

	// Effacement général fenêtres dimensions mobiles  des différents modes
	virtual void		EffaceFenDimDlg(bool MajSelectedUserItem = true)			= 0;

	// Sauvegarde palette (ligne ou colonne ou page)
	virtual bool		SendPalette(JStream &Stream, int Mode)						= 0;

	// Récupération palette (ligne ou colonne ou page)
	virtual bool		RecvPalette(JStream &Stream, int Mode)						= 0;

	// Recopie elmt palette
	virtual bool		CopyPalette(CAbstractMode * pPaletteMode)					= 0;

	// Sauvegarde et récupération des palettes temporaires
	virtual bool		SaveMemento(JMemento	* pMemento, int Mode)				= 0;
	virtual bool		LoadMemento(JMemento	* pMemento, int Mode)				= 0;

	// Récupère les infos segments et IBuilder via les infos Questions et Items
	virtual void		SetSegmentIBuilder()										= 0;

	// Positionne le Pseudo Item (items indépendants)
	virtual void		SetPseudoItem()												= 0;

	// Nombre de boites dimensions actives
	virtual JUnt32		NbDimDlg()													= 0;

	// Delete the selected item construits (seulement les items) sans afficher message
	virtual bool		DeleteItemConstruit(JInt32 ID)								= 0;

protected:
	CDlgItems* m_pDlgItem;
};
