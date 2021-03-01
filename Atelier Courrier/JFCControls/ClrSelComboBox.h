/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CClrSelComboBox
//
//	Descriptif:
//	ComboBox avec couleur de sélection personnalisable
*/

#pragma once


class AFX_EXT_CLASS CClrSelComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CClrSelComboBox)

public:
	CClrSelComboBox();
	virtual ~CClrSelComboBox();

	void SetSelectionColor(COLORREF selclr);///permet de modifier la couleur de la sélection

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);///dessine l'item avec la bonne couleur
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);///pour OWNERDRAWVARIABLE


private:
	COLORREF m_selColor;///la couleur de la sélection
};


