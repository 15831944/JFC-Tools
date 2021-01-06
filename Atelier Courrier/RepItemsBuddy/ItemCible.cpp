// ItemCible.cpp : fichier d'impl�mentation
//

#include "stdafx.h"
#include "RepItemsBuddy.h"
#include "ItemCible.h"
#include ".\itemcible.h"


// CItemCible

IMPLEMENT_DYNAMIC(CItemCible, CCmdTarget)
CItemCible::CItemCible()
{
	EnableAutomation();
}

CItemCible::~CItemCible()
{
}


void CItemCible::OnFinalRelease()
{
	// Lorsque la derni�re r�f�rence pour un objet automation est lib�r�e
	// OnFinalRelease est appel�. La classe de base supprime automatiquement
	// l'objet. Un nettoyage suppl�mentaire est requis pour votre
	// objet avant d'appeler la classe de base.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CItemCible, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CItemCible, CCmdTarget)
	DISP_PROPERTY(CItemCible, "text", m_label, VT_BSTR)
	DISP_PROPERTY_EX_ID(CItemCible, "ident", dispidident, GetIdent, SetNotSupported, VT_I4)
END_DISPATCH_MAP()

// Remarque�: Nous avons ajout� une prise en charge pour IID_IItemCible afin de prendre en charge les liaisons de type s�curis�
//  � partir de VBA. Cet IID doit correspondre au GUID qui est attach� � 
//  dispinterface dans le fichier .IDL.

// {5F358FF4-0546-4400-9BDD-CBCF71B74D6C}
static const IID IID_IItemCible =
{ 0x5F358FF4, 0x546, 0x4400, { 0x9B, 0xDD, 0xCB, 0xCF, 0x71, 0xB7, 0x4D, 0x6C } };

BEGIN_INTERFACE_MAP(CItemCible, CCmdTarget)
	INTERFACE_PART(CItemCible, IID_IItemCible, Dispatch)
END_INTERFACE_MAP()


// Gestionnaires de messages CItemCible

LONG CItemCible::GetIdent(void)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	// TODO�: ajoutez ici le code de votre gestionnaire de dispatch

	return (LONG)m_ident;
}
