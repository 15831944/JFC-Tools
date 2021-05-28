// MultiUserEDI.mFileFunction
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using System.IO;
using System.Text;
using MultiUserEDI.My;

namespace MultiUserEDI
{
    [StandardModule]
    internal sealed class mFileFunction
    {
        public const short PATHBACKSLASH = 7;

        public const short NODELETE = 76;

        public const short BACKSLASH = 6;

        public const short SLASH = 5;

        public const short PATH = 4;

        public const short FILENAME = 3;

        public const short EXTENSION_ONLY = 2;

        public const short FILENAME_ONLY = 1;

        public const short PATH_FILENAME_ONLY = 8;

        public static string ParsePath(string szPath, short nOperation)
        {
            //return System.IO.Path.GetDirectoryName(szPath);

            //checked
            //{
            int num = Strings.InStrRev(szPath, ".") - 1;
            int num2 = Strings.InStrRev(szPath, "\\");
            int num3 = Strings.Len(szPath);
            //return nOperation switch

            //    {

            //        2 => Strings.Right(szPath, num3 - num),
            //        1 => Strings.Mid(szPath, num2 + 1, num - num2),
            //        3 => Strings.Right(szPath, num3 - num2),
            //        4 => Strings.Left(szPath, num2),
            //        8 => Strings.Left(szPath, num),
            //        _ => szPath,
            //    };

            return Strings.Right(szPath, num3 - num) + Strings.Mid(szPath, num2 + 1, num - num2) +
                   Strings.Right(szPath, num3 - num2) + Strings.Left(szPath, num2) + Strings.Left(szPath, num) + szPath;

        }

        public static string ReadInfoToFile(string NomFic, short nOperation = 0)
        {
            string result = "";
            checked
            {
                if (File.Exists(NomFic))
                {
                    result = File.ReadAllText(NomFic, Encoding.Default);
                    string[] array = Strings.Split(result, "\r\n");
                    for (int i = array.Length - 1; i >= 0; i += -1)
                    {
                        if (Operators.CompareString(array[i], "", TextCompare: false) != 0)
                        {
                            array = (string[])Utils.CopyArray(array, new string[i + 1]);
                            break;
                        }
                    }
                    result = Strings.Join(array, "\r\n");
                    if (nOperation != 76)
                    {
                        File.Delete(NomFic);
                    }
                }
                return result;
            }
        }

        public static void WriteInfoToFile(string NomFic, string svInfo)
        {
            if (File.Exists(NomFic))
            {
                File.Delete(NomFic);
            }
            File.WriteAllText(NomFic, svInfo, Encoding.Default);
        }

        public static string Add(string svString, short nOperation, string svPath = "")
        {
            switch (nOperation)
            {
                case 5:
                    if ((Operators.CompareString(svString, "", TextCompare: false) != 0) & (Operators.CompareString(svString, "/", TextCompare: false) != 0))
                    {
                        if (Operators.CompareString(Strings.Right(svString, 1), "/", TextCompare: false) != 0)
                        {
                            svString += "/";
                        }
                        if (Operators.CompareString(Strings.Left(svString, 1), "/", TextCompare: false) != 0)
                        {
                            svString = "/" + svString;
                        }
                    }
                    else
                    {
                        svString = "/";
                    }
                    break;
                case 6:
                    if (Operators.CompareString(svString, "", TextCompare: false) != 0 && Operators.CompareString(Strings.Right(svString, 1), "\\", TextCompare: false) != 0)
                    {
                        svString += "\\";
                    }
                    break;
                case 4:
                    if (Operators.CompareString(svPath, "", TextCompare: false) != 0)
                    {
                        svPath = Add(svPath, 6);
                        if ((Operators.CompareString(Strings.Mid(svString, 2, 1), ":", TextCompare: false) != 0) & (Operators.CompareString(Strings.Mid(svString, 2, 1), "\\", TextCompare: false) != 0))
                        {
                            svString = svPath + svString;
                        }
                    }
                    break;
                case 7:
                    svString = Add(svString, 6);
                    svString = Add(svString, 4, svPath);
                    break;
            }
            return svString;
        }

        public static bool IsDirectoryWritable(string path)
        {
            DirectoryInfo directoryInfo = new DirectoryInfo(path);
            if (!directoryInfo.Exists)
            {
                return false;
            }
            return (directoryInfo.Attributes & FileAttributes.ReadOnly) != FileAttributes.ReadOnly;
        }

        public static bool IsFileReadOnly(string sFile)
        {
            return MyProject.Computer.FileSystem.GetFileInfo(sFile).IsReadOnly;
        }
    }
}
