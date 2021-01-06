#if !defined(AFX_ATPGraphGraduation_H__ECBEC2DB_0947_447D_8853_9396AE6DA263__INCLUDED_)
#define AFX_ATPGraphGraduation_H__ECBEC2DB_0947_447D_8853_9396AE6DA263__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires

/////////////////////////////////////////////////////////////////////////////
// CATPGraphGraduation

class CATPGraphGraduation : public JFControl
{
public:
	// le constructeur
	CATPGraphGraduation();

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	void SetMax(JFlt64 max);
	JFlt64 GetMax() const;

	void SetPourcent(bool set = true);

	// le destructeur
	virtual ~CATPGraphGraduation();

private:
	// les fonctions pour dessiner l'axe
	JBool OnBeginDrawing(CDC & dc);
	JVoid OnDrawAxe(CDC & dc, JInt32 x1, JInt32 y1, JInt32 x2, JInt32 y2);
	JVoid OnDrawGraduation(CDC & dc, JInt32 x1, JInt32 y1, JInt32 x2, JInt32 y2);
	JVoid OnDrawBorder(CDC & dc, JInt32 x1, JInt32 y1, JInt32 x2, JInt32 y2);
	JVoid OnEndDrawing(CDC & dc);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

	//{{AFX_VIRTUAL(CATPGraphGraduation)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CATPGraphGraduation)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	JFlt64 m_VALMax; // le GRP maximum
	bool m_bpourcent;

	CFont m_Font; // la fonte
	CPen  m_PenG; // le stylo G
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPGraphGraduation_H__ECBEC2DB_0947_447D_8853_9396AE6DA263__INCLUDED_)
