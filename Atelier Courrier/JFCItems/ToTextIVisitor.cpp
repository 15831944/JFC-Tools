#include "stdafx.h"
#include "totextivisitor.h"
#include "IBuilder.h"
#include "IBQuestion.h"
#include "IBItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString DecalTxt = "    ";

////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CToTextIVisitor::CToTextIVisitor(void)
{
	m_OpStrate		=	"";
	m_OpQuestion	=	"";
	m_OpModalite	=	"";
	m_pBuilder		=	0;
	m_pstrDescript	=	0;
	m_pTerrain		=	0;
	m_pIConst		=	0;
	m_ModaDim		=	0;
	m_pQuestion		=	0;
	
	// Init N° du segment en cours
	m_NoSegment		=   1;
	m_NoQuestion	=	1;
	m_NoModalite	=   1;

}

///////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CToTextIVisitor::~CToTextIVisitor(void)
{
}

JBool CToTextIVisitor::ToDescription(CIBuilder *pBuilder, CItemsConstruitsBase *pItemConst, CTerrain * pTerrain, CString * pstrDescript)
{
	m_pBuilder		=	pBuilder;
	m_pstrDescript	=	pstrDescript;
	m_pTerrain		=	pTerrain;
	m_pIConst		=	pItemConst;

	if(m_pTerrain && m_pstrDescript && m_pBuilder)
		pBuilder->Accept(*this);
	else
		return false;
	return true;
}

void CToTextIVisitor::VisitStart(CIBuilder & Builder)
{
	std::string str;
	// Builder.OperatorAsString(str,Builder.GetOperator());
	Builder.OperatorAsXML(str,Builder.GetOperator());
	m_OpStrate = str.c_str();
}

void CToTextIVisitor::VisitEnd(CIBuilder & Builder)
{
}

void CToTextIVisitor::VisitStart(CIBSegment & Segment)
{
	std::string str;
	// Segment.OperatorAsString(str,Segment.GetOperator());
	Segment.OperatorAsXML(str,Segment.GetOperator());
	m_OpQuestion.Format(_T("%s"), str.c_str());

	CString Temp;

	if (m_NoSegment == 1)
		// Uniquement Segment et son n°
		Temp.Format(_T("Segment %d \n"), m_NoSegment);
	else
		// Opérateur entre segments + Segment et son n°
		Temp.Format(_T("%s\n Segment %d\n"), m_OpStrate, m_NoSegment);

	m_pstrDescript->Append (Temp);

	m_pQuestion = 0;

	// Passe au segment suivant
	m_NoSegment++;

	// Init N° question et N° Modalite
	m_NoQuestion = 1;
	m_NoModalite = 1;
}

void CToTextIVisitor::VisitEnd(CIBSegment & Segment)
{
	m_pstrDescript->Append (_T("\n"));
}

void CToTextIVisitor::VisitStart(CIBQuestion & Question)
{
	// Find the question label
	CString qName = _T("error");
	m_pQuestion = m_pTerrain->GetQuestionByID(Question.GetID());
	if(m_pQuestion)
		qName.Format(_T("%s"),m_pQuestion->GetLabel().AsJCharPtr());

	CString Temp;
	if (m_NoQuestion == 1)
		// Pas d'opérateur devant 1er elmt question
		Temp.Format(_T("%s %s\n"), DecalTxt, qName);
	else
		// Opérateur devant tous les autes elmts questions
		Temp.Format(_T("%s %s %s\n"),DecalTxt, m_OpQuestion, qName);

	m_pstrDescript->Append (Temp);
	m_ModaDim = 0;

	// Passe à question suivante et init N° Modalite
	m_NoQuestion++;
	m_NoModalite = 1;
}

void CToTextIVisitor::VisitEnd(CIBQuestion & Question)
{
	// m_pstrDescript->Append (_T("\n"));
	m_pQuestion = 0;
}

void CToTextIVisitor::Visit(CIBModalite & Modalite)
{
	// Output modalite bloc begin with operator and type
	std::string strOpe;
	// Modalite.OperatorAsString(str,Modalite.GetOperator());
	Modalite.OperatorAsXML(strOpe,Modalite.GetOperator());
	CString Temp;

	JList<JInt32> Values;
	Modalite.GetValues(Values);

	// Saut de ligne
	// m_pstrDescript->Append (_T("\n"));

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		CString Libelle = _T("error");
		if(m_pQuestion)
		{
			const IModalite * pMod = m_pQuestion->GetIModalite(Values.GetItem(),m_ModaDim);
			if(pMod)
				Libelle = pMod->GetLabel().AsJCharPtr();
		}

		if (m_NoModalite == 1)
			// Pas d'opérateur devant 1ere modalite
			Temp.Format(_T("%s%s %s\n"),DecalTxt, DecalTxt,Libelle);
		else
			// On ajoute opérateur devant les autres modalités
			Temp.Format(_T("%s%s %s %s\n"),DecalTxt,DecalTxt, strOpe.c_str(),Libelle);

		m_pstrDescript->Append (Temp);

		// Passe à valeur modalité suivante
		m_NoModalite++;
	}
	// Output modalite bloc end
	m_pstrDescript->Append (_T("\n"));
	m_ModaDim++;
	
}

void CToTextIVisitor::Visit(CIBNumValue & NumValue)
{
	return ; 

	// A VOIR
	CString Temp;

	JList<JFlt32> Values;
	NumValue.GetValues(Values);

	m_pstrDescript->Append (_T("\n"));

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		CString strDiv;
		if(Values.GetItem() < 0.0)
			strDiv.Format(_T("#"));
		else
			strDiv.Format(_T("%0.f"),Values.GetItem());

		Temp.Format(_T("%s\n"),strDiv);
		m_pstrDescript->Append (Temp);
	}
	// Output values bloc end
	m_pstrDescript->Append (_T("\n"));
}

void CToTextIVisitor::Visit(CIBClass & Class)
{
	// A FAIRE
	return;
}

void CToTextIVisitor::Visit(CIBQuant & Quant)
{
	// A FAIRE
	return;
}

void CToTextIVisitor::Visit(CIBPoint & Point)
{
	// A FAIRE
	return;
}

void CToTextIVisitor::VisitStart(CIBItem & Item) 
{
	CString Type = _T("items");
	CString Name = _T("Pseudo question");
	IItem *pItem = m_pIConst->GetIItemByID(Item.GetID());

	if(pItem)
	{
		if(pItem->IsQuestionInduite())
			Type = _T("induite");
		if(pItem->IsPalette())
			Type = _T("palette");
		else if(pItem->IsCritereModifie())
			Type = _T("modifie");
		else if(pItem->IsCritereQuantile())
			Type = _T("quantile");

		Name = pItem->GetLabel().AsJCharPtr(); 
	}

	// Output modalite bloc begin with operator and type
	// Find the question label
	CString Temp;
	Temp.Format(_T("%s\n"),m_OpQuestion);
	// m_pstrDescript->Append (Temp);

	std::string str;
	// Item.OperatorAsString(str,Item.GetOperator());
	Item.OperatorAsXML(str,Item.GetOperator());

	JList<JInt32> Values;
	Item.GetValues(Values);

	Temp.Format(_T("%s\n"), str.c_str());
	// m_pstrDescript->Append (Temp);

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		CString Libelle = _T("error");
		pItem = m_pIConst->GetIItemByID(Values.GetItem());
		if(pItem)
			Libelle = pItem->GetLabel().AsJCharPtr();

		if (m_NoQuestion == 1)
			// Pas d'opérateur devant 1er item
			Temp.Format(_T("%s %s\n"),DecalTxt, Libelle);
		else
			// Opérateur devant tous les autres items
			Temp.Format(_T("%s %s %s\n"),DecalTxt, str.c_str(),Libelle);

		m_pstrDescript->Append(Temp);

		// Passe à item suivant
		m_NoQuestion++;
	}
	// Output modalite bloc end
	// m_pstrDescript->Append (Temp);
}

void CToTextIVisitor::VisitEnd(CIBItem & Item) 
{
	CString Type = _T("items");
	IItem *pItem = m_pIConst->GetIItemByID(Item.GetID());

	if(pItem)
	{
		if(pItem->IsQuestionInduite())
			Type = _T("induite");
		if(pItem->IsPalette())
			Type = _T("palette");
		else if(pItem->IsCritereModifie())
			Type = _T("modifie");
		else if(pItem->IsCritereQuantile())
			Type = _T("quantile");
	}

	// Output modalite bloc begin with operator and type
	// Find the question label
	CString Temp;
	Temp.Format(_T("%s\n"),Type);
	// m_pstrDescript->Append (Temp);
}
