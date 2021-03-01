// ABPoint.cpp: implementation of the CABPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ABPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CABPoint::CABPoint()
{
	m_Value = -1;
	SetOperator(IBloc::Nop);
}

CABPoint::~CABPoint()
{
	Reset();
}

// Resets to zero this object
JBool	CABPoint::Reset()
{
	m_Value = -1;
	SetOperator(IBloc::Nop);
	return true;
}

// Validate this object removing all empty sub objects
JBool	CABPoint::IsValid()
{
	if(m_Value<0 ||  m_Operator==IBloc::Nop)
		return false;
	return true;
}

// Converts this object to a string
JBool	CABPoint::AsString(std::string & str) const
{
	str.append(_T("P"));
	OperatorAsString(str, m_Operator);

	str.append(_T("%"));
	str.append(IntToString(m_Value));

	return true;
}

// Builds this object from a string
JBool	CABPoint::FromString(const JChar ** pstr)
{
	OPERATOR CurOp = IBloc::Nop;
	
	while(**pstr != ')')
	{

		switch(**pstr)
		{
			case '>': // Greater than 
			{
				CurOp = IBloc::GT;
				(*pstr)++;
			}
			break;

			case '<': // Less than or Equals
			{
				CurOp = IBloc::LT;
				(*pstr)++;// Skip past the less
			}
			break;

			case '=': // Equals
			{
				switch(CurOp)
				{
					case IBloc::GT:
					{
						CurOp = IBloc::GTE;
					}
					break;

					case IBloc::LT:
					{
						CurOp = IBloc::LTE;
					}
					break;

					case IBloc::Nop:
					{
						CurOp = IBloc::EQ;
					}
					break;

				}
				(*pstr)++;
			}
			break;

			case '%': // Value
			{
				SetOperator(CurOp);
				++(*pstr);
				m_Value = ToIntValue(pstr);
			}
			break;

			default:
				return false;

		}
	}
	return true;
}
