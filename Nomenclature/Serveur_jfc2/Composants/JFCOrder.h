// JFCOrder.h: interface for the JFCOrder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JFCORDER_H__DD417460_8D54_11D3_8716_005004F93C23__INCLUDED_)
#define AFX_JFCORDER_H__DD417460_8D54_11D3_8716_005004F93C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class JFCOrder  
{
public:
	// le constructeur
	JFCOrder();

	// la fonction pour definir le nombre d'element
	int SetSize( int taille );

	// la fonction pour obtenir le nombre d'element
	int GetSize( void );

	// la fonction pour obtenir un element
	int GetAt( int position );

	// la fonction pour obtenir le nombre d'element physique
	int GetPhysicalSize( void );

	// la fonction pour obtenir un element physique
	int GetPhysicalAt( int position );

	// la fonction pour permuter deux elements physiques
	int Swap( int gauche, int droite );

	// la fonction pour modifier l'etat de visibilite d'un element physique
	int SetState( int element, unsigned long etat );

	// la fonction pour modifier l'etat de visibilite d'un element physique
	int UnsetState( int element, unsigned long etat );

	// la fonction pour rendre visible un element physique
	int ResetState( int element );

	// le destructeur
	virtual ~JFCOrder();

protected:
	// le pointeur sur le tableau des elements
	int* m_Elements;

	// le pointeur sur le tableau des etats
	unsigned long* m_Etats;

	// le pointeur sur le tableau des positions
	int* m_Positions;

	// le pointeur sur le tableau des index
	int* m_Index;

	// le nombre d'elements
	int m_NbElements;

	// le nombre d'index
	int m_NbIndex;

	// la fonction pour construire les tableaux
	int _MakeTableaux( int taille, int* &elements, unsigned long* &etats, int* &positions, int* &index );

	// la fonction pour remplir le tableau des index
	void _MakeIndex( void );
};

#endif // !defined(AFX_JFCORDER_H__DD417460_8D54_11D3_8716_005004F93C23__INCLUDED_)
