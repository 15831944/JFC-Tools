// MultiUserEDI.mFileIni
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;


namespace MultiUserEDI
{
    [StandardModule]
    internal sealed class mFileIni
    {
        private const short MAX_LEN = 640;

        [DllImport("kernel32", CharSet = CharSet.Ansi, EntryPoint = "WritePrivateProfileStringA", ExactSpelling = true, SetLastError = true)]
        private static extern short WritePrivateProfileString([MarshalAs(UnmanagedType.VBByRefStr)] ref string MyApplicationName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MyKeyName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MykeyString, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MyFileName);

        [DllImport("kernel32", CharSet = CharSet.Ansi, EntryPoint = "GetPrivateProfileStringA", ExactSpelling = true, SetLastError = true)]
        private static extern short GetPrivateProfileString([MarshalAs(UnmanagedType.VBByRefStr)] ref string NomModule, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MotCle, [MarshalAs(UnmanagedType.VBByRefStr)] ref string StringParDefaut, [MarshalAs(UnmanagedType.VBByRefStr)] ref string ReturnedString, short Longueur, [MarshalAs(UnmanagedType.VBByRefStr)] ref string NomFichierIni);

        public static void Select_WriteIniString(string NomModule, string MotClé, string Chaine, string FichierIni)
        {
            if (Operators.CompareString(Chaine, null, TextCompare: false) == 0)
            {
                DeleteIniString(NomModule, MotClé, FichierIni);
            }
            else
            {
                WritePrivateProfileString(ref NomModule, ref MotClé, ref Chaine, ref FichierIni);
            }
        }

        public static string Select_GetIniString(ref string NomModule, ref string MotCle, ref string FichierIni)
        {
            string ReturnedString = Strings.Space(640);
            string StringParDefaut = "";
            GetPrivateProfileString(ref NomModule, ref MotCle, ref StringParDefaut, ref ReturnedString, 640, ref FichierIni);
            return Strings.Left(ReturnedString, checked(Strings.Len(Strings.Trim(ReturnedString)) - 1));
        }

        private static void DeleteIniString(string NomModule, string MotClé, string FichierIni)
        {
            List<string> list = new List<string>(File.ReadAllLines(FichierIni, Encoding.Default));
            bool flag = false;
            for (int i = 0; i < list.Count; i = checked(i + 1))
            {
                string text = list[i];
                if (Strings.StrComp(text, "[" + NomModule + "]", CompareMethod.Text) == 0)
                {
                    flag = true;
                    continue;
                }
                if (Operators.CompareString(Strings.Left(text, 1), "[", TextCompare: false) == 0 && flag)
                {
                    break;
                }
                if (Strings.StrComp(Strings.Split(text, "=")[0], MotClé, CompareMethod.Text) == 0 && flag)
                {
                    list.RemoveAt(i);
                    i = checked(i - 1);
                }
            }
            File.WriteAllLines(FichierIni, list, Encoding.Default);
        }
    }
}