#pragma once

class CDialogOffre2;

/************************************************************************/
/* Classe de Base contenant les fonctionnalités communes aux dialogues	*/
/* qui représentent des onglets de la boite Offre						*/
/************************************************************************/
class COngletOffre2 : public CDialog
{
	DECLARE_DYNAMIC(COngletOffre2)

	public:

		COngletOffre2(UINT nIDTemplate);
		~COngletOffre2();		

		// assigne le pointeur vers dialogue de l'Offre 
		void SetData (CDialogOffre2 * pDlgOffre2);
		
		// à redefinir dans les onglets (retourne true si l'état de la sélection est correct)
		virtual bool IsSelectionOk() = 0;

	protected:

		CDialogOffre2 * m_pDlgOffre;
};
