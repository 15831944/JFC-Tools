/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CFormatListBox
//
//	Descriptif:
//	ListBox sp�cialis�e pour l'affichage de l'�tat attriu� des formats dans DlgTarif
*/

#pragma once

#include <JFormat.h>
#include <JFC_MultiListBox.h>



class CDlgTarif;

class CFormatListBox : public JFC_MultiListBox<JFormat const *>
{
public:
	CFormatListBox(void);
	virtual ~CFormatListBox(void);

	CDlgTarif * m_dlgParent;

private:
	virtual bool IsAttribue(int ID) const;/// red�finition de la m�thode

};
