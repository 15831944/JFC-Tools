#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
// #include "JFCSelListBox.h"
#include "JFCListBox.h"
#include "JATPDocCreuset.h"

class CATPListePupitre_A2 :	public JFCListBox	//public JFCSelListBox
{
public:
	// Constructeur
	CATPListePupitre_A2(void);

	// Destructeur
	~CATPListePupitre_A2(void);

	// la fonction pour d�finir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre � jour le composant
	virtual JVoid Update();

	// Changement de terrain
	virtual JBool ChangeSelection();

protected:
	// la fonction pour appliquer les mises � jour
	JVoid OnUpdate();

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

};
