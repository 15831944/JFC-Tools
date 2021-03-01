// MultiUserEDI.mFunction
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using System;

namespace MultiUserEDI
{
    [StandardModule]
    internal sealed class mFunction
    {
        public static int StrCompTextVersions(string version1, string version2)
        {
            int result = 0;
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
    }
}
