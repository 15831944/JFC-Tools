using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace Facility_Management
{
    public class MacAddress
    {
        public static bool bVirtual;

        public static string GetValue(string sMacAddressCurrent = "")
        {
            NetworkInterface[] allNetworkInterfaces = NetworkInterface.GetAllNetworkInterfaces();
            string[] array = new string[1];
            int num = 0;
            if (!Information.IsArray(allNetworkInterfaces))
            {
                return "000000000000";
            }
            bool flag = false;
            while (true)
            {
                NetworkInterface[] array2 = allNetworkInterfaces;
                foreach (NetworkInterface networkInterface in array2)
                {
                    if ((((networkInterface.NetworkInterfaceType == NetworkInterfaceType.Ethernet) & (bVirtual | !networkInterface.Description.Contains("Virtual"))) && !flag) | (flag & (networkInterface.NetworkInterfaceType == NetworkInterfaceType.Wireless80211)))
                    {
                        byte[] addressBytes = networkInterface.GetPhysicalAddress().GetAddressBytes();
                        checked
                        {
                            array = (string[])Utils.CopyArray(array, new string[num + 1]);
                            array[num] = "";
                            int num2 = addressBytes.Length - 1;
                            for (int j = 0; j <= num2; j++)
                            {
                                array[num] += addressBytes[j].ToString("X2");
                            }
                            num++;
                        }
                    }
                }
                if (!(num == 0 && !flag))
                {
                    break;
                }
                flag = true;
            }
            if (num == 0)
            {
                return "000000000000";
            }
            try
            {
                if ((Operators.CompareString(sMacAddressCurrent, "", TextCompare: false) != 0) & (sMacAddressCurrent.Length == 12))
                {
                    string[] array3 = array;
                    foreach (string text in array3)
                    {
                        if (sMacAddressCurrent.Contains(text))
                        {
                            return text;
                        }
                    }
                }
                return array[0];
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                string result = "000000000000";
                ProjectData.ClearProjectError();
                return result;
            }
        }
    }

}
