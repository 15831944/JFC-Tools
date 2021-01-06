// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPColors.h"

// on inclut les définitions nécessaires
#include "JFCEditGrid.h"
#include ".\jfceditgrid.h"

//////////////////
// le constructeur

JFCEditGrid::JFCEditGrid()
{
	// on initialise l'état
	m_Etat = 0;
}

JFCEditGrid::~JFCEditGrid()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCEditGrid, CEdit)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
	ON_WM_KILLFOCUS()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// JFCEditGrid message handlers

BOOL JFCEditGrid::PreTranslateMessage(MSG* pMsg)
{
	// on teste le message
	if (pMsg->message == WM_KEYDOWN)
	{
		// on teste la touche
		if (pMsg->wParam == VK_RETURN)
		{
			// on récupère la fenêtre parente
			CWnd* pParent = this->GetParent();

			// on teste l'état
			if (m_Etat == 1)
			{
				// on modifie l'état
				m_Etat = 0;

				// on envoie le message
				if (pParent != NULL) pParent->SendMessage(JFC_CANCEL_EDITING, 0, 0);

				// on quitte
				return (TRUE);
			}
			else if (m_Etat == 2)
			{
				// on modifie l'état
				m_Etat = 0;

				// on envoie le message
				if (pParent != NULL) pParent->SendMessage(JFC_END_EDITING, 0, 0);

				// puis on va sur la cellule suivante (de haut en bas, si derniere cellule basse on va en haut sur a colonne suivante)
				// on envoie le message ssi on est en mode mono cellule
				POINT * pPoint = &pMsg->pt;
				if (pParent != NULL) pParent->SendMessage(JFC_BEGIN_NEW_EDITING, 0, (LPARAM)pPoint);

				// on quitte
				return (TRUE);
			}
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			// on récupère la fenêtre parente
			CWnd* pParent = this->GetParent();

			// on modifie l'état
			m_Etat = 0;

			// on envoie le message
			if (pParent != NULL) pParent->SendMessage(JFC_CANCEL_EDITING, 0, 0);

			// on quitte
			return (TRUE);
		}
		// Gestion touche aller sur cellule en dessous
		else if (pMsg->wParam == VK_DOWN)
		{
			// on récupère la fenêtre parente
			CWnd* pParent = this->GetParent();

			if (m_Etat == 2)
			{
				// on modifie l'état
				m_Etat = 0;

				// on envoie le message
				POINT * pPoint = &pMsg->pt;
				if (pParent != NULL) pParent->SendMessage(JFC_BEGIN_NEW_EDITING_DOWN, 0, (LPARAM)pPoint);

				// on quitte
				return (TRUE);
			}
		}
		// Gestion touche aller sur cellule au dessus
		else if (pMsg->wParam == VK_UP)
		{
			// on récupère la fenêtre parente
			CWnd* pParent = this->GetParent();

			if (m_Etat == 2)
			{
				// on modifie l'état
				m_Etat = 0;

				// on envoie le message
				POINT * pPoint = &pMsg->pt;
				if (pParent != NULL) pParent->SendMessage(JFC_BEGIN_NEW_EDITING_UP, 0, (LPARAM)pPoint);

				// on quitte
				return (TRUE);
			}
		}
		// Gestion touche aller sur cellule à gauche
		else if (pMsg->wParam == VK_LEFT)
		{
			// on récupère la fenêtre parente
			CWnd* pParent = this->GetParent();

			if (m_Etat == 2)
			{
				// on modifie l'état
				m_Etat = 0;

				// on envoie le message
				POINT * pPoint = &pMsg->pt;
				if (pParent != NULL) pParent->SendMessage(JFC_BEGIN_NEW_EDITING_LEFT, 0, (LPARAM)pPoint);

				// on quitte
				return (TRUE);
			}
		}
		// Gestion touche aller sur cellule à gauche
		else if (pMsg->wParam == VK_RIGHT)
		{
			// on récupère la fenêtre parente
			CWnd* pParent = this->GetParent();

			if (m_Etat == 2)
			{
				// on modifie l'état
				m_Etat = 0;

				// on envoie le message
				POINT * pPoint = &pMsg->pt;
				if (pParent != NULL) pParent->SendMessage(JFC_BEGIN_NEW_EDITING_RIGHT, 0, (LPARAM)pPoint);

				// on quitte
				return (TRUE);
			}
		}
	}
	else if (pMsg->message == 512)
	{
		/*
		// on récupère la fenêtre parente
		CWnd* pParent = this->GetParent();

		if (m_Etat == 2)
		{
			// on modifie l'état
			m_Etat = 0;

			// on envoie le message
			if (pParent != NULL) pParent->SendMessage(JFC_END_EDITING, 0, 0);

			// puis on va sur la cellule suivante (de haut en bas, si derniere cellule basse on va en haut sur a colonne suivante)
			// on envoie le message ssi on est en mode mono cellule
			POINT * pPoint = &pMsg->pt;
			if (pParent != NULL) pParent->SendMessage(JFC_BEGIN_EDITING, 0, (LPARAM)pPoint);

			// on quitte
			return (TRUE);
		}
		*/
	}

	// on appelle le gestionnaire de base
	return (this->CEdit::PreTranslateMessage(pMsg));
}

int JFCEditGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// on appelle le gestionnaire de base
	if (this->CEdit::OnCreate(lpCreateStruct) == -1) return (-1);

	// on récupère le DC de la fenêtre
	CDC* pDC = this->GetWindowDC(); if (pDC == 0) return (-1);

	// on crée la fonte
	m_FontS.CreatePointFont(CATPColors::GetFontSizeS(), CATPColors::GetFontName(), pDC);

	// on définit la fonte
	this->SetFont(&m_FontS);

	// on libère le DC
	this->ReleaseDC(pDC);

	// on initialise l'état
	m_Etat = 0;

	// on continue la création
	return (0);
}

void JFCEditGrid::OnEnChange()
{
	// on modifie l'état
	m_Etat = 2;
}

void JFCEditGrid::OnSetFocus(CWnd* pOldWnd)
{
	// on modifie l'état
	m_Etat = 1;

	// on appelle le gestionnaire de base
	this->CEdit::OnSetFocus(pOldWnd);
}

void JFCEditGrid::OnKillFocus(CWnd* pNewWnd)
{
	// on récupère la fenêtre parente
	CWnd* pParent = this->GetParent();

	// on teste l'état
	if (m_Etat == 1)
	{
		// on modifie l'état
		m_Etat = 0;

		// on envoie le message
		if (pParent != NULL) pParent->SendMessage(JFC_CANCEL_EDITING, 0, 0);
	}
	else if (m_Etat == 2)
	{
		// on modifie l'état
		m_Etat = 0;

		// POINT * pPoint = this->GetP

		// on envoie le message
		if (pParent != NULL) pParent->SendMessage(JFC_END_EDITING, 0, 0);
	}

	// on appelle le gestionnaire de base
	this->CEdit::OnKillFocus(pNewWnd);
}

void JFCEditGrid::OnDestroy()
{
	// on modifie l'état
	m_Etat = 0;

	// on détruit la fonte
	m_FontS.DeleteObject();

	// on appelle le gestionnaire de base
	this->CEdit::OnDestroy();
}

void JFCEditGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		bool Ok = true;
	}
}