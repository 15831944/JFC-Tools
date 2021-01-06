/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CRedItemListBox
//
//	Descriptif:
//	Redéfinition de JFC_MultiListBox permettant d'afficher les nombres d'individus dans des tooltips
*/

#pragma once

#include <IItem.h>
//JFCControl
#include <JFC_MultiListBox.h>

class CDlgCible;

class CRedItemListBox : public JFC_MultiListBox<IItem*>
{
public:

	CRedItemListBox(void);
	virtual ~CRedItemListBox(void);

	CDlgCible * m_pDlgParent;

	// Niveau d'interdiction
	virtual JUnt32 DegreInterdit(int ID) const;

	// Avec en + retour des terrains interdits
	// virtual JUnt32 DegreInterdit(int ID, JMap <JUnt32, JMap<JUnt32, JUnt32> > &MapTerrainsInterdits) const;

protected:
	DECLARE_MESSAGE_MAP()

	int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	TCHAR* m_pchTip;
	WCHAR* m_pwchTip;
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	/*
	// Item ou palette sont-ils attribués
	virtual bool IsAttribue(int ID) const;

	// Item ou palettte sont-ils interdits
	virtual bool IsInterdit(int ID) const;
	*/

	// Niveau d'interdiction
	// virtual JUnt32 DegreInterdit(int ID) const;

private:
	/*
	bool IsPaletteAttribue(int ID) const;
	bool IsItemAttribue(int ID) const;

	// Item ou palette interdit à l'utilisation
	bool IsPaletteInterdit(int ID) const;
	bool IsItemInterdit(int ID) const;
	*/

	// Degré d'interdiction des items ou palettes
	// JUnt32 DegrePaletteInterdit(int ID, JMap <JUnt32, JMap<JUnt32, JUnt32> > &MapTerrainsInterdits) const;
	// JUnt32 DegreItemInterdit(int ID, JMap <JUnt32, JMap<JUnt32, JUnt32> > &MapTerrainsInterdits) const;

	JUnt32 DegrePaletteInterdit(int ID) const;
	JUnt32 DegreItemInterdit(int ID) const;
};
