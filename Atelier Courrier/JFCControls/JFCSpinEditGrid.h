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
			semSpinEdit = 0,	// mode Spin : la colonne de gauche contient des fl�ches pour modifier les valeurs
			semEdit				// mode Edit : la colonne de gauche contient des cell d'edition 'normales'
		};

		////////////////////////////////////////////////////////////
		// index des colonnes Spin Edit Grid 
		typedef enum eSpinEditGridColumnIndex
		{
			segLibelle = 0,		// 1�re colonne : libell�s
			segReel,			// 2nde colonne : r�el
			segPondere			// 3�me colonne : ponder�
		};

	public:

		////////////////////////////////////////////////////////////
		// le constructeur
		JFCSpinEditGrid(const int iNumLines = 0, const int iNumDecimales = 2, eSpinEditGridMode eMode = semSpinEdit);

		////////////////////////////////////////////////////////////
		// le destructeur
		virtual ~JFCSpinEditGrid();

		////////////////////////////////////////////////////////////
		// le callback appell� lors de la premi�re mise � jour
		void OnInitialUpdate();

		////////////////////////////////////////////////////////////
		// la fonction pour d�finir le texte d'un titre
		void SetHeaderText(eSpinEditGridColumnIndex eColIndex, LPCTSTR sTitre);

		////////////////////////////////////////////////////////////
		// les fonctions pour d�finir la valeur d'une cellule ( iLineIdx d�bute � 1 pour la 1�re ligne)
		void SetCellText(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex, LPCTSTR lpNewValue);
		void SetCellText(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex, double dNewValue);

		////////////////////////////////////////////////////////////
		// Les fonctions pour r�cup�rer es valeurs d'une cellule
		LPCSTR GetCellText(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex);
		double GetCellValue(const int iLineIdx, const eSpinEditGridColumnIndex eColIndex);

		////////////////////////////////////////////////////////////
		// la fonction pour ajouter des lignes ( si iInsertAfter == -1 les lignes sont ajout�es � la fin de la grille)
		BOOL AddLines (int iInsertAt = -1, int iNumNewLines = 1);

		////////////////////////////////////////////////////////////
		// la fonction pour supprimer une ligne
		BOOL DeleteLines (int iLineToDelete = -1);

		////////////////////////////////////////////////////////////
		// la fonction pour d�finir le nombre de d�cimales d'une SpinCell (passer -1 � iSpinLine pour toutes les SpinCells)
		void SetSpinCellsNumDecimales (const int iNumDecimales, const int iSpinLine = -1 );

		////////////////////////////////////////////////////////////
		// la fonction pour d�finir les bornes des cellules de la colonne des SpinEdit
		void SetSpinCellsBounds (const double dMinVal, const double dMaxVal);

	protected:

		DECLARE_MESSAGE_MAP()

		////////////////////////////////////////////////////////////
		// le callback appelle lorsque l'on commence � modifier une cellule
		void OnModifyCell (ROWCOL nRow, ROWCOL nCol);

		////////////////////////////////////////////////////////////
		// callback appel� lorsqu'une cellule est modifi�e
		afx_msg LRESULT SpinEditCellEdited(WPARAM wParam, LPARAM lParam);

		virtual void OnSpinEditCellEdited (ROWCOL nRow, ROWCOL nCol);

		// ligne et colonnes en train d'�tre �dit�es (utile dans OnSpinEditCellEndEditing() )
		int m_nRow;
		int m_nCol;

	private:

		const int m_iNumRows;

		CGXStyle m_SpinColStyle;

		// on d�finit le type : vecteur contenant le nbr de d�cimales pour chaque ligne
		typedef std::vector<int> vNumDecimalesVec;

		vNumDecimalesVec m_DecimalesVec;

		int m_iGlobalDecimalesNum;	// le nombre de d�cimales par d�faut pour la cr�ation de nouvelles lignes

		double m_dMinVal;			// valeur mini saisissable dans les SpinEditCells
		double m_dMaxVal;			// valeur maxi saisissable dans les SpinEditCells

		eSpinEditGridMode m_Mode;	// mode de la grille, 'avec spin' ou 'sans spin'
};

#endif //!!defined(BUILD_FOR_ATELIER_INTERNET)