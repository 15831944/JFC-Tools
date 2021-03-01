#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
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

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// Changement de terrain
	virtual JBool ChangeSelection();

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

};
