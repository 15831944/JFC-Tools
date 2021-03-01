#pragma once

class COffre2MainDialog;
class JATPOffre;

/************************************************************************/
/* Classe de Base repr�sentant un onglet de la boite Offre				*/
/*																		*/
/* Dans cette classe abstraite sont rassembl�es les fonctionnalit�s		*/
/* communes aux															*/
/* dialogues repr�sentant un onglet de la boite Offre					*/
/************************************************************************/
class COffre2OngletBase : public CDialog
{
	DECLARE_DYNAMIC(COffre2OngletBase)

	public:

		COffre2OngletBase(UINT nIDTemplate, COffre2MainDialog * pDialogOffre);
		~COffre2OngletBase();		
		
		// � redefinir dans les onglets (retourne true si l'�tat de la s�lection est correct)
		virtual bool IsSelectionOk() const = 0;

		// remplit la partie de l'offre correspondant � l'onglet
		virtual void FillOffre(JATPOffre * pOffre) {}

		// notifie l'onglet qu'il vient d'�tre s�lectionn�/d�selectionn�
		virtual void NotifySelectionChange(bool bSelect) {}

	protected:
		virtual void OnOK();
		virtual void OnCancel();

		COffre2MainDialog * m_pDlgOffre;
};
