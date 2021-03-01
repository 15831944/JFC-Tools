#pragma once
#include "palettemode.h"

class AFX_EXT_CLASS CLineMode : public CPaletteMode
{
public:
					CLineMode(CDlgItems *pDlgItems);
	virtual			~CLineMode(void);

public:
	// *************************************************************************************************************************
	// **************************** Abstract base class overrides **************************************************************
	// *************************************************************************************************************************
	/// StartMode the Mode
	virtual void	StartMode();

	/// StopMode the Mode
	virtual void	StopMode();

	/// RecupMode the Mode
	virtual void	RecupMode();

	/// Generate Items based on the current valid configuration
	bool	BuildSelectedItems(bool RecupSel = false);

	/// Returns an IItem with the current configuration
	void	GetSelectedItems(JList< CItemWrapper *> &Items);

	/// Returns an IItem with the current configuration
	void	SetSelectedItems(JList< CItemWrapper *> &Items);

	/// Button Accept pushed
	bool	OnAccept(CString &Name);

	// Renvoi si on est en mode croisemnt de palette
	bool	ModeCroisementPalette();

	// Sauvegarde palette (ligne ou colonne ou page)
	bool	SendPalette(JStream &Stream, int Mode);

	// Récupération palette (ligne ou colonne ou page)
	bool	RecvPalette(JStream &Stream, int Mode);

	// Recopie elmt palette
	bool	CopyPalette(CAbstractMode * pPaletteModeDst);

	// Sauvegarde et récupération des palettes temporaires
	bool	SaveMemento(JMemento	* pMemento, int Mode);
	bool	LoadMemento(JMemento	* pMemento, int Mode);

	// Rend moyenne active ou pas sur item ligne 
	void	SetMoyennaActive(JBool Active = false){m_MoyenneActive = Active;};

	// Récup si moyenna active sur cet item
	JBool   GetMoyenneActive(){return m_MoyenneActive;};

protected:
	// *************************************************************************************************************************
	// **************************** PaletteMode base class overrides **************************************************************
	// *************************************************************************************************************************
	// returns the current modes label
	virtual void	SetModeTitle(void);
	/// Returns a string to the current dimension mode
	virtual CString GetDimensionModeString(int Mode);
	/// Update the IBuilder
	void	UpdateIBuilder(bool Changed, bool bValues);

	// builds the palette
	bool	BuildPalette(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label, bool Mode = false);
	// builds the palette Linear
	bool	BuildPaletteL(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label);
	// builds the palette Crossed
	bool	BuildPaletteX(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label);

	/// Updates the current display
	void	UpdateDisplay();

protected:
	/// returns true if the palette is valid
	bool	IsValidPalette();

protected:
	// Récupération palette builder(ligne/colonne ou page) et transfert dans map builder courant
	void RecupSelMapCur(int Mode);

};
