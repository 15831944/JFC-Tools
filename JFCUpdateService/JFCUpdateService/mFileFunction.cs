using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace JFCUpdateService
{
    [StandardModule]
    internal sealed class mFileFunction
    {
        public const short PATH = 4;

        public const short FILENAME = 3;

        public const short EXTENSION_ONLY = 2;

        public const short FILENAME_ONLY = 1;

        public const short PATH_FILENAME_ONLY = 8;

        public static string ParsePath(string szPath, short nOperation)
        {
            checked
            {
                int num = Strings.InStrRev(szPath, ".") - 1;
                int num2 = Strings.InStrRev(szPath, "\\");
                int num3 = Strings.Len(szPath);
                switch (nOperation)
                {
                    case 2:
                        return Strings.Right(szPath, num3 - num);
                    case 1:
                        return Strings.Mid(szPath, num2 + 1, num - num2);
                    case 3:
                        return Strings.Right(szPath, num3 - num2);
                    case 4:
                        return Strings.Left(szPath, num2);
                    case 8:
                        return Strings.Left(szPath, num);
                    default:
                        return szPath;
                }
            }
        }

        public static string StrAddLastSlash(string svString)
        {
            if (Operators.CompareString(svString, "", TextCompare: false) != 0 && Operators.CompareString(Strings.Right(svString, 1), "\\", TextCompare: false) != 0)
            {
                svString += "\\";
            }
            return svString;
        }
    }
}