// Vect_Poids.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "RepItemsBuddy.h"
#include "Vect_Poids.h"
#include ".\vect_poids.h"


// CVect_Poids

IMPLEMENT_DYNAMIC(CVect_Poids, CCmdTarget)
CVect_Poids::CVect_Poids()
{
	EnableAutomation();
	m_vect = NULL;
}

CVect_Poids::~CVect_Poids()
{
}


void CVect_Poids::OnFinalRelease()
{
	// Lorsque la dernière référence pour un objet automation est libérée
	// OnFinalRelease est appelé. La classe de base supprime automatiquement
	// l'objet. Un nettoyage supplémentaire est requis pour votre
	// objet avant d'appeler la classe de base.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CVect_Poids, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CVect_Poids, CCmdTarget)
	DISP_PROPERTY_EX_ID		(CVect_Poids, "Count",					dispidCount,				GetCount,				SetNotSupported,	VT_I4)
	DISP_PROPERTY_PARAM_ID	(CVect_Poids, "PoidsIndiv",				dispidPoidsIndiv,			GetPoidsIndiv,			SetNotSupported,	VT_R8, VTS_I4)
	DISP_FUNCTION_ID		(CVect_Poids, "VectPoidsIndivAsArray",	dispidVectPoidsIndivAsArray,VectPoidsIndivAsArray,	VT_I4,				VTS_VARIANT)
END_DISPATCH_MAP()

// Remarque : Nous avons ajouté une prise en charge pour IID_IVect_Poids afin de prendre en charge les liaisons de type sécurisé
//  à partir de VBA. Cet IID doit correspondre au GUID qui est attaché à 
//  dispinterface dans le fichier .IDL.

// {20570A63-13D8-4A6E-903A-CC00C8B1E485}
static const IID IID_IVect_Poids =
{ 0x20570A63, 0x13D8, 0x4A6E, { 0x90, 0x3A, 0xCC, 0x0, 0xC8, 0xB1, 0xE4, 0x85 } };

BEGIN_INTERFACE_MAP(CVect_Poids, CCmdTarget)
	INTERFACE_PART(CVect_Poids, IID_IVect_Poids, Dispatch)
END_INTERFACE_MAP()


// Gestionnaires de messages CVect_Poids

LONG CVect_Poids::GetCount(void)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	if(m_vect)
		return m_vect->GetCount();
	return 0;
}

DOUBLE CVect_Poids::GetPoidsIndiv(LONG idInd)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	if(m_vect)
	{
		m_vect->MoveTo(idInd);
		if(m_vect->IsValid())
			return m_vect->GetItem();
	}
	
	return 0;
	
}

HRESULT CVect_Poids::VectPoidsIndivAsArray(VARIANT &vArray)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
 
	// make sure it's a double array

	if (V_VT(&vArray) != (VT_BYREF | VT_ARRAY | VT_R8))
		AfxThrowOleDispatchException(1001, "Type Mismatch in Parameter. Pass a Double array by reference");

	SAFEARRAY **ppsa = V_ARRAYREF(&vArray);

	double dVal = 0;

	long i = 0;
	for (m_vect->MoveFirst() ; m_vect->IsValid();m_vect->MoveNext())
	{
		dVal = m_vect->GetItem();
		if (FAILED(SafeArrayPutElement(*ppsa, &i, (void*)&dVal))) goto error;
		++i;
	}

	return S_OK;

error:
	AfxThrowOleDispatchException(1003, "Unexpected Failure in VectPoidsIndivAsArray method");

	return 0;
}