#pragma once
#include "inumericcell.h"

#include "..\Config\BuildConfig.h"
#if !defined(BUILD_FOR_ATELIER_INTERNET)

class JFCNumericCell : public CGXEditControl, public INumericCell
{
	public:

		JFCNumericCell(CGXGridCore* pGrid, UINT nID);

		virtual ~JFCNumericCell();

		// mesage envoy� lors de la fin d'une �dition de cellule
		static const UINT SPINEDITCELL_END_EDIT = WM_USER + 57;

	protected:

		////////////////////////////////////////////////////////////
		// la fonction de traitement des appuis sur les touches du clavier
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

		DECLARE_MESSAGE_MAP()

		////////////////////////////////////////////////////////////
		// callback appel� lorsqu'une cellule est modifi�e
		BOOL OnEndEditing();
};

#endif