// CalcQuantiValuesVisitor.cpp: implementation of the CCalcValuesVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CalcQuantiValuesVisitor.h"
#include "ABuilder.h"
#include "ABQuestion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcValuesVisitor::CCalcValuesVisitor()
{

}

CCalcValuesVisitor::~CCalcValuesVisitor()
{
	m_ValuesMap.Reset();
}

JBool	CCalcValuesVisitor::Calculate(CABuilder *pBuilder, JFLT32VECTOR &Values, JInt32 &Divider, CTerrain * pTerrain)
{
	m_Valid				=	true;
	m_pTerrain			=	pTerrain;

	// Construction des elmts pour calcul value 
	pBuilder->Accept(*this);

	if(m_Valid)
	{
		// Récupère les valeurs quanti
		GetVarValues(Values, Divider);
	}
	return m_Valid ;
}

void CCalcValuesVisitor::VisitStart(CABQuestion & Question)
{
	m_pQuestion = 0;
	m_ValuesMap.Reset();

	// Init Map des identifiants Termes Construits
	MapIdTerme.Reset();

	// Init du vecteur cumul quantitatif
	VectCumulQuanti.RemoveAll();
}

void CCalcValuesVisitor::VisitEnd(CABTerme & Terme)
{
	int Ind;

	if(!Terme.HasOperator())
	{
		// Get a Question pointer
		if(!m_pQuestion)
			m_pQuestion = m_pTerrain->GetQuestionByRepAbs(Terme.GetID());

		if(m_pQuestion && m_pQuestion->HasValues())
		{
			JInt32 IdTerme = Terme.GetID();

			// Récupère le vecteur values par individus
			const JFLT32VECTOR * pValues = m_pTerrain->GetVarNumValuesByRepAbs(IdTerme);

			// Vérifie d'abord si elment non deja comptabilisé
			MapIdTerme.MoveTo(IdTerme);

			if (!MapIdTerme.IsValid())
			{
				if (pValues)
				{
					if (VectCumulQuanti.GetCount() == 0)
						VectCumulQuanti.SetSize(pValues->GetCount());

					for (Ind=0, pValues->MoveFirst(); pValues->IsValid(); pValues->MoveNext(), ++Ind)
					{
						// Valeur indiv sur la moda
						long ValIndiv = (long)pValues->GetItem();
						if (ValIndiv > 0)
						{
							VectCumulQuanti.GetAt(Ind) += ValIndiv;
						}
					}
	
					// Suite Analyse avec Xavier
					// On libère le vecteur
					JFLT32VECTOR *pVal = (JFLT32VECTOR *)pValues;
					pVal->Reset();
				}

				// On ajoute au map des items à vérifier
				MapIdTerme.Add(IdTerme) = 0;
			}

		
			/* AVANT
			JInt32	Divider = m_pTerrain->GetVarDividerByRepAbs(Terme.GetID());
			if(pValues)
			{
				m_ValuesMap.MoveTo(pValues);
				if(!m_ValuesMap.IsValid())
				{
					m_ValuesMap.Add(pValues) = Divider;
				}
			}
			*/
		}
	}
}

void CCalcValuesVisitor::GetVarValues(JVector<JFlt32> & Values, JInt32 & Divider)
{
	JMap<JFlt32,JBool> ValUnique;
	ValUnique.Reset();
	JBool FlagFirst = true;

	long Val;
	int Ind;

	// Lecture Mode Cumul Valeurs Quantitatives  (pour Balmétrie notamment)  // Par défaut on reste en mode calcul sans cumul  (modif Juillet 2013)
	JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 1) != 0?true:false;

	if (CumulQuanti)
	{
		if(VectCumulQuanti.GetCount())
		{
			// Modif pour prendre en compte valeurs cumulées maxi (19 Juillet 2013)
			double Tot = 0;
			for(int Ind=0 ; Ind < VectCumulQuanti.GetCount(); Ind++)
			{
				// Puis récupérer les valeurs valides
				Val = VectCumulQuanti.GetAt(Ind);
				if (Val == 0) Val = -1;
				ValUnique.MoveTo(Val);
				if(!ValUnique.IsValid())
				{
					ValUnique.Add(Val) = true;
				}

				Tot += Val;
			}

			bool Ok = true;
		}
	}
	else
	{
		// ---------------- AVANT
		for(m_ValuesMap.MoveFirst(); m_ValuesMap.IsValid(); m_ValuesMap.MoveNext())
		{
			Divider = m_ValuesMap.GetItem();
			const JFLT32VECTOR *pVal = m_ValuesMap.GetKey();
			if(pVal)
			{
				for(pVal->MoveFirst(); pVal->IsValid(); pVal->MoveNext())
				{
					ValUnique.MoveTo(pVal->GetItem());
					if(!ValUnique.IsValid())
					{
						JFlt32 Val = pVal->GetItem();	
						ValUnique.Add(pVal->GetItem()) = true;
					}
				}
			}
		}
	}

	Values.Reset();
	Values.SetCount(ValUnique.GetCount());
	for(Values.MoveFirst(),ValUnique.MoveFirst(); ValUnique.IsValid(); Values.MoveNext(), ValUnique.MoveNext())
	{
		Values.GetItem() = ValUnique.GetKey();
	}
}

/* Ancien Mode Cumul Quanti
void CCalcValuesVisitor::GetVarValues(JVector<JFlt32> & Values, JInt32 & Divider)
{
	JMap<JFlt32,JBool> ValUnique;
	ValUnique.Reset();
	JBool FlagFirst = true;

	CArray<long, long &>	VectCumulQuanti;
	long Val;
	int Ind;

	// Generate vector of Variable Numeric Values
	if(m_ValuesMap.GetCount())
	{
		// Lecture Mode Cumul Valeurs Quantitatives  (pour Balmétrie notamment)  // Par défaut on reste en mode calcul sans cumul  (modif Juillet 2013)
		// JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 0) != 0?true:false;
		JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 1) != 0?true:false;

		if (CumulQuanti)
		{
			// Il faut maintenant cumuler les différentes values sur chaque indiv
			for(m_ValuesMap.MoveFirst(); m_ValuesMap.IsValid(); m_ValuesMap.MoveNext())
			{
				const JFLT32VECTOR *pVal = m_ValuesMap.GetKey();
				if(pVal)
				{
					if (FlagFirst == true)
					{
						VectCumulQuanti.SetSize(pVal->GetCount());
					}
					FlagFirst = false;

					for (Ind=0, pVal->MoveFirst(); pVal->IsValid(); pVal->MoveNext(), ++Ind)
					{
						// Valeur indiv sur la moda
						long ValIndiv = (long)pVal->GetItem();

						if (ValIndiv > 0)
						{
							VectCumulQuanti.GetAt(Ind) += ValIndiv;
						}
					}
				}
			}

			// Modif pour prendre en compte valeurs cumulées maxi (19 Juillet 2013)
			for(int Ind=0 ; Ind < VectCumulQuanti.GetCount(); Ind++)
			{
				// Puis récupérer les valeurs valides
				Val = VectCumulQuanti.GetAt(Ind);
				if (Val == 0) Val = -1;
				ValUnique.MoveTo(Val);
				if(!ValUnique.IsValid())
				{
					ValUnique.Add(Val) = true;
				}
			}
		}
		else
		{
			// ---------------- AVANT
			for(m_ValuesMap.MoveFirst(); m_ValuesMap.IsValid(); m_ValuesMap.MoveNext())
			{
				Divider = m_ValuesMap.GetItem();
				const JFLT32VECTOR *pVal = m_ValuesMap.GetKey();
				if(pVal)
				{
					for(pVal->MoveFirst(); pVal->IsValid(); pVal->MoveNext())
					{
						ValUnique.MoveTo(pVal->GetItem());
						if(!ValUnique.IsValid())
						{
							JFlt32 Val = pVal->GetItem();	
							ValUnique.Add(pVal->GetItem()) = true;
						}
					}
				}
			}
		}
	}

	Values.Reset();
	Values.SetCount(ValUnique.GetCount());
	for(Values.MoveFirst(),ValUnique.MoveFirst(); ValUnique.IsValid(); Values.MoveNext(), ValUnique.MoveNext())
	{
		Values.GetItem() = ValUnique.GetKey();
	}
}
*/

