// PrintFilm.h: interface for the CPrintFilm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTFILM_H__0C5A3CDF_0265_4EDD_B640_069DD2CDB7D6__INCLUDED_)
#define AFX_PRINTFILM_H__0C5A3CDF_0265_4EDD_B640_069DD2CDB7D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CampagneFilm.h"
#include "CalculProduitFilm.h"

class CPrintFilm : public CObj_Printer   
{
public:

	void SetInfo(CCampagneFilm * pCampagneFilm);
	CPrintFilm();
	virtual ~CPrintFilm();

private:

	// Stockage des diff�rentes info tableaux � imprimer
	CWordArray m_TableType;

	// Nombre de tableaux affili�s � l'affichage grille film
	int m_NbTabGrilleFilm;

	// Phase d'initialisation des tableaux � imprimer
	void Initialise(CDC * pDC,SIniPrint * pInit);

	// Pahse initialisation formatage impression grille film
	void InitGrilleFilm();

	// Les diff�rents blocs � imprimer
	bool DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig,short PremierNrTab,short DernierNrTab);
	bool DrawPieds(CDC * pDC, CRect * pR, short NrPage, short NrLig);
	bool DrawTab(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig);
	bool DrawVTab(CDC * pDC, CRect * pR_haut, CRect * pR_bas,short NrTab);
	bool DrawCol(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig,short NrCol,short NbCol);

	// La campagne films � imprimer
	CCampagneFilm * m_pCampagneFilm;

	// R�sultat des entr�es
	CCalculProduitFilm * calcul;
	CCalculProduitFilm::CResultatNbEntree * m_pResultEntree;

};

#endif // !defined(AFX_PRINTFILM_H__0C5A3CDF_0265_4EDD_B640_069DD2CDB7D6__INCLUDED_)
