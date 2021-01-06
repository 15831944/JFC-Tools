/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	TarifItem
//
//	Descriptif:
//	Classe repr�sentant un tarif pour �tre ins�r� dans un JFC_MultiListBox
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