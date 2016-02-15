using System;
using FileHelpers;

namespace ATR_Report
{
    [FixedLengthRecord(FixedMode.AllowLessChars)]
    public class Station
    {
        [FieldFixedLength(6)]
        [FieldTrim(TrimMode.Right)]
        public string Nr;

        [FieldFixedLength(6)]
        [FieldTrim(TrimMode.Right)]
        public string Name5C;

        [FieldFixedLength(2)]
        [FieldTrim(TrimMode.Right)]
        public string M;

        [FieldFixedLength(2)]
        [FieldTrim(TrimMode.Right)]
        public string T;

        [FieldFixedLength(3)]
        [FieldTrim(TrimMode.Right)]
        public string TY;

        [FieldFixedLength(31)]
        [FieldTrim(TrimMode.Right)]
        public string LongName;

        [FieldFixedLength(4)]
        [FieldTrim(TrimMode.Right)]
        public string NrT;

        [FieldFixedLength(6)]
        [FieldTrim(TrimMode.Right)]
        public string Regte;

        [FieldFixedLength(4)]
        [FieldTrim(TrimMode.Right)]
        public string Ord;

        [FieldFixedLength(13)]
        [FieldTrim(TrimMode.Right)]
        public string Logo;

        [FieldFixedLength(4)]
        [FieldTrim(TrimMode.Right)]
        public string FAM;

        [FieldFixedLength(7)]
        [FieldTrim(TrimMode.Right)]
        public string DateFrom;

        [FieldFixedLength(7)]
        [FieldTrim(TrimMode.Right)]
        public string DateTo;

        [FieldFixedLength(6)]
        [FieldOptional]
        [FieldTrim(TrimMode.Right)]
        public string Coef;

        [FieldFixedLength(3)]
        [FieldOptional]
        [FieldTrim(TrimMode.Right)]
        public string NbOpt;

        [FieldFixedLength(1000)]
        [FieldOptional]
        [FieldTrim(TrimMode.Right)]
        public string Options;

    }
}
