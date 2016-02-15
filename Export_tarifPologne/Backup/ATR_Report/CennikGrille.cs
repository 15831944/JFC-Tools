using System;
using FileHelpers;

namespace ATR_Report
{
    [DelimitedRecord(";")]
    public class CennikOpis
    {
        public string motcle;
        public int nrordre;
        public string libelle;
        public int nbgrille;

    }
    [DelimitedRecord(";")]
    public class CennikGrille
    {
        public string keyword;
        public int Nr;
        public int Stat;
       // [FieldConverter(ConverterKind.Date, "ddmmyyyy")]
        public DateTime StartDate;
       // [FieldConverter(ConverterKind.Date, "ddmmyyyy")]
        public DateTime EndDate;
        public string StartTime;
        public string EndTime;
        public int Format;
        public int Curr;
        public int BiemCost;
        public int BlocQty;
    }
    [DelimitedRecord(";")]
    public class CennikBloc
    {
        public string descr;
        public int blocnr;
        public string heuredebut;
        public string heurefin;
        public int jourdebut;
        public int jourfin;
        public string prix;
        public int ecrannbr;

    }
    [DelimitedRecord("|")]
    public class CennikExcel
    {
        public string period;
        public int day;
        public int price;
    }
}