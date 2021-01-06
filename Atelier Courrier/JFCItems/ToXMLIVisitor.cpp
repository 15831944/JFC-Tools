#include "stdafx.h"
#include ".\toxmlivisitor.h"
#include "IBuilder.h"
#include "IBQuestion.h"
#include "IBItem.h"
#include "StringToXML.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CToXMLIVisitor::CToXMLIVisitor(void)
{
	m_OpStrate	=	"";
	m_OpQuestion	=	"";
	m_OpModalite	=	"";
	m_pBuilder	=	0;
	m_pstrXML	=	0;
	m_pTerrain	=	0;
	m_pIConst	=	0;
	m_ModaDim = 0;
	m_pQuestion = 0;
}

CToXMLIVisitor::~CToXMLIVisitor(void)
{
}

JBool CToXMLIVisitor::ToXML(CIBuilder *pBuilder, CItemsConstruitsBase * pItemConst, CTerrain * pTerrain, CString * pstrXML)
{
	m_pBuilder	=	pBuilder;
	m_pstrXML	=	pstrXML;
	m_pTerrain	=	pTerrain;
	m_pIConst	=	pItemConst;

	if(m_pTerrain && m_pstrXML && m_pBuilder)
		pBuilder->Accept(*this);
	else
		return false;
	return true;
}

void CToXMLIVisitor::VisitStart(CIBuilder & Builder)
{
	std::string str;
	Builder.OperatorAsXML(str,Builder.GetOperator());
	m_OpStrate.Format(_T("operator=\"%s\""), str.c_str());
}

void CToXMLIVisitor::VisitEnd(CIBuilder & Builder)
{
}

void CToXMLIVisitor::VisitStart(CIBSegment & Segment)
{
	std::string str;
	Segment.OperatorAsXML(str,Segment.GetOperator());
	m_OpQuestion.Format(_T("operator=\"%s\""), str.c_str());

	CString Temp;
	Temp.Format(_T("<strate %s>\n"),m_OpStrate);
	m_pstrXML->Append (Temp);
	m_pQuestion = 0;
}

void CToXMLIVisitor::VisitEnd(CIBSegment & Segment)
{
	m_pstrXML->Append (_T("</segment>\n"));
}

void CToXMLIVisitor::VisitStart(CIBQuestion & Question)
{
	// Find the question label
	CString qName = _T("libelle=\"error\"");
	m_pQuestion = m_pTerrain->GetQuestionByID(Question.GetID());
	if(m_pQuestion)
		qName.Format(_T("libelle=\"%s\""),CStringToXML::TextToXML(m_pQuestion->GetLabel().AsJCharPtr()));

	CString Temp;
	Temp.Format(_T("<question %s %s>\n"),m_OpQuestion, qName);
	m_pstrXML->Append (Temp);
	m_ModaDim = 0;
}

void CToXMLIVisitor::VisitEnd(CIBQuestion & Question)
{
	m_pstrXML->Append (_T("</question>\n"));
	m_pQuestion = 0;
}

void CToXMLIVisitor::Visit(CIBModalite & Modalite)
{
	// Output modalite bloc begin with operator and type
	std::string str;
	Modalite.OperatorAsXML(str,Modalite.GetOperator());
	CString Temp;

	JList<JInt32> Values;
	Modalite.GetValues(Values);

	switch(m_pQuestion->DimensionType(m_ModaDim))
	{
		case IQuestion::MODALITE: 
			Temp.Format(_T("<modalite operator=\"%s\" type=\"M\" count=\"%d\">\n"), str.c_str(), Values.GetCount());
			break;

		case IQuestion::ELEMENT: 
			Temp.Format(_T("<modalite operator=\"%s\" type=\"E\" count=\"%d\">\n"), str.c_str(), Values.GetCount());
			break;

		case IQuestion::NODIM: 
			Temp.Format(_T("<modalite operator=\"%s\" type=\"N\" count=\"%d\">\n"), str.c_str(), Values.GetCount());
			break;
	}

	m_pstrXML->Append (Temp);

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		CString Libelle = _T("error");
		if(m_pQuestion)
		{
			
			const IModalite * pMod = m_pQuestion->GetIModalite(Values.GetItem(),m_ModaDim);
			if(pMod)
				Libelle = CStringToXML::TextToXML(pMod->GetLabel().AsJCharPtr());
		}
		Temp.Format(_T("<libmoda>%s</libmoda>\n"),Libelle);
		m_pstrXML->Append (Temp);
	}
	// Output modalite bloc end
	m_pstrXML->Append (_T("</modalite>\n"));
	m_ModaDim++;
}

void CToXMLIVisitor::Visit(CIBNumValue & NumValue)
{
	CString Temp;

	JList<JFlt32> Values;
	NumValue.GetValues(Values);

	Temp.Format(_T("<values count=\"%d\">\n"),Values.GetCount());
	m_pstrXML->Append (Temp);

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		CString strDiv;
		if(Values.GetItem() < 0.0)
			strDiv.Format(_T("#"));
		else
			strDiv.Format(_T("%0.f"),Values.GetItem());

		Temp.Format(_T("<value>%s</value>\n"),strDiv);
		m_pstrXML->Append (Temp);
	}
	// Output values bloc end
	m_pstrXML->Append (_T("</values>\n"));
}

void CToXMLIVisitor::Visit(CIBClass & Class)
{
	CString Temp;

	JList<JFlt32> Values;
	Class.GetValues(Values);

	Temp.Format(_T("<classes count=\"%d\">\n"),Values.GetCount());
	m_pstrXML->Append (Temp);

	JList<IBloc::OPERATOR> Ops;
	Class.GetOperators(Ops);
	for(Values.MoveFirst(),Ops.MoveFirst(); Values.IsValid(); Values.MoveNext(),Ops.MoveNext())
	{
		CString strDiv;
		if(Values.GetItem() < 0.0)
			strDiv.Format(_T("#"));
		else
			strDiv.Format(_T("%0.f"),Values.GetItem());

		std::string str;
		Class.OperatorAsXML(str,Ops.GetItem());

		Temp.Format(_T("<class operator=\"%s\" value=\"%s\"></class>\n"),str.c_str(),strDiv);
		m_pstrXML->Append (Temp);
	}
	// Output classes bloc end
	m_pstrXML->Append (_T("</classes>\n"));
}

void CToXMLIVisitor::Visit(CIBQuant & Quant)
{
	std::string str;
	CString Temp;
	JInt32 Value = 0;
	JList<JInt32> Values;

	Temp.Format(_T("<quant"),str.c_str(), Value);
	m_pstrXML->Append (Temp);

	Quant.GetValues(Values);
	Values.MoveFirst();
	if(Values.IsValid())
		Value = Values.GetItem();
	else
		Value = 0;
	Temp.Format(" segment=\"%d\" ",Value);
	m_pstrXML->Append (Temp);

	Values.MoveNext();
	if(Values.IsValid())
		Value = Values.GetItem();
	else
		Value = 0;
	Temp.Format(" nbsegments=\"%d\" ",Value);
	m_pstrXML->Append (Temp);

	Temp.Format(_T("></quant>\n"),str.c_str(), Value);
	m_pstrXML->Append (Temp);
}

void CToXMLIVisitor::Visit(CIBPoint & Point)
{
	CString Temp;

	std::string str;
	Point.OperatorAsXML(str,Point.GetOperator());

	JInt32 Value = Point.GetValue();

	Temp.Format(_T("<point operator=\"%s\" value=\"%d\"></point>\n"),str.c_str(), Value);
	m_pstrXML->Append (Temp);
}

void CToXMLIVisitor::VisitStart(CIBItem & Item) 
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

		Name = CStringToXML::TextToXML(pItem->GetLabel().AsJCharPtr()); 
	}

	// Output modalite bloc begin with operator and type
	// Find the question label
	CString Temp;
	Temp.Format(_T("<%s %s libelle=\"%s\">\n"),Type, m_OpQuestion, Name);
	m_pstrXML->Append (Temp);

	std::string str;
	Item.OperatorAsXML(str,Item.GetOperator());

	JList<JInt32> Values;
	Item.GetValues(Values);

	Temp.Format(_T("<modalite operator=\"%s\" type=\"M\" count=\"%d\">\n"), str.c_str(),Values.GetCount());
	m_pstrXML->Append (Temp);

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		CString Libelle = _T("error");
		pItem = m_pIConst->GetIItemByID(Values.GetItem());
		if(pItem)
			Libelle = CStringToXML::TextToXML(pItem->GetLabel().AsJCharPtr());

		Temp.Format(_T("<item>%s</item>\n"),Libelle);
		m_pstrXML->Append(Temp);
	}
	// Output modalite bloc end
	m_pstrXML->Append (_T("</modalite>\n"));
	m_pstrXML->Append (Temp);
}

void CToXMLIVisitor::VisitEnd(CIBItem & Item) 
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
	Temp.Format(_T("</%s>\n"),Type);
	m_pstrXML->Append (Temp);
}
