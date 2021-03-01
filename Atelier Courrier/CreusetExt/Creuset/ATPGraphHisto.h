#if !defined(AFX_ATPGraphHisto_H__F98DB27B_0EF5_497D_9020_C611D83937DC__INCLUDED_)
#define AFX_ATPGraphHisto_H__F98DB27B_0EF5_497D_9020_C611D83937DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPGraphHisto

class CATPGraphHisto : public JFCViewport
{
public:
	// le constructeur
	CATPGraphHisto();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	void SetMax(JFlt64 max);
	JFlt64 GetMax() const;

	// le destructeur
	virtual ~CATPGraphHisto();

protected:

	virtual void CalcValues(const JITPDATHypoFormat & DATHypoFormat, int i, JFlt64 &Value) = 0;

	// les fonctions pour dessiner les éléments
	bool OnBeginDrawing(CDC & dc);
	void OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j);
	void OnEndDrawing(CDC & dc);

	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

	//{{AFX_VIRTUAL(CATPGraphHisto)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CATPGraphHisto)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	JATPDocCreuset* m_pATPDocCreuset; // le document

private:
	JFlt64 m_ValMax; // le GRP maximum
	JFlt64 m_GRDMax; // le GRD maximum

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPGraphHisto_H__F98DB27B_0EF5_497D_9020_C611D83937DC__INCLUDED_)
