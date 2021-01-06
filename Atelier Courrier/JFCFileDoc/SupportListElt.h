//
// descritpion : Elements de la listbox d'affichager et de sélection des supports (boite Offre, onglet Titres)
//
// author : Aurélien Rainone
//

#ifndef SUPPORTLIST_ELT_H
#define SUPPORTLIST_ELT_H

class IElem;

//=============================================================================	
// classe spécifique à la liste d'affichage des titres
class SupportListElt
//=============================================================================	
{

	public:
		
		typedef enum eSupportListEltType
		{
			sletTitre,
			sletCouplagefixe,
			sletCouplagelibre,
			sletFamille
		};

		SupportListElt(IElem * pElem_, eSupportListEltType type, bool bIsApparie_ = false);
		~SupportListElt();

		IElem * pElem;

		const JChar* GetLabel() const;
		JUnt32 Id;

		const bool			bIsTitre;
		const bool			bIsCouplage;
		const bool			bIsFamille;
		const bool			bIsApparie;
		
		const bool			bIsCouplageFixe;
		const bool			bIsCouplageLibre;

		bool				bIsSupportFromSource;

		BOOL iSelected;			// sélectionné par l'utilsateur

	private:

		const JChar *	pcszLabel;
		size_t			labelLen;
};

#endif //!SUPPORTLIST_ELT_H
