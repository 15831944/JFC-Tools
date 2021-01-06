/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CFamille
//
//	Descriptif:
//	Famille de titre dans l'arborescence des familles
//	DesignPattern Classe composite Classe acceptant un visiteur.
//	!une famille contient soit des titres soit des centres
*/

#if !defined(AFX_FAMILLE_H__89C13529_6F06_4DC1_81CF_EA440AAB16B1__INCLUDED_)
#define AFX_FAMILLE_H__89C13529_6F06_4DC1_81CF_EA440AAB16B1__INCLUDED_

#pragma once

//#include "Composite.h"
#include "ILot.h"


class AFX_EXT_CLASS CFamille : public ILot  
{

public:
	CFamille();
	CFamille(JUnt32 ID, const JChar* lbl);
	virtual ~CFamille();

	virtual JBool IsLot() const;

	virtual JVoid Accept (IVisiteur* v);
	
};

#endif // !defined(AFX_FAMILLE_H__89C13529_6F06_4DC1_81CF_EA440AAB16B1__INCLUDED_)
