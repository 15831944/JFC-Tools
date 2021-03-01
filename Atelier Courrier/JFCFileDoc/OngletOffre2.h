#pragma once

class CDialogOffre2;

/************************************************************************/
/* Classe de Base contenant les fonctionnalit�s communes aux dialogues	*/
/* qui repr�sentent des onglets de la boite Offre						*/
/************************************************************************/
class COngletOffre2 : public CDialog
{
	DECLARE_DYNAMIC(COngletOffre2)

	public:

		COngletOffre2(UINT nIDTemplate);
		~COngletOffre2();		

		// assigne le pointeur vers dialogue de l'Offre 
		void SetData (CDialogOffre2 * pDlgOffre2);
		
		// � redefinir dans les onglets (retourne true si l'�tat de la s�lection est correct)
		virtual bool IsSelectionOk() = 0;

	protected:

		CDialogOffre2 * m_pDlgOffre;
};
