// ItemCible.cpp : fichier d'implémentation
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
	// Lorsque la dernière référence pour un objet automation est libérée
	// OnFinalRelease est appelé. La classe de base supprime automatiquement
	// l'objet. Un nettoyage supplémentaire est requis pour votre
	// objet avant d'appeler la classe de base.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CItemCible, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CItemCible, CCmdTarget)
	DISP_PROPERTY(CItemCible, "text", m_label, VT_BSTR)
	DISP_PROPERTY_EX_ID(CItemCible, "ident", dispidident, GetIdent, SetNotSupported, VT_I4)
END_DISPATCH_MAP()

// Remarque : Nous avons ajouté une prise en charge pour IID_IItemCible afin de prendre en charge les liaisons de type sécurisé
//  à partir de VBA. Cet IID doit correspondre au GUID qui est attaché à 
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

	// TODO : ajoutez ici le code de votre gestionnaire de dispatch

	return (LONG)m_ident;
}
