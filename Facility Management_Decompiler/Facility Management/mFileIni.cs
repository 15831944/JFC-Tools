using Facility_Management.My;
using System;
using System.Runtime.InteropServices;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace Facility_Management
{
    internal sealed class mFileIni
    {
        private const short MAX_LEN = 640;

        [DllImport("kernel32", CharSet = CharSet.Ansi, EntryPoint = "WritePrivateProfileStringA", ExactSpelling = true, SetLastError = true)]
        private static extern short WritePrivateProfileString([MarshalAs(UnmanagedType.VBByRefStr)] ref string MyApplicationName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MyKeyName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MykeyString, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MyFileName);

        [DllImport("kernel32", CharSet = CharSet.Ansi, EntryPoint = "GetPrivateProfileStringA", ExactSpelling = true, SetLastError = true)]
        private static extern short GetPrivateProfileString([MarshalAs(UnmanagedType.VBByRefStr)] ref string NomModule, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MotCle, [MarshalAs(UnmanagedType.VBByRefStr)] ref string StringParDefaut, [MarshalAs(UnmanagedType.VBByRefStr)] ref string ReturnedString, short Longueur, [MarshalAs(UnmanagedType.VBByRefStr)] ref string NomFichierIni);

        public static void Select_WriteIniString(string NomModule, string MotClé, string Chaine, string FichierIni)
        {
            if (Operators.CompareString(Chaine, "", TextCompare: false) == 0)
            {
                Chaine = null;
            }
            WritePrivateProfileString(ref NomModule, ref MotClé, ref Chaine, ref FichierIni);
        }

        public static string Select_GetIniString(ref string NomModule, ref string MotCle, ref string FichierIni)
        {
            string ReturnedString = Strings.Space(640);
            string StringParDefaut = "";
            GetPrivateProfileString(ref NomModule, ref MotCle, ref StringParDefaut, ref ReturnedString, 640, ref FichierIni);
            string text = Strings.Left(ReturnedString, checked(Strings.Len(Strings.Trim(ReturnedString)) - 1));
            if (text.Contains("<ISPROJECTDIR>"))
            {
                text = text.Replace("<ISPROJECTDIR>", MyProject.Forms.FormBox.ISPROJECTDIR);
            }
            if (text.Contains("<DATA>"))
            {
                text = text.Replace("<DATA>", MyProject.Forms.FormBox.DATA);
            }
            if (text.Contains("<DATAFRET>"))
            {
                text = text.Replace("<DATAFRET>", MyProject.Forms.FormBox.DATAFRET);
            }
            return text;
        }
    }
}
