//============================
// fichier: ARMLContext.cpp
//
// date: 06/09/2001
// auteur: JB
//============================

// on inclut les fichiers n�cessaires
#include "stdafx.h"
#include "ARMLContext.h"
#include "ARMLStateError.h"
#include "ARMLStateBegin.h"
#include "./../Composite/JFCDrawSetFont.h"
#include "./../Composite/JFCDrawBeginPage.h"
#include "./../Composite/JFCDrawEndPage.h"

//==================================================================
// le constructeur:
//==================================================================
Context::Context(const char * filename, DrawDocument * pDrawDoc)
{
	// on ouvre le fichier
	m_pFile = new CStdioFile(filename, CFile::modeRead|CFile::shareExclusive);
	m_FileLine = "";
	
	// on fixe le pointeur sur le document
	m_pDrawDoc = pDrawDoc;

	// on fixe les param�tres
	m_PageWidth = 0;
	m_PageHeight = 0;
	m_LineHeight = 0;
	m_NbLinePage = 0;
	m_NbLineFooter = 0;
	m_NbLineHeader = 0;
	m_NumLine = 0;
	m_NbPage = 0;

	// on cr�e la page courante
	m_pPage = new DrawPage();

	// on initialise la fonte
	m_Font = 0;

	// on initialise la taille du header
	m_Header.SetSize(0, 10);

	// on initialise la taille du body
	m_BodyFrames.SetSize(0, 10);

	// 
}

//==================================================================
// les fonctions pour parametrer le contexte:
//==================================================================
void Context::SetPageWidth(long pagewidth)
{
	// on fixe la largeur de page
	m_PageWidth = pagewidth;
}

void Context::SetPageHeight(long pageheight)
{
	// on fixe la hauteur de page
	m_PageHeight = pageheight;
}

void Context::SetLineHeight(long lineheight)
{
	// on fixe la hauteur de ligne
	m_LineHeight = lineheight;
}

void Context::SetNbLineHeader(long nblineheader)
{
	// on fixe le nombre de lignes de l'ent�te
	m_NbLineHeader = nblineheader;
}

void Context::SetNbLineFooter(long nblinefooter)
{
	// on fixe le nombre de lignes du pied de page
	m_NbLineFooter = nblinefooter;
}

void Context::SetNumLine(long numline)
{
	// on fixe le num�ro de la ligne
	m_NumLine = numline;
}

void Context::SetNbLinePage(long nbline)
{
	// on fixe le nombre de lignes par page
	m_NbLinePage = nbline;
}

//==================================================================
// les fonctions r�cup�rer les param�tres du contexte:
//==================================================================
long Context::GetPageWidth()
{
	// on renvoie la largeur de page
	return (m_PageWidth);
}

long Context::GetPageHeight()
{
	// on renvoie la hauteur de page
	return (m_PageHeight);
}

long Context::GetLineHeight()
{
	// on renvoie la hauteur de ligne
	return (m_LineHeight);
}

long Context::GetHorzPos(long posrelative)
{
	// on renvoie la position r�elle
	return (posrelative * m_PageWidth / 100);		// 100_160_240
}

long Context::GetVertPos(long numline)
{
	// on renvoie la position verticale
	return (numline * m_LineHeight - m_LineHeight / 2);
}

long Context::GetNumLine()
{
	// on renvoie le num�ro de la ligne en cours
	return (m_NumLine);
}

long Context::GetNbLinePage()
{
	// on renvoie le nombre de lignes du document
	return (m_NbLinePage);
}

long Context::GetNbLineFooter()
{
	// on renvoie le nombre de ligne du pied de page
	return (m_NbLineFooter);
}

long Context::GetNbLineHeader()
{
	// on renvoie le nombre de ligne du pied de page
	return (m_NbLineHeader);
}

//==================================================================
// la fonction pour r�cup�rer les cadres de la page:
//==================================================================
void Context::GetBodyFrames(CArray<CFrame, CFrame &> & bodyframes)
{
	bodyframes.Copy(m_BodyFrames);
}

void Context::SetBodyFrames(const CArray<CFrame, CFrame &> & bodyframes)
{
	m_BodyFrames.Copy(bodyframes);
}
//==================================================================
// la fonction pour ajouter la page au document:
//==================================================================
void Context::AddPage()
{
	// on ajoute la commande de d�but de page
	m_pPage->AddChild(DrawBeginPage());

	// on ajoute la police � la page
	m_pPage->AddChild(DrawSetFont(m_Font));

	// on boucle sur les cadres du header
	for (long i = 0, max = m_Header.GetSize(); i<max ; i++)
	{
		// on cr�e le cadre 
		m_Header[i].CreateFrame(m_NbPage+1, m_pPage);
	}
	// on boucle sur les cadres du corps
	for (long i = 0, max = m_BodyFrames.GetSize(); i < max; i++)
	{
		// on cr�e les cadres
		m_BodyFrames[i].CreateFrame(1, m_pPage);
	}
	// on boucle sur les tableaux du corps
	for (long i = 0, max = m_BodyTables.GetSize(); i < max; i++)
	{
		m_BodyTables[i].CreateTable(1, m_pPage);
	}
	// on cr�e le logo
	m_Logo.CreateLogo(m_pPage);

	// on cr�e l'adresse
	m_Adress.CreateAdress(m_NbPage+1, m_pPage);

	// on ajoute le code de fin de page
	m_pPage->AddChild(DrawEndPage());

	// on ajoute la page au document
	m_pDrawDoc->AddChild(*m_pPage);

	// on lib�re la page
	delete m_pPage;

	// on alloue � nouveau la page
	m_pPage = new DrawPage();

	// on remet les tableaux du corps � 0
	//m_BodyFrames.SetSize(0, 10);
	m_BodyTables.SetSize(0, 10);

	// on replace la ligne courante
	m_NumLine = m_StartBodyLine;

	// on incr�mente le nombre de page
	m_NbPage += 1;
}

//==================================================================
// la fonction pour fixer la fonte du document:
//==================================================================
void Context::SetFont(long fonte)
{
	// on teste si la fonte existe d�j�
	m_Font = fonte;
}

//==================================================================
// la fonction pour ajouter un cadre dans le header:
//==================================================================
void Context::AddHeader(CFrame & frame)
{
	// on ajoute le cadre au tableau
	m_Header.Add(frame);
}

//==================================================================
// la fonction pour ajouter un cadre dans le corps:
//==================================================================
void Context::AddBody(CFrame & frame)
{
	// on ajoute le cadre au tableau
	if (m_BodyFrames.GetSize() > 0)
	{
		if (frame.m_StartPosY == m_BodyFrames[0].m_StartPosY) m_BodyFrames.SetSize(0, 10);
	}
	m_BodyFrames.Add(frame);
}

//==================================================================
// la fonction pour ajouter une table dans le corps:
//==================================================================
void Context::AddBody(CTable & table)
{
	// on ajoute la table 
	m_BodyTables.Add(table);
}

//==================================================================
// la fonction pour remplir les informations de l'adresse:
//==================================================================
void Context::SetAdress(CAdress & adress)
{
	// on modifie l'adresse
	m_Adress = adress;
}

//==================================================================
// la fonction pour remplir les informations du logo:
//==================================================================
void Context::SetLogo(CLogo & logo)
{
	// on modifie le logo
	m_Logo = logo;
}

//==================================================================
// la fonction pour lancer la gestion des �tats:
//==================================================================
bool Context::Manage()
{
	if (m_pFile == 0) return (false);

	// on cr�e un �tat Begin
	StateBegin * pStateBegin = new StateBegin(this);

//	_asm{int 3};

	// on teste le code de retour
	if (pStateBegin->Manage())
	{
		// on fixe le nombre de pages du document
		m_pDrawDoc->SetNbPage(m_NbPage);

		// on fixe la largeur de page
		m_pDrawDoc->SetPageHeight(m_PageHeight);

		// on fixe la hauteur de page
		m_pDrawDoc->SetPageWidth(m_PageWidth);
		
		// on renvoie le code de succ�s
		return (true);
	}
	else
	{
		// on renvoie le code d'erreur
		return (false);
	}
}

//==================================================================
// les fonctions pour r�cup�rer les informations du fichier:
//==================================================================
bool Context::GetNextLine(CString & Line)
{
	try
	{
		if (m_pFile->ReadString(m_FileLine) != 0)
		{
			Line = m_FileLine;
			return (true);
		}
		else return (false);
	}
	catch (CFileException e)
	{
		// on r�cup�re le pointeur sur l'�tat actuel
		State * pState = m_CurrentState;
		
		// on cr�e un �tat erreur
		StateError * pStateError = new StateError(this);

		// on lib�re l'�tat appelant
		delete (pState);

		// on g�re l'�tat erreur
		return (pStateError->Manage());
	}
}	

void Context::GetCurrentLine(CString & Line)
{
	// on modifie le param�tre
	Line = m_FileLine;
}

bool Context::GetLineFields(CStringArray & FieldArray)
{
	// on initialise la taille du tableau de chaines
	FieldArray.SetSize(0, 20);

	// le champ en cours
	CString Field = "";

	// le flag sigalant qu'on est dans un champ texte
	bool textopen = false;
	
	// on boucle sur les caract�res de la ligne
	for (long i = 0, max = m_FileLine.GetLength(); i < max ; i++)
	{
		if (m_FileLine.GetAt(i) == '\\')
		{
			// on ajoute le caract�re suivant
			if (i < (max-1))
			{
				Field += CString(m_FileLine.GetAt(i+1));
				i+=1;
				continue;
			}
			
		}
		if (m_FileLine.GetAt(i) == '\"')
		{
			textopen = !textopen;
		}
		if (!textopen)
		{
			// on teste le caract�re
			if (m_FileLine.GetAt(i) != ' ' && m_FileLine.GetAt(i)!=';')
			{
				// on ajoute le caract�re � la chaine en cours
				Field += CString(m_FileLine.GetAt(i));
			}
			else
			{
				// on ajoute le champs au tableau
				if (Field != "") FieldArray.Add(Field);

				// on r�initialise le champ
				Field = "";
			}
		}
		else
		{
			// on ajoute le caract�re
			Field += CString(m_FileLine.GetAt(i));
		}
	}
	// on ajoute le dernier champ si n�cessaire
	if (Field != "") FieldArray.Add(Field);

	// on renvoie le code de succ�s ou d'�chec
	return (!textopen);
}

//==================================================================
// la fonction pour modifier l'�tat courant:
//==================================================================
void Context::SetState(State * currentstate)
{
	// on change l'�tat courant
	m_CurrentState = currentstate;
}

//==================================================================
// le destructeur:
//==================================================================
Context::~Context()
{
	// on ferme le fichier
	m_pFile->Close();

	// on lib�re le fichier
	delete m_pFile;

	// on lib�re la page courante
	delete (m_pPage);
}

