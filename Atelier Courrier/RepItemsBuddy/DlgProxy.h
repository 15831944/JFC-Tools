// DlgProxy.h�: fichier d'en-t�te
//

#pragma once

class CRepItemsBuddyDlg;
#include "RepItems.h"


// cible de la commande CRepItemsBuddyProxy

class CRepItemsBuddyProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CRepItemsBuddyProxy)

public:
	CRepItemsBuddyProxy();           // constructeur prot�g� utilis� par la cr�ation dynamique

// Attributs
	CRepItemsBuddyDlg* m_pDialog;
// Op�rations
public:

// Substitutions
	public:
	virtual void OnFinalRelease();

// Impl�mentation
	virtual ~CRepItemsBuddyProxy();

protected:
	// Fonctions g�n�r�es de la table des messages

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CRepItemsBuddyProxy)

	// Fonctions g�n�r�es de la table des r�partitions OLE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	enum 
	{
		dispidSetPoids			= 10L,
		dispidGetListPoids		= 9L,
		dispidTabIndiv			= 9L,
		dispidtabIndiv			= 9L,
		dispidTabVectIndiv		= 9L,
		dispidtoto				= 10,
		dispidtest				= 9L,
		dispidGetVect_Item		= 8L,
		dispidGetItem			= 7L,
		dispidMoveNextItem		= 6L,
		dispidIsValidItem		= 5L,
		dispidMoveFirstItem		= 4L,
		dispidVect_Poids		= 3,
		dispidLoadTerrain		= 2L,
		dispidShowRepItem		= 1L
	};

public:

	void		ShowRepItem();
	BOOL		LoadTerrain(LONG idSrc, LONG idTerr);
	BSTR		GetListPoids();
	void		SetPoids(LONG idPoids);

	IDispatch*	GetVect_Poids	(void);
	void		MoveFirstItem	(void);
	BOOL		IsValidItem		(void);
	void		MoveNextItem	(void);
	IDispatch*	GetItem			(void);
	IDispatch*	GetVect_Item	(LONG idItem);
	void		ListIndiv		(LPCTSTR List);

private:
	CDlgItems * m_pRepItems;
	CTerrain * m_pTerrain;
	JList<const IItem *> m_lstItem;
	int m_idSrc;
	int m_idTerr;
	int m_idTypePoids;

protected:
};

