#if !defined(AFX_ATPGraphCrb_H__1C1B2C6F_064D_4189_BDC5_840D293EC7FE__INCLUDED_)
#define AFX_ATPGraphCrb_H__1C1B2C6F_064D_4189_BDC5_840D293EC7FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CATPGraphCrb

class CATPGraphCrb : public JFCViewport
{
public:
	// le constructeur
	CATPGraphCrb();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPGraphCrb();

	void SetMax(JFlt64 max);
	JFlt64 GetMax() const;


protected:
	// les fonctions pour dessiner les éléments

	virtual void CalcValues(const JITPDATHypoFormat & DATHypoFormat, int idxHypo, int i, JFlt64 &A2, JFlt64 &A3, JFlt64 &A4) = 0;

	bool OnBeginDrawing(CDC & dc);
	void OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j);
	void OnEndDrawing(CDC & dc);

	void DrawSection(Graphics& grf, Brush & brush, Pen &pen, JInt32 x1, JInt32 x2, JInt32 x3, JInt32 x4, JInt32 y1, JInt32 y2, JInt32 y3, JInt32 y4, JInt32 y5, bool bfill);

	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

	//{{AFX_VIRTUAL(CATPGraphCrb)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CATPGraphCrb)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	JATPDocCreuset* m_pATPDocCreuset; // le document

private:
	JFlt64 m_ValMax; // la couverture maximum

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPGraphCrb_H__1C1B2C6F_064D_4189_BDC5_840D293EC7FE__INCLUDED_)
