//
// Fichier: JFCEditDecimal.cpp
// Auteur:  Aur�lien Rainone
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"

// on inclut les d�finitions n�cessaires
#include "JFCEditDecimal.h"

//////////////////
// le constructeur

JFCEditDecimal::JFCEditDecimal()
{
	// on donne la valeur par d�faut
	m_AcceptNegativeNumbers = false;

	// on donne la valeur par d�faut
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
// les fonctions pour la gestion des nombres n�gatifs

void JFCEditDecimal::SetNegativeNumbers (const bool bAcceptNegativeNumbers)
{
	m_AcceptNegativeNumbers = bAcceptNegativeNumbers;
}

const bool JFCEditDecimal::AcceptNegativeNumbers () const
{
	return m_AcceptNegativeNumbers;
}

////////////////////////////////////////////////////////////
// les fonctions pour la gestion du nombre de d�cimales
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
		
		// test des caract�res num�riques
		if	( (nChar >= '0' && nChar <= '9')	// caract�res num�riques
			||(nChar == '.' || nChar == ',')	// caract�res s�parateurs : virgule
			||(nChar == '-'))					// caract�re '-'

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
		// tests des caract�res sp�ciaux
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
	// on r�cup�re le text de l'EditBox
	CString Text; this->GetWindowTextA(Text);

	// on cr�e une chaine temporaire
	CString Temp; Temp = Text;

	// on r�cup�re la position du curseur
	int start_pos, end_pos; this->GetSel(start_pos, end_pos);

	// si une partie du texte est selectionn�e
	if (start_pos != end_pos)
	{
		Temp = Temp.Left(start_pos) + Temp.Right(Temp.GetLength() - end_pos);
	}

	// on ajoute � la chaine le caract�re num�rique saisi
	Temp.Insert (start_pos, nChar);

	// r�cup�re la position de la virgule
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
		
		// on compte le nombre de d�cimales
		int decimals = Temp.GetLength() - 1 - dot_pos;

		if (decimals > m_NumDecimals)
		{
			return false;
		}
	}
	//on v�rifie la position du '-' pour les nombres n�gatifs
	int minus_pos = Temp.Find("-");	
	if (m_AcceptNegativeNumbers == false && minus_pos != -1)
	{
		return false;
	}
	// on v�rifie que le - est bien le 1er caract�re
	else if (m_AcceptNegativeNumbers && minus_pos != 0)
	{
		return false;
	}
	
	// le texte est valid�, on quitte
	return true;
}


int JFCEditDecimal::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// on appelle le gestionnaire de base
	if (this->CEdit::OnCreate(lpCreateStruct) == -1) return (-1);

	// on r�cup�re le DC de la fen�tre
	CDC* pDC = this->GetWindowDC(); if (pDC == 0) return (-1);

	// on cr�e la fonte
	m_FontS.CreatePointFont(80, _T("Arial"), pDC);

	// on d�finit la fonte
	this->SetFont(&m_FontS);

	// on lib�re le DC
	this->ReleaseDC(pDC);

	// on continue la cr�ation
	return (0);
}

void JFCEditDecimal::OnDestroy()
{
	// on d�truit la fonte
	m_FontS.DeleteObject();

	// on appelle le gestionnaire de base
	this->CEdit::OnDestroy();
}