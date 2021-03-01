#pragma once
// on inclut les définitions nécessaires
#include "JLib.h"
#include "JFCCalendar.h"

/////////////////////////////////////////////////////////////////////////////
// JFCMiniCalendar


class AFX_EXT_CLASS JFCMiniCalendar : public JFCCalendar
{
public:
	// le constructeur
	JFCMiniCalendar();

	// le destructeur
	virtual ~JFCMiniCalendar();

	// les fonctions pour manipuler la période selectionnee

private:

	// les fonctions pour dessiner les éléments
	virtual bool OnBeginDrawing(CDC & dc);
	virtual void OnDrawState(CDC & dc, RECT rect, LONG i, LONG j, LONG etat);
	virtual void OnDrawItem(CDC & dc, RECT rect, LONG i, LONG j);
	virtual void OnEndDrawing(CDC & dc);

private:

protected:
	//{{AFX_VIRTUAL(JFCMiniCalendar)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCMiniCalendar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

