//
// Fichier: JFCEditDecimal.cpp
// Auteur:  Aurélien Rainone
//

// on inclut les définitions nécessaires
#include "stdafx.h"

// on inclut les définitions nécessaires
#include "JFCEditDecimal.h"

//////////////////
// le constructeur

JFCEditDecimal::JFCEditDecimal()
{
	// on donne la valeur par défaut
	m_AcceptNegativeNumbers = false;

	// on donne la valeur par défaut
	m_NumDecimals = 3;
}

JFCEditDecimal::~JFCEditDecimal()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCEditDecimal, CEdit)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// les fonctions pour la gestion des nombres négatifs

void JFCEditDecimal::SetNegativeNumbers (const bool bAcceptNegativeNumbers)
{
	m_AcceptNegativeNumbers = bAcceptNegativeNumbers;
}

const bool JFCEditDecimal::AcceptNegativeNumbers () const
{
	return m_AcceptNegativeNumbers;
}

////////////////////////////////////////////////////////////
// les fonctions pour la gestion du nombre de décimales
void JFCEditDecimal::SetNumberOfDecimals (const int iNumDecimals)
{
	m_NumDecimals = iNumDecimals;
}

const int JFCEditDecimal::GetNumberOfDecimals () const
{
	return m_NumDecimals;
}


BOOL JFCEditDecimal::PreTranslateMessage(MSG* pMsg)
{
	// on teste le message
	if (pMsg->message == WM_CHAR)	
	{
		char nChar =(UINT)pMsg->wParam;
		
		// test des caractères numériques
		if	( (nChar >= '0' && nChar <= '9')	// caractères numériques
			||(nChar == '.' || nChar == ',')	// caractères séparateurs : virgule
			||(nChar == '-'))					// caractère '-'

		{
			if (ValidateValue(nChar))
			{
				// on appelle le gestionnaire de base
				this->CEdit::PreTranslateMessage(pMsg);

				// on quitte
				return FALSE;	
			}
			else
			{
				return TRUE;
			}
		}
		// test du moins
		else if ( nChar == '-')
		{
			if (ValidateValue(nChar))
			{
				// on appelle le gestionnaire de base
				this->CEdit::PreTranslateMessage(pMsg);

				// on quitte
				return FALSE;	
			}
			else
			{
				return TRUE;
			}
		}
		// tests des caractères spéciaux
		else if ( nChar == VK_BACK
			   || nChar == VK_ESCAPE
			   || nChar == VK_TAB
			   || nChar == VK_RETURN
			   || nChar == VK_DELETE)
		{
			// on appelle le gestionnaire de base
			this->CEdit::PreTranslateMessage(pMsg);

			// on quitte
			return FALSE;	
		}
		// sinon on quitte sans traiter le message
		return TRUE;
	}

	// on appelle le gestionnaire de base
	return (this->CEdit::PreTranslateMessage(pMsg));
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour valider le text de l'EditBox avant de l'afficher
bool JFCEditDecimal::ValidateValue (char nChar)
{
	// on récupère le text de l'EditBox
	CString Text; this->GetWindowTextA(Text);

	// on crée une chaine temporaire
	CString Temp; Temp = Text;

	// on récupère la position du curseur
	int start_pos, end_pos; this->GetSel(start_pos, end_pos);

	// si une partie du texte est selectionnée
	if (start_pos != end_pos)
	{
		Temp = Temp.Left(start_pos) + Temp.Right(Temp.GetLength() - end_pos);
	}

	// on ajoute à la chaine le caractère numérique saisi
	Temp.Insert (start_pos, nChar);

	// récupère la position de la virgule
	int dot_pos = Temp.FindOneOf (",.");

	// il y a une virgule
	if (dot_pos != -1)
	{
		// on s'assure qu'il n'y a qu'une
		CString tmp = Temp.Mid(dot_pos+1);
		if (tmp.FindOneOf (",.") != -1)
		{
			return false;
		}
		
		// on compte le nombre de décimales
		int decimals = Temp.GetLength() - 1 - dot_pos;

		if (decimals > m_NumDecimals)
		{
			return false;
		}
	}
	//on vérifie la position du '-' pour les nombres négatifs
	int minus_pos = Temp.Find("-");	
	if (m_AcceptNegativeNumbers == false && minus_pos != -1)
	{
		return false;
	}
	// on vérifie que le - est bien le 1er caractère
	else if (m_AcceptNegativeNumbers && minus_pos != 0)
	{
		return false;
	}
	
	// le texte est validé, on quitte
	return true;
}


int JFCEditDecimal::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// on appelle le gestionnaire de base
	if (this->CEdit::OnCreate(lpCreateStruct) == -1) return (-1);

	// on récupère le DC de la fenêtre
	CDC* pDC = this->GetWindowDC(); if (pDC == 0) return (-1);

	// on crée la fonte
	m_FontS.CreatePointFont(80, _T("Arial"), pDC);

	// on définit la fonte
	this->SetFont(&m_FontS);

	// on libère le DC
	this->ReleaseDC(pDC);

	// on continue la création
	return (0);
}

void JFCEditDecimal::OnDestroy()
{
	// on détruit la fonte
	m_FontS.DeleteObject();

	// on appelle le gestionnaire de base
	this->CEdit::OnDestroy();
}