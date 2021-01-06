/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	TitreInXListBox
//
//	Descriptif:
//	ListBox sp�cialis� pour afficher l'�tat d'attribution d'un titre
*/

#pragma once

#include <IElem.h>
#include <JFC_MultiListBox.h>


class TitreInXListBox : public JFC_MultiListBox<IElem const *>
{
public:
	TitreInXListBox(void);
	virtual ~TitreInXListBox(void);

	JMap<int,int> m_mapIDRef;

	virtual int CompareItem( LPCOMPAREITEMSTRUCT lpCompareItemStruct);

private:
	virtual bool IsAttribue(int ID) const;/// red�finition de la m�thode

};
