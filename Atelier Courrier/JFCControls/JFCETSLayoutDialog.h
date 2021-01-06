#pragma once
#include "etslayout.h"
#include "JFCUpdate.h"

class AFX_EXT_CLASS JFCETSLayoutDialog :
	public ETSLayoutDialog
{
public:
	// les constructeurs
	JFCETSLayoutDialog(UINT nIDTemplate, CWnd* pParent = NULL);

	// le destructeur
	virtual ~JFCETSLayoutDialog();

	// la fonction pour mettre � jour les vues
	virtual VOID UpdateAllViews(const JFCUpdate & Message);

protected:
	// les modes de d�placement
	enum { MGER_NONE = 0, MGER_MOVE, MGER_SIZE, MGER_SHIFT, MGER_STRETCH, MGER_SHIFTANDMOVE, MGER_SHIFTANDSIZE, MGER_STRETCHANDSIZE };

protected:
	// la fonction pour configurer les modes de d�placement
	void SetMode(CWnd* pWnd, LONG HMode, LONG VMode);

	// la fonction pour configurer la gestion des ascenseurs
	void SetScrollBar(CWnd* pWnd, CWnd* pHScroll, CWnd* pVScroll);

private:
	// la d�finition d'un �l�ment
	struct Link
	{
		LONG  m_HPos;     // la position horizontale initiale
		LONG  m_VPos;     // la position verticale initiale
		LONG  m_HSize;    // la taille horizontale initiale
		LONG  m_VSize;    // la taille verticale initiale
		LONG  m_HMode;    // le mode de d�placement horizontal
		LONG  m_VMode;    // le mode de d�placement vertical
		CWnd* m_pHScroll; // l'ascenseur horizontal
		CWnd* m_pVScroll; // l'ascenseur vertical
	};

	CMap<CWnd*, CWnd*, Link, Link &> m_Items; // les �l�ments

};
