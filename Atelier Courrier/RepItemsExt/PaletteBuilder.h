#pragma once

#include "IItem.h"
#include "IQuestion.h"

class CPaletteBuilder
{
public:
											CPaletteBuilder();
											CPaletteBuilder(const CPaletteBuilder &rhs);
	virtual									~CPaletteBuilder();
	virtual bool							IsDimensionSelected(int Dim);
	virtual int								GetSelectedDimensionIndex(int Dim);
	virtual int								GetSelectedDimensionMode(int Dim);
	virtual void							SetDimSelection(int Index, bool Add, int Mode);
	virtual void							ResetDimSelection();
	virtual void							SetNumValues(const JList<float> & NumValues);
	virtual const JList<float> &			GetNumValues() const;
	CPaletteBuilder &						operator=(const CPaletteBuilder &rhs);
	void									Reset();
	void									Swap(CPaletteBuilder & rhs);

	CIBQuestion *							m_pIBquestion;
	CIBItem	*								m_pIBitem;
	JMap<int, CIBModalite *>				m_Map_pIBModalite;
	JMap<int, JInt32 >						m_Map_DimType;
	JMap<int, JList<const IModalite *> >	m_Map_pIModalite;
	JList<float>							m_List_NumValue;

	// Etat des boutons dimensions
	JList<int>								m_List_DimSel;
	JList<int>								m_List_DimMode;

	// Libellé par défaut pour les croisements de palette
	CString									m_DefaultLabel;

	// Nom réel donné au libellé croisement palette
	CString									m_Label;

protected:
	JMap<int,int>							m_Map_DimSel;
};

