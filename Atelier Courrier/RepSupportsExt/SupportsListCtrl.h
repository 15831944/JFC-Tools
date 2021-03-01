/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CSupportsListCtrl
//
//	Descriptif:
//	ListCtrl sp�cilis�e pour afficher si un support existe dans les familles et centre d'interets.
*/

#pragma once

#include "JFCListCtrl.h"
// CSupportsListCtrl

class CSupportsListCtrl : public JFCListCtrl
{
	DECLARE_DYNAMIC(CSupportsListCtrl)

public:
	CSupportsListCtrl();
	virtual ~CSupportsListCtrl();

	void Sort();


private:
	virtual bool IsAttribue(int ID) const;

};


