#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
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

	// la fonction pour d�finir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre � jour le composant
	virtual JVoid Update();

	// Changement de cible
	virtual JBool ChangeSelection();

protected:
	// la fonction pour dessiner l'�l�ment
	JVoid OnDrawItem(CDC & dc, RECT rect);

	// la fonction pour appliquer les mises � jour
	JVoid OnUpdate();

	// Requ�te pour savoir si cible valide sur tous les terrains s�lectionn�s
	JBool AuMoins1TerrainNonAutorise(JUnt32 IdCible, CString & TxtAvertissement);

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};
