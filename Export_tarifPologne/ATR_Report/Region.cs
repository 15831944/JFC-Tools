using System;
using FileHelpers;

namespace ATR_Report
{
    [FixedLengthRecord(FixedMode.AllowLessChars)]
    public class region
    {
        [FieldFixedLength(4)]
        [FieldTrim(TrimMode.Right)]
        public string Nr;

        [FieldFixedLength(6)]
        [FieldTrim(TrimMode.Right)]
        public string Code;

        [FieldFixedLength(11)]
        [FieldTrim(TrimMode.Right)]
        public string ShortName;

        [FieldFixedLength(31)]
        [FieldTrim(TrimMode.Right)]
        public string LongName;

        [FieldFixedLength(9)]
        [FieldTrim(TrimMode.Right)]
        public string ShorterName;

        [FieldFixedLength(6)]
        [FieldTrim(TrimMode.Right)]
        public string Rmini;


    } 

}
