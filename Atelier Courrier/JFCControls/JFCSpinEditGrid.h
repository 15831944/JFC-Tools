#pragma once

#include <vector>
#include "stdafx.h"

// JFCSpinEditGrid
#include "../Config/BuildConfig.h"
#if !defined(BUILD_FOR_ATELIER_INTERNET)

class AFX_EXT_CLASS JFCSpinEditGrid : public CGXGridWnd
{
	public:

		////////////////////////////////////////////////////////////
		// mode de la grid
		typedef enum eSpinEditGridMode
		{
			semSpinEdit = 0,	// mode Spin : la colonne de gauche contient des flêches pour modifier les valeurs
			semEdit				// mode Edit : la colonne de gauche contient des cell d'edition 'normales'
		};

		////////////////////////////////////////////////////////////
		// index des colonnes Spin Edit Grid 
		typedef enum eSpinEditGridColumnIndex
		{
			segLibelle = 0,		// 1ère colonne : libellés
			segReel,			// 2nde colonne : réel
			segPondere			// 3ème colonne : ponderé
		};

	public:

		////////////////////////////////////////////////////////////
		// le constructeur
		JFCSpinEditGrid(const int iNumLines = 0, const int iNumDecimales = 2, eSpinEditGridMode eMode = semSpinEdit);

		////////////////////////////////////////////////////////////
		// le destructeur
		virtual ~JFCSpinEditGrid();

		////////////////////////////////////////////////////////////
		// le callback appellé lors de la première mise à jour
		void OnInitialUpdate();

		////////////////////////////////////////////////////////////
		// la fonction pour définir le texte d'un titre
		void SetHeaderText(eSpinEditGridColumnIndex eColIndex, LPCTSTR sTitre);

		////////////////////////////////////////////////////////////
		// les fonctions pour définir la valeur d'une cellule ( iLineIdx débute à 1 pour la 1ère ligne)
		void SetCellText(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex, LPCTSTR lpNewValue);
		void SetCellText(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex, double dNewValue);

		////////////////////////////////////////////////////////////
		// Les fonctions pour récupérer es valeurs d'une cellule
		LPCSTR GetCellText(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex);
		double GetCellValue(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex);

		////////////////////////////////////////////////////////////
		// la fonction pour ajouter des lignes ( si iInsertAfter == -1 les lignes sont ajoutées à la fin de la grille)
		BOOL AddLines (int iInsertAt = -1, int iNumNewLines = 1);

		////////////////////////////////////////////////////////////
		// la fonction pour supprimer une ligne
		BOOL DeleteLines (int iLineToDelete = -1);

		////////////////////////////////////////////////////////////
		// la fonction pour définir le nombre de décimales d'une SpinCell (passer -1 à iSpinLine pour toutes les SpinCells)
		void SetSpinCellsNumDecimales (const int iNumDecimales, const int iSpinLine = -1 );

		////////////////////////////////////////////////////////////
		// la fonction pour définir les bornes des cellules de la colonne des SpinEdit
		void SetSpinCellsBounds (const double dMinVal, const double dMaxVal);

	protected:

		DECLARE_MESSAGE_MAP()

		////////////////////////////////////////////////////////////
		// le callback appelle lorsque l'on commence à modifier une cellule
		void OnModifyCell (ROWCOL nRow, ROWCOL nCol);

		////////////////////////////////////////////////////////////
		// callback appelé lorsqu'une cellule est modifiée
		afx_msg LRESULT SpinEditCellEdited(WPARAM wParam, LPARAM lParam);

		virtual void OnSpinEditCellEdited (ROWCOL nRow, ROWCOL nCol);

		// ligne et colonnes en train d'être éditées (utile dans OnSpinEditCellEndEditing() )
		int m_nRow;
		int m_nCol;

	private:

		const int m_iNumRows;

		CGXStyle m_SpinColStyle;

		// on définit le type : vecteur contenant le nbr de décimales pour chaque ligne
		typedef std::vector<int> vNumDecimalesVec;

		vNumDecimalesVec m_DecimalesVec;

		int m_iGlobalDecimalesNum;	// le nombre de décimales par défaut pour la création de nouvelles lignes

		double m_dMinVal;			// valeur mini saisissable dans les SpinEditCells
		double m_dMaxVal;			// valeur maxi saisissable dans les SpinEditCells

		eSpinEditGridMode m_Mode;	// mode de la grille, 'avec spin' ou 'sans spin'
};

#endif //!!defined(BUILD_FOR_ATELIER_INTERNET)