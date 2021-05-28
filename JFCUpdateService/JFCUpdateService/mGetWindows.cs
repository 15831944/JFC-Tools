using System.Runtime.InteropServices;
using JFCUpdateService;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace JFCUpdateService
{
    [StandardModule]
    internal sealed class mGetWindows
    {
        private delegate int EnumWindowsCallback(int hWnd, int lParam);

        private static string m_sPattern;

        private static int m_lhFind;

        [DllImport("user32", CharSet = CharSet.Ansi, EntryPoint = "GetWindowTextA", ExactSpelling = true, SetLastError = true)]
        private static extern int GetWindowText(int Hwnd, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpString, int cch);

        [DllImport("user32.dll", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        private static extern int EnumWindows(EnumWindowsCallback callback, int lParam);

        private static int EnumWinProc(int Hwnd, int lParam)
        {
            m_lhFind = 0;
            string lpString = Strings.Space(128);
            int windowText = GetWindowText(Hwnd, ref lpString, 128);
            if (windowText > 0)
            {
                lpString = Strings.Left(lpString, windowText);
                if (lParam == 0)
                {
                    lpString = Strings.UCase(lpString);
                }
                if (LikeOperator.LikeString(lpString, m_sPattern, CompareMethod.Binary))
                {
                    m_lhFind = Hwnd;
                    return 0;
                }
            }
            return 1;
        }

        public static int FindWindowHwnd(ref string sWild)
        {
            m_sPattern = Strings.UCase(sWild);
            EnumWindows(EnumWinProc, 0);
            return m_lhFind;
        }

        public static bool FindWindow(ref string TaskWindowsList)
        {
            string text = "";
            string[] arySrc = Strings.Split(TaskWindowsList, "|");
            arySrc = (string[])Utils.CopyArray(arySrc, new string[21]);
            bool flag = false;
            int num = 0;
            while (true)
            {
                string sWild = Strings.Trim(arySrc[num]);
                if (Operators.CompareString(sWild, "", TextCompare: false) == 0)
                {
                    break;
                }
                if (FindWindowHwnd(ref sWild) != 0)
                {
                    text = ((Operators.CompareString(text, "", TextCompare: false) != 0) ? (text + "\r\n" + sWild) : sWild);
                    flag = true;
                }
                num = checked(num + 1);
            }
            if (flag)
            {
                text = "Veulliez quitter l'application suivante:\r\n\r\n" + text + "\r\n\r\npuis relancer l'application.";
                Interaction.MsgBox(text, MsgBoxStyle.Exclamation);
            }
            return flag;
        }
    }
}