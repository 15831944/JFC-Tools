/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// S�rialisation des �l�ments essentiels du pupitre
//
#pragma once
#include "PaletteBuilder.h"
#include "JLib.h"

class CSerialisePaletteBuilder
{
public:
	// Constructeur
	CSerialisePaletteBuilder(void);

	// Destructeur
	~CSerialisePaletteBuilder(void);

	// Stockage des composants palette pupitre
	static JVoid Send(JStream & Stream, JMap<int, CPaletteBuilder> * pMapPaletteBuilder, 
		              const JMap<JInt32,JBool> &m_SelUserItems,
					  CString &DefaultPaletteXLabel);

	// R�cup�ration des composants palette pupitre
	static JVoid Recv(JStream & Stream, JMap<int, CPaletteBuilder> * pMapPaletteBuilder, 
		              JMap<JInt32,JBool> &m_SelUserItems,
					  CString &DefaultPaletteXLabel);

private:

	// Sauvegarde CIBQuestion 	
	static JVoid Send_CIBQuestion	(JStream & Stream, const CPaletteBuilder & PaletteBuilder);

	// Sauvegarde CIBItem
	static JVoid Send_CIBItem		(JStream & Stream, const CPaletteBuilder & PaletteBuilder);

	// Sauvegarde Map_pIBModalite
	static JVoid Send_MapIBModalite	(JStream & Stream, const CPaletteBuilder & PaletteBuilder);

	// Sauvegarde Map_Dim_Type
	static JVoid Send_MapDimType	(JStream & Stream, const CPaletteBuilder & PaletteBuilder);

	// Sauvegarde Map_pIModalite
	static JVoid Send_MapIModalite		(JStream & Stream, const CPaletteBuilder & PaletteBuilder);

	// Sauvegarde Liste des valeurs num�riques List_NumValue
	static JVoid Send_MapListNumValue	(JStream & Stream, const CPaletteBuilder & PaletteBuilder);

	// Sauvegarde List_DimSel
	static JVoid Send_ListDimSel		(JStream & Stream, const CPaletteBuilder & PaletteBuilder);

	// Sauvegarde List_DimMode
	static JVoid Send_ListDimMode		(JStream & Stream, const CPaletteBuilder & PaletteBuilder);

	// Sauvegarde des Libell� par d�faut  : m_DefautLabel et Libell� Utilisateur: m_Label	 
	static JVoid Send_Libelles			(JStream & Stream, const CPaletteBuilder & PaletteBuilder);

	// Sauvegarde des user items s�lectionn�s
	static JVoid Send_SelUserItems		(JStream & Stream, const JMap<JInt32,JBool> &m_SelUserItems);
	
	// R�cup�ration CIBQuestion 	
	static JVoid Recv_CIBQuestion		(JStream & Stream, CPaletteBuilder & PaletteBuilder);

	// R�cup�ration CIBItem
	static JVoid Recv_CIBItem			(JStream & Stream, CPaletteBuilder & PaletteBuilder);

	// R�cup�ration Map_pIBModalite
	static JVoid Recv_MapIBModalite		(JStream & Stream, CPaletteBuilder & PaletteBuilder);

	// R�cup�ration Map_Dim_Type
	static JVoid Recv_MapDimType		(JStream & Stream, CPaletteBuilder & PaletteBuilder);

	// R�cup�ration Map_pIModalite
	static JVoid Recv_MapIModalite		(JStream & Stream, CPaletteBuilder & PaletteBuilder);

	// R�cup�ration Liste des valeurs num�riques List_NumValue
	static JVoid Recv_MapListNumValue	(JStream & Stream, CPaletteBuilder & PaletteBuilder);

	// R�cup�ration List_DimSel
	static JVoid Recv_ListDimSel		(JStream & Stream, CPaletteBuilder & PaletteBuilder);

	// R�cup�ration List_DimMode
	static JVoid Recv_ListDimMode		(JStream & Stream, CPaletteBuilder & PaletteBuilder);

	// R�cup�ration des Libell� par d�faut  : m_DefautLabel et Libell� Utilisateur: m_Label	 
	static JVoid Recv_Libelles			(JStream & Stream, CPaletteBuilder & PaletteBuilder);

	// R�cup�ration des user items s�lectionn�s
	static JVoid Recv_SelUserItems		(JStream & Stream, JMap<JInt32,JBool> &m_SelUserItems);
};
