#pragma once

#include "JFCMiniCalendar.h"
#include "JATPDocCreuset.h"

class CAbscisseGrph : public JFCMiniCalendar
{
public:
	CAbscisseGrph(void);
	~CAbscisseGrph(void);

	// la fonction pour d�finir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);
	// la fonction pour mettre � jour le composant
	virtual JVoid Update();

protected:
	// la fonction pour appliquer les mises � jour
	JVoid OnUpdate();

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};
