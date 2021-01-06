#pragma once

#include "stdafx.h"
#include "JFCSpinEditGrid.h"
#include "INumericCell.h"

#include "..\Config\BuildConfig.h"
#if !defined(BUILD_FOR_ATELIER_INTERNET)

class AFX_EXT_CLASS CGXSpinEditCell :	public CGXSpinEdit, public INumericCell
{
	public:

		////////////////////////////////////////////////////////////
		// le constructeur
		CGXSpinEditCell(CGXGridCore* pGrid, UINT nID);

		////////////////////////////////////////////////////////////
		// le destructeur
		virtual ~CGXSpinEditCell();

		// mesage envoyé lors de la fin d'une édition de cellule
		static const UINT SPINEDITCELL_END_EDIT = WM_USER + 57;

	protected:

		////////////////////////////////////////////////////////////
		// la fonction des gestion du click sur les boutons du SpinEdit
		void OnClickedButton(CGXChild* pChild);

		////////////////////////////////////////////////////////////
		// la fonction de traitement des appuis sur les touches du clavier
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

		DECLARE_MESSAGE_MAP()

		////////////////////////////////////////////////////////////
		// callback appelé lorsqu'une cellule est modifiée
		BOOL OnEndEditing();
};


#endif