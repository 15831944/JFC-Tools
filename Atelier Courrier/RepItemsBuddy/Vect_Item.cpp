// Vect_Item.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "RepItemsBuddy.h"
#include "Vect_Item.h"
#include ".\vect_item.h"


// CVect_Item

IMPLEMENT_DYNAMIC(CVect_Item, CCmdTarget)
CVect_Item::CVect_Item()
{
	EnableAutomation();
}

CVect_Item::~CVect_Item()
{
}


void CVect_Item::OnFinalRelease()
{
	// Lorsque la dernière référence pour un objet automation est libérée
	// OnFinalRelease est appelé. La classe de base supprime automatiquement
	// l'objet. Un nettoyage supplémentaire est requis pour votre
	// objet avant d'appeler la classe de base.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CVect_Item, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CVect_Item, CCmdTarget)
	DISP_PROPERTY_EX_ID		(CVect_Item,	"Count",			dispidCount,			GetCount, SetNotSupported,		VT_I4)
	DISP_PROPERTY_PARAM_ID	(CVect_Item,	"Indiv",			dispidIndiv,			GetIndiv, SetNotSupported,		VT_I1, VTS_I4)
	DISP_PROPERTY_PARAM_ID	(CVect_Item,	"IndivAsInt",		dispidIndivAsInt,		GetIndivAsInt, SetNotSupported, VT_I4, VTS_I4)
	DISP_FUNCTION_ID		(CVect_Item,	"VectIndivAsArray", dispidVectIndivAsArray, VectIndivAsArray,				VT_I4, VTS_VARIANT)
END_DISPATCH_MAP()

// Remarque : Nous avons ajouté une prise en charge pour IID_IVect_Item afin de prendre en charge les liaisons de type sécurisé
//  à partir de VBA. Cet IID doit correspondre au GUID qui est attaché à 
//  dispinterface dans le fichier .IDL.

// {DC6A2CAB-04A3-42C0-8464-3A5EB46016E8}
static const IID IID_IVect_Item =
{ 0xDC6A2CAB, 0x4A3, 0x42C0, { 0x84, 0x64, 0x3A, 0x5E, 0xB4, 0x60, 0x16, 0xE8 } };

BEGIN_INTERFACE_MAP(CVect_Item, CCmdTarget)
	INTERFACE_PART(CVect_Item, IID_IVect_Item, Dispatch)
END_INTERFACE_MAP()


// Gestionnaires de messages CVect_Item

LONG CVect_Item::GetCount(void)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	if(m_params.GetResults())
		return m_params.GetResults()->GetSize();

	return 0;
}

BOOL CVect_Item::GetIndiv(LONG idInd)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	if(m_params.GetResults())
		return m_params.GetResults()->GetAt(idInd);

	return 0;
}

LONG CVect_Item::GetIndivAsInt(LONG idInd)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	if(m_params.GetResults())
		return m_params.GetResults()->GetAt(idInd);

	return 0;
}

HRESULT CVect_Item::VectIndivAsArray(VARIANT &vArray)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// make sure it's a integer array
	if (V_VT(&vArray) != (VT_BYREF | VT_ARRAY | VT_I4))

		AfxThrowOleDispatchException(1001, "Type Mismatch in Parameter. Pass a Integer array by reference");

	SAFEARRAY **ppsa = V_ARRAYREF(&vArray);

	long lUBound;

	SafeArrayGetUBound(*ppsa, 1, &lUBound);

	LONG Val = 0;

	for (long i = 1; i <= lUBound; i++)
	{
		Val = (LONG) m_params.GetResults()->GetAt(i);
		if (FAILED(SafeArrayPutElement(*ppsa, &i, (void*)&Val))) goto error;
	} 
	
	return S_OK;

	error:
		AfxThrowOleDispatchException(1003, "Unexpected Failure in VectIndivAsArray method");

	return 0;
}