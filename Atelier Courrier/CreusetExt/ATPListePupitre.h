#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JFCSelListBox.h"
#include "JATPDocCreuset.h"

class CATPListePupitre : public JFCSelListBox
{
public:
	// Constructeur
	CATPListePupitre(void);

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// Destructeur
	~CATPListePupitre(void);

protected:
	// la fonction pour dessiner l'élément
	// JVoid OnDrawItem(CDC & dc, RECT rect);

	// la fonction pour appliquer les mises à jour
	// JVoid OnUpdate();

protected:
	//{{AFX_MSG(CATPListePupitre)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

};
