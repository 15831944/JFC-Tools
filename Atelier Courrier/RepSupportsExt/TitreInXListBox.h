/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	TitreInXListBox
//
//	Descriptif:
//	ListBox spécialisé pour afficher l'état d'attribution d'un titre
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
	virtual bool IsAttribue(int ID) const;/// redéfinition de la méthode

};
