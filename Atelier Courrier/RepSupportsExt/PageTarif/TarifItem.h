/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	TarifItem
//
//	Descriptif:
//	Classe représentant un tarif pour être inséré dans un JFC_MultiListBox
*/



#pragma once

#include <Tarifs.h>

class TarifItem  
{

public:
	TarifItem(JUnt32 id, const JChar* lbl, CTarifManuel* trf);
	virtual ~TarifItem();

	virtual JUnt32 GetID() const;
	virtual const JLabel & GetLabel() const;
	virtual CTarifManuel * GetTarifManuel() const;

protected:
	TarifItem();
	JUnt32 m_ID;
	JLabel m_Label;
	CTarifManuel* m_pTarif;

};