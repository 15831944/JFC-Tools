#pragma once
#include "inumericcell.h"

#include "..\Config\BuildConfig.h"
#if !defined(BUILD_FOR_ATELIER_INTERNET)

class JFCNumericCell : public CGXEditControl, public INumericCell
{
	public:

		JFCNumericCell(CGXGridCore* pGrid, UINT nID);

		virtual ~JFCNumericCell();

		// mesage envoyé lors de la fin d'une édition de cellule
		static const UINT SPINEDITCELL_END_EDIT = WM_USER + 57;

	protected:

		////////////////////////////////////////////////////////////
		// la fonction de traitement des appuis sur les touches du clavier
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

		DECLARE_MESSAGE_MAP()

		////////////////////////////////////////////////////////////
		// callback appelé lorsqu'une cellule est modifiée
		BOOL OnEndEditing();
};

#endif