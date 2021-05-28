using System;
using System.Collections;
using System.IO;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;


namespace JFCUpdateService
{
    [StandardModule]
    internal sealed class mFunction
    {
        public static int StrCompTextVersions(string version1, string version2)
        {
            int result = 0;
            if (Operators.CompareString(version2, null, TextCompare: false) == 0)
            {
                return 1;
            }
            if (Operators.CompareString(version1, null, TextCompare: false) == 0)
            {
                return -1;
            }
            checked
            {
                try
                {
                    string[] array = version1.Split('.');
                    string[] array2 = version2.Split('.');
                    int num = array.Length - 1;
                    for (int i = 0; i <= num; i++)
                    {
                        if (Conversion.Val(array[i]) > Conversion.Val(array2[i]))
                        {
                            return 1;
                        }
                        if (Conversion.Val(array[i]) < Conversion.Val(array2[i]))
                        {
                            return -1;
                        }
                    }
                    return result;
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    result = 1;
                    ProjectData.ClearProjectError();
                    return result;
                }
            }
        }

        public static bool IsDirectoryWritable(string path)
        {
            return (new DirectoryInfo(path).Attributes & FileAttributes.ReadOnly) != FileAttributes.ReadOnly;
        }

        public static string ArrayStringOf(ArrayList f, string findstring, int startindex = 0)
        {
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = f.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    string text = Conversions.ToString(enumerator.Current);
                    int num = f.LastIndexOf(text);
                    if (text.Contains(findstring) && num > startindex)
                    {
                        return text.Replace(findstring, null);
                    }
                }
            }
            finally
            {
                if (enumerator is IDisposable)
                {
                    (enumerator as IDisposable).Dispose();
                }
            }
            return null;
        }

        public static string ConvertOctets(double Octets)
        {
            if (Octets >= 1099511627776.0)
            {
                return Virgul(Octets / 1099511627776.0) + "To";
            }
            if (Octets >= 1073741824.0)
            {
                return Virgul(Octets / 1073741824.0) + "Go";
            }
            if (Octets >= 1048576.0)
            {
                return Virgul(Octets / 1048576.0) + "Mo";
            }
            if (Octets >= 1024.0)
            {
                return Virgul(Octets / 1024.0) + "Ko";
            }
            return Strings.Trim(Conversions.ToString(Octets)) + "o";
        }

        private static string Virgul(double Val_Renamed)
        {
            int num = Strings.Len(Strings.Trim(Conversions.ToString(Conversion.Int(Val_Renamed))));
            int digits = ((num < 3) ? checked(3 - num) : 0);
            return Strings.Trim(Conversions.ToString(Math.Round(Val_Renamed, digits)));
        }
    }
}