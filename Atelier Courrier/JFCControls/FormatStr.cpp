#include "StdAfx.h"
#include ".\FormatStr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char CFormatStr::m_Decimal = '.';

CFormatStr::CFormatStr(void)
{
}

CFormatStr::~CFormatStr(void)
{
}

bool CFormatStr::SetDecimalPoint(char Decimal)
{
	long Dec = (long)Decimal; if (Dec < 0) Dec += 255;
	if (Dec <= 32 || Dec >= 128) return (false);
	m_Decimal = Decimal;
	return (true);
}

char CFormatStr::GetDecimalPoint()
{
	return (m_Decimal);
}

//		CString Fmt = _T("### ### ### ### ###");
//		Fmt = _T("###,##");
//		Fmt = _T("### ###.##");

void CFormatStr::FormatNum(const CString &Fmt, CString &Num, const char Decimal)
{
	CString End			=	"";
	CString Beginning	=	"";
	int LPN				=	Num.GetLength();
	int LPF				=	Fmt.GetLength();
	int PointPN			=	Num.ReverseFind(CFormatStr::m_Decimal);
	int PointPF			=	Fmt.ReverseFind(Decimal);
	int ppn				=	0;
	int ppf				=	0;

	if(LPN>0 && LPF>0)
	{
		// Check if we have a Decimal
		if(PointPN == -1 || PointPF == -1)
		{
			if(PointPN == -1)
				PointPN = Num.GetLength()-1;
			else
				PointPN--;

			if(PointPF == -1)
				PointPF = Fmt.GetLength()-1;
			else
				PointPF--;
		}
		else
		{
			End.AppendChar(CFormatStr::m_Decimal);

			ppn = PointPN+1;
			ppf = PointPF+1;

			while(ppn < LPN)
			{
				if(ppf >= LPF || Fmt.GetAt(ppf) ==  '#')
					End.AppendChar(Num.GetAt(ppn++)); 
				else
					End.AppendChar(Fmt.GetAt(ppf)); 
				ppf++;
			}
			PointPN--;
			PointPF--;
		}

		ppn = PointPN;
		ppf = PointPF;
		while(ppn >= 0)
		{
			if(ppf < 0 || Fmt.GetAt(ppf) ==  '#')
				Beginning.Insert(0,Num.GetAt(ppn--)); 
			else
				Beginning.Insert(0,Fmt.GetAt(ppf)); 
			ppf--;
		}
	}

	Num = Beginning + End;
}

CString CFormatStr::FormatDate(JDate date)
{
	JInt32 jour, mois, annee;
	CString str;
	str = "";
	if (date.IsValid())
	{
		date.GetDate(jour, mois, annee);
		str.Format("%02d/%02d/%d ", jour, mois, annee);
	}
	return str;
}

CString CFormatStr::FormatDateLong(JDate Date, JTime Time)
{
	CString TxtDate;
	JInt32	Day;
	JInt32	Month;
	JInt32	Year;
	JInt32	Hour;
	JInt32	Minute;
	JInt32	Second;
	CString StrJour;
	CString StrMois;

	// Récupère date
	Date.GetDate(Day,Month,Year); 

	switch (Date.GetDayOfWeek())
	{
	case 1:
		StrMois = "Lundi";
		break;
	case 2:
		StrMois = "Mardi";
		break;
	case 3:
		StrMois = "Mercredi";
		break;
	case 4:
		StrMois = "Jeudi";
		break;
	case 5:
		StrMois = "Vendredi";
		break;
	case 6:
		StrMois = "Samedi";
		break;
	case 7:
		StrMois = "Dimanche";
		break;
	}

	switch (Month)
	{
	case 1:
		StrMois = "Janvier";
		break;
	case 2:
		StrMois = "Février";
		break;
	case 3:
		StrMois = "Mars";
		break;
	case 4:
		StrMois = "Avril";
		break;
	case 5:
		StrMois = "Mai";
		break;
	case 6:
		StrMois = "Juin";
		break;
	case 7:
		StrMois = "Juillet";
		break;
	case 8:
		StrMois = "Août";
		break;
	case 9:
		StrMois = "Septembre";
		break;
	case 10:
		StrMois = "Octobre";
		break;
	case 11:
		StrMois = "Novembre";
		break;
	case 12:
		StrMois = "Décembre";
		break;

	}

	// Récupère time
	Time.GetTime(Hour,Minute,Second); 

	// Formate info date - time
	TxtDate.Format("%s %02d %s %d à %02dh%02d% 02ds ", StrJour, Day, StrMois, Year, Hour, Minute, Second);
	//Dlg.SetLibelle(JLabel(TxtDateArchive));
	return TxtDate;
}

CString CFormatStr::FormatCout(double value, int nbDecimales)
{
	CString outStr;
	CString Fmt;

	switch(nbDecimales)
	{
	case 0:
		outStr.Format(_T("%.0f"), value);
		Fmt = _T("### ### ### ### ### € ");
		FormatNum(Fmt, outStr);
		break;
	case 1:
		outStr.Format(_T("%.1f € "), value);
		Fmt = _T("### ### ### ### ###,#");
		FormatNum(Fmt, outStr);
		break;
	case 2:
	default:
		outStr.Format(_T("%.2f € "), value);
		Fmt = _T("### ### ### ### ###,##");
		FormatNum(Fmt, outStr);
		break;
	}
	return outStr;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
///#################################################################################################///
///////////////////////////////////////////////////////////////////////////////////////////////////////

CCompareTitre::CCompareTitre(void)
{
}

CCompareTitre::~CCompareTitre(void)
{
}

int CCompareTitre::CompareTitre(CString str1, CString str2)
{
	CCompareTitre::PrepareStr(str1);
	CCompareTitre::PrepareStr(str2);

	return str1.CompareNoCase(str2);
}

void CCompareTitre::PrepareStr(CString &str)
{
	str.MakeLower();
	str = str.TrimLeft();
	str = str.TrimRight();

	str.Replace('é', 'e');
	str.Replace('è', 'e');
	str.Replace('ê', 'e');

	if (str.Find("l'") == 0)
		str.Delete(0,2);

	if (str.Find("le ") == 0)
		str.Delete(0,3);
	
	if (str.Find("la ") == 0)
		str.Delete(0,3);

	if (str.Find("les ") == 0)
		str.Delete(0,4);
}

