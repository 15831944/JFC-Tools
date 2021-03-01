#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JFCSelListBox.h"
#include "JFCListBox.h"
#include "JATPDocCreuset.h"
// #include "RedItemListBox.h"

class CATPListePupitre_A4 :	public JFCListBox // public CRedItemListBox 	
{
public:
	// Constructeur	
	CATPListePupitre_A4(void);

	// Destructeur
	~CATPListePupitre_A4(void);

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// Changement de cible
	virtual JBool ChangeSelection();

protected:
	// la fonction pour dessiner l'élément
	JVoid OnDrawItem(CDC & dc, RECT rect);

	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

	// Requête pour savoir si cible valide sur tous les terrains sélectionnés
	JBool AuMoins1TerrainNonAutorise(JUnt32 IdCible, CString & TxtAvertissement);

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};
