#pragma once
#include "atpcreusethypo_e2.h"

#include "InPlaceCombo.h"

class CATPCreusetHypoQP_E2 :
	public CATPCreusetHypo_E2
{
public:
	CATPCreusetHypoQP_E2(void);
	virtual ~CATPCreusetHypoQP_E2(void);

protected:
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);
	JVoid OnLButtonDown(LONG i, LONG j);

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

	CInPlaceCombo * m_pCombo;
	afx_msg void SetSubstitutFormat();
	int m_idSupport;

};
