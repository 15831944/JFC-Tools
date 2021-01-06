#pragma once

class COffre2MainDialog;
class JATPOffre;

/************************************************************************/
/* Classe de Base représentant un onglet de la boite Offre				*/
/*																		*/
/* Dans cette classe abstraite sont rassemblées les fonctionnalités		*/
/* communes aux															*/
/* dialogues représentant un onglet de la boite Offre					*/
/************************************************************************/
class COffre2OngletBase : public CDialog
{
	DECLARE_DYNAMIC(COffre2OngletBase)

	public:

		COffre2OngletBase(UINT nIDTemplate, COffre2MainDialog * pDialogOffre);
		~COffre2OngletBase();		
		
		// à redefinir dans les onglets (retourne true si l'état de la sélection est correct)
		virtual bool IsSelectionOk() const = 0;

		// remplit la partie de l'offre correspondant à l'onglet
		virtual void FillOffre(JATPOffre * pOffre) {}

		// notifie l'onglet qu'il vient d'être sélectionné/déselectionné
		virtual void NotifySelectionChange(bool bSelect) {}

	protected:
		virtual void OnOK();
		virtual void OnCancel();

		COffre2MainDialog * m_pDlgOffre;
};
