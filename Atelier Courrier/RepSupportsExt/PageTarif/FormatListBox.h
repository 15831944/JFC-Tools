/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CFormatListBox
//
//	Descriptif:
//	ListBox spécialisée pour l'affichage de l'état attriué des formats dans DlgTarif
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
	virtual bool IsAttribue(int ID) const;/// redéfinition de la méthode

};
