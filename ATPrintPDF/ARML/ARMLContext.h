//===========================
// fichier: ARMLContext.h
//
// date: 06/09/2001
// auteur: JB
//===========================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _ARMLCONTEXT_H_

// on définit une macro pour identifier le fichier
#define _ARMLCONTEXT_H_

// on inclut les fichiers nécessaires
#include "AfxTempl.h"
#include "ARMLState.h"
#include "ARMLFrame.h"
#include "ARMLAdress.h"
#include "ARMLLogo.h"
#include "ARMLTable.h"
#include "./../Composite/JFCDrawPage.h"
#include "./../Composite/JFCDrawDocument.h"

class State;

class Context
{
public:
	// le constructeur
	Context(const char * filename, DrawDocument * pDrawDoc);

	// les fonctions pour paramétrer le contexte
	void SetPageWidth(long pagewidth);
	void SetPageHeight(long pageheight);
	void SetLineHeight(long lineheight);
	void SetNbLineHeader(long nblineheader);
	void SetNbLineFooter(long nblinefooter);
	void SetNbLinePage(long nbline);
	void SetNumLine(long numline);
	void SetStartBodyLine(long numline) { m_StartBodyLine = numline;}
	
	// les fonctions pour récupérer les paramètres du contexte
	long GetPageWidth();
	long GetPageHeight();
	long GetLineHeight();
	long GetHorzPos(long posrelative);
	long GetVertPos(long numline);
	long GetNbLineHeader();
	long GetNbLineFooter();
	long GetNumLine();
	long GetNbLinePage();
	long GetStartBodyLine(){return m_StartBodyLine;}

	// la fonction pour récupérer les cadres d'une page
	void GetBodyFrames(CArray<CFrame, CFrame &> & bodyframes);
	void SetBodyFrames(const CArray<CFrame, CFrame &> & bodyframes);
	
	// la fonction pour ajouter la page courante au document
	void AddPage();

	// la fonction pour fixer la fonte du document
	void SetFont(long fonte);

	// la fonction pour ajouter un cadre dans le header
	void AddHeader(CFrame & frame);

	// la fonction pour ajouter un cadre dans le corps
	void AddBody(CFrame & frame);

	// la fonction pour ajouter une table dans le corps
	void AddBody(CTable & table);

	// la fonction pour remplir les informations de l'adresse
	void SetAdress(CAdress & adress);

	// la fonction pour remplir les informations du logo
	void SetLogo(CLogo & logo);

	// la fonction pour lancer le traitement des états
	bool Manage();

	// la fonction pour récupérer la ligne courante du fichier
	void GetCurrentLine(CString & Line);

	// la fonction pour lire la ligne suivante du fichier
	bool GetNextLine(CString & Line);

	// la fonction pour récupérer la liste des champs de la ligne courante
	bool GetLineFields(CStringArray & FieldArray);

	// la fonction pour modifier l'état courant
	void SetState(State * currentstate);

	// le destructeur
	~Context();
protected:
	// le fichier ARML
	CStdioFile * m_pFile;

	// la ligne courante du fichier
	CString m_FileLine;

	// le document ARML
	DrawDocument * m_pDrawDoc;

	// les paramètres du contexte
	long m_PageWidth;
	long m_PageHeight;
	long m_LineHeight;
	long m_NbLinePage;
	long m_NbLineHeader;
	long m_NbLineFooter;
	long m_StartBodyLine;

	// la ligne courante de la page
	long m_NumLine;

	// le nombre de page
	long m_NbPage;

	// la page courante
	DrawPage * m_pPage;

	// la fonte du document
	//FontManager * m_pFont;
	// l'indice de la fonte du document
	long m_Font;

	// l'entête courant
	CArray<CFrame, CFrame &> m_Header;

	// l'adresse courante
	CAdress m_Adress;

	// le logo courant
	CLogo m_Logo;

	// les cadres du corps de la page courante
	CArray<CFrame, CFrame &> m_BodyFrames;

	// les tableaux du corps de la page courante
	CArray<CTable, CTable &> m_BodyTables;

	// l'état courant
	State * m_CurrentState;
	
};
// fin du test sur la macro
#endif

