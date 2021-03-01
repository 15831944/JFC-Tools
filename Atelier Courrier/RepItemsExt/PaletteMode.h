#pragma once

#include "itemmode.h"
#include "IItem.h"
#include "IQuestion.h"
#include "PaletteBuilder.h"

class AFX_EXT_CLASS CPaletteMode :	public CItemMode
{
public:
					CPaletteMode(CDlgItems *pDlgItems);
	virtual			~CPaletteMode();

public:
	// *************************************************************************************************************************
	// **************************** Abstract base class overrides **************************************************************
	// *************************************************************************************************************************
	enum MODE_TABLEAU
	{
		Line		=	1,
		Column		=	2,
		Page		=	3,
	};

	/// StartMode the Mode
	virtual void	StartMode();

	/// StopMode the Mode
	virtual void	StopMode();

	/// RecupMode the Mode
	void	RecupMode();

	// Updates the current display
	virtual void	UpdateDisplay();

	/// Button Dimension2 pushed
	void	OnChange_Dim_2_PaletteSel();

	// Button Dimension2 d'office actif
	void	OnSetDimSelection2();

	/// Button Dimension1 pushed
	void	OnChange_Dim_1_PaletteSel();

	// Button Dimension1 d'office actif
	void	OnSetDimSelection1();

	/// Message sent from modeless Modality and Element dialogs
	LRESULT OnChange_Dim_x_PaletteSel(WPARAM wParam, LPARAM lParam);

	/// Message sent from modeless Modality and Element dialogs
	LRESULT OnSelChange_ListDimension_x(WPARAM wParam, LPARAM lParam);

	/// List Items selection changed
	virtual void OnSelchange_ListItems();

	/// Message sent from values dialog
	virtual void OnSelchange_ListValue();

	/// Message sent from values dialog
	virtual void OnChange_Value_PaletteSel();

	/// Button Accept pushed
	virtual bool	OnAccept(CString &Name);

	/// Button SAS Raz pushed
	void	OnSAS_Raz();

	/// Suppression segment actif
	void	OnSegment_Raz();

	/// Context menu selected
	void	OnContextMenu(CWnd* pWnd, CPoint point);

	/// Questions to palette
	void	OnQuestions_Tous();

	/// Displays the selected theme, support window
	void	UpdateTheme(int ID);

	/// Update the items construits list
	void	UpdateListItems();

	/// Update the Question list box
	void	UpdateQuestionList(long BlockId=0);

	// Finds all current questions in all segment or delete them
	bool	FindDelete_IBMapquestion(JMap <JInt32,JInt32> &MapQuestion, int &SelSeg);

	/// Generate Items based on the current valid configuration
	bool	BuildSelectedItemsPalette();

	/// Returns an IItem with the current configuration
	void	GetSelectedItems(JList< CItemWrapper *> &Items);

	// Positionne les items sélectionnés directement
	void	SetSelectedItems(JList< CItemWrapper *> &Items);

	// Quel mode pour le Sas
	virtual bool	ModeCroisementPalette();

	// Sauvegarde palette (ligne ou colonne ou page)
	virtual bool	SendPalette(JStream &Stream, int Mode);

	// Récupération palette (ligne ou colonne ou page)
	virtual bool	RecvPalette(JStream &Stream, int Mode);

	// Recopie elmt palette
	virtual bool	CopyPalette(CAbstractMode * pPaletteMode);

	// Sauvegarde et récupération des palettes temporaires
	virtual bool		SaveMemento(JMemento	* pMemento, int Mode);
	virtual bool		LoadMemento(JMemento	* pMemento, int Mode);

	// Récupère les infos segments et IBuilder via les infos Questions et Items
	virtual void	SetSegmentIBuilder();

	// Positionne le pseudo item
	virtual void	SetPseudoItem();

protected:
	// *************************************************************************************************************************
	// **************************** ItemMode base class overrides **************************************************************
	// *************************************************************************************************************************
	// returns the current modes label
	virtual void	SetModeTitle(void);

	/// Update the Ibuilder selections
	virtual void	UpdateIBuilder(bool Changed, bool bValues);

	/// Returns a string to the current dimension mode
	virtual CString GetDimensionModeString(int Mode);

	/// Update segment tabs
	void	UpdateSegmentTabs(int Sel);

	/// Updates the Values from values dlg
	bool	UpdateItemValues(bool bValues);
	void	QuestionChanged(IQuestion * pCurQuestion);

	/// Update the Modalite list box
	void	UpdateDimensionsList();

	/// Update the Modalite list box 1 uniquement
	void	UpdateFiltreDimension1List();

protected:
	/// Validate the label
	void ValidateLabel(CString & Label);

protected:
	/// Menu delete modalites selected
	void	DeleteDimension1();

	// resets the selected dimensions
	void	ResetDimSelection();

	// Sets the dimension selection
	void	SetDimSelection(int Index, bool Add);

	// retrieves the palette values
	void	GetPaletteValues(CPaletteBuilder &PBuilder);

	// builds the palette
	virtual bool	BuildPalette(JList<IItem *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label, bool Mode = false);
	virtual bool	BuildPaletteL(JList<IItem *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label);
	virtual bool	BuildPaletteX(JList<IItem *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label);

	// builds the palette comme dans mode line
	bool	BuildPaletteModeLine(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label, bool Mode = false);

	// builds the palette Linear comme dans mode line
	bool	BuildPaletteLModeLine(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label);

	// builds the palette Crossed comme dans mode line
	bool	BuildPaletteXModeLine(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label);

protected:
	JMap<int,CPaletteBuilder>	m_Map_PaletteBuilder;
	CPaletteBuilder				m_CurPaletteBuilder;
	int							m_CurMode;

	// Les map palettes builder pour sauvegarder chaque état ligne / colonne ou page
	JMap<int,CPaletteBuilder>	m_Map_PaletteBuilderLigne;
	JMap<int,CPaletteBuilder>	m_Map_PaletteBuilderColonne;
	JMap<int,CPaletteBuilder>	m_Map_PaletteBuilderPage;

	// Mise à jour des maps ligne colonne page lié au map builder courant
	void UpdateMapModePupitre();

	///	Selected Items
	JList<CItemWrapper *>	m_SelectedItems;

	// Noms pour les intitulés palette (set dans mode pupitre surtout)
	CString					m_DefaultPaletteXLabel;
	CString					m_PaletteXLabel;

};

