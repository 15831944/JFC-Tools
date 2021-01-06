// IBloc.cpp: implementation of the IBloc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IBloc.h"

#include <string>
#include <locale>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IBloc::IBloc()
{
	m_Operator = IBloc::Nop;
}

IBloc::~IBloc()
{

}

// Gets a value from a string
JInt32	IBloc::ToIntValue(const JChar ** pstr) const
{
	std::string Value;

	while(**pstr)
	{
		if(isdigit(**pstr))
		{
			Value.append(*pstr,sizeof(JChar));
			(*pstr)++;
		}
		else
			break;
	}
	JInt32 IValue = atoi(Value.c_str());
	return IValue;
}

// Changes an Int32 value to a string
std::string	IBloc::IntToString(JInt32 IValue) const
{
		JChar Buffer[50];
		sprintf_s(Buffer, 50, "%d",IValue);
		std::string str = Buffer;
		return str;
}

// Gets an Flt32 value from a string
JFlt32	IBloc::ToFltValue(const JChar ** pstr) const
{
	std::string Value;

	while(**pstr)
	{
		if(isdigit(**pstr) || **pstr == '-' || **pstr == '.' || **pstr == ',')
		{
			Value.append(*pstr, sizeof(JChar));
			(*pstr)++;
		}
		else
			break;
	}
	JFlt32 FValue = (JFlt32) atof(Value.c_str());
	return FValue;
}

// Changes a Flt32 value to a string
std::string	IBloc::FltToString(JFlt32 FValue) const
{
		JChar Buffer[50];
		sprintf_s(Buffer, 50, "%f",FValue);
		std::string str = Buffer;
		return str;
}

//Appends the operator to the string
JBool	IBloc::OperatorAsString(std::string & str, OPERATOR Op) const
{
	switch(Op)
	{
		case IBloc::Nop :
			return true;

		case IBloc::And :
			str.append(_T("&"));
			return true;

		case IBloc::Or :
			str.append(_T("|"));
			return true;

		case IBloc::Not :
			str.append(_T("!"));
			return true;

		case IBloc::Xor :
			str.append(_T("^"));
			return true;

		case IBloc::LT :
			str.append(_T("<"));
			return true;

		case IBloc::GT :
			str.append(_T(">"));
			return true;

		case IBloc::LTE :
			str.append(_T("<="));
			return true;

		case IBloc::GTE :
			str.append(_T(">="));
			return true;

		case IBloc::EQ :
			str.append(_T("="));
			return true;
			
		case IBloc::ALL :
			str.append(_T("*"));
			return true;
		default : return false;
	}
}

//Appends the operator to the string
JBool	IBloc::OperatorAsXML(std::string & str, OPERATOR Op) const
{
	switch(Op)
	{
		case IBloc::Nop :
			return true;

		case IBloc::And :
			str.append(_T("ET"));
			return true;

		case IBloc::Or :
			str.append(_T("OU"));
			return true;

		case IBloc::Not :
			str.append(_T("NI"));
			return true;

		case IBloc::Xor :
			str.append(_T("EX"));
			return true;

		case IBloc::LT :
			str.append(_T("&#60;"));
			return true;

		case IBloc::GT :
			str.append(_T("&#62;"));
			return true;

		case IBloc::LTE :
			str.append(_T("&#60;="));
			return true;

		case IBloc::GTE :
			str.append(_T("&#62;="));
			return true;

		case IBloc::EQ :
			str.append(_T("="));
			return true;
			
		case IBloc::ALL :
			str.append(_T("*"));
			return true;
		default : return false;
	}
}
