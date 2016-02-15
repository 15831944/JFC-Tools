// JFCOrder.cpp: implementation of the JFCOrder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JFCOrder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JFCOrder::JFCOrder()
{
	// on initialise les pointeurs sur les tableaux
	m_Elements  = NULL;
	m_Etats     = NULL;
	m_Positions = NULL;
	m_Index     = NULL;

	// on initialise le nombre d'elements et le nombre d'index
	m_NbElements = 0;
	m_NbIndex    = 0;
}

int JFCOrder::_MakeTableaux( int taille, int* &elements, unsigned long* &etats, int* &positions, int* &index )
{
	int i;

	// on alloue le bloc des elements
	elements = new int[taille];
	if ( elements == NULL )
		return -2;

	// on alloue le bloc des etats
	etats = new unsigned long[taille];
	if ( etats == NULL )
	{
		delete [] elements;
		return -2;
	}

	// on alloue le bloc des positions
	positions = new int[taille];
	if ( positions == NULL )
	{
		delete [] etats;
		delete [] elements;
		return -2;
	}

	// on alloue le bloc des index
	index = new int[taille];
	if ( index == NULL )
	{
		delete [] positions;
		delete [] etats;
		delete [] elements;
		return -2;
	}

	// on initialise les tableaux
	for ( i = 0; i < taille; i += 1 )
	{
		elements[i]  = i;
		etats[i]     = 0;
		positions[i] = i;
		index[i]     = i;
	}

	// on quitte
	return 0;
}

void JFCOrder::_MakeIndex( void )
{
	int i;

	// l'index est initialement vide
	m_NbIndex = 0;

	// on parcourt tous les elements pour remplir l'index
	for ( i = 0; i < m_NbElements; i += 1 )
	{
		// on teste si on doit mettre l'element dans l'index
		if ( m_Etats[i] == 0 )
		{
			// on met l'element dans l'index
			m_Index[m_NbIndex] = i;

			// on incremente la taille de l'index
			m_NbIndex += 1;
		}
	}

	// on quitte
	return;
}

int JFCOrder::SetSize( int taille )
{
	int*           elements;
	unsigned long* etats;
	int*           positions;
	int*           index;
	int            ret;

	// on verifie la validite de la taille
	if ( taille < 0 )
		return -1;

	// on initialise les pointeurs sur les nouveaux tableaux
	elements  = NULL;
	etats     = NULL;
	positions = NULL;
	index     = NULL;

	// on teste si on doit allouer de nouveaux blocs de memoire
	if ( taille > 0 )
	{
		// on construit de nouveaux tableaux
		ret = _MakeTableaux( taille, elements, etats, positions, index );
		if ( ret != 0 )
			return ret;
	}

	// on teste si on doit liberer l'ancien bloc des elements
	if ( m_Elements != NULL )
		delete [] m_Elements;

	// on teste si on doit liberer l'ancien bloc des etats
	if ( m_Etats != NULL )
		delete [] m_Etats;

	// on teste si on doit liberer l'ancien bloc des positions
	if ( m_Positions != NULL )
		delete [] m_Positions;

	// on teste si on doit liberer l'ancien bloc des index
	if ( m_Index != NULL )
		delete [] m_Index;

	// on modifie les pointeurs sur les tableaux
	m_Elements  = elements;
	m_Etats     = etats;
	m_Positions = positions;
	m_Index     = index;

	// on modifie le nombre d'elements et le nombre d'index
	m_NbElements = taille;
	m_NbIndex    = taille;

	// on quitte
	return 0;
}

int JFCOrder::GetSize( void )
{
	// on teste si on doit remplir un nouveau tableau d'index
	if ( m_NbIndex < 0 )
		_MakeIndex();

	// on renvoie le nombre d'index
	return m_NbIndex;
}

int JFCOrder::GetAt( int position )
{
	// on teste si on doit remplir un nouveau tableau d'index
	if ( m_NbIndex < 0 )
		_MakeIndex();

	// on verifie la validite de la position
	if ( position < 0 || position >= m_NbIndex )
		return -1;

	// on renvoie l'element demande
	return m_Elements[m_Index[position]];
}

int JFCOrder::GetPhysicalSize( void )
{
	// on renvoie le nombre total d'elements
	return m_NbElements;
}

int JFCOrder::GetPhysicalAt( int position )
{
	// on verifie la validite de la position
	if ( position < 0 || position >= m_NbElements )
		return -1;

	// on renvoie l'element demande
	return m_Elements[position];
}

int JFCOrder::Swap( int gauche, int droite )
{
	int           tempo1;
	unsigned long tempo2;

	// on verifie la validite de l'element gauche
	if ( gauche < 0 || gauche >= m_NbElements )
		return -1;

	// on verifie la validite de l'element droite
	if ( droite < 0 || droite >= m_NbElements )
		return -1;

	// on permute gauche et droite dans le tableau des elements
	tempo1             = m_Elements[gauche];
	m_Elements[gauche] = m_Elements[droite];
	m_Elements[droite] = tempo1;

	// on permute gauche et droite dans le tableau des etats
	tempo2          = m_Etats[gauche];
	m_Etats[gauche] = m_Etats[droite];
	m_Etats[droite] = tempo2;

	// on met a jour le tableau des positions
	m_Positions[m_Elements[gauche]] = gauche;
	m_Positions[m_Elements[droite]] = droite;

	// on invalide l'index
	m_NbIndex = -1;

	// on quitte
	return 0;
}

int JFCOrder::SetState( int element, unsigned long etat )
{
	// on verifie la validite de l'element
	if ( element < 0 || element >= m_NbElements )
		return -1;

	// on positionne l'etat de l'element
	m_Etats[m_Positions[element]] |= etat;

	// on invalide l'index
	m_NbIndex = -1;

	// on quitte
	return 0;
}

int JFCOrder::UnsetState( int element, unsigned long etat )
{
	// on verifie la validite de l'element
	if ( element < 0 || element >= m_NbElements )
		return -1;

	// on annule l'etat de l'element
	m_Etats[m_Positions[element]] &= ~etat;

	// on invalide l'index
	m_NbIndex = -1;

	// on quitte
	return 0;
}

int JFCOrder::ResetState( int element )
{
	// on verifie la validite de l'element
	if ( element < 0 || element >= m_NbElements )
		return -1;

	// on annule l'etat de l'element
	m_Etats[m_Positions[element]] = 0;

	// on invalide l'index
	m_NbIndex = -1;

	// on quitte
	return 0;
}

JFCOrder::~JFCOrder()
{
	// on teste si on doit liberer le bloc des elements
	if ( m_Elements != NULL )
		delete [] m_Elements;

	// on teste si on doit liberer le bloc des etats
	if ( m_Etats != NULL )
		delete [] m_Etats;

	// on teste si on doit liberer le bloc des positions
	if ( m_Positions != NULL )
		delete [] m_Positions;

	// on teste si on doit liberer le bloc des index
	if ( m_Index != NULL )
		delete [] m_Index;

	// on reinitialise les pointeurs sur les tableaux
	m_Elements  = NULL;
	m_Etats     = NULL;
	m_Positions = NULL;
	m_Index     = NULL;

	// on reinitialise le nombre d'elements et le nombre d'index
	m_NbElements = 0;
	m_NbIndex    = 0;
}
